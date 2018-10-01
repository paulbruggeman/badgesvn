#include <plib.h>
#include "badge16.h"
#include "ir.h"

/*
    38khz IR timer code and interupt code
    120 fps timer interupt code
    LED PWM
    backlight PWM

    Author: Paul Bruggeman
    paul@killercats.com
    4/2015
*/

#define SYS_FREQ 			(40000000L)

#define TOGGLES_PER_SEC		38000
#define T2_TICK       		(SYS_FREQ/TOGGLES_PER_SEC)
#define T2_TICK_DIV2       	(SYS_FREQ/TOGGLES_PER_SEC/2)

/* for touchPad and screen compositing*/
/* changing this number will definitely change up touchpad timings */
#define TOUCH_TOGGLES       120
#define T3_TICK       		(SYS_FREQ/TOUCH_TOGGLES)

/* same as IR for audio though */
#define T4_TICK       		(SYS_FREQ/TOGGLES_PER_SEC)

void doPWM();

void timerInit(void)
{
    OpenTimer2(T2_ON | T2_SOURCE_INT, T2_TICK);
    // set up the timer interrupt with a priority of 2
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);


    OpenTimer3(T3_ON | T3_SOURCE_INT, T3_TICK);
    // set up the timer interrupt with a priority of 5
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_5);

    OpenTimer4(T4_ON | T4_SOURCE_INT, T4_TICK);
    // set up the timer interrupt with a priority of 6
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_6);

    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    IEC0bits.INT1IE=0; // 2015 disable this interrupt
    TRISBbits.TRISB0 = 1; // 2015 IR IN
    CNPDBbits.CNPDB0 = 0;  // pulldown off
    CNPUBbits.CNPUB0 = 0;  // pullup off

    TRISBbits.TRISB13 = 0; // 2015 IR == OUTPUT
    CNPDBbits.CNPDB13 = 0;  // pulldown off
    CNPUBbits.CNPUB13 = 1;  // pullup on == IR off since driving transistor inverts val

    SYSKEY = 0x0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    CFGCONbits.IOLOCK = 0; // unlock configuration
    INT1Rbits.INT1R = 0b0010; // remap RPB0 to INT1
    CFGCONbits.IOLOCK = 1; // relock configuration

    SYSKEY = 0x0;

    INTCONbits.INT1EP=0; // edge polarity
    IPC1bits.INT1IP=1; // interrupt priority
    IPC1bits.INT1IS=1; // interrupt sub priority
    IEC0bits.INT1IE=1; // enable this interrupt

    IEC0bits.T2IE=1; // also enable timer2 interupt
}

// EXT4 int enabled when IR recv is 0 otherwise 1+
unsigned int G_IRsendVal = 0;
unsigned int G_IRrecvVal = 0;

unsigned int G_IRrecv = 0;
unsigned int G_IRsend = 0;

unsigned char G_IRerror = 0;

unsigned char G_bitCnt = 0;
unsigned char G_firstHalf = 0;
unsigned char G_lastHalf = 0;
unsigned char G_halfCount = 0;

/* 
  this code is based on RC5 timing from badge 2013
  almost certainly doesnt work for RC5 with a 38khz receiver

  min 6 cycle burst
  with burst (6-35 cycles) min gap between = 10 cycle
  max 2000 short burst/second
  with burst > 70 cycle needs game of 1.2 * burst len
*/
void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler(void)
{
   void do_audio();
   void do_leds();
   static unsigned int sendOne = 0;
   static unsigned int sendZero = 0;
   static unsigned char lowHalf = 1;
   static unsigned char highHalf = 1;

   // LATBbits.LATB8 == DBG pin shake sensor near LCD pins
   // each timer interrupt is 1/38khz
   if (G_IRrecv == 1) {
	if (G_bitCnt > 31) {
	    /* make sure queue not full */
	    if (((IRpacketInNext+1) % MAXPACKETQUEUE) != IRpacketInCurr) {
		IRpacketsIn[IRpacketInNext].v = G_IRrecvVal;
		/* overwrite if error */
		if (G_IRerror) {
		   IRpacketsIn[IRpacketInNext].p.address = IR_ERROR;
		   IRpacketsIn[IRpacketInNext].p.data = G_IRerror;
		   G_IRerror = 0;
		}

		/* if packet is not for us don't keep it */
		if ((IRpacketsIn[IRpacketInNext].p.badgeId == 0) 
		 | (IRpacketsIn[IRpacketInNext].p.badgeId == G_sysData.badgeId)) {
		    IRpacketInNext++;
		    IRpacketInNext %= MAXPACKETQUEUE;
		}
	    }
	    G_IRrecv = 0; // flag new value available
	    G_bitCnt = 0;
	    G_halfCount = 0;
	}
	else {
	    G_halfCount++;
	    // 32 interrupts for each half of bit send
	    // for 64 total per bit

	    if (G_halfCount == 16) G_firstHalf = !PORTBbits.RB0; 
	    if (G_halfCount == 48) G_lastHalf = !PORTBbits.RB0;

	    if (G_halfCount > 63) {
		G_IRrecvVal <<= 1 ;
		//LATBbits.LATB0 = G_lastHalf; // dbg LED output

		if ((G_firstHalf + G_lastHalf) != 1) G_IRerror++; /* increment errors. Error if not all manchester low->high, high->low transitions */
		G_IRrecvVal |= G_lastHalf;  

		G_bitCnt++;
		G_halfCount = 0;
	   }
	}
	// clear the interrupt flag
	mT2ClearIntFlag();

	return;
   }

   /* if we get this far then we are not in a receive */
   if (G_IRsend) {
        // 3 sections for IR send:
        // 1) init and looping vars
        // 2) send a zero
        // 3) send a one

        // break byte into bits to send
	// init, and looping. is the one or zero is done?
	if ((sendOne == 0) && (sendZero == 0)) {
	   if (G_bitCnt < 32) {
		// high order bit first
		if (G_bitCnt == 0) 
		   sendOne = 1; // send start bit
		else
		   sendOne = (G_IRsendVal & ((unsigned long long)0b10000000000000000000000000000000 >> G_bitCnt));

		sendZero = (sendOne == 0);
		G_bitCnt++;
	   }
	   else {
		G_IRsend = 0;
		G_bitCnt = 0;

	   }
	}


      // ;    A "1" in Manchester goes from Low-High over the bit period
      // ;    cycle. During the pulsing period, the carrier frequency should
      // ;    be 38kHz, and the duty cycle of the carrier should be about 1/4.
      // ;------------------------------------------------------------------

      //      ; LOW HALF 889us
      // 32 cycles off

      //      ; HIGH HALF (889us)
      //      ; Toggle 7us on, 21us off for 38kHz for the duration
      //      ; Pulsing 7 on, 21 off yields a 1/4 time duty cycle for carrier.
      //      ; Has 32 pulses of these periods 32*28us
      //      ;

      // each timer interrupt is 1/38khz
      if (sendOne) {
	 G_halfCount++;

	 if (lowHalf) {
	    // this is off for 32 counts

	    LATBbits.LATB13 = 0;

	    if (G_halfCount > 31) {
	      lowHalf = 0;
	      G_halfCount = 0;
	    }
	 }
	 else { 
	   // this is on for 7us (1/4 duty cycle) in each 
	   // 32 count 1/38khz interrupt

	   // high half
	   // 4 assignments is about 1us
	   // so 7us is about 4 * 7 assignments

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 0;

	    if (G_halfCount > 31) {
	      lowHalf = 1;
	      G_halfCount = 0;
	      sendOne = 0;
	    }
	 }
      }

      // ;------------------------------------------------------------------
      // ;  SendZero
      // ;
      // ;            A "0" in Manchester goes from High-Low over the bit period.
      // ;    The high period is a series of pulses of duty cycle 1/4 at a
      // ;    frequency of 36kHz.  This implementation yields 35.71kHz.
      // ;------------------------------------------------------------------
      // 
      //      ; HIGH HALF (889us)
      //      ; Toggle 7us on, 21us off for 35.7kHz (~36kHz) for the duration
      //      ; Pulsing 7 on, 21 off yields a 1/4 time duty cycle for carrier.
      //      ; Has 32 pulses of these periods 32*28us = 896us (~889us)
      //
      if (sendZero) {
	 G_halfCount++;
	 if (highHalf) {
	   // high half
	   // 4 assignments is about 1us
	   // so 7us is about 4 * 7 assignments

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;
	   LATBbits.LATB13 = 1;

	   LATBbits.LATB13 = 0;

	   if (G_halfCount > 31) {
	     highHalf = 0;
	     G_halfCount = 0;
	   }
	 }
	 else {
	   //      ; LOW HALF (889us = 889 instr cycles)

	   LATBbits.LATB13 = 0;

	   if (G_halfCount > 31) {
	      highHalf = 1;
	      G_halfCount = 0;
	      sendZero = 0;
	   }
	 }
      }
   }
   else { /* not currently sending, check if IR queue is empty */
	if (IRpacketOutNext != IRpacketOutCurr) {
	    G_IRsendVal = IRpacketsOut[IRpacketOutCurr].v;
	    G_IRsend = 1;
	    IRpacketOutCurr++;
	    IRpacketOutCurr %= MAXPACKETQUEUE;
	}
   }

   // clear the interrupt flag
   mT2ClearIntFlag();

   return;
}

// input changed on RB0
void __ISR( _EXTERNAL_1_VECTOR, IPL1) Int1Interrupt(void)
{ 
   // if not sending, signal in receive
   if ((G_IRsend == 0) & (G_IRrecv == 0)) {
	G_IRrecv = 1;

	// firstHalf was zero, this is one, so bit is 1
	G_firstHalf = 0; // inverted from what it was
	G_halfCount = 32;
	G_IRrecvVal = 0;

   }
   IFS0bits.INT1IF = 0;
}

void __ISR(_TIMER_3_VECTOR, IPL5) Timer3Handler(void)
{
   void touchInterrupt();

   mT3ClearIntFlag(); // clear the interrupt flag
   touchInterrupt(); /* ADC for each button */
}

void __ISR(_TIMER_4_VECTOR, IPL6) Timer4Handler(void)
{

   mT4ClearIntFlag(); // clear the interrupt flag
   doAudio();
   doPWM();
}

unsigned char G_red_cnt=0;
unsigned char G_red_pwm=0;

unsigned char G_green_cnt=0;
unsigned char G_green_pwm=0;

unsigned char G_blue_cnt=0;
unsigned char G_blue_pwm=0;

unsigned char G_bright=0;

unsigned char G_backlight=255;
unsigned char G_backlight_cnt=0;


void led_brightness(unsigned char bright) {
   G_bright = bright;
}

void red(unsigned char onPWM) {
    onPWM >>= G_bright;
    G_red_pwm = onPWM; 
    G_red_cnt = 0;
}

void green(unsigned char onPWM) {
    onPWM >>= G_bright;
    G_green_pwm = onPWM; 
    G_green_cnt = 0;
}

void blue(unsigned char onPWM) {
    onPWM >>= G_bright;
    G_blue_pwm = onPWM; 
    G_blue_cnt = 0;
}

void doPWM()
{
    /* red */
    G_red_cnt++;
    if (G_red_cnt < G_red_pwm)
        LATCbits.LATC0 = 1;
    else
        LATCbits.LATC0 = 0;

    // just let it wrap around if (G_red_cnt == 255) G_red_cnt = 0;

    /* Green */
    G_green_cnt++;
    if (G_green_cnt < G_green_pwm)
        LATBbits.LATB3 = 1;
    else
        LATBbits.LATB3 = 0;

    // just let it wrap around if (G_green_cnt == 255) G_green_cnt = 0;

    /* Blue */
    G_blue_cnt++;
    if (G_blue_cnt < G_blue_pwm)
        LATCbits.LATC1 = 1;
    else
        LATCbits.LATC1 = 0;

    // just let it wrap around if (G_blue_cnt == 255) G_blue_cnt = 0;

    G_backlight_cnt++;
    if (G_backlight_cnt < G_backlight)
        LATCbits.LATC9 = 1;
    else
        LATCbits.LATC9 = 0;
}

void backlight(unsigned char bright) {
    G_backlight = bright; 
    G_backlight_cnt = 0;
}
