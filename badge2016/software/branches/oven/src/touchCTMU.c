#include "TimeDelay.h"
#include "plib.h"
#include "touchCTMU.h"
/*
    touch CTMU interupt function
    loosly based on Microchip touch demo

    Author: Paul Bruggeman
    paul@killercats.com
    4/2015

    Additional code
    Author: Michael 
*/

unsigned int touchstamp=0;
unsigned int currentstamp=0;


#define NUM_VALUES_FOR_AVERAGE 16
#define TRIP_VALUE 2048
#define SLOW_AVERAGE_DELAY 16

/* see data sheet for ADC channel info */
static unsigned short int ButtonADCChannels[NUM_DIRECT_KEYS] = {0,1,3,4};
unsigned short int ButtonVmeasADC[NUM_DIRECT_KEYS]={0,0,0,0}; // Report out all voltages at once

unsigned int startTouch = 0;
unsigned int endTouch = 0;
unsigned int duration = 0;
unsigned char Nnops=3;
unsigned short int CurrentButtonStatus=0; // Bit field of buttons that are pressed
static unsigned char slowAverageCount = 0;
//Slow Average for auto-calibrating touch channels
unsigned short int slowAverage[NUM_DIRECT_KEYS] = {40000, 40000, 40000, 40000};
unsigned short int tripValue[NUM_DIRECT_KEYS] = {32000, 32000, 32000, 32000};

bool isVerticalSliding = false;
bool isHorizontalSliding = false;
#define SLIDE_START_TOP 1
#define SLIDE_START_BOTTOM 2
#define SLIDE_START_LEFT 3
#define SLIDE_START_RIGHT 4
unsigned char slideStart = 0;

/* 
   these values are updated at the last state of the touchInterrupt() 
   sample+averaging cycle so are valid anytime
   they can be used in conjunction with the timestamp
   by  touch functions to see how fast buttons are changing
*/

struct sample_t sample[NUMSAMPLES] = {{0, 0, {0 ,0 ,0 ,0}}};

/* a MAXSAMPLE is kept to use for testing if a button is pressed, it is sample NUMSAMPLE-1 */
# define MAXSAMPLE (NUMSAMPLES-1)

/* new way using MAXSAMPLE to calculate */
unsigned short int sampleButtonStatus = 0; // Bit field of buttons that are pressed

/* 9 -> because 4 touch buttons and physical button and 4 slide directions]*/
unsigned int buttonTimestamp[9] = {0}; /* timestamp of when a button last changed state */
unsigned int lastHandledTimestamp[9] = {0}; /* timestampe of when a button was last handled */

short int G_buttonDetectValue=0x800; // cap touch detect value. old way was 32767, the midpoint between  0..65535

// void (*menu_escape_cb)(unsigned char, int);
void (*menu_escape_cb)(void) = NULL;

/*
  interrupt CTMU state
*/
enum {
      TOUCH_INIT=0,
      TOUCH_IDLE,
      TOUCH_SAMPLE_START,
      TOUCH_GROUNDCHARGEPUMP_START,
      TOUCH_GROUNDCHARGEPUMP_STOP,
      TOUCH_CHARGECIRCUIT_START,
      TOUCH_ATOD_START,
      TOUCH_CHARGECIRCUIT_STOP,
      TOUCH_ATOD_STOP,
      TOUCH_ATOD_SAMPLE,
      TOUCH_ATOD_MATH,
      TOUCH_ATOD_NEXTBUTTON,
      TOUCH_STORE_SAMPLES,
      TOUCH_COMPUTE_BUTTONS
} ;

unsigned char touchState = TOUCH_INIT;

void initTouch()
{
    touchState = TOUCH_INIT;
    // touchInterrupt(); // PEB: don't force it, as this may overlap a real interrupt happening
}

unsigned char G_button=0; // physical button on/off
unsigned char G_buttonDebounce=0; // button debounce period
unsigned char G_buttonCnt=0; // physical button on period

/*
   This code is run from timer3 interrupt in timer1_int.c
   It is an unrolled / statefull version of getTouch()
*/
void touchInterrupt()
{
    static unsigned long int ADC_Sum; // For averaging multiple ADC measurements, persistant
    static unsigned short int iAvg; // Averaging index persistant

    unsigned short int //Naverages = 32, // Number of averages < 2^22 (22=32-10 bits of ADC)
                       //Log2Naverages = 5; // Right shift equal to 1/Naverages
                       Naverages = 16, // Number of averages < 2^22 (22=32-10 bits of ADC)
                       Log2Naverages = 4; // Right shift equal to 1/Naverages

    static short int iButton=0; // Button Index, persistent across calls

    static unsigned short int VmeasADC, VavgADC; // Measured Voltages, 65536 = Full Scale

    static unsigned int tmpCurrentButtonStatus=0; // temp version of Bit field of buttons that are pressed. not updated till all are ready
    static unsigned int timestamp=0;

    timestamp++; // 1/120 sec. this will wrap around in 414 days = ((((pow(2, 32))/ 120) / 3600) / 24)
    currentstamp = timestamp;

    /* See if button is pushed and debounce-  Bit 3 of port C */

#define NEWDEBOUNCE
#ifdef NEWDEBOUNCE
    if (PORTCbits.RC3 == 0)  /* active low, debounce reset as long as its on */
	G_buttonDebounce = 1;
    else
        G_buttonCnt = 0;

    if (G_buttonDebounce != 0) { /* debounce in progress */
	G_buttonDebounce++;
	G_buttonCnt++;

	G_button = 1;

	/* turn off button after phy button off and quiet period */
	if (G_buttonDebounce > 8) { /* button is pretty snappy-  8/120  = 1/15 second  */
		G_button = 0;
		G_buttonCnt = 0;
		G_buttonDebounce = 0;
	}

        if (G_buttonCnt == 255) { /* still being held after 255/120 = 2 1/8 sec */
           if (menu_escape_cb != NULL) menu_escape_cb();
           G_buttonCnt = 0;
        }
    }
#else
    if (!PORTCbits.RC3) {
        if (G_buttonCnt > 4) /* 4/120 = 1/30 sec. plenty of debounce */
        {
            G_button = 1;
        }

        if (G_buttonCnt == 255) {
           if (menu_escape_cb != NULL) menu_escape_cb();
           G_buttonCnt=0;
        }
        else
           G_buttonCnt++;
    }
    else {
        G_buttonCnt = 0;
        G_button = 0;
    }
#endif



    switch (touchState) {
        case TOUCH_INIT:
            /* setup analog pins */
            // PEB RA0/CTED1 RA1/CTED2 RB1/CTED12 RB2/CTED13
            // make them inputs
            TRISAbits.TRISA0 = 1;
            TRISAbits.TRISA1 = 1;
            TRISBbits.TRISB1 = 1;
            TRISBbits.TRISB2 = 1;
            ANSELA = (1<<0) | (1<<1); //RA0,1
            ANSELB = (1<<1) | (1<<2); //RB1,2
            DelayMs(1);

            // CTMU Setup
            CTMUCONbits.IRNG = 3;  // 1 = 0.55uA 2 = 5.5uA 3 = 55uA 0 = 550uA
            CTMUCONbits.ON = 1;    // Turn on CTMU
            DelayMs(1);            // Wait 1 msec

            // ADC Setup
            AD1CON2 = 0x0; // VR+ = AVDD, V- = AVSS, Don't scan, MUX A only

            // ADC clock derived from peripheral buss clock
            // Tadc = 4 * Tpbus = 4 * 25 ns = 100 ns > 65 ns required
            // Tadc = 2*(    1       +1)*Tpbus
            // Tadc = 2*(AD1CON3<7:0>+1)*Tpbus
            AD1CON3 = 1;    // PEB 16 * Tpb;
            AD1CSSL = 0x0;        // No channels scanned
            IEC0bits.AD1IE = 0; // Disable ADC interrupts
            AD1CON1bits.ON = 1; // Turn on ADC

            CurrentButtonStatus = 0;
            tmpCurrentButtonStatus = 0;

            sampleButtonStatus = 0;
            sample[MAXSAMPLE].ButtonVmeasADC[0] = 0;
            sample[MAXSAMPLE].ButtonVmeasADC[1] = 0;
            sample[MAXSAMPLE].ButtonVmeasADC[2] = 0;
            sample[MAXSAMPLE].ButtonVmeasADC[3] = 0;
            sample[MAXSAMPLE].timestamp = 0;
            sample[MAXSAMPLE].buttonStatus = 0;

	    buttonTimestamp[LEFT_SLIDER] = 0;
	    buttonTimestamp[RIGHT_SLIDER] = 0;
	    buttonTimestamp[TOP_SLIDER] = 0;
	    buttonTimestamp[BOTTOM_SLIDER] = 0;
	    buttonTimestamp[BUTTON] = 0;

            touchState++;
            break;

        case TOUCH_IDLE:
            //for ( iButton = 0; iButton < NUM_DIRECT_KEYS; iButton++ )
            //{
            /* RA0 = AN0, RA1 = AN1, RB1 = AN3, RB2 = AN4 */
            AD1CHSbits.CH0SA = ButtonADCChannels[iButton];
    //        for ( iAvg = 0; iAvg < Naverages; iAvg++ )
    //        {
            touchState++;
            break;

        case TOUCH_SAMPLE_START:
            AD1CON1bits.SAMP = 1;     // Manual sampling start
            touchState++;
            break;

        case TOUCH_GROUNDCHARGEPUMP_START:
            CTMUCONbits.IDISSEN = 1;  // Ground charge pump
            touchState++;
            break;

        case TOUCH_GROUNDCHARGEPUMP_STOP:
            CTMUCONbits.IDISSEN = 0;  // End drain of circuit
            touchState++;
            break;

        case TOUCH_CHARGECIRCUIT_START:
//          switch (iButton)
//          {
//                /* demo cases have calibrated waits hard wired */
//                case 0:
//                case 1:
//                case 2:
//                case 3:
            CTMUCONbits.EDG1STAT = 1; // Begin charging the circuit
            switch (iButton) {
                unsigned char nops;

                case LEFT_SLIDER:
                    for (nops=0; nops<Nnops+1; nops++);
                    break;

                case RIGHT_SLIDER:
                    for (nops=0; nops<Nnops+8; nops++);
                    break;

                case TOP_SLIDER:
                    for (nops=0; nops<Nnops; nops++);
                    break;

                case BOTTOM_SLIDER:
                    for (nops=0; nops<Nnops; nops++);
                    break;
            }
            touchState++;
            // break; // fall though interrupt takes to long
        case TOUCH_ATOD_START:
            AD1CON1bits.SAMP = 0;     // Begin analog-to-digital conversion
            touchState++;
            // break; // fall through, interrupt takes to long
        case TOUCH_CHARGECIRCUIT_STOP:
            CTMUCONbits.EDG1STAT = 0; // Stop charging circuit
            touchState++;
            break;

        case TOUCH_ATOD_STOP:       // wait for AtoD to finish. This could take several interrupts
            if (AD1CON1bits.DONE) touchState++;
            break;

        case TOUCH_ATOD_SAMPLE:
            AD1CON1bits.DONE = 0; // ADC conversion done, clear flag
            VmeasADC = ADC1BUF0;  // Get the value from the ADC
            ADC_Sum += VmeasADC;  // Update averaging sum

            iAvg++;
            if (iAvg < Naverages) {
                touchState = TOUCH_SAMPLE_START; /* back to sample another */
            }
            else {
                iAvg = 0;
                touchState++;
            }
            break;

        case TOUCH_ATOD_MATH:
            if ( Log2Naverages-6 > 0 ) {
                VavgADC = ADC_Sum >> (Log2Naverages-6); // Full scale = 2^10<<6 = 65536
            }
            else {
                VavgADC = ADC_Sum << (6-Log2Naverages); // Full scale = 2^10<<6 = 65536
            }

           if (VavgADC < tripValue[iButton])
            {
                tmpCurrentButtonStatus |= (1 << iButton);
            }
            
            ButtonVmeasADC[iButton] = VavgADC;
            ADC_Sum = 0;

            touchState++;
            break;

        case TOUCH_ATOD_NEXTBUTTON:
            iButton++;
            if (iButton < NUM_DIRECT_KEYS) {
                touchState = TOUCH_IDLE; /* next button */
            }
            else {
                iButton = 0; /* start over */
                touchState++;
            }
            break;

        case TOUCH_STORE_SAMPLES:
            /* store MAX values in MAXSAMPLE */
            if (sample[MAXSAMPLE].ButtonVmeasADC[0] < ButtonVmeasADC[0])    sample[MAXSAMPLE].ButtonVmeasADC[0] = ButtonVmeasADC[0];
            if (sample[MAXSAMPLE].ButtonVmeasADC[1] < ButtonVmeasADC[1])    sample[MAXSAMPLE].ButtonVmeasADC[1] = ButtonVmeasADC[1];
            if (sample[MAXSAMPLE].ButtonVmeasADC[2] < ButtonVmeasADC[2])    sample[MAXSAMPLE].ButtonVmeasADC[2] = ButtonVmeasADC[2];
            if (sample[MAXSAMPLE].ButtonVmeasADC[3] < ButtonVmeasADC[3])    sample[MAXSAMPLE].ButtonVmeasADC[3] = ButtonVmeasADC[3];
            sample[MAXSAMPLE].timestamp =         sample[1].timestamp;
            sample[MAXSAMPLE].buttonStatus =      sample[1].buttonStatus;

            sample[1].timestamp =         sample[0].timestamp;
            sample[1].buttonStatus =      sample[0].buttonStatus;
            sample[1].ButtonVmeasADC[0] = sample[0].ButtonVmeasADC[0];
            sample[1].ButtonVmeasADC[1] = sample[0].ButtonVmeasADC[1];
            sample[1].ButtonVmeasADC[2] = sample[0].ButtonVmeasADC[2];
            sample[1].ButtonVmeasADC[3] = sample[0].ButtonVmeasADC[3];

            /* copy to output status */
            sample[0].timestamp = timestamp;
            sample[0].buttonStatus = tmpCurrentButtonStatus;
            sample[0].ButtonVmeasADC[0] = ButtonVmeasADC[0];
            sample[0].ButtonVmeasADC[1] = ButtonVmeasADC[1];
            sample[0].ButtonVmeasADC[2] = ButtonVmeasADC[2];
            sample[0].ButtonVmeasADC[3] = ButtonVmeasADC[3];

            //Calcuate new slow averages if the  counter has reached the delay
            if (slowAverageCount == SLOW_AVERAGE_DELAY)
            {
                //if the slider isn't pressed
                if (ButtonVmeasADC[0] > tripValue[0])
                {
                    //Trust the new high value
                    if (ButtonVmeasADC[0] > slowAverage[0]) {
                        slowAverage[0] = ButtonVmeasADC[0];
                    }
                    else {
                        //otherwise, use a cheap moving average
                        slowAverage[0] = slowAverage[0]
                                + ((ButtonVmeasADC[0] >> 4)
                                - (slowAverage[0] >> 4));
                    }
                    //A change of 1/16th indicates a press
                    tripValue[0] = slowAverage[0] - (slowAverage[0] >> 4);
                }
                //if the slider isn't pressed
                if (ButtonVmeasADC[1] > tripValue[1])
                {
                    //Trust the new high value
                    if (ButtonVmeasADC[1] > slowAverage[1]) {
                        slowAverage[1] = ButtonVmeasADC[1];
                    }
                    else {
                        //otherwise, use a cheap moving average
                        slowAverage[1] = slowAverage[1]
                                + ((ButtonVmeasADC[1] >> 4)
                                - (slowAverage[1] >> 4));
                    }
                    //A change of 1/16th indicates a press
                    tripValue[1] = slowAverage[1] - (slowAverage[1] >> 4);
                }
                //if the slider isn't pressed
                if (ButtonVmeasADC[2] > tripValue[2])
                {
                    //Trust the new high value
                    if (ButtonVmeasADC[2] > slowAverage[2]) {
                        slowAverage[2] = ButtonVmeasADC[2];
                    }
                    else {
                        //otherwise, use a cheap moving average
                        slowAverage[2] = slowAverage[2]
                                + ((ButtonVmeasADC[2] >> 4)
                                - (slowAverage[2] >> 4));
                    }
                    //A change of 1/16th indicates a press
                    tripValue[2] = slowAverage[2] - (slowAverage[2] >> 4);
                }
                //if the slider isn't pressed
                if (ButtonVmeasADC[3] > tripValue[3])
                {
                    //Trust the new high value
                    if (ButtonVmeasADC[3] > slowAverage[3]) {
                        slowAverage[3] = ButtonVmeasADC[3];
                    }
                    else {
                        //otherwise, use a cheap moving average
                        slowAverage[3] = slowAverage[3]
                                + ((ButtonVmeasADC[3] >> 4)
                                - (slowAverage[3] >> 4));
                    }
                    //A change of 1/16th indicates a press
                    tripValue[3] = slowAverage[3] - (slowAverage[3] >> 4);
                }
                slowAverageCount = 0;
            }
            slowAverageCount++;
            
            touchState++;
            break;


        case TOUCH_COMPUTE_BUTTONS:
            
            /* new MAXSAMPLED button values */
            if ((sample[0].ButtonVmeasADC[LEFT_SLIDER] < tripValue[LEFT_SLIDER])
                && ((sample[0].ButtonVmeasADC[LEFT_SLIDER] + 4000) < sample[0].ButtonVmeasADC[RIGHT_SLIDER]))  //HACK: [ML] Prevent left and right
            {
                if (!isHorizontalSliding){
                    slideStart = SLIDE_START_LEFT;
                    isHorizontalSliding = true;
                    startTouch = timestamp;
                }

		/* check if already on */
		if ((buttonTimestamp[LEFT_SLIDER] == 0) && !(sampleButtonStatus & LEFT_SLIDER_MASK)) {
			buttonTimestamp[LEFT_SLIDER] = touchstamp = timestamp; /* apps can tell if start/stop by checking timestamp */
			sampleButtonStatus |= LEFT_SLIDER_MASK;
		}
	    } else {
		buttonTimestamp[LEFT_SLIDER] = 0;
		sampleButtonStatus &= (0xFFFF ^ LEFT_SLIDER_MASK); /* mask off slider bit */
	    }

            if ((sample[0].ButtonVmeasADC[RIGHT_SLIDER] < tripValue[RIGHT_SLIDER])
                && ((sample[0].ButtonVmeasADC[RIGHT_SLIDER] + 4000) < sample[0].ButtonVmeasADC[LEFT_SLIDER]))  //HACK: [ML] Prevent left and right
            {
                if (!isHorizontalSliding){
                    slideStart = SLIDE_START_RIGHT;
                    isHorizontalSliding = true;
                    startTouch = timestamp;
                }

		/* check if already on, otherwise ignore and DONT update timestamp  */
		if ((buttonTimestamp[RIGHT_SLIDER] == 0) && !(sampleButtonStatus & RIGHT_SLIDER_MASK)) {
			buttonTimestamp[RIGHT_SLIDER] = touchstamp = timestamp; /* apps can tell if start/stop by checking timestamp */
			sampleButtonStatus |= RIGHT_SLIDER_MASK;
		}
	    } else {
		buttonTimestamp[RIGHT_SLIDER] = 0;
		sampleButtonStatus &= (0xFFFF ^ RIGHT_SLIDER_MASK); /* mask off slider bit */
	    }

            if ((sample[0].ButtonVmeasADC[TOP_SLIDER] < tripValue[TOP_SLIDER])
                && ((sample[0].ButtonVmeasADC[TOP_SLIDER] + 4000) < sample[0].ButtonVmeasADC[BOTTOM_SLIDER]))   //HACK: [ML] Prevent top and bottom
            {
                if (!isVerticalSliding){
                    slideStart = SLIDE_START_TOP;
                    isVerticalSliding = true;
                    startTouch = timestamp;
                }
		/* check if already on, otherwise ignore and DONT update timestamp  */
		if ((buttonTimestamp[TOP_SLIDER] == 0) && !(sampleButtonStatus & TOP_SLIDER_MASK)) {
			buttonTimestamp[TOP_SLIDER] = touchstamp = timestamp; /* apps can tell if start/stop by checking timestamp */
			sampleButtonStatus |= TOP_SLIDER_MASK;
		}
	    } else {
		buttonTimestamp[TOP_SLIDER] = 0;
		sampleButtonStatus &= (0xFFFF ^ TOP_SLIDER_MASK); /* mask off slider bit */
	    }

            if ((sample[0].ButtonVmeasADC[BOTTOM_SLIDER] < tripValue[BOTTOM_SLIDER])
                && ((sample[0].ButtonVmeasADC[BOTTOM_SLIDER] + 4000) < sample[0].ButtonVmeasADC[TOP_SLIDER] ))   //HACK: [ML] Prevent top and bottom
            {
                if (!isVerticalSliding){
                    slideStart = SLIDE_START_BOTTOM;
                    isVerticalSliding = true;
                    startTouch = timestamp;
                }
		/* check if already on, otherwise ignore and DONT update timestamp  */
		if ((buttonTimestamp[BOTTOM_SLIDER] == 0) && !(sampleButtonStatus & BOTTOM_SLIDER_MASK)) {
			buttonTimestamp[BOTTOM_SLIDER] = touchstamp = timestamp; /* apps can tell if start/stop by checking timestamp */
			sampleButtonStatus |= BOTTOM_SLIDER_MASK;
		}
	    } else {
		buttonTimestamp[BOTTOM_SLIDER] = 0;
		sampleButtonStatus &= (0xFFFF ^ BOTTOM_SLIDER_MASK); /* mask off slider bit */
	    }

            if (isVerticalSliding)
            {
                if ((sample[0].ButtonVmeasADC[TOP_SLIDER] < tripValue[TOP_SLIDER])
                    || (sample[0].ButtonVmeasADC[BOTTOM_SLIDER] < tripValue[BOTTOM_SLIDER]))
                {
                    //Still sliding
                }
                else
                {
                    endTouch = timestamp;
                    duration = endTouch - startTouch;
                    
                    if ((slideStart == SLIDE_START_TOP) && (duration > 1100))
                    {
                        buttonTimestamp[SLIDE_DOWN] = timestamp;
                        sampleButtonStatus |= SLIDE_DOWN_MASK; /* mask off slider bit */
                        sampleButtonStatus &= (0xFFFF ^ SLIDE_UP_MASK);
                    }
                    else if ((slideStart == SLIDE_START_BOTTOM) && (duration > 1100))
                    {
                        buttonTimestamp[SLIDE_UP] = timestamp;
                        sampleButtonStatus |= SLIDE_UP_MASK; /* mask off slider bit */
                        sampleButtonStatus &= (0xFFFF ^ SLIDE_DOWN_MASK);
                    }
                    else
                    {
                        //Sliding was canceled
                    }
                    isVerticalSliding = false;
                }
            }
            
            if (isHorizontalSliding)
            {

                if ((sample[0].ButtonVmeasADC[LEFT_SLIDER] < tripValue[LEFT_SLIDER])
                    || (sample[0].ButtonVmeasADC[RIGHT_SLIDER] < tripValue[RIGHT_SLIDER]))
                {
                    //Still sliding
                }
                else
                {                    
                    endTouch = timestamp;
                    duration = endTouch - startTouch;
                    if ((slideStart == SLIDE_START_LEFT) && (duration > 1100))
                    {
                        
                        buttonTimestamp[SLIDE_RIGHT] = timestamp;
                        sampleButtonStatus |= SLIDE_RIGHT_MASK; /* mask off slider bit */
                        sampleButtonStatus &= (0xFFFF ^ SLIDE_LEFT_MASK);
                    }
                    else  if((slideStart == SLIDE_START_RIGHT) && (duration > 1100))
                    {
                        buttonTimestamp[SLIDE_LEFT] = timestamp;
                        sampleButtonStatus |= SLIDE_LEFT_MASK; /* mask off slider bit */
                        sampleButtonStatus &= (0xFFFF ^ SLIDE_RIGHT_MASK);
                    }
                    else
                    {
                        //Sliding was canceled
                    }
                    isHorizontalSliding = false;
                }
            }

            if (G_button == 1)  {
		if ((buttonTimestamp[BUTTON] == 0) && !(sampleButtonStatus & BUTTON_MASK)) {
			buttonTimestamp[BUTTON] = touchstamp = timestamp; /* apps can tell if start/stop by checking timestamp */
			sampleButtonStatus |= BUTTON_MASK;
		}
	    } else {
		buttonTimestamp[BUTTON] = 0;
		sampleButtonStatus &= (0xFFFF ^ BUTTON_MASK); /* mask off button bit */
	    }

            /* old way */
            CurrentButtonStatus = tmpCurrentButtonStatus;
            if (G_button == 1)
                CurrentButtonStatus |= (1 <<4);

            tmpCurrentButtonStatus = 0;
            touchState++;
            break;

        default:
            touchState = TOUCH_IDLE;
            break;
    }
}

unsigned char _getYPos()
{
    unsigned int deltaTop, deltaBottom;
    unsigned int topPosition, bottomPosition;
    unsigned char position;

    if ((sample[0].ButtonVmeasADC[TOP_SLIDER] < (tripValue[TOP_SLIDER]))
            || (sample[0].ButtonVmeasADC[BOTTOM_SLIDER] < (tripValue[BOTTOM_SLIDER])))
    {
        deltaTop = slowAverage[TOP_SLIDER] - sample[0].ButtonVmeasADC[TOP_SLIDER];
        deltaBottom = slowAverage[BOTTOM_SLIDER] - sample[0].ButtonVmeasADC[BOTTOM_SLIDER];
        topPosition = (deltaTop * 100)/(deltaTop + deltaBottom);
        bottomPosition = (deltaBottom * 100)/(deltaTop + deltaBottom);
        position = (unsigned char)topPosition;
        if (position == 0) position = 1;
    }
    else
    {
        position = 0;
    }
    return position;
}

unsigned char _getXPos()
{

    unsigned int deltaLeft, deltaRight;
    unsigned int leftPosition, rightPosition;
    unsigned char position;

    if ((sample[0].ButtonVmeasADC[LEFT_SLIDER] < (tripValue[LEFT_SLIDER]))
            || (sample[0].ButtonVmeasADC[RIGHT_SLIDER] < (tripValue[RIGHT_SLIDER])))
    {
        deltaLeft = slowAverage[LEFT_SLIDER] - sample[0].ButtonVmeasADC[LEFT_SLIDER];
        deltaRight = slowAverage[RIGHT_SLIDER] - sample[0].ButtonVmeasADC[RIGHT_SLIDER];
        leftPosition = (deltaLeft * 100)/(deltaLeft + deltaRight);
        rightPosition = (deltaRight * 100)/(deltaRight + deltaLeft);
        position = (unsigned char)rightPosition;
        if (position == 0) position = 1;
    }
    else
    {
        position = 0;
    }
    return position;
}

unsigned char getHorizontalPosition()
{
    if (PORTRAIT_MODE)
        return _getXPos();
    else
        return _getYPos();
}

unsigned char getVerticalPosition()
{
    if (PORTRAIT_MODE)
        return _getYPos();
    else
        return _getXPos();
}

bool IsSlideUp()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & SLIDE_UP_MASK) &&
            (buttonTimestamp[SLIDE_UP] != lastHandledTimestamp[SLIDE_UP]) &&
            (lastHandledTimestamp[SLIDE_UP] = buttonTimestamp[SLIDE_UP]));
    }
    else
    {
           return ((sampleButtonStatus & SLIDE_LEFT_MASK) &&
            (buttonTimestamp[SLIDE_LEFT] != lastHandledTimestamp[SLIDE_LEFT]) &&
            (lastHandledTimestamp[SLIDE_LEFT] = buttonTimestamp[SLIDE_LEFT]));
    }
}

bool IsSlideDown()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & SLIDE_DOWN_MASK) &&
            (buttonTimestamp[SLIDE_DOWN] != lastHandledTimestamp[SLIDE_DOWN]) &&
            (lastHandledTimestamp[SLIDE_DOWN] = buttonTimestamp[SLIDE_DOWN]));
    }
    else
    {
        return ((sampleButtonStatus & SLIDE_RIGHT_MASK) &&
            (buttonTimestamp[SLIDE_RIGHT] != lastHandledTimestamp[SLIDE_RIGHT]) &&
            (lastHandledTimestamp[SLIDE_RIGHT] = buttonTimestamp[SLIDE_RIGHT]));
    }
}

bool IsSlideLeft()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & SLIDE_LEFT_MASK) &&
            (buttonTimestamp[SLIDE_LEFT] != lastHandledTimestamp[SLIDE_LEFT]) &&
            (lastHandledTimestamp[SLIDE_LEFT] = buttonTimestamp[SLIDE_LEFT]));
    }
    else
    {
        return ((sampleButtonStatus & SLIDE_DOWN_MASK) &&
            (buttonTimestamp[SLIDE_DOWN] != lastHandledTimestamp[SLIDE_DOWN]) &&
            (lastHandledTimestamp[SLIDE_DOWN] = buttonTimestamp[SLIDE_DOWN]));
    }
}

bool IsSlideRight()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & SLIDE_RIGHT_MASK) &&
            (buttonTimestamp[SLIDE_RIGHT] != lastHandledTimestamp[SLIDE_RIGHT]) &&
            (lastHandledTimestamp[SLIDE_RIGHT] = buttonTimestamp[SLIDE_RIGHT]));
    }
    else
    {
        return ((sampleButtonStatus & SLIDE_UP_MASK) &&
            (buttonTimestamp[SLIDE_UP] != lastHandledTimestamp[SLIDE_UP]) &&
            (lastHandledTimestamp[SLIDE_UP] = buttonTimestamp[SLIDE_UP]));
    }
}

bool IsTapUp()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & TOP_SLIDER_MASK) &&
            (buttonTimestamp[TOP_SLIDER] != lastHandledTimestamp[TOP_SLIDER]) &&
            (lastHandledTimestamp[TOP_SLIDER] = buttonTimestamp[TOP_SLIDER]));
    }
    else
    {
        return ((sampleButtonStatus & LEFT_SLIDER_MASK) &&
            (buttonTimestamp[LEFT_SLIDER] != lastHandledTimestamp[LEFT_SLIDER]) &&
            (lastHandledTimestamp[LEFT_SLIDER] = buttonTimestamp[LEFT_SLIDER]));
    }
}

bool IsTapDown()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & BOTTOM_SLIDER_MASK) &&
            (buttonTimestamp[BOTTOM_SLIDER] != lastHandledTimestamp[BOTTOM_SLIDER]) &&
            (lastHandledTimestamp[BOTTOM_SLIDER] = buttonTimestamp[BOTTOM_SLIDER]));
    }
    else
    {
        return ((sampleButtonStatus & RIGHT_SLIDER_MASK) &&
            (buttonTimestamp[RIGHT_SLIDER] != lastHandledTimestamp[RIGHT_SLIDER]) &&
            (lastHandledTimestamp[RIGHT_SLIDER] = buttonTimestamp[RIGHT_SLIDER]));
    }
}

bool IsTapLeft()
{
    if (PORTRAIT_MODE)
    {
        return ((sampleButtonStatus & LEFT_SLIDER_MASK) &&
            (buttonTimestamp[LEFT_SLIDER] != lastHandledTimestamp[LEFT_SLIDER]) &&
            (lastHandledTimestamp[LEFT_SLIDER] = buttonTimestamp[LEFT_SLIDER]));
    }
    else
    {

        return ((sampleButtonStatus & BOTTOM_SLIDER_MASK) &&
        (buttonTimestamp[BOTTOM_SLIDER] != lastHandledTimestamp[BOTTOM_SLIDER]) &&
        (lastHandledTimestamp[BOTTOM_SLIDER] = buttonTimestamp[BOTTOM_SLIDER]));
    }
}

bool IsTapRight()
{
    if (PORTRAIT_MODE)
    {
           return ((sampleButtonStatus & RIGHT_SLIDER_MASK) &&
            (buttonTimestamp[RIGHT_SLIDER] != lastHandledTimestamp[RIGHT_SLIDER]) &&
            (lastHandledTimestamp[RIGHT_SLIDER] = buttonTimestamp[RIGHT_SLIDER]));
    }
    else
    {
        return ((sampleButtonStatus & TOP_SLIDER_MASK) &&
            (buttonTimestamp[TOP_SLIDER] != lastHandledTimestamp[TOP_SLIDER]) &&
            (lastHandledTimestamp[TOP_SLIDER] = buttonTimestamp[TOP_SLIDER]));
    }
}