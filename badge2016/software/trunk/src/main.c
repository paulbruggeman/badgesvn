#include "plib.h"
#include "badge16.h"
#include "flash.h"
#include "colors.h"
#include "menu.h"
#include "assetList.h"
#include "ir.h"
#include "debug.h"
#include "flash.h"
#include "CTF.h"
#include "screensavers.h"
#include "touchCTMU.h"
#include "stopwatch.h"

extern rtccDate G_date ;
extern char USB_In_Buffer[];
extern char USB_Out_Buffer[];
// used in touchCTMU, charge time
extern unsigned char Nnops;

// used in touchCTMU, button value
extern unsigned short int CurrentButtonStatus;

// array of 4 touch slides sample averages from touchCTMU
extern unsigned short int ButtonVmeasADC[];

extern unsigned char G_red_pwm, G_green_pwm, G_blue_pwm;
extern rtccTime G_time ;
// in used in S6B33 samsung controller
extern unsigned char G_contrast1;
extern unsigned short int sampleButtonStatus; // Bit field of buttons that are pressed
extern unsigned short int slowAverage[];

volatile unsigned char getUSBSuspendControl();
volatile unsigned char USBDeviceStateDETACHED();
volatile unsigned char USBDeviceStateATTACHED();
volatile unsigned char USBDeviceStatePOWERED();
volatile unsigned char USBDeviceStateDEFAULT();
volatile unsigned char USBDeviceStateADDRESS();
volatile unsigned char USBDeviceStateCONFIGURED();
    
#define FORTH
#ifdef FORTH
void resume();
char emit_buffer;
char key_buffer;
char forth_mode = 0;
// forth key buffer on c side
#define MAX_USBKEYBUFFER 64
unsigned char usbkey_buffer[MAX_USBKEYBUFFER] ;
unsigned char usbkey_ptr=0 ;
unsigned char usbkey_fillptr=0 ;
#endif

/*
   will be save/restored from flash
*/
//const unsigned short flashedBadgeId = 0x1; /* MAGIC value for script. It will be replaced by awk script in final flashing */
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

unsigned char  NextUSBOut=0;


char ctf_serial_buffer[MAX_USBKEYBUFFER]={0};
unsigned char ctf_serial_index =0;
unsigned char ctf_serial_attempts = 0;
char ctf_tmp_serial_string[32]={0};
int saved_debug_state=0;
char reset_state = 0;
#define CTF_BITBANGING_VALUES_LEN   32
char bitbanging_index=0;
const int ctf_bitbanging_values[]={
    5,
    8,
    3,
    1,
    9,
    2,
    1,
    7,
    4,
    5,
    2,
    8,
    9,
    4,
    3,
    6,
    2,
    7,
    5,
    6,
    1,
    2,
    7,
    4,
    3,
    6,
    8,
    4,
    5,
    4,
    2 
};
/*
 powerConfig
 
 */
void powerConfig(void)
{
    // ToDo:
    // Figure out which of these we must keep!
//- PMD1bits.AD1MD = 1; // ADC Pwr off - needed for touchCTMU
//- PMD1bits.CTMUMD = 1; // CTMU Off - needed for touchCTMU
    PMD1bits.CVRMD = 1; // comparator voltage reference off
    PMD2bits.CMP1MD = 1; // Comparator 1 off
    PMD2bits.CMP2MD = 1; // Comparator 2 off
    PMD2bits.CMP3MD = 1; // Comparator 3 off
    PMD3bits.IC1MD = 1; // Input compare 1 off
    PMD3bits.IC2MD = 1; // Input compare 2 off
    PMD3bits.IC3MD = 1; // Input compare 3 off
    PMD3bits.IC4MD = 1; // Input compare 4 off
    PMD3bits.IC5MD = 1; // Input compare 5 off
    PMD3bits.OC1MD = 1; // output compare 1 off
    PMD3bits.OC2MD = 1; // output compare 2 off
    PMD3bits.OC3MD = 1; // output compare 3 off
    PMD3bits.OC4MD = 1; // output compare 4 off
    PMD3bits.OC5MD = 1; // output compare 5 off
//- PMD4bits.T1MD = 1; // timer1 off - needed for timers and interrupts
//- PMD4bits.T2MD = 1; // timer2 off - needed for timers and interrupts
//- PMD4bits.T3MD = 1; // timer3 off - needed for timers and interrupts
//- PMD4bits.T4MD = 1; // timer4 off - needed for timers and interrupts
//- PMD4bits.T5MD = 1; // timer5 off - needed for timers and interrupts
    PMD5bits.U1MD = 1; // UART1 off
    PMD5bits.U2MD = 1; // UART2 off
    PMD5bits.SPI1MD = 1; // SPI 1 off
    PMD5bits.SPI2MD = 1; // SPI 2 off
    PMD5bits.I2C1MD = 1; // I2C1 off
    PMD5bits.I2C2MD = 1; // I2C2 off
//- PMD5bits.USBMD = 1; //  USB off - needed for USB communications
//  PMD6bits.RTCCMD = 1; // real-time clock off  ???????
//  PMD6bits.REFOMD = 1; // ref clk out off  ????????
    PMD6bits.PMPMD = 1; // Parallel Master Port off
}

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

// set in touchCTMU, used for power management
extern unsigned int G_currentstamp;
extern unsigned int G_touchstamp;
extern unsigned char packets_seen;

// dormant returns 1 if touch/buttons are dormant for 2 minutes, otherwise returns 0
unsigned char dormant(void) {
	//unsigned char tmpstr[32];
	//sprintf(tmpstr, "Dormant: %u\r\n", G_currentstamp);
	//DEBUGSTR(tmpstr);
    // Has it been long enough since the last interaction?
    //if (G_currentstamp >= (G_touchstamp + 860000)){
    return 0;
    if (G_currentstamp >= (G_touchstamp + 420000)){
        //if (IRpacketInCurr == IRpacketInNext) {  
        if(!packets_seen){
            return 1;
        }
        else {
            // wake on IR receive and reset timer
            G_touchstamp = G_currentstamp;
            return 0;
        }
    } 
    else {
        return 0;
    }
}


const char hextab[]={"0123456789ABCDEF"};

// controls USB heartbeat blink
static unsigned char debugBlink=1;

/*
  ctf_print_ser
  INPUT: String
  OUTPUT: VOID
  Usage: ctf_print_ser("Hello World")
  Note: I was tired of puting a single char at a time in the array
*/
void ctf_print_ser(char *s){
    int index=0;
    while (s[index] != '\0'){
        USB_Out_Buffer[NextUSBOut++] = s[index++];
    }
}

void print_forth_flag1(){
    ctf_print_ser("FLAG:B33NF0RTH3D");
}

void send_forth_flag(char bid){
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_APP5;
    pkt.p.badgeId = bid; /* all */
    pkt.p.data = 0;

    IRqueueSend(pkt);
}

#define SCREEN_SAVE_POPUP_DELAY 400000
//#define SCREEN_SAVE_POPUP_DELAY 150000
unsigned int screen_save_popup_cnt = SCREEN_SAVE_POPUP_DELAY;
#define POPUP_LENGTH 8000
unsigned short popup_time = POPUP_LENGTH;
unsigned char brightScreen = 1;
extern unsigned short anim_cnt;
unsigned char current_screen_saver = 0;
extern unsigned char redraw_main_menu;

#define HIGH_PROB_THRESH 100
#define MEDIUM_PROB_THRESH 30
#define LOW_PROB_THRESH 15

void do_screen_save_popup(){


    static unsigned char prob_val = 50;
    if(popup_time == POPUP_LENGTH)
        prob_val = irbit2(G_currentstamp)%100;

    if(popup_time){

        if(prob_val < MEDIUM_PROB_THRESH){

            switch(current_screen_saver%6){
                case 0:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    smiley();
                    break;
                case 1:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    bluescreen();
                    break;
                case 2:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    hack_the_dragon();
                    break;
                case 3:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    for_president();
                    break;
                case 4:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    scoreBoard();
                    break;
                case 5:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    disp_ir_draw();
                    break;
            }
        }
        else if(prob_val < HIGH_PROB_THRESH){
            switch(current_screen_saver%6){
                case 0:
                    //if(popup_time == POPUP_LENGTH)
                    //    popup_time = POPUP_LENGTH/100;
                    //disp_asset_saver();
                    //break;
                case 1:
                    stupid_rects();
                    break;
                case 2:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/70;
                    dotty();
                    break;
                case 3:
                    carzy_tunnel_animator();
                    break;
                case 4:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    matrix();
                    break;
                case 5:
                    if(popup_time == POPUP_LENGTH)
                        popup_time = POPUP_LENGTH/100;
                    just_the_badge_tips();
                    break;

            }
        }
        popup_time--;
    }
    else{ // stop the popup!
        FbClear();
        popup_time = POPUP_LENGTH;
        screen_save_popup_cnt = SCREEN_SAVE_POPUP_DELAY;
        anim_cnt = 0;
        red(0);
        green(0);
        blue(0);
        current_screen_saver++; // TODO: add randomness to this
        //current_screen_saver = G_currentstamp%5;
    }

}
unsigned char is_dormant = 0;

void ProcessIO(void)
{
    unsigned char nread=0;
    int i;
    unsigned char tmpstr[32];

    //Blink the LEDs according to the USB device status
    //very handy if you lock up when trying to run off of battery
    //BlinkUSBStatus();

    //initialize assuming the badge is active (backlight is powered);
    brightScreen = 1;

    if(dormant() && !is_dormant)
        is_dormant = 1;

    if(is_dormant){
        if(TOP_TAP_AND_CONSUME
                || BOTTOM_TAP_AND_CONSUME
                || LEFT_TAP_AND_CONSUME
                || RIGHT_TAP_AND_CONSUME
                || BUTTON_PRESSED_AND_CONSUME
                || packets_seen){
                //|| (IRpacketInCurr != IRpacketInNext)){
            is_dormant = 0;
            packets_seen = 0;
            brightScreen = 1;
            backlight(G_sysData.backlight);
            popup_time = POPUP_LENGTH;
            screen_save_popup_cnt = SCREEN_SAVE_POPUP_DELAY;
            redraw_main_menu = 1;//hack
            //reset timer
            G_touchstamp = G_currentstamp;
            return;
        }else if (brightScreen){
            backlight(0);
            brightScreen = 0;
        }

        if(screen_save_popup_cnt){
            screen_save_popup_cnt--;
            PowerSaveIdle();
        }
        else if(!screen_save_popup_cnt){
            if(!brightScreen){
                brightScreen = 1;
                backlight(G_sysData.backlight);
            }
            do_screen_save_popup();
        }
        return;
    }
    else
        menus();


    IRhandler(); /* do any pending IR callbacks */

    /*
       forthmode: 
        enter mode in badge usb mode with: "!" 
        exit forth with forth word "yeild"
	turn echo off:
        0 echo !
	turn echo on:
        1 echo !
    */
    if (forth_mode) {
	ForthUSB() ;
	resume();
	return;
    }

    if (mchipUSBnotReady()) return;

    nread = getsUSBUSART(USB_In_Buffer, 64);

    //CTF game serial interface	
    if(ctf_get_server_state()==1){
        saved_debug_state=debugLevelGet();
        debugLevelSet(10000); //turn off the debuging if server is on
        reset_state=1;
        //New line, print message
        
            //clear buffer char
            
            switch(ctf_get_serial_state()){
                case CTF_LOGIN:
                    if(USB_In_Buffer[0] != 0) {
                        ctf_print_ser("User:");
                        ctf_set_serial_state(CTF_HACKIT);
                        USB_In_Buffer[0] = 0;
                    }
                    break;
                
                case CTF_HACKIT:
                    if(USB_In_Buffer[0] != 0) {
                    ctf_print_ser("admin\r\n");
                    ctf_print_ser("pass:");
                    ctf_print_ser("OVERIDING.....\r\n");
                    ctf_set_serial_state(CTF_INJECTION1);
                    USB_In_Buffer[0] = 0;
                    sw_ctf_test = 1; //.1 s
                    }
                    break;
                    
                case CTF_INJECTION1:
                    if(sw_ctf_test == 0){
                        sw_ctf_test = 1; //.1 s
                        ctf_print_ser(" __      __ ____ ______________  ____ _____________  \r\n");
                        ctf_print_ser("/  \\    /  \\    |   \\__    ___/ |    |   \\______   \\ \r\n");
                        ctf_set_serial_state(CTF_INJECTION2);
                    }
                    break;
                
                case CTF_INJECTION2:
                    if(sw_ctf_test == 0){
                        sw_ctf_test = 1; //.1 s
                        ctf_print_ser("\\   \\/\\/   /    |   / |    |    |    |   /|     ___/ \r\n");
                        ctf_print_ser(" \\        /|    |  /  |    |    |    |  / |    |     \r\n");
                        ctf_set_serial_state(CTF_INJECTION3);
                    }
                    break;
                    
                case CTF_INJECTION3:
                    if(sw_ctf_test == 0){
                        sw_ctf_test = 1; //.1 s
                        ctf_print_ser("  \\__/\\  / |______/   |____|    |______/  |____|     \r\n");
                        ctf_print_ser("       \\/                                            \r\n");
                        ctf_set_serial_state(CTF_INJECTION4);
                    }
                    break;    
                    
                case CTF_INJECTION4:
                    if(sw_ctf_test == 0){
                        sw_ctf_test = 1; //.1 s
                        ctf_print_ser("You made it! \r\n");
                        ctf_set_serial_state(CTF_INJECTION5);
                    }
                    break;
                case CTF_INJECTION5:
                    ctf_print_ser("Type Y if you can me:");
                    ctf_set_serial_state(CTF_INJECTION6);
                    break;
                case CTF_INJECTION6:
                    if(USB_In_Buffer[0] != 0){
                        if(USB_In_Buffer[0] == 'y' || (USB_In_Buffer[0] == 'Y')){
                            ctf_print_ser("Great! \r\n");
                            ctf_set_serial_state(CTF_HELLO);
                        }else{
                            USB_In_Buffer[0] = 0;
                            ctf_print_ser("Well Crap... Resetting\r\n");
                            ctf_set_serial_state(CTF_LOGIN);
                        }
                    }
                    break;
                    
                case CTF_HELLO:    
                    ctf_print_ser("I figured this would be much easier to communicate with.\r\n");
                    ctf_set_serial_state(CTF_HELLO2);
                    break;
                    
                case CTF_HELLO2:    
                    ctf_print_ser("You will need to come back here to get info back to me.\r\n");
                    ctf_set_serial_state(CTF_HELLO3);
                    break;
                case CTF_HELLO3:    
                    ctf_print_ser("moving forward.\r\n");
                    ctf_set_serial_state(CTF_HELLO4);
                    break;
                
                case CTF_HELLO4:    
                    ctf_print_ser("Another Challenge down, have a code!: ");
                    ctf_set_serial_state(CTF_HELLO5);
                    break;
                    
                case CTF_HELLO5:    
                    //ctf_print_ser(CTF_SERIAL);
                    ctf_print_ser(ctf_hw[9]);
                    ctf_print_ser(ctf_hw[10]);
                    ctf_print_ser(ctf_hw[9]);
                    ctf_print_ser(ctf_hw[10]);
                    ctf_print_ser(ctf_num[4]);
                    ctf_print_ser(ctf_num[2]);
                    ctf_print_ser(ctf_num[0]);
                    ctf_set_serial_state(CTF_DONE);
                    break;
                    
                case CTF_EYECOLOR_SER:
                    ctf_print_ser("Enter decoded number: ");
                    ctf_set_serial_state(CTF_EYECOLOR_SER2);
                    ctf_serial_index =0;
                    
                    //clear USB in buffer of junk
                    //for(i=0; i< CDC_DATA_IN_EP_SIZE; i++){
                    for(i=0; i< MAX_USBKEYBUFFER; i++){
                        USB_In_Buffer[i]=0;
                    }
                    break;
                
                case CTF_EYECOLOR_SER2:
                    //If there is data on the USB UART
                    if(USB_In_Buffer[0] != 0){
                        //If that data is not a return or new line and the data accumulated is not longer than 32 chars.
                        if((USB_In_Buffer[0] != '\r') && (USB_In_Buffer[0] != '\n') && (ctf_serial_index < 32)){
                            //echo back
                            USB_Out_Buffer[NextUSBOut++] = USB_In_Buffer[0];
                            ctf_serial_buffer[ctf_serial_index]= USB_In_Buffer[0];
                            ctf_serial_index++;
                        }else{
                            if(ctf_serial_buffer[0] == '3' && (ctf_serial_buffer[1] == '5') && (ctf_serial_buffer[2] == '2')){
                                ctf_print_ser("\r\nGreat! CTF code: ");
                                //ctf_print_ser(CTF_IO);
                                ctf_print_ser(ctf_hw[10]);
                                ctf_print_ser(ctf_hw[9]);
                                ctf_print_ser(ctf_hw[8]);
                                ctf_print_ser(ctf_hw[8]);
                                ctf_print_ser(ctf_num[2]);
                                ctf_print_ser(ctf_num[4]);
                                ctf_print_ser(ctf_num[6]);
                                ctf_serial_attempts = 0;
                                ctf_set_serial_state(CTF_EYECOLOR_PASS);
                            }else{
                                ctf_print_ser("\r\nNope... Try again\r\n");
                                //ctf_serial_attempts++;
                                ctf_set_serial_state(CTF_DONE);
                            }
                            
                            
                        }
                        USB_In_Buffer[0]=0;
                    }
                    break;
                    
                case CTF_EYECOLOR_PASS:
                    
                    break;
                    
                case CTF_BITBANGING_SER:
                    ctf_print_ser("Enter anything, the lower nibble will be written.\r\n");
                    ctf_set_serial_state(CTF_BITBANGING2_SER);
                    ctf_serial_index =0;
                    
                    //clear USB in buffer of junk
                    //for(i=0; i< CDC_DATA_IN_EP_SIZE; i++){
                    for(i=0; i< MAX_USBKEYBUFFER; i++){
                        USB_In_Buffer[i]=0;
                    }
                    break;
                    
                case CTF_BITBANGING2_SER:
                    //If there is data on the USB UART
                    if(USB_In_Buffer[0] != 0){
                        //If that data is not a return or new line and the data accumulated is not longer than 32 chars.
                        if((USB_In_Buffer[0] != '\r') && (USB_In_Buffer[0] != '\n') && (ctf_serial_index < 32)){
                            //echo back
                            ctf_print_ser("Wrote: ");
                            USB_Out_Buffer[NextUSBOut++] = USB_In_Buffer[0];
                            ctf_serial_buffer[ctf_serial_index]= USB_In_Buffer[0];
                            ctf_serial_index=0;
                            ctf_print_ser("\n\r");
                            
                            //CTF_FP_1_DIR = 0;
                            CTF_FP_2_DIR = 0;
                            CTF_FP_3_DIR = 0;
                            CTF_FP_4_DIR = 0;
                            CTF_FP_5_DIR = 0;
                            //CTF_FP_6_DIR = 0;
                            //CTF_FP_7_DIR = 0;
                            
                            
                            
                            
                            if(USB_In_Buffer[0] & 0b00000001){
                                CTF_FP_2_L = 1;
                            }else{
                                CTF_FP_2_L = 0;
                            }
                            
                            if(USB_In_Buffer[0] & 0b00000010){
                                CTF_FP_3_L = 1;
                            }else{
                                CTF_FP_3_L = 0;
                            }
                            
                            if(USB_In_Buffer[0] & 0b00000100){
                                CTF_FP_4_L = 1;
                            }else{
                                CTF_FP_4_L = 0;
                            }
                            
                            if(USB_In_Buffer[0] & 0b00001000){
                                CTF_FP_5_L = 1;
                            }else{
                                CTF_FP_5_L = 0;
                            }
                            
                                                        
                        }
                        USB_In_Buffer[0]=0;
                    }
                    break;
                
                case CTF_BITBANGING_CHAL_SER:
                    ctf_print_ser("I am writing a number out. What is it?\r\n");
                    ctf_set_serial_state(CTF_BITBANGING_CHAL_SER2);
                    ctf_serial_index =0;
                    ctf_print_ser(": ");
                    
                    
                    #warning put random writes here
                    CTF_FP_2_DIR = 0;
                    CTF_FP_3_DIR = 0;
                    CTF_FP_4_DIR = 0;
                    CTF_FP_5_DIR = 0;
                    
                    if(ctf_bitbanging_values[bitbanging_index] & 0b00000001){
                        CTF_FP_2_L = 1;
                    }else{
                        CTF_FP_2_L = 0;
                    }
                    
                    if(ctf_bitbanging_values[bitbanging_index] & 0b00000010){
                        CTF_FP_3_L = 1;
                    }else{
                        CTF_FP_3_L = 0;
                    }
                    
                    if(ctf_bitbanging_values[bitbanging_index] & 0b00000100){
                        CTF_FP_4_L = 1;
                    }else{
                        CTF_FP_4_L = 0;
                    }
                    
                    if(ctf_bitbanging_values[bitbanging_index] & 0b00001000){
                        CTF_FP_5_L = 1;
                    }else{
                        CTF_FP_5_L = 0;
                    }
                    
                    
                    //clear USB in buffer of junk
                    //for(i=0; i< CDC_DATA_IN_EP_SIZE; i++){
                    for(i=0; i< MAX_USBKEYBUFFER; i++){
                        USB_In_Buffer[i]=0;
                    }
                    break;
                    
                case CTF_BITBANGING_CHAL_SER2:
                    //If there is data on the USB UART
                    
                    
                    if(USB_In_Buffer[0] != 0){
                        //If that data is not a return or new line and the data accumulated is not longer than 32 chars.
                        if((USB_In_Buffer[0] != '\r') && (USB_In_Buffer[0] != '\n') && (ctf_serial_index < 32)){
                            //echo back
                            USB_Out_Buffer[NextUSBOut++] = USB_In_Buffer[0];
                            ctf_serial_buffer[ctf_serial_index]= USB_In_Buffer[0];
                            ctf_serial_index=0;
                        }else{
                            if(ctf_serial_buffer[0] == ctf_bitbanging_values[bitbanging_index]+48){
                                ctf_print_ser("\r\nNice! CTF code: ");
                                //ctf_print_ser(CTF_IO);
                                ctf_print_ser(ctf_hw[2]);
                                ctf_print_ser(ctf_hw[1]);
                                ctf_print_ser(ctf_hw[3]);
                                ctf_print_ser(ctf_hw[3]);
                                ctf_print_ser(ctf_num[0]);
                                ctf_print_ser(ctf_num[4]);
                                ctf_print_ser(ctf_num[2]);
                                ctf_set_serial_state(CTF_BITBANGING_PASS);
                            }else{
                                ctf_print_ser("\r\nNope... Changing number. Try again\r\n");
                                //ctf_print_ser("\r\nDebug, was:");
                                //ctf_print_ser(ctf_bitbanging_values[bitbanging_index]);
                                //ctf_print_ser("\r\nDebug, got:");
                                //ctf_print_ser(ctf_serial_buffer[0]);
                                
                                if(bitbanging_index< CTF_BITBANGING_VALUES_LEN){
                                    bitbanging_index++;
                                }else{
                                    bitbanging_index=0;
                                }
                                //ctf_serial_attempts++;
                                ctf_set_serial_state(CTF_DONE);
                            }
                            
                            
                        }
                        USB_In_Buffer[0]=0;
                    }
                    break;    
                    
                
                    
                case CTF_DONE:
                    
                    break;
                    
            }

            USB_In_Buffer[0] = 0;
      
    }else{
        if(reset_state == 1){
        debugLevelSet(saved_debug_state);
        reset_state=0;
        }
    }


    if(nread > 0) {
	// if first char == ! then call Forth system. currently never returns
	if (USB_In_Buffer[0] == '!') {
		USB_In_Buffer[0] = 0;
		forth_mode = 1;
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
//		sprintf(tmpstr, "%04X\r\n", imgno);
		DEBUGSTR(tmpstr );

		FbImage(imgno,0); 
		
		USB_In_Buffer[0] = 0;
	}
			 
	if ((USB_In_Buffer[0] == 13) || (USB_In_Buffer[0] == 10)) {
		unsigned char tmpstr[32];

//		DEBUGSTR("MENU R ");
//		sprintf(tmpstr, "%08X", (unsigned long)runningApp);
//		DEBUGSTR(tmpstr);

//		DEBUGSTR(" S ");
//		sprintf(tmpstr, "%08X", (unsigned long)getSelectedMenu());
//		DEBUGSTR(tmpstr);

//		DEBUGSTR(" C ");
//		sprintf(tmpstr, "%08X\r\n", (unsigned long)getCurrMenu());
//		DEBUGSTR(tmpstr);

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

//		DEBUGSTR("FREQ ");
//		sprintf(tmpstr, "%04X", (unsigned int)freq);
//		DEBUGSTR(tmpstr);


//		DEBUGSTR("DUR ");
//		sprintf(tmpstr, "%04X\r\n", (unsigned int)dur);
//		DEBUGSTR(tmpstr);

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
//		sprintf(tmpstr, " BADGE:%0d ", (unsigned char)G_sysData.badgeId);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " SEK:%d ", (unsigned char)G_sysData.sekrits);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " ACH:%d ", (unsigned char)G_sysData.achievements);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " LED:%d ", (unsigned char)G_sysData.ledBrightness);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " BL:%d\r\n", (unsigned char)G_sysData.backlight);
//		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}


	if (USB_In_Buffer[0] == '{') {
	    void IRqueueSend(union IRpacket_u pkt) ;
	    union IRpacket_u pkt;

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
	    union IRpacket_u pkt;

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
		G_contrast1 -= 4;

		DEBUGCHAR(48 +  (unsigned char)G_contrast1 / 100);
		DEBUGCHAR(48 + ((unsigned char)G_contrast1 % 100) / 10);
		DEBUGCHAR(48 + ((unsigned char)G_contrast1 % 100) % 10);
	        DEBUGSTR("\r\n");

		LCDReset();

		USB_In_Buffer[0] = 0;
	}

	if ((USB_In_Buffer[0] == '=') || (USB_In_Buffer[0] == '+')) {
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

//		sprintf(tmpstr, "BTN %04X\r\n", (unsigned short)sampleButtonStatus);
//		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	 if (USB_In_Buffer[0] == '/') {
		unsigned char tmpstr[32];

//		sprintf(tmpstr, "AVG %04X", (unsigned short)ButtonVmeasADC[0]);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " %04X", (unsigned short)ButtonVmeasADC[1]);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " %04X", (unsigned short)ButtonVmeasADC[2]);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " %04X\r\n", (unsigned short)ButtonVmeasADC[3]);
//		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == 'z') {
		unsigned char tmpstr[32];

//		sprintf(tmpstr, "SLO %04X", (unsigned short)slowAverage[0]);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " %04X", (unsigned short)slowAverage[1]);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " %04X", (unsigned short)slowAverage[2]);
//		DEBUGSTR(tmpstr);

//		sprintf(tmpstr, " %04X\r\n", (unsigned short)slowAverage[3]);
//		DEBUGSTR(tmpstr);

		USB_In_Buffer[0] = 0;
	}

	if (USB_In_Buffer[0] == 'Z') {
		static unsigned char position = 0;
		unsigned char tmpstr[32];

//		position = getVerticalPosition();
//		sprintf(tmpstr, "POS %d", (unsigned short)position);
//		DEBUGSTR(tmpstr);

//		position = getHorizontalPosition();
//		sprintf(tmpstr, " %d\r\n", (unsigned short)position);
//		DEBUGSTR(tmpstr);

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
