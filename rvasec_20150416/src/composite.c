// void on_exit(void); is already defined
// #include "badge15.h"

#include <stdlib.h>
#include "global.h"
#include "assets.h"
#include "assetList.h"
#include "time_date.h"
#include "menu.h"
#include "timer1_int.h"
#include "LCDcolor.h"
#include "composite.h"
#include "S6B33.h"
#include "colors.h"
#include "composite.h"

#define uCHAR (unsigned char *)

void interpretDl();


/*

    display list compositor
    Author: Paul Bruggeman
    paul@killercats.com
    4/2015



    Doing complex graphics with a serial LCD display is a problem. Parallel interface LCDs
    with read/write displays are better, but access is still slower than system ram.

    Shadow buffering the LCD would take more than half the entire ram of this system:
    132*132*2 == 34848. For system like this, a display list is the typical approach solution

    There are 2 parts to the display list problem:

    1) display list creation: Dl helper function
    2) display list interpretation: interpretDL()

    This engine is designed to work on one scan line at a time
    it has a scanline buffer:  unsigned short scanline[LCD_XSIZE]
    that is copied to the LCD when a scanline is done

    Traditional display lists have an advantage in that they can be created 
    external to the device and compiled into the app. Games will typically do this
    to squeeze more FPS out of hardware just by optimizing how things are rendered 
    into the scanline buffer

    see .h for struct info

linux test case debugging:

cc -g -O0 -c assetList.c
cc -g -O0 -DCOMPTEST -o composite composite.c assetList.o

#generate debug info:
./composite > o

#filter output:
grep ^sl o > oo

*/

#ifdef COMPTEST
#include <stdio.h>
   /* DEBUG is for DlComposite()  */
   #define DEBUG(X) X

   /* DEBUG1 is for other DlXXX()  */
   #define DEBUG1(X) X

/* 
   for linux comp test function
   just stub functions not needed
*/
void drawLCD1(unsigned char assetId, int frame) {};
void drawLCD2(unsigned char assetId, int frame) {};
void drawLCD4(unsigned char assetId, int frame) {};
void drawLCD8(unsigned char assetId, int frame) {};
void scanLCD8(unsigned char assetId, unsigned char y, unsigned char x, unsigned char lineCurrent) {};
void setNote(unsigned short freq, unsigned short dur){};
void setBeep(unsigned short freq) {};
void nextNote_cb(unsigned char assetId, int frame) {};
void nextMIDI_cb(unsigned char assetId, int frame)  {};

int main(int argc, char *argv)
{
    void testComposite();

    testComposite();
}

#else
   /* mplab code path */
   #define DEBUG(X)
   #define DEBUG1(X)
#endif

/* helper macro to increment and check array counter */
#define DLNEXT \
	G_dl.currItem++; \
	if (G_dl.currItem >= MAXDISPLAYLIST) G_dl.currItem = (MAXDISPLAYLIST-1);

/* the display list */
struct displayList_t G_dl;

#define LCD_XSIZE 132
#define LCD_YSIZE 132

/* the output buffer */
unsigned short scanLine[LCD_XSIZE] ;

unsigned char G_font = FONT;
unsigned char G_fontHeight = 8;

void DlInit() {
    unsigned short i;

    G_dl.currItem = 0;
    G_dl.buffer = FRONT_BUFFER;
    G_dl.pos.x = 0;
    G_dl.pos.y = 0;

    G_dl.stats.maxItems = 0;
    G_dl.stats.clears = 0;
    G_dl.stats.BGcolor = 0;
    G_dl.stats.color = 0;
    G_dl.stats.transColor = 0;
    G_dl.stats.move = 0;
    G_dl.stats.aux = 0;
    G_dl.stats.colorSpan = 0;
    G_dl.stats.fontSpan = 0;
    G_dl.stats.picSpan = 0;

    for (i=0; i < MAXDISPLAYLIST; i++) G_dl.displayList[i].type = LCD_UNUSED;
    
}

void DlMove(unsigned char x, unsigned char y)
{
    G_dl.pos.x = x;
    G_dl.pos.y = y;

    if (G_dl.currItem == 0) {
	G_dl.displayList[G_dl.currItem].type = LCD_MOVE;
	G_dl.displayList[G_dl.currItem].dData.pos.x = G_dl.pos.x;
	G_dl.displayList[G_dl.currItem].dData.pos.y = G_dl.pos.y;
	DLNEXT;
    } else {
        if (!((G_dl.displayList[G_dl.currItem - 1].type == LCD_MOVE) 
          && (G_dl.displayList[G_dl.currItem - 1].dData.pos.x == G_dl.pos.x)
          && (G_dl.displayList[G_dl.currItem - 1].dData.pos.y == G_dl.pos.y))) {
	    G_dl.displayList[G_dl.currItem].type = LCD_MOVE;
	    G_dl.displayList[G_dl.currItem].dData.pos.x = G_dl.pos.x;
	    G_dl.displayList[G_dl.currItem].dData.pos.y = G_dl.pos.y;
	    DLNEXT;
	}
    }
}

void DlClear()
{
    G_dl.displayList[G_dl.currItem].type = LCD_CLEAR;
    G_dl.displayList[G_dl.currItem].dData.pos.x = G_dl.pos.x;
    G_dl.displayList[G_dl.currItem].dData.pos.y = G_dl.pos.y;
    DLNEXT;
}

void DlColor(unsigned short color)
{
    G_dl.displayList[G_dl.currItem].type = LCD_COLOR;
    G_dl.displayList[G_dl.currItem].dData.color = color;
    DLNEXT;
}

void DlBackgroundColor(unsigned short color)
{
    G_dl.displayList[G_dl.currItem].type = LCD_BACKGROUND_COLOR;
    G_dl.displayList[G_dl.currItem].dData.color = color;
    DLNEXT;
}

void DlPicture(unsigned char picID, unsigned char seqNum)
{
    unsigned char y;

    DlMove(G_dl.pos.x, G_dl.pos.y);
    for (y=0; y < assetList[picID].y; y++) {
	if (G_dl.pos.y < LCD_YSIZE) {
	   /* aux info is for picure lookup x,y info */
	   G_dl.displayList[G_dl.currItem].type = LCD_PICTURE_SPAN;
	   G_dl.displayList[G_dl.currItem].dData.pic.ID = picID;
	   G_dl.displayList[G_dl.currItem].dData.pic.imageNo = seqNum;
	   G_dl.displayList[G_dl.currItem].aux.scanLine = y;      /* scan line offset into char */
	   DLNEXT;
	}
	DlMove(G_dl.pos.x, G_dl.pos.y + 1); /* referencing this is tricky since it updates G_dl.pos.x G_dl.pos.y */
    }
}

/* 
   DlTransparentIndex, also sometimes called key/chromakey color AKA bluescreen/greenscreen
   using this Index in an image means to use what is already in the scanline[] buffer 
   instead of this pixel
*/
void DlTransparentIndex(unsigned short index)
{
    G_dl.displayList[G_dl.currItem].type = LCD_TRANSPARENT_COLOR;
    G_dl.displayList[G_dl.currItem].dData.index = index;
    DLNEXT;
}

void DlSprite(unsigned char picId, unsigned char imageNo)
{
    unsigned char y;

    DlMove(G_dl.pos.x, G_dl.pos.y); /* probably should remove */
    for (y=0; y < assetList[G_font].x; y++) {
	/* aux info is for picure lookup x,y info */
	G_dl.displayList[G_dl.currItem].type = LCD_FONT_SPAN;
	G_dl.displayList[G_dl.currItem].dData.pic.ID = G_font;
	G_dl.displayList[G_dl.currItem].dData.pic.imageNo = imageNo;
	G_dl.displayList[G_dl.currItem].aux.scanLine = y;      /* scan line offset into char */
	DLNEXT;

	DlMove(G_dl.pos.x, G_dl.pos.y + 1); /* referencing this is tricky since it updates G_dl.pos.x G_dl.pos.y */
    }
}

void DlCharacter(unsigned char charin)
{
    unsigned char y;

    DEBUG1( printf("\n-------------\nDlCharacter char %c pos x %d pos y %d\n", charin, G_dl.pos.x, G_dl.pos.y);)

    /*
	massaged from Jon's LCD code.
    */
    if (charin >= 'a' && charin <= 'z') 
	charin -= 97;
    else {
	if (charin >= 'A' && charin <= 'Z')
		charin -= 65;
	else {
	    if (charin >= '0' && charin <= '9')
		charin -= 22;
	    else {
	        switch (charin) {
		case '.':
		    charin = 36;
		    break;

		case ':':
		    charin = 37;
		    break;

		case '!':
		    charin = 38;
		    break;

		case '-':
		    charin = 39;
		    break;

		case '_':
		    charin = 40;
		    break;

		default:
		    charin = 41;
		}
	    }
	}
    }

    DlMove(G_dl.pos.x, G_dl.pos.y);
    for (y=0; y < assetList[G_font].y; y++) {
	if (G_dl.pos.y < LCD_YSIZE) {
	    G_dl.displayList[G_dl.currItem].type = LCD_FONT_SPAN;
	    G_dl.displayList[G_dl.currItem].dData.pic.ID = G_font;
	    G_dl.displayList[G_dl.currItem].dData.pic.imageNo = charin;
	    G_dl.displayList[G_dl.currItem].aux.scanLine = y;      /* scan line offset into char */
	    DLNEXT;

	}
	DlMove(G_dl.pos.x, G_dl.pos.y + 1); /* referencing this is tricky since it updates G_dl.pos.x G_dl.pos.y */
    }
}

void DlFilledRectangle(unsigned char width, unsigned char height)
{
    unsigned char y;

    DlMove(G_dl.pos.x, G_dl.pos.y);
    for (y=0; y < height; y++) {
	if (G_dl.pos.y < LCD_YSIZE) {
	    G_dl.displayList[G_dl.currItem].type = LCD_SPAN;
	    G_dl.displayList[G_dl.currItem].dData.pos.x = width;
	    G_dl.displayList[G_dl.currItem].dData.pos.y = y;
	    DLNEXT;

	}
	DlMove(G_dl.pos.x, G_dl.pos.y + 1); /* referencing this is tricky since it updates G_dl.pos.x G_dl.pos.y */
    }
}

/*
  a point is a span of length 1
*/
void DlPoint(unsigned char x, unsigned char y)
{
    DlMove(x, y);
    if (G_dl.pos.y < LCD_YSIZE) {
	/* point has span dimensions of 0 */
	G_dl.displayList[G_dl.currItem].type = LCD_SPAN;
	G_dl.displayList[G_dl.currItem].dData.pos.x = 1;
	G_dl.displayList[G_dl.currItem].dData.pos.y = y;
        DLNEXT;
    }
}

void DlPrintChar(unsigned char charin, unsigned char x, unsigned char y)
{
    DlMove(x, y);
    DlCharacter(charin);
}

void DlHorizontalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    DlMove(x1, y1);
    if (G_dl.pos.y < LCD_YSIZE) {
	G_dl.displayList[G_dl.currItem].type = LCD_SPAN;
	G_dl.displayList[G_dl.currItem].dData.pos.x = x2 - x1;
	G_dl.displayList[G_dl.currItem].dData.pos.y = y1;
	DLNEXT;
    }
}

void DlVerticalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    unsigned char y;

    for (y=y1; y<y2; y++) DlPoint(x1, y);
}

void DlLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;

    for(;;) {
	DlPoint(x0, y0); /* optimise this: join multiple y==y points into one segments */

	if (x0==x1 && y0==y1) break;

	e2 = err;
	if (e2 > -dx) { err -= dy; x0 += sx; }
	if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void DlWriteLine(unsigned char *string)
{
    unsigned char j, x, y;

    x = G_dl.pos.x;
    y = G_dl.pos.y;
    
    for(j=0; string[j] != 0; j++) {
	DlMove(x + j * assetList[G_font].x, y);
	if (G_dl.pos.y < LCD_YSIZE) {
	    DlCharacter(string[j]);
	}
    }
}

void DlRectangle(unsigned char width, unsigned char height)
{
    unsigned char x, y;

    x = G_dl.pos.x;
    y = G_dl.pos.y;
    DlVerticalLine(x,         y, x,         y + height);
    DlVerticalLine(x + width, y, x + width, y + height);

    DlHorizontalLine(x, y,          x + width, y         );
    DlHorizontalLine(x, y + height, x + width, y + height);
}

#ifdef COMPTEST
char *LCD_Functions[] = {
    "LCD_UNUSED",
    "LCD_CLEAR", 
    "LCD_BACKGROUND_COLOR",
    "LCD_COLOR", 
    "LCD_TRANSPARENT_COLOR", 
    "LCD_MOVE",  
    "LCD_SPAN",  
    "LCD_FONT_SPAN",  
    "LCD_PICTURE_SPAN",  
};
#endif


/* 
  eventually rendering will be done by an interrupt driven function
  this is to help separate willing and rendering
*/
void DlSwapBuffers()
{
   if (G_dl.buffer == FRONT_BUFFER) {
	G_dl.buffer = BACK_BUFFER;
	interpretDl();
   }
   else {
   	G_dl.buffer = FRONT_BUFFER;
	interpretDl();
   }
}

/* 0b0000000000000000 - 0b1111111111111111 */
unsigned short FGColor=0, BGColor=0;

/* faster to do an index for key 0-255 */
unsigned short TRIndex=256;

void interpretDl()
{
    unsigned short item;
    struct vector_t pos;
    unsigned short assetId;
    unsigned char sl, p;

    if (G_dl.currItem == 0) return; /* nothing todo */

#ifndef COMPTEST
    S6B33_rect(0, 0, LCD_XSIZE-1, LCD_YSIZE-1); /* define rectangle */
#endif

    DEBUG(printf("NUM ITEMS %d\n\n", G_dl.currItem);)
    for (sl=0; sl<LCD_YSIZE; sl++) {
	DEBUG(printf("\n-------------\nscanline %d items %d\n", sl, G_dl.currItem);)
	DEBUG(fflush(stdout);)

	pos.x = pos.y = 255;

	for(p=0; p<LCD_XSIZE; p++) scanLine[p] = BGColor;

	for (item=0; item < G_dl.currItem; item++) {
	    DEBUG(printf("\n-- sl %d item %d type %s\n", sl, item, LCD_Functions[G_dl.displayList[item].type]);)
	    DEBUG(fflush(stdout);)

	    switch (G_dl.displayList[item].type) {
		case LCD_UNUSED: /* empty */
		    break;

		case LCD_CLEAR: /* reset LCD? */
		    G_dl.stats.clears++;
		    break;

		case LCD_BACKGROUND_COLOR: /* 16 bit background fill color */
		    BGColor = G_dl.displayList[item].dData.color;

	    	    DEBUG(printf("-- BGCOLOR %d\n", G_dl.displayList[item].dData.color);)
	    	    DEBUG(fflush(stdout);)
		    G_dl.stats.BGcolor++;
		    break;

		case LCD_COLOR: /* 16 bit color */
		    FGColor = G_dl.displayList[item].dData.color;
		    G_dl.stats.color++;
		    break;

		case LCD_TRANSPARENT_COLOR: /* 16 bit transparent color */
		    TRIndex = G_dl.displayList[item].dData.index;
		    G_dl.stats.transColor++;
		    break;

		case LCD_MOVE:  /* (x 8bit, y 8bit) address where the next stuff goes */
		    /* move current pos. */
		    pos.x = G_dl.displayList[item].dData.pos.x;
		    pos.y = G_dl.displayList[item].dData.pos.y;

		    /* yes, process it */
	    	    DEBUG(printf("-- ADDR %d %d\n", pos.x, pos.y);)
	    	    DEBUG(fflush(stdout);)
		    G_dl.stats.move++;
		    break;

		case LCD_SPAN:  /* (x2 8bit) fill w color from last x->x2 span  */
		    if (pos.y == sl) {
			unsigned char ep, p;

		        ep = pos.x + G_dl.displayList[item].dData.pos.x;
			if (ep >= LCD_XSIZE) ep = (LCD_XSIZE-1);

		        for (p = pos.x; p < ep; p++) {
			    //if (pos.x < (LCD_XSIZE-assetList[assetId].x))
			    if (p < LCD_XSIZE)
			        scanLine[p] = FGColor;
			}

	    	        DEBUG(printf("sl %d LCD_SPAN px %d dl.x %d py %d\n", sl, pos.x, G_dl.displayList[item].dData.pos.x, pos.y);)
	    	        DEBUG(fflush(stdout);)
		        G_dl.stats.colorSpan++;
		    }
		    break;

		case LCD_FONT_SPAN:
		    if (pos.y == sl) {
			unsigned char pixbyte, p;
			unsigned short ci;

			assetId = G_dl.displayList[item].dData.pic.ID;

			pixbyte = assetList[assetId].pixdata[
                                        (G_dl.displayList[item].dData.pic.imageNo * assetList[assetId].x) + 
                                        G_dl.displayList[item].aux.scanLine
                                  ];

			/*  assetList[assetId].x == width of a font charater */
			for (p = 0; p < assetList[assetId].x ; p++) {
			    if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

			    ci = ((pixbyte >> p) & 0x1); /* ci = color index */
			    if (ci != TRIndex) {
			        /* pos.x == offset into scan buffer */
				if (ci == 0)
			            scanLine[p + pos.x] = BGColor;
				else
			            scanLine[p + pos.x] = FGColor;
			    }
			}
	    		DEBUG(printf("sl %d FONT_SPAN assetId %d posx %d posy %d auxx %d auxy %d\n", sl, assetId, pos.x, pos.y, aux.x, aux.y);)
	    		DEBUG(fflush(stdout);)
		        G_dl.stats.fontSpan++;
		    }
		    break;

		case LCD_PICTURE_SPAN:
		    if (pos.y == sl) {
			unsigned char r, g, b, pixbyte, *cmap, *pixdata, p;
			unsigned short ci;
			unsigned short pixel;

			assetId = G_dl.displayList[item].dData.pic.ID;

			switch (assetList[assetId].type) {
			case PICTURE8BIT:
			    /* aux.y == picture line, assetList.x == pic xres */
			    pixdata = uCHAR(&(assetList[assetId].pixdata[
						  G_dl.displayList[item].aux.scanLine * assetList[assetId].x 
						+ G_dl.displayList[item].dData.pic.imageNo * assetList[assetId].x * assetList[assetId].y]));

			    for (p = 0; p < assetList[assetId].x; p++) {
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

			        pixbyte = *pixdata; /* 8 pixels per byte */

			        ci = pixbyte;
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
			        pixdata++;
			    }
			    break;

			case PICTURE4BIT:
			    /* aux.y == picture line, assetList.x == pic xres */
			    //pixdata = uCHAR(&(assetList[assetId].pixdata[aux.y * (assetList[assetId].x >> 1) + aux.x * (assetList[assetId].x >> 1) * assetList[assetId].y]));

			    pixdata = uCHAR(&(assetList[assetId].pixdata[
						  G_dl.displayList[item].aux.scanLine * (assetList[assetId].x >> 1)
						+ G_dl.displayList[item].dData.pic.imageNo * (assetList[assetId].x >> 1) * assetList[assetId].y]));

			    for (p = 0; p < (assetList[assetId].x); /* not inc */ ) {
			        pixbyte = *pixdata++; /* 2 pixels per byte */

				/* 1st pixel */
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				ci = ((pixbyte >> 4) & 0xF);
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
				p++;

				/* 2nd pixel */
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				ci = pixbyte & 0xF;
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
				p++;
			    }
			    break;

			case PICTURE2BIT:
			    /* aux.y == picture line, assetList.x == pic xres */
			    //pixdata = uCHAR(&(assetList[assetId].pixdata[aux.y * (assetList[assetId].x >> 2) + aux.x * (assetList[assetId].x >> 2) * assetList[assetId].y]));

			    pixdata = uCHAR(&(assetList[assetId].pixdata[
						  G_dl.displayList[item].aux.scanLine * (assetList[assetId].x >> 2)
						+ G_dl.displayList[item].dData.pic.imageNo * (assetList[assetId].x >> 2) * assetList[assetId].y]));

			    for (p = 0; p < (assetList[assetId].x); /* no inc */) {
			        pixbyte = *pixdata++; /* 4 pixels per byte */

				/* ----------- 1st pixel ----------- */
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				ci = ((pixbyte >> 6) & 0x3);
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
				p++;

				/* ----------- 2nd pixel ----------- */
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				ci = ((pixbyte >> 4) & 0x3);
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
				p++;

				/* ----------- 3rd pixel ----------- */
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				ci = ((pixbyte >> 2) & 0x3);
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
				p++;

				/* ----------- 4th pixel ----------- */
			        if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				ci = ((pixbyte) & 0x3);
			        if (ci != TRIndex) { /* transparent? */
			            cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

			            r = cmap[0];
			            g = cmap[1];
			            b = cmap[2];

			            pixel = ((((r >> 3) & 0b11111) << 11 )
				          |  (((g >> 3) & 0b11111) <<  6 )
				          |  (((b >> 3) & 0b11111)       )) ;

			            /* pos.x == offset into scan buffer */
			            scanLine[p + pos.x] = pixel;
			        }
				p++;

			    }
			    break;

			case PICTURE1BIT: /* same as a font */
			    {
				/* aux.x = character/offset into asset. aux.y = scan/row of character */
			        //pixdata = uCHAR(&(assetList[assetId].pixdata[aux.y * (assetList[assetId].x >> 3) + aux.x * (assetList[assetId].x >> 3) * assetList[assetId].y]));

			        pixdata = uCHAR(&(assetList[assetId].pixdata[
						  G_dl.displayList[item].aux.scanLine * (assetList[assetId].x >> 3)
						+ G_dl.displayList[item].dData.pic.imageNo * (assetList[assetId].x >> 3) * assetList[assetId].y]));

				pixbyte = *pixdata;
				for (p = 0; p < assetList[assetId].x ; p++) {
				    if ((p + pos.x) > (LCD_XSIZE-1)) continue; /* clip x */

				    ci = ((pixbyte >> p) & 0x1); /* ci = color index */
				    if (ci != TRIndex) { /* transparent? */
					/* pos.x == offset into scan buffer */
					if (ci == 0)
					    scanLine[p + pos.x] = BGColor;
					else
					    scanLine[p + pos.x] = FGColor;
				    }
				}
			    }
			    break;

			default:
			    break;
			}

	    		DEBUG(printf("sl %d PICTURE_SPAN assetId %d posx %d posy %d auxx %d auxy %d\n", sl, assetId, pos.x, pos.y, aux.x, aux.y);)
	    		DEBUG(fflush(stdout);)
		        G_dl.stats.picSpan++;
		    }
		    break;

		default:
		    break;
	    } /* switch */
	} /* item */

#ifdef COMPTEST
	unsigned char out[LCD_XSIZE];

	DEBUG(printf("sl %d LCD SEND assetId %d posx %d posy %d auxx %d auxy %d\n", sl, assetId, pos.x, pos.y, aux.x, aux.y);)
	DEBUG(fflush(stdout);)

	for(p=0; p<LCD_XSIZE; p++) out[p] = scanLine[p];
#else
	for(p=0; p < LCD_XSIZE; p++) S6B33_pixel(scanLine[p]);
#endif

    }/* scan line */

    if (G_dl.stats.maxItems < G_dl.currItem) G_dl.stats.maxItems = G_dl.currItem;
}

/*

   Press the 'c' key repeatedly to animate the bowling ball back and forth
   and the chip up and down

   Draw items in the order of backgroup priority. so background color first,
   whatever, then any foreground elements last in the list

   Clipping is not done yey, so you will get crap on the edges if you exceed 0 or 132
   it should not be a problem to finish when I get some time

*/
unsigned short ci0=0;
unsigned short ci1=1;
void testComposite()
{
    static unsigned char bowl=50, bowldir=5;
    static unsigned char chip=64, chipdir=5;
    static unsigned char text=32, textdir=5;
    static unsigned char box=16, boxdir=5;
    static unsigned char text2=32, text2dir=5;
    static unsigned char anim=20, animdir=1;

    DlInit();
    DlClear();

    DlTransparentIndex(ci0); /* index into cmap, not a color */

    DlMove(0, 0);
    //DlPicture(DRBOB,0);
    DlPicture(KITTEN,0);

    DlColor(RED);
    DlMove(16, 16);
    DlFilledRectangle(16, 16);

    /* this will be covered up by the text and chip and boxes */
    //DlMove(64, 110);
    if (bowl > 128) bowldir = -bowldir;
    if (bowl < 5) bowldir = -bowldir;
    bowl += bowldir;
    DlMove(bowl, 110);
    DlPicture(BOWLBALL,0);

    DlBackgroundColor(BLACK);

    if (text > 110) textdir = -textdir;
    if (text < 16) textdir = -textdir;
    text += textdir;
    DlColor(RED);
    DlMove(32,  text);
    DlWriteLine(uCHAR("DR BOB"));

    DlBackgroundColor(BLACK);

    //if (text2 > 132-48-9) text2dir = -text2dir;
    if (text2 > 132-6) text2dir = -text2dir;
    if (text2 < 5) text2dir = -text2dir;
    text2 += text2dir;
    DlColor(WHITE);
    DlMove(text2,  94);
    DlWriteLine(uCHAR("SMOKEM"));

    //DlTransparentIndex(ci1); /* index into cmap, not a color */
    if (chip > 125) chipdir = -chipdir;
    if (chip < 5) chipdir = -chipdir;
    chip += chipdir;
    DlMove(50, chip);
    DlPicture(CHIP,0);

    /* top of everything else */
    DlColor(0b1111111111100000); /* yellow */
    DlMove(96, 96);
    DlFilledRectangle(16, 16);

    /* top of everything else */
    DlColor(0b0000011111111111); 

    if (box > 132) boxdir = -boxdir;
    if (box < 1) boxdir = -boxdir;
    box += boxdir;
    DlMove(box, 100);
    DlFilledRectangle(16, 16);

    DlBackgroundColor(RED);
    DlColor(BLUE);
    /* top of everything else */
    DlMove(box, 64);
    if (anim > 40) animdir = -animdir;
    if (anim == 1) animdir = -animdir;
    anim += animdir;
    DlPicture(FONT, anim);

    DlSwapBuffers();
}


