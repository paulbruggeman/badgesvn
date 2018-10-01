#include <stdlib.h>
//#include "plib.h"
#include "const.h"
#include "S6B33.h"
#include "LCDcolor.h"
#include "assets.h"
#include "assetList.h"

/*
    LCD  serial (S6Bxx) interface driver
    Author: Paul Bruggeman
    paul@killercats.com
    4/2015
*/

/* CS_ and _RESET are held low */
/* so the LCD is doing nothing  until lcd_init_device happens */
void LCDInitPins(void) {
   /* RC9 == Backlight LED PIN active==high */
   //TRISCbits.TRISC9 = 0;    /* output */
   BACKLIGHT_TRIS = 0;    /* output */
   //LATCbits.LATC9 = 0;      /* init low */
   BACKLIGHT_PIN = 0;      /* init low */
   //CNPDCbits.CNPDC9 = 1;    /* pulldown == on */
   BACKLIGHT_PULLDOWN = 1;    /* pulldown == on */

   /* RC8 == _RESET PIN active==low*/
   //TRISCbits.TRISC8 = 0;    /* output */
   nRESET_TRIS = 0;    /* output */
   //RESET_PIN = 0;      /* init low */
   nRESET_PIN = 0;      /* init low */
   //CNPDCbits.CNPDC8 = 1;    /* pulldown == on */
   nRESET_PULLDOWN = 1;    /* pulldown == on */

   /* RC7 == A0 PIN */
   //TRISCbits.TRISC7 = 0;    /* output */
   A0_TRIS = 0;    /* output */
   //LATCbits.LATC7 = 0;      /* init low */
   A0_PIN = 0;      /* init low */
   //CNPDCbits.CNPDC7 = 1;    /* pulldown == on */
   A0_PULLDOWN = 1;    /* pulldown == on */

   /* RC6 == SDA PIN */
   //TRISCbits.TRISC6 = 0;    /* output */
   SDA_TRIS = 0;    /* output */
   //LATCbits.LATC6 = 0;      /* init low */
   SDA_PIN = 0;      /* init low */
   //CNPDCbits.CNPDC6 = 1;    /* pulldown == on */
   SDA_PULLDOWN = 1;    /* pulldown == on */

   /* RB9 == SCLK PIN */
   //TRISBbits.TRISB9 = 0;    /* output */
   SCLK_TRIS = 0;    /* output */
   //LATBbits.LATB9 = 0;      /* init low */
   SCLK_PIN = 0;      /* init low */
   //CNPDBbits.CNPDB9 = 1;    /* pulldown == on */
   SCLK_PULLDOWN = 1;    /* pulldown == on */

   /* RB8 == _CS PIN active==low */
   //TRISBbits.TRISB8 = 0;    /* output */
   nCS_TRIS = 0;    /* output */
   //nCS_PIN = 0;      /* init low */
   nCS_PIN = 0;      /* init low */
   //CNPDBbits.CNPDB8 = 1;    /* pulldown == on */
   nCS_PULLDOWN = 1;    /* pulldown == on */
}

void LCDDelay()
{
   unsigned int i;
   for(i=0; i<1000; i++) ;
}

void LCDReset(void) {

   nCS_PIN = 0; /* CS = yes */
   nRESET_PIN = 0; /* reset = yes */
   LCDDelay();

   nRESET_PIN = 1; /* reset = no */
   LCDDelay();

   nCS_PIN = 1; /* CS = no */
   LCDDelay();

   S6B33_init_device(); /* set important internal registers */
}

void LCDcolor(unsigned short pixel)
{
    unsigned char i,j;

    S6B33_rect(0, 0, 131, 131); /* display is really 132x132 */

    for (i=0; i<132; i++)
       for (j=0; j<132; j++)
               S6B33_pixel(pixel);
}

void LCDblack()
{
    LCDcolor(0b0000000000000000);
}

void LCDwhite()
{
    LCDcolor(0b1111111111111111);
}

void LCDred()
{
    LCDcolor(0b1111100000000000);
}

void LCDgreen()
{
    LCDcolor(0b0000011111100000);
}

void LCDblue()
{
    LCDcolor(0b0000000000011111);
}


#define BARS
#ifdef BARS
void LCDBars()
{
    unsigned char i,j;
    unsigned short pixel;

    S6B33_rect(0, 0, 131, 131); /* display is really 132x132 */

    for (i=0; i<16; i++) {
       for (j=0; j<132; j++) {
            //S6B33_pixel(j << 10 | i << 5 | j); /* red & blue, green=0 */

            //if (((j>(i%4)) % 8) == 0)
            if ((j % 16) == 0) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b1111111111111111); /* white */
            }

            if ((j % 16) == 1) {
               /*          rrrrrggggggbbbbb); */
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b1111111111111111); /* white */
            }


            if ((j % 16) == 2) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b0000000000000000); /* black */
            }

            if ((j % 16) == 3) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b0000000000000000); /* black */
            }


            if ((j % 16) == 4) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b1111100000000000); /* r */
            }

            if ((j % 16) == 5) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b1111100000000000); /* r */
            }


            if ((j % 16) == 6) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b0000011111100000); /* g */
            }

            if ((j % 16) == 7) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b0000011111100000); /* g */
            }


            if ((j % 16) == 8) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b0000000000011111); /* b */
            }

            if ((j % 16) == 9) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b0000000000011111); /* b */
            }



            if ((j % 16) == 10) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b1111100000011111); /* magenta */
            }

            if ((j % 16) == 11) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b1111111111111111); /* white */
               else
                   S6B33_pixel(0b1111100000011111); /* magenta */
            }



            if ((j % 16) == 12) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b0000011111111111); /* cyan */
            }

            if ((j % 16) == 13) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b0000011111111111); /* cyan */
            }


            if ((j % 16) == 14) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b1111111111100000); /* yellow */
            }

            if ((j % 16) == 15) {
               /*          rrrrrggggggbbbbb); */
               if (j < 32)
                   S6B33_pixel(0b0000000000000000); /* white */
               else
                   S6B33_pixel(0b1111111111100000); /* yellow */
            }

        }
    }
    /* yellow */
    for (i=16; i<33; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

    /* magenta */
    for (i=33; i<49; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

    /* cyan */
    for (i=49; i<66; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

    /* red */
    for (i=66; i<82; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

    /* green */
    for (i=82; i<98; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

    /* blue */
    for (i=98; i<114; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)0 >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

    /* grey */
    for (i=114; i<132; i++)
       for (j=0; j<132; j++) {
            pixel = (unsigned short)(
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) << 11 ) | 
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  6 ) |
                 ( (((unsigned short)j >> 2) & 0b0000000000011111) <<  0)
            );
            S6B33_pixel(pixel);
       }

}
#else
void LCDBars()
{
}
#endif

#ifdef NONSCAN
void LCDline(int x0, int y0, int x1, int y1, unsigned short color) {//do not use unless line is diagonal
  void LCDputPixel(unsigned char x, unsigned char y, unsigned short color);

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    LCDputPixel(x0,y0, color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 > -dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void LCDLogo(void) {
}

// need a 1 bit shadow array to draw to then blit
void LCDputPixel(unsigned char x,//places one pixel
        unsigned char y,
        unsigned short color)
{
    S6B33_rect(y, x, 0, 0);
    S6B33_pixel(color);
}

#endif 


