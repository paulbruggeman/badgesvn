#include "plib.h"
#include "badge16.h"
#include "flash.h"
#include "colors.h"
#include "menu.h"
#include "assetList.h"
#include "ir.h"
#include "debug.h"
#include "flash.h"

#define FORTH
#ifdef FORTH
extern void resume();
extern char emit_buffer;
extern char key_buffer;
// forth key buffer on c side
#define MAX_USBKEYBUFFER 64
unsigned char usbkey_buffer[MAX_USBKEYBUFFER] ;
unsigned char usbkey_ptr=0 ;
unsigned char usbkey_fillptr=0 ;
#endif

/*
   will be save/restored from flash
*/
const unsigned short flashedBadgeId=0xefbe; /* MAGIC value for script. It will be replaced by awk script in final flashing */
struct sysData_t G_sysData;
unsigned short G_peerBadgeId; /* who we are talking to */
/*
  main is actually in microchip.c
  however, this file contains the main code:

  UserInit()
  ProcessIO()

*/

unsigned int USBbufferSizeIn();
unsigned int USBbufferSizeOut();
volatile int mchipUSBnotReady();
volatile unsigned char USBtransferReady();

extern char USB_In_Buffer[];
extern char USB_Out_Buffer[];

unsigned char  NextUSBOut=0;

void UserInit(void)
{
#ifdef FORTH
	emit_buffer = -1;
	key_buffer = -1;
#endif

    CFGCONbits.JTAGEN = 0;

    /* RGB LED */
    TRISCbits.TRISC0 = 0;    /* output */
    LATCbits.LATC0 = 0;      /* red init low */
    CNPDCbits.CNPDC0 = 0;    /* pulldown == off */


    TRISCbits.TRISC1 = 0;    /* output */
    LATCbits.LATC1 = 0;      /* blue init low */
    CNPDCbits.CNPDC1 = 0;    /* pulldown == off */

    TRISBbits.TRISB3 = 0;    /* output */
    LATBbits.LATB3 = 0;      /* green init low */
    CNPDBbits.CNPDB3 = 0;    /* pulldown == off */

    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    TRISA = 0;
    LATA = 0;
    TRISB = 0;
    LATB = 0;
    TRISC = 0;
    LATC = 0;

    LCDInitPins();
    LATCbits.LATC0 = 1;      /* RED */

    LCDReset();
    LATCbits.LATC1 = 1;      /* BLUE */

    FbInit();
  
    initTouch();
    
    LATBbits.LATB3 = 1;      /* GREEN */
    LATCbits.LATC9 = 1;      /* backlight on. you will see nothing if it is off */
       
    //unused (was Ghost Game Pin (left most programming pin))
    //LATAbits.LATA7 = 0;
    
    /* button init */
    TRISCbits.TRISC3 = 1; // button == input
    CNPUCbits.CNPUC3 = 1; // pullup == on

    /* dev board with hot wired RC4 */
    TRISCbits.TRISC4 = 0;
    CNPUCbits.CNPUC4 = 0;
    CNPDCbits.CNPDC4 = 1;

    /* speaker pull down init */
    TRISAbits.TRISA9 = 0;	// piezo == output
    LATAbits.LATA9 = 0;      // piezo init off
    CNPUAbits.CNPUA9 = 0;    // RA9  pull up == off
    CNPDAbits.CNPDA9 = 0;    /* pulldown == off */

    timerInit();
    setupRTCC();

    sysDataRead(&G_sysData);
    G_sysData.badgeId = flashedBadgeId; /* in case a badge badgeId is stored in sysData */

    G_peerBadgeId = 0;
    G_sysData.backlight = 255;

    menus(); /* this has the effect of clearing the fb before USB is done initing */
}

// used in touchCTMU, charge time
extern unsigned char Nnops;

// used in touchCTMU, button value
extern unsigned short int CurrentButtonStatus;

// array of 4 touch slides sample averages from touchCTMU
extern unsigned short int ButtonVmeasADC[];

const char hextab[]={"0123456789ABCDEF"};

// controls USB heartbeat blink
static unsigned char debugBlink=1;

void ProcessIO(void)
{
    unsigned char nread=0;
    int i;

    //Blink the LEDs according to the USB device status
    //very handy if you lock up when trying to run off of battery
    BlinkUSBStatus();

    menus();
    FbSwapBuffers();
    IRhandler(); /* do any pending IR callbacks */

    if (mchipUSBnotReady()) return;

    nread = getsUSBUSART(USB_In_Buffer, 64);
    if(nread > 0) {
	// if first char == ! then call Forth system. currently never returns
	if (USB_In_Buffer[0] == '!') {
		USB_In_Buffer[0] = 0;
		resume();
	}

	if ((USB_In_Buffer[0] == 'p') || (USB_In_Buffer[0] == 'P')) {
		void clearscreen(unsigned short color);
		if (USB_In_Buffer[0] == 'p') {
		   static unsigned char y=0;

		   FbClear();
		   FbBackgroundColor(BLACK);
		   //printchar(' ', 115,63,BLACK);
		   FbMove(10,  20);
		   FbColor(WHITE);
		   FbWriteLine((unsigned char *)"0123456789");
		   y += 8;
		   if (y>128) y = 0;
		}

		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == 'b') || (USB_In_Buffer[0] == 'B')) {
		static unsigned char bright = 255;

		if (USB_In_Buffer[0] == 'b') bright += 10; // let it wrap
		if (USB_In_Buffer[0] == 'B') bright -= 10; // let it wrap

		backlight(bright);

		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == 'N') || (USB_In_Buffer[0] == 'n')) {
		static unsigned char imgno = 0;
		unsigned char tmpstr[32];

		if (USB_In_Buffer[0] == 'n') {
		    imgno++;			// see assetList.c for image Nums
		    if (imgno > LASTASSET) imgno--;
		} else {
		    if (USB_In_Buffer[0] == 'N')
		        if (imgno != 0) imgno--;
		}
		DEBUGSTR("IMG ");
		sprintf(tmpstr, "%04X\r\n", imgno);
		DEBUGSTR(tmpstr );

		FbImage(imgno,0); 
		
		USB_In_Buffer[0] = 0;
	}
			 
	if ((USB_In_Buffer[0] == 13) || (USB_In_Buffer[0] == 10)) {
		unsigned char tmpstr[32];

		DEBUGSTR("MENU R ");
		sprintf(tmpstr, "%08X", (unsigned long)runningApp);
		DEBUGSTR(tmpstr);

		DEBUGSTR(" S ");
		sprintf(tmpstr, "%08X", (unsigned long)getSelectedMenu());
		DEBUGSTR(tmpstr);

		DEBUGSTR(" C ");
		sprintf(tmpstr, "%08X\r\n", (unsigned long)getCurrMenu());
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	// keyboard audio
	if ( (USB_In_Buffer[0] == 'q') 
	|| (USB_In_Buffer[0] == 'w')
	|| (USB_In_Buffer[0] == 'e')
	|| (USB_In_Buffer[0] == 'r')
	|| (USB_In_Buffer[0] == 't')
	|| (USB_In_Buffer[0] == 'y')
	|| (USB_In_Buffer[0] == 'u')
	|| (USB_In_Buffer[0] == 'i')
	|| (USB_In_Buffer[0] == '<')
	|| (USB_In_Buffer[0] == '>') ) {
		static unsigned short freq=0;
		static unsigned short dur=0;
		unsigned char tmpstr[32];

		if (USB_In_Buffer[0] == '<') dur -= 256;
		if (USB_In_Buffer[0] == '>') dur += 256;
		if (USB_In_Buffer[0] == 'q') freq = 512;
		if (USB_In_Buffer[0] == 'w') freq = 256;
		if (USB_In_Buffer[0] == 'e') freq = 128;
		if (USB_In_Buffer[0] == 'r') freq = 64;
		if (USB_In_Buffer[0] == 't') freq = 32;
		if (USB_In_Buffer[0] == 'y') freq = 16;
		if (USB_In_Buffer[0] == 'u') freq = 8;
		if (USB_In_Buffer[0] == 'i') freq = 4;

		if (1) { /* should be a CTF FLAG? */
		    void IRqueueSend(union IRpacket_u pkt) ;
		    union IRpacket_u pkt;

		    pkt.p.command = IR_WRITE;
		    pkt.p.address = IR_LIVEAUDIO;
		    pkt.p.badgeId = G_peerBadgeId; /* all */
		    pkt.p.data = (((dur >> 8) & 0xF) << 12) | (freq & 0x0FFF) ;

		    IRqueueSend(pkt);
	            //G_IRsendVal = pkt.v;
	            //G_IRsend = 1;
		}
		setNote(freq, dur);

		DEBUGSTR("FREQ ");
		sprintf(tmpstr, "%04X", (unsigned int)freq);
		DEBUGSTR(tmpstr);


		DEBUGSTR("DUR ");
		sprintf(tmpstr, "%04X\r\n", (unsigned int)dur);
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '0') {
		debugBlink = !debugBlink;
		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == 'a') || (USB_In_Buffer[0] == 'A')) {
		static unsigned int r=0;

		if (USB_In_Buffer[0] == 'A') r--;
		if (USB_In_Buffer[0] == 'a') r++;
		red(r);

		if (1) { /* should be a CTF FLAG? */
		    extern unsigned char G_red_pwm, G_green_pwm, G_blue_pwm;
		    void IRqueueSend(union IRpacket_u pkt) ;
		    union IRpacket_u pkt;

		    pkt.p.command = IR_WRITE;
		    pkt.p.address = IR_LED;
		    pkt.p.badgeId = G_peerBadgeId;
		    pkt.p.data = PACKRGB( G_red_pwm, G_green_pwm, G_blue_pwm);

		    IRqueueSend(pkt);
		}
		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == 's') || (USB_In_Buffer[0] == 'S')) {
		static unsigned int g=0;

		if (USB_In_Buffer[0] == 'S') g--;
		if (USB_In_Buffer[0] == 's') g++;
		green(g);

		if (1) { /* should be a CTF FLAG? */
		    extern unsigned char G_red_pwm, G_green_pwm, G_blue_pwm;
		    void IRqueueSend(union IRpacket_u pkt) ;
		    union IRpacket_u pkt;

		    pkt.p.command = IR_WRITE;
		    pkt.p.address = IR_LED;
		    pkt.p.badgeId = G_peerBadgeId;
		    pkt.p.data = PACKRGB( G_red_pwm, G_green_pwm, G_blue_pwm);

		    IRqueueSend(pkt);
		}

		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == 'd') || (USB_In_Buffer[0] == 'D')) {
		static unsigned int b=0;

		if (USB_In_Buffer[0] == 'D') b--;
		if (USB_In_Buffer[0] == 'd') b++;
		blue(b);

		if (1) { /* should be a CTF FLAG? */
		    extern unsigned char G_red_pwm, G_green_pwm, G_blue_pwm;
		    void IRqueueSend(union IRpacket_u pkt) ;
		    union IRpacket_u pkt;

		    pkt.p.command = IR_WRITE;
		    pkt.p.address = IR_LED;
		    pkt.p.badgeId = G_peerBadgeId; /* all */
		    pkt.p.data = PACKRGB( G_red_pwm, G_green_pwm, G_blue_pwm);

		    IRqueueSend(pkt);
		}
		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '\\') {
		void flashErasePage();

		flashErasePage();

		DEBUGSTR("erase\r\n");

		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == 'o') || (USB_In_Buffer[0] == 'O')) {
		unsigned char NVwrite(unsigned char appId, unsigned char dataId, unsigned char *data, unsigned char dataSize);
		unsigned char NVread(unsigned char appId, unsigned char dataId, unsigned char *data, unsigned char dataSize);
		unsigned char data[8];
		unsigned char cnt[2], i;

		if (USB_In_Buffer[0] == 'O') {
		    static unsigned short writeCnt=0, dataId, appId;

		    strcpy(data, "part ");
		    cnt[0] = 48 + writeCnt;
		    cnt[1] = 0;
		    strcat(data, cnt);

		    appId = 1; /* this would normally be the appId which is the menu position of the app */
		    dataId = writeCnt; /* this ID is used to retrieve this bit of data */
		    NVwrite(appId, dataId, data, 8);

		    DEBUGCHAR('W');
		    DEBUGCHAR(48+writeCnt);

		    writeCnt++;
		    writeCnt %= 8;
		}

		if (USB_In_Buffer[0] == 'o') {
		    static unsigned short readCnt=0, dataId, appId;

		    appId = 1; /* this would normally be the appId which is the menu position of the app */
		    dataId = readCnt; /* this ID is used to retrieve this bit of data */
		    NVread(appId, dataId, data, 8);

		    DEBUGCHAR('R');
		    DEBUGCHAR(48+readCnt);

		    readCnt++;
		    readCnt %= 8;
		}

		for (i=0; i<8; i++)
		   DEBUGCHAR(data[i]);

		DEBUGSTR("\r\n");

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '&') {
		strcpy(G_sysData.name, "mybadge");
		G_sysData.badgeId = flashedBadgeId; /* in case a badge badgeId is stored in sysData */
		strcpy(G_sysData.sekrits, "skrt1");
		strcpy(G_sysData.achievements, "ach8");
		G_sysData.ledBrightness = 4;/* shifted/divided by this */
		G_sysData.backlight = 200;/* 0-255 */

		DEBUGSTR("fwrite");
		sysDataWrite(&G_sysData);

		DEBUGSTR("\r\n");

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '*') {
		unsigned char i;

		G_sysData.name[0] = 0;
		G_sysData.badgeId = 0;
		G_sysData.sekrits[0] = 0;
		G_sysData.achievements[0] = 0;
		G_sysData.ledBrightness = 9;/* shifted/divided by this */
		G_sysData.backlight = 100;/* 0-255 */

		sysDataRead(&G_sysData);
		G_sysData.badgeId = flashedBadgeId; /* override */

		DEBUGCHAR('>');
		DEBUGSTR(G_sysData.name);
		DEBUGCHAR('<');
		DEBUGCHAR(' ');

		unsigned char tmpstr[32];
		sprintf(tmpstr, " BADGE:%0d ", (unsigned char)G_sysData.badgeId);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " SEK:%d ", (unsigned char)G_sysData.sekrits);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " ACH:%d ", (unsigned char)G_sysData.achievements);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " LED:%d ", (unsigned char)G_sysData.ledBrightness);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " BL:%d\r\n", (unsigned char)G_sysData.backlight);
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}


	if (USB_In_Buffer[0] == '{') {
	    void IRqueueSend(union IRpacket_u pkt) ;
	    union IRpacket_u pkt;
	    extern rtccDate G_date ;

	    pkt.p.command = IR_WRITE;
	    pkt.p.address = IR_DATEYYMM;
	    pkt.p.badgeId = G_peerBadgeId;
	    pkt.p.data = (((G_date.year & 0xFF ) << 8) | (G_date.mon & 0xFF));
	    IRqueueSend(pkt);

	    pkt.p.command = IR_WRITE;
	    pkt.p.address = IR_DATEDDAMPM;
	    pkt.p.badgeId = G_peerBadgeId;
	    pkt.p.data = (((G_date.wday & 0xFF ) << 8) | (G_date.mday & 0xFF));
	    IRqueueSend(pkt);

	    DEBUGSTR("DATETIME\r\n");
	    USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '}') {
	    void IRqueueSend(union IRpacket_u pkt) ;
	    union IRpacket_u pkt;
	    extern rtccTime G_time ;

//		    pkt.p.command = IR_WRITE;
//		    pkt.p.address = IR_DATEYYMM;
//		    pkt.p.badgeId = 0; /* all */
//		    pkt.p.data = (G_date.year << 4) | G_date.mon;
//		    IRqueueSend(pkt);

	    pkt.p.command = IR_WRITE;
	    pkt.p.address = IR_TIME;
	    pkt.p.badgeId = 0; /* all */
	    pkt.p.data = (((G_time.hour & 0xFF) << 8) | (G_time.min & 0xFF));

	    IRqueueSend(pkt);

	    DEBUGSTR("TIME\r\n");
	    USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '-') {
		void LCDReset(void);
		// in used in S6B33 samsung controller
		extern unsigned char G_contrast1;

		G_contrast1 -= 4;

		DEBUGCHAR(48 +  (unsigned char)G_contrast1 / 100);
		DEBUGCHAR(48 + ((unsigned char)G_contrast1 % 100) / 10);
		DEBUGCHAR(48 + ((unsigned char)G_contrast1 % 100) % 10);
	        DEBUGSTR("\r\n");

		LCDReset();

		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == '=') || (USB_In_Buffer[0] == '+')) {
		void LCDReset(void);
		// in used in S6B33 samsung controller
		extern unsigned char G_contrast1;

		G_contrast1 += 4;

		DEBUGCHAR(48 +  (unsigned char)G_contrast1 / 100);
		DEBUGCHAR(48 + ((unsigned char)G_contrast1 % 100) / 10);
		DEBUGCHAR(48 + ((unsigned char)G_contrast1 % 100) % 10);
	        DEBUGSTR("\r\n");

		//S6B33_contrast(G_contrast1);
		LCDReset();

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == '[') {
		void initTouch();

		Nnops -= 1;
		if (Nnops == 255) Nnops = 0;

		initTouch();
		DEBUGSTR("NOP ");
		DEBUGCHAR(48 +  (unsigned char)Nnops / 100);
		DEBUGCHAR(48 + ((unsigned char)Nnops % 100) / 10);
		DEBUGCHAR(48 + ((unsigned char)Nnops % 100) % 10);
		DEBUGSTR("\r\n");

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == ']') {
		void initTouch();

		if (Nnops != 255) Nnops += 1;

		initTouch();
		DEBUGSTR("NOP ");
		DEBUGCHAR(48 +  (unsigned char)Nnops / 100);
		DEBUGCHAR(48 + ((unsigned char)Nnops % 100) / 10);
		DEBUGCHAR(48 + ((unsigned char)Nnops % 100) % 10);
		DEBUGSTR("\r\n");

		USB_In_Buffer[0] = 0;
	}
	

	if (USB_In_Buffer[0] == ',') {
		DEBUGSTR("NNOP ");
		DEBUGCHAR(48 +  (unsigned char)Nnops / 100);
		DEBUGCHAR(48 + ((unsigned char)Nnops % 100) / 10);
		DEBUGCHAR(48 + ((unsigned char)Nnops % 100) % 10);
		DEBUGSTR("\r\n");

		USB_In_Buffer[0] = 0;
	}
	

	if (USB_In_Buffer[0] == '.') {
		extern unsigned short int sampleButtonStatus; // Bit field of buttons that are pressed
		unsigned char tmpstr[32];

		sprintf(tmpstr, "BTN %04X\r\n", (unsigned short)sampleButtonStatus);
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	 if (USB_In_Buffer[0] == '/') {
		unsigned char tmpstr[32];

		sprintf(tmpstr, "AVG %04X", (unsigned short)ButtonVmeasADC[0]);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " %04X", (unsigned short)ButtonVmeasADC[1]);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " %04X", (unsigned short)ButtonVmeasADC[2]);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " %04X\r\n", (unsigned short)ButtonVmeasADC[3]);
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == 'z') {
		extern unsigned short int slowAverage[];
		unsigned char tmpstr[32];

		sprintf(tmpstr, "SLO %04X", (unsigned short)slowAverage[0]);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " %04X", (unsigned short)slowAverage[1]);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " %04X", (unsigned short)slowAverage[2]);
		DEBUGSTR(tmpstr);

		sprintf(tmpstr, " %04X\r\n", (unsigned short)slowAverage[3]);
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == 'Z') {
		static unsigned char position = 0;
		unsigned char tmpstr[32];

		position = getVerticalPosition();
		sprintf(tmpstr, "POS %d", (unsigned short)position);
		DEBUGSTR(tmpstr);

		position = getHorizontalPosition();
		sprintf(tmpstr, " %d\r\n", (unsigned short)position);
		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	/* echo command */
	for (i=0; i<nread; i++,NextUSBOut++) {
	   USB_Out_Buffer[NextUSBOut] = USB_In_Buffer[i];
	}
	nread = 0;
    }

    /* dump debug buffer */
    if (G_debugBufferFilled != 0) {
	for (i=0; i<G_debugBufferFilled; i++,NextUSBOut++) {
	    USB_Out_Buffer[NextUSBOut] = G_debugBuffer[i];

	    if ( (NextUSBOut+1) > USBbufferSizeOut()) 
		break; /* too big! punt. buffer overrun */
	}
	G_debugBufferFilled=0;
	G_debugBuffer[G_debugBufferFilled]=0;
    }

    // echo back to USB
    if ((USBtransferReady()) && (NextUSBOut > 0)) {
	putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
	NextUSBOut = 0;
    }

    CDCTxService();
}

#define mInitAllLEDs()      LATB &= 0x0; TRISB &= 0x0;

#define mLED_1              LATCbits.LATC0 /* red */
#define mLED_2              LATCbits.LATC1  /* blue */
#define mLED_3              LATBbits.LATB3 /* green */
    
#define mGetLED_1()         mLED_1
#define mGetLED_2()         mLED_2
#define mGetLED_3()         mLED_3

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
    
#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()        mLED_3 = 0;
    
#define mLED_1_Toggle()     mLED_1 = !mLED_1;
 
void BlinkUSBStatus(void)
{
    static int led_count=0;
    extern volatile unsigned char getUSBSuspendControl();
    extern volatile unsigned char USBDeviceStateDETACHED();
    extern volatile unsigned char USBDeviceStateATTACHED();
    extern volatile unsigned char USBDeviceStatePOWERED();
    extern volatile unsigned char USBDeviceStateDEFAULT();
    extern volatile unsigned char USBDeviceStateADDRESS();
    extern volatile unsigned char USBDeviceStateCONFIGURED();

    if(led_count == 0) led_count = 100000U;
    led_count--;

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(getUSBSuspendControl() == 1) {
        if(led_count==0) {
            mLED_1_On();
            mLED_2_On();
        }
    }
    else {
	if (USBDeviceStateDETACHED())
        {
            mLED_Both_Off();
        }
        else if (USBDeviceStateATTACHED())
        {
            mLED_Both_On();
        }
        else if (USBDeviceStatePOWERED())
        {
            mLED_Only_1_On();
        }
        else if (USBDeviceStateDEFAULT())
        {
            mLED_Only_2_On();
        }
        else if (USBDeviceStateADDRESS())
        {
            if(led_count == 0) {
                mLED_1_Toggle();
                mLED_2_Off();
            }
        }
        else if (USBDeviceStateCONFIGURED()) {
            if(debugBlink && (led_count==0)) {
                if (mGetLED_1()) {
			mLED_1_Off();
		}
		else {
			mLED_1_On();
		}

                if (mGetLED_2()) {
			mLED_2_Off();
		}
		else {
			mLED_2_On();
		}

                if (mGetLED_3())  {
			mLED_3_Off();
		}
		else {
			mLED_3_On();
		}
            }
        }
    }
}

/*
   This is the USB call back function when the Forth system 
   is in "control"
   so the loop needs to call the "main" function
*/
void ForthUSB() {
    //  BlinkUSBStatus();

    if (mchipUSBnotReady()) return;

	// if all read, reset buffer
    if (usbkey_fillptr == usbkey_ptr) {
        usbkey_fillptr = 0;
        usbkey_ptr = 0;
    }

	// if not full see if more available
	//if (usbkey_fillptr != MAX_USBKEYBUFFER) {
    if ( (MAX_USBKEYBUFFER - usbkey_fillptr) > 0) {
        int cnt=0;

        if ( (cnt = getsUSBUSART((char *)&(usbkey_buffer[usbkey_fillptr]), (MAX_USBKEYBUFFER - usbkey_fillptr) )) != 0) {
            usbkey_fillptr += cnt;
        }
    }

	// forth key buffer empty?
    if (key_buffer == -1) {
        // any chars available?
        if (usbkey_ptr != usbkey_fillptr) {
            key_buffer = usbkey_buffer[usbkey_ptr];
            usbkey_ptr++;
        }
    }

	// buffer output chars
    if (emit_buffer != -1) {
        USB_Out_Buffer[NextUSBOut++] = emit_buffer;
        emit_buffer = -1;
    }

    if((USBtransferReady()) && (NextUSBOut > 0)) {
        putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
        NextUSBOut = 0;
    }

    CDCTxService();
}
