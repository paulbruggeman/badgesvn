#include "plib.h"
#include "S6B33.h"
#include "LCDcolor.h"

/*
    Samsung S6Bxx driver
    Author: Paul Bruggeman
    paul@killercats.com
    4/2015
*/


#define PIN_BACKLIGHT LATCbits.LATC9
#define PIN__RESET LATCbits.LATC8
#define PIN_A0 LATCbits.LATC7
#define PIN_SDA LATCbits.LATC6
#define PIN_SCLK LATBbits.LATB9
#define PIN__CS LATBbits.LATB8

#define PIN_DC PIN_A0

// The high/low on the A0 pin tells the LCD if we are sending a data or command
#define LCD_COMMAND 0
#define LCD_DATA    1

#define PIN_SDIN PIN_SDA
#define PIN_SCE PIN__CS


/* controller commands- S6B33B2 */
#define NOP                  0x00
#define OSCILLATION_MODE     0x02
#define DRIVER_OUTPUT_MODE   0x10
#define DCDC_SELECT          0x20
#define BIAS_SET             0x22
#define DCDC_CLOCK_DIV       0x24
#define DCDC_AMP_ONOFF       0x26
#define TEMP_COMPENSATION    0x28
#define CONTRAST_CONTROL1    0x2A
#define CONTRAST_CONTROL2    0x2B
#define STANDBY_OFF          0x2C
#define STANDBY_ON           0x2D
#define DDRAM_BURST_OFF      0x2E
#define DDRAM_BURST_ON       0x2F
#define ADDRESSING_MODE      0x30
#define ROW_VECTOR_MODE      0x32
#define N_LINE_INVERSION     0x34
#define FRAME_FREQ_CONTROL   0x36
#define RED_PALETTE          0x38
#define GREEN_PALETTE        0x3A
#define BLUE_PALETTE         0x3C
#define ENTRY_MODE           0x40
#define X_ADDR_AREA          0x42
#define Y_ADDR_AREA          0x43
#define RAM_SKIP_AREA        0x45
#define DISPLAY_OFF          0x50
#define DISPLAY_ON           0x51
#define SPEC_DISPLAY_PATTERN 0x53
#define PARTIAL_DISPLAY_MODE 0x55
#define PARTIAL_START_LINE   0x56
#define PARTIAL_END_LINE     0x57
#define AREA_SCROLL_MODE     0x59
#define SCROLL_START_LINE    0x5A
#define DATA_FORMAT_4K_XRGB  0x60
#define DATA_FORMAT_4K_RGBX  0x61
#define OTP_MODEOFF          0xEA


void S6B33_send_command(unsigned char data) {
  PIN_SCE = 0; /* chip enable active low */
  PIN_DC = LCD_COMMAND; /* COMMAND = 8 bit */

  PIN_SDIN = (data & 0b10000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b01000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b00100000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b00010000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b00001000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b00000100) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b00000010) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b00000001) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SCE = 1;
}

void S6B33_send_data(unsigned short data) {
  PIN_SCE = 0; /* chip enable active low */

  PIN_DC = LCD_DATA; /* data = 16 bit */

  PIN_SDIN = (data & 0b1000000000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0100000000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0010000000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0001000000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000100000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000010000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000001000000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000100000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000010000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000001000000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000000100000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000000010000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000000001000) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000000000100) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000000000010) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;

  PIN_SDIN = (data & 0b0000000000000001) ? 1 : 0;
  PIN_SCLK = 1;
  PIN_SCLK = 0;
//  PIN_SDIN = 0;


  PIN_SCE = 1;
}

unsigned char G_bias  = 0b00000000; /* 0x00 = 1/4  0x11 = 1/5 0x22 = 1/6 0x33 = 1/7 */

// PEB 20150529 unsigned char G_entry = 0b10000000; 0b00000000 inc Y when X=Xend VS 0b00000010 inc X when Y=Yend
unsigned char G_entry = 0b10000000; /* 0x80 */

// PEB WAS 20150313 unsigned char G_outputMode = 0b00000010; /* 0x02 lines=132 SDIR=0 SWP=1 CDIR=0 */
// unsigned char G_outputMode = 0b00000110; /* 0x02 lines=132 SDIR=0 SWP=1 CDIR=0 */
unsigned char G_outputMode = 0b00000110; /* 0x02 lines=132 SDIR=0 SWP=1 CDIR=0 */

/* 0x11 = fose/32 & fose/16 -> set clock fpck=fose/32(Normal)/fpck=fose/16(partial1)-------*/ 
unsigned char G_clockDiv = 0b00010001; /* default = fose/32 & fose/64  normal and partial modes each */

unsigned char G_DCDCselect = 0b00000000; /* step up multplier 1, 1.5 and 2 */

unsigned char G_displayPattern = 0b00000000; /* 0 = normal, 1 = inverted, 2&3 read datasheet */

unsigned char G_addressMode = 0b00011101; /* 0b0001 1101 65536 colors default GSM=00 DSG=1 SGF=1 SGP=10 SGM=1  */

unsigned char G_rowVector = 0b00001110; /* default row vector type=Diagonal INC=111 V=0-----*/ 

unsigned char G_contrast1 = 0b00110100; /* 52 = 0x34 48 = hex 0x30 */

unsigned char G_contrast2 = 0b00110100; /* 52 = 0x34 48 = hex 0x30 */


void S6Delay()
{
   unsigned int i;
   for(i=0; i<10000; i++) ;
}

void S6B33_init_device(void)
{
    S6B33_send_command(STANDBY_ON);  /* standby on == display clocks off */
    S6B33_send_command(DCDC_AMP_ONOFF);
    S6B33_send_command(0x00);        /* booster off */ 

    S6Delay();

    S6B33_send_command(STANDBY_OFF);  // standby on == display clocks off

    S6B33_send_command(OSCILLATION_MODE); // internal osc. 0 = external
    S6B33_send_command(0x01);

    S6B33_send_command(DCDC_AMP_ONOFF);
    S6B33_send_command(0x01); /*------booster1 on---------------*/ 

    S6B33_send_command(DCDC_AMP_ONOFF);
    S6B33_send_command(0x09); /*------booster1 on and amp on---------*/ 

    S6B33_send_command(DCDC_AMP_ONOFF);
    S6B33_send_command(0x0b); /*------booster2 on-------------*/ 

    S6B33_send_command(DCDC_AMP_ONOFF);
    S6B33_send_command(0x0f); /*------booster3 on-------------*/ 

//    S6B33_send_command(TEMP_COMPENSATION);
//    S6B33_send_command(0x01); /*------temp compsation ratio -0.05%------*/ 

    /* TREAD LIGHTLY THIS ONE MAY KILL DISPLAYS */
    S6B33_send_command(DCDC_SELECT);
    /* for voltage range input = 3.3v */
    S6B33_send_command(G_DCDCselect);  /* 0b0000 0000  normal & partial mode DC step up = * 1.0 */
    //S6B33_send_command(0x5); /* 0b0000 0101  normal & partial mode DC step up = * 1.5 */
    /* for < 2.8v */
//    S6B33_send_command(0xA);  /* 0b0000 1010  normal & partial mode DC step up = * 2.0 */



    /* this may save battery if run slower = 0x11 */
    /* fyi, the PDF pg 33 doc image swapped  */
    S6B33_send_command(DCDC_CLOCK_DIV);
    S6B33_send_command(G_clockDiv);  /* default = fose/32 & fose/64  normal and partial modes each */
    // S6B33_send_command(0x011); /* 0x11 = fose/32 & fose/16 -> set clock fpck=fose/32(Normal)/fpck=fose/16(partial1)-------*/ 

    S6B33_send_command(DRIVER_OUTPUT_MODE);
    /* bits- 0 0 DLN1 DLN0 0 SDIR SWP CDIR*/
    /* SWP = RGB vs BGR */
    /* SDIR = segment scanning dir (X) */
    /* CDIR = common scanning dir (Y) */
    /* DLN0,1 = 00=132 01=144, 10=162, 11=96  LCD panel physical y res */
    /* init -> SDIR = 0, SWP = 0, CDR = 0 */
    // PEB 20150313 WAS ->    S6B33_send_command(G_outputMode); /* lines=132 SDIR=0 SWP=0 CDIR=0 */
    S6B33_send_command(G_outputMode); /* lines=132 SDIR=1 SWP=1 CDIR=0 */

    S6B33_send_command(ENTRY_MODE);
    /* 0x0 ==increment Y when X=Xmax, 0x80 == increment X when Y=Ymax */
    /* I am guessing RMW only works for parallel version of these displays */
    //S6B33_send_command(0x0);
    S6B33_send_command(G_entry);

    /* 0x11 = 1/5 & 1/5 -> 0x[0-3][0-3] bias normal and partial 0=1/4 1=1/5 2=1/6 3=1/7 bias --------*/ 
    // 0x00 = 1/4  0x11 = 1/5 0x22 = 1/6 0x33 = 1/7
    // 1/5 = 4V max , 1/6 = 3.3V max  1/4 = 3V max
    S6B33_send_command(BIAS_SET); 
    S6B33_send_command(G_bias); 
 
    S6B33_send_command(SPEC_DISPLAY_PATTERN);
    S6B33_send_command(G_displayPattern); /* 0 = normal, 1 = inverted, 2&3 read datasheet */

    S6B33_send_command(ADDRESSING_MODE);
    S6B33_send_command(G_addressMode); /* 0b0001 1101 65536 colors default GSM=00 DSG=1 SGF=1 SGP=10 SGM=1  */

    S6B33_send_command(ROW_VECTOR_MODE);
	S6B33_send_command(G_rowVector); /* default row vector type=Diagonal ,INC=111-----*/ 

    /*------x address set from 00 to 127--------*/ 
    S6B33_send_command(X_ADDR_AREA); 
    S6B33_send_command(0x00); 
    //S6B33_send_command(0x7F); // 128
    S6B33_send_command(0x83);   // 132 display is really 132x132 overscaned
 
    /*------y address set from 00 to 127--------*/ 
    S6B33_send_command(Y_ADDR_AREA); 
    S6B33_send_command(0x00); 
    //S6B33_send_command(0x7F); // 128
    S6B33_send_command(0x83);   // display is really 132x132


    S6B33_send_command(CONTRAST_CONTROL1); 
    S6B33_send_command(G_contrast1);	 /* contrast1 v1 = 3.757v  max=4v */ 

    /* used for partial "display mode" which we dont use */
    S6B33_send_command(CONTRAST_CONTROL2); 
    S6B33_send_command(G_contrast2); /* contrast2 set v1 to 3.757v max=4v */ 
  
    //S6B33_send_command(N_LINE_INVERSION);
    ////WriteData(WRCOMM,0x89);//cd 
    ////com_out(0x07);
    //S6B33_send_command(0x07);

    S6B33_send_command(PARTIAL_DISPLAY_MODE);
    S6B33_send_command(0x0); /* partial display mode off */

    S6B33_send_command(DISPLAY_ON);
}

/* window of LCD. Send byte will auto-inc x and wrap at xsize and inc y */
void S6B33_rect(int x, int y, int width, int height)
{
    S6B33_send_command(ENTRY_MODE);
    //S6B33_send_command(0x82); /* auto inc y instead of x */
    // also works    S6B33_send_command(0x80);
    S6B33_send_command(G_entry);

    S6B33_send_command(X_ADDR_AREA);
    S6B33_send_command(x);
    S6B33_send_command(x + width);

    S6B33_send_command(Y_ADDR_AREA);
    S6B33_send_command(y);
    S6B33_send_command(y + height);
}

void S6B33_bias(unsigned char data)
{
   S6B33_send_command(BIAS_SET); 
   S6B33_send_command(data); /* 0x11 = 1/5 & 1/5 -> 0x[0-3][0-3] bias normal and partial 0=1/4 1=1/5 2=1/6 3=1/7 bias --------*/ 
}

void S6B33_contrast(unsigned char data)
{
   S6B33_send_command(CONTRAST_CONTROL1); 
   S6B33_send_command(data);
   S6B33_send_command(CONTRAST_CONTROL2); 
   S6B33_send_command(data); /* contrast2 set v1 to 3.757v max=4v */ 
}
	
void S6B33_pixel(unsigned short pixel)
{
    S6B33_send_data(pixel);
}

void S6B33_flip()
{
    S6B33_send_command(DRIVER_OUTPUT_MODE);
    S6B33_send_command(0x07); /* 0b0111 SDIR/SWP/CDIR */
}
