#include <plib.h>

#include "badge15.h"


/*
    simple asset management and display lib
    Author: Paul Bruggeman
    paul@killercats.com
    4/2015
*/

/* 
   255 = no asset active
*/
unsigned char G_videoAssetId = 255;
unsigned int G_videoFrame = 0;

void drawAsset(unsigned char assetId)
{
    G_videoAssetId = assetId;
    assetList[assetId].datacb(assetId, G_videoFrame);
}

void drawLCD1(unsigned char assetId, int frame)
{
    unsigned char i, j, p, r, g, b, pixbyte, *cmap, *pixdata;
    unsigned short pixel ;

    // ??? S6B33_rect(0, 0, assetList[assetId].y - 1, assetList[assetId].x - 1);
    // S6B33_rect(0, 0, assetList[assetId].x - 1, assetList[assetId].y - 1);
    S6B33_rect(0, 0, assetList[assetId].y - 1, assetList[assetId].x - 1);

    pixdata = &(assetList[assetId].pixdata[0]);	
    for (i=0; i < assetList[assetId].y; i++) {	
       for (j=0; j < assetList[assetId].x/8; j++ ) { 
            pixbyte = *pixdata++; /* 8 pixels per byte */

            for (p=0; p<8; p++) {
                // 1st pixel
                //cmap = &(assetList[assetId].data_cmap[(unsigned short)((pixbyte>>(7-p)) & 0x1) * 3]);
                cmap = &(assetList[assetId].data_cmap[(unsigned short)((pixbyte>>p) & 0x1) * 3]);
                r = cmap[0];
                g = cmap[1];
                b = cmap[2];

                pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                          ( ((g >> 3) & 0b11111) <<  6 ) |
                          ( ((b >> 3) & 0b11111)       )) ;

                S6B33_pixel(pixel);
         }
      }
   }
}

void drawLCD2(unsigned char assetId, int frame)
{
    unsigned char i, j, r, g, b, pixbyte, *cmap, *pixdata;
    unsigned short pixel ;

    S6B33_rect(0, 0, assetList[assetId].x - 1, assetList[assetId].y - 1);

    pixdata = &(assetList[assetId].pixdata[0]);
    for (i=0; i < assetList[assetId].y; i++) {
       for (j=0; j < assetList[assetId].x/4; j++ ) { // 4 pixels at a ime
            pixbyte = *pixdata++; /* 2 pixels per byte */

            // 1st pixel
            cmap = &(assetList[assetId].data_cmap[(unsigned short)((pixbyte>>6) & 0x3) * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);

            // 2nd pixel
            cmap = &(assetList[assetId].data_cmap[(unsigned short)((pixbyte>>4) & 0x3) * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);

            // 3rd pixel
            cmap = &(assetList[assetId].data_cmap[(unsigned short)((pixbyte>>2) & 0x3) * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);

            // 2nd pixel
            cmap = &(assetList[assetId].data_cmap[(unsigned short)(pixbyte & 0x3) * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);
      }
   }
}

void drawLCD4(unsigned char assetId, int frame)
{
    unsigned char i, j, r, g, b, pixbyte, *cmap, *pixdata;
    unsigned short pixel ;

    S6B33_rect(0, 0, assetList[assetId].x - 1, assetList[assetId].y - 1);

    pixdata = &(assetList[assetId].pixdata[0]);
    for (i=0; i < assetList[assetId].y; i++) {
       for (j=0; j < assetList[assetId].x/2; j++ ) { // 2 pixels at a ime
            pixbyte = *pixdata++; /* 2 pixels per byte */

            // 1st pixel
            cmap = &(assetList[assetId].data_cmap[(unsigned short)(pixbyte>>4) * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);

            // 2nd pixel
            cmap = &(assetList[assetId].data_cmap[(unsigned short)(pixbyte & 0xF) * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);
      }
   }
}

void drawLCD8(unsigned char assetId, int frame)
{
    unsigned char i, j, r, g, b, pixbyte, *cmap;
    unsigned short pixel;

    S6B33_rect(0, 0, assetList[assetId].x - 1, assetList[assetId].y - 1);

    for (i=0; i < assetList[assetId].y; i++) {
       for (j=0; j < assetList[assetId].x; j++) {
            pixbyte = assetList[assetId].pixdata[i * assetList[assetId].x + j];
            cmap = &(assetList[assetId].data_cmap[(unsigned short)pixbyte * 3]);
            r = cmap[0];
            g = cmap[1];
            b = cmap[2];

            pixel = ( ( ((r >> 3) & 0b11111) << 11 ) |
                      ( ((g >> 3) & 0b11111) <<  6 ) |
                      ( ((b >> 3) & 0b11111)       )) ;

            S6B33_pixel(pixel);
      }
   }
}

unsigned char G_audioAssetId = 255;
unsigned int G_audioFrame = 0; /* persistant current "frame" of audio, like a "frame" of video */
unsigned short G_currentNote=0;
unsigned short G_duration = 0;
unsigned short G_duration_cnt = 0;
unsigned short G_freq_cnt = 0;
unsigned short G_freq = 0;

unsigned short G_mute = 0;

void playAsset(unsigned char assetId) 
{
    G_audioAssetId = assetId;
    assetList[assetId].datacb(assetId, 0); /* install asset frame=0 */
}

void setNote(unsigned short freq, unsigned short dur) {
    if (G_mute) return;

   if (dur <= freq) dur = (freq << 1); /* to short to be play with PWM, so double it */

   G_freq = freq;
   G_duration = dur;

   G_freq_cnt = 0;
   G_duration_cnt = 0;
}

// RA9
void doAudio()
{
   if (G_duration == 0) return;

   G_freq_cnt++; /* current note freq counter */
   G_duration_cnt++;

   if (G_duration_cnt != G_duration) {
       if (G_freq_cnt == G_freq)  {
          G_freq_cnt = 0;
          LATAbits.LATA9 = 1; // on
       }
       else 
          LATAbits.LATA9 = 0; // off
   }
   else {
       G_duration = 0;

       LATAbits.LATA9 = 0; // off
       if (G_audioAssetId != 255) assetList[G_audioAssetId].datacb(G_audioAssetId, G_audioFrame) ; /* callback routine */
   }
   G_audioFrame++;
}

#ifdef DEBUGAUDIO
#include "./USB/usb_function_cdc.h"
#endif

/* callback to feed next note to the audio function */
// void mario_cb(int frame) 
void nextNote_cb(unsigned char assetId, int frame) 
{
   G_duration_cnt = 0;
   G_freq_cnt = 0;

   if (frame == 0) {
       G_freq = 0;
       G_duration = 0;
       G_currentNote = 0;
   } 

   if (G_currentNote < assetList[assetId].x) {
	/* unsigned char to short conversion turns out to have weird sign extension problems */
	G_duration = ((unsigned short)(assetList[assetId].data_cmap[G_currentNote] & 0xFF) * 3 * 38) ;

	green(assetList[assetId].data_cmap[G_currentNote] & 0xFF);

	if (assetList[assetId].pixdata[G_currentNote] == 0)
		G_freq = 0;
	else
		G_freq = 38000 / ((unsigned short)(assetList[assetId].pixdata[G_currentNote] & 0xFF) * 8);

	blue((unsigned short)(assetList[assetId].pixdata[G_currentNote] & 0xFF));

	if (G_duration <= G_freq) G_duration = (G_freq << 1); /* to short to be play with PWM, so double it */
#ifdef DEBUGAUDIO
{
	extern char USB_Out_Buffer[];
	extern unsigned char NextUSBOut;
	extern char hextab[];

	USB_Out_Buffer[NextUSBOut++] = 'N';
	USB_Out_Buffer[NextUSBOut++] = 'T';
	USB_Out_Buffer[NextUSBOut++] = ' ';
	
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_currentNote  >> 12) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_currentNote  >>  8) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_currentNote  >>  4) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_currentNote       ) & 0xF];
	
	USB_Out_Buffer[NextUSBOut++] = 'F';
	USB_Out_Buffer[NextUSBOut++] = 'R';
	USB_Out_Buffer[NextUSBOut++] = ' ';
	
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)assetList[assetId].pixdata[G_currentNote]  >> 12) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)assetList[assetId].pixdata[G_currentNote]  >>  8) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)assetList[assetId].pixdata[G_currentNote]  >>  4) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)assetList[assetId].pixdata[G_currentNote]       ) & 0xF];
	
	USB_Out_Buffer[NextUSBOut++] = 'F';
	USB_Out_Buffer[NextUSBOut++] = 'C';
	
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_freq  >> 12) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_freq  >>  8) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_freq  >>  4) & 0xF];
	USB_Out_Buffer[NextUSBOut++] = hextab[((unsigned short)G_freq       ) & 0xF];
	
	USB_Out_Buffer[NextUSBOut++] = '\r';
	USB_Out_Buffer[NextUSBOut++] = '\n';
	USB_Out_Buffer[NextUSBOut++] = 0;
	
	if ((USBUSARTIsTxTrfReady()) && (NextUSBOut > 0)) {
	putUSBUSART(&USB_Out_Buffer[0], NextUSBOut);
	NextUSBOut = 0;
	}
}
#endif
	G_currentNote++;
   }
   else {
	G_freq = 0;
	G_duration = 0;
	G_currentNote = 0;      /* reset for next asset */
	G_audioAssetId = 255; /* clear curent asset */
   }
}

static unsigned char midi_offTime[] = {
/* a_  0 */ 173, 
/* a#  1 */ 163, 
/* b_  2 */ 154, 
/* c_  3 */ 145, 
/* c#  4 */ 137, 
/* d_  5 */ 129, 
/* d#  6 */ 122, 
/* e_  7 */ 115, 
/* f_  8 */ 109, 
/* f#  9 */ 103, 
/* g_ 10 */ 97, 
/* g# 11 */ 91 
}; 

void nextMIDI_cb(unsigned char assetId, int frame) 
{
   G_duration_cnt = 0;
   G_freq_cnt = 0;

   if (frame == 0) {
	   G_freq = 0;
	   G_duration = 0;
	   G_currentNote = 0;
   } 

   if (G_currentNote < assetList[assetId].x) {
	unsigned char note;

	/* midi sample 1/43 = 0.0116 sec */
	/* 38000 * (11/1000) = 418 */
	G_duration = 345;

	red(assetList[assetId].data_cmap[G_currentNote]);

	/* midi c = 60 */
	note = assetList[assetId].data_cmap[G_currentNote] % 12;
	note += 3;  /* 3 = c */
	if (note > 11) note -= 11; /* wrapped */

	G_freq = ((midi_offTime[note] - 91) * 3);

	G_currentNote++;
   }
   else {
	G_freq = 0;
	G_duration = 0;
	G_currentNote = 0;	  /* reset for next asset */
	G_audioAssetId = 255; /* clear curent asset */
   }
}


