#include <stdlib.h>
#include "badge16.h"
#include "fb.h"
#include "assetList.h"
#include "colors.h"

#define uCHAR (unsigned char *)
struct framebuffer_t G_Fb;

/*
    memory map frame buffer for 16 bit S6B33 type LCD's

    Author: Paul Bruggeman
    paul@killercats.com
    1/2016

*/

#define LCD_XSIZE 132
#define LCD_YSIZE 132

/* the output buffer */
#define FBSIZE (LCD_XSIZE * LCD_YSIZE)
unsigned short LCDbuffer[FBSIZE] ;

#define BUFFER( ADDR ) LCDbuffer[(ADDR)]

void FbInit() {
    G_Fb.buffer = LCDbuffer;
    G_Fb.pos.x = 0;
    G_Fb.pos.y = 0;
    G_Fb.font = FONT;
    G_Fb.fontHeight = 8;

    G_Fb.color = 255;
    G_Fb.BGcolor = 0;
    G_Fb.transMask = 0;
    G_Fb.transIndex = 0;
    G_Fb.changed = 0;
}

void FbMove(unsigned char x, unsigned char y)
{
    G_Fb.pos.x = x;
    G_Fb.pos.y = y;

    if (G_Fb.pos.x >= LCD_XSIZE) G_Fb.pos.x = LCD_XSIZE-1;
    if (G_Fb.pos.y >= LCD_YSIZE) G_Fb.pos.y = LCD_YSIZE-1;
}

void FbClear()
{
    unsigned short i;

    S6B33_rect(0, 0, LCD_XSIZE-1, LCD_YSIZE-1);

    G_Fb.pos.x = 0;
    G_Fb.pos.y = 0;
    G_Fb.changed = 0;

    for (i=0; i<(LCD_XSIZE * LCD_YSIZE); i++) {
	BUFFER(i) = G_Fb.BGcolor;
	S6B33_pixel(G_Fb.BGcolor);
    }
}

void FbTransparency(unsigned short transparencyMask)
{
    G_Fb.transMask = transparencyMask;
}

void FbColor(unsigned short color)
{
    G_Fb.color = color;
}

void FbBackgroundColor(unsigned short color)
{
    G_Fb.BGcolor = color;
}

void FbImage(unsigned char assetId, unsigned char seqNum)
{
    switch (assetList[assetId].type) {
	case PICTURE1BIT:
	    FbImage1bit(assetId, seqNum);
	    break;
	
	case PICTURE2BIT:
	    FbImage2bit(assetId, seqNum);
	    break;
	
	case PICTURE4BIT:
	    FbImage4bit(assetId, seqNum);
	    break;
	
	case PICTURE8BIT:
	    FbImage8bit(assetId, seqNum);
	    break;

	default: 
	    break;
    }
}

void FbImage8bit(unsigned char assetId, unsigned char seqNum)
{
    unsigned char y, yEnd, x;
    unsigned char *pixdata, pixbyte, ci, *cmap, r, g, b;
    unsigned short pixel;

    /* clip to end of LCD buffer */
    yEnd = G_Fb.pos.y + assetList[assetId].y;
    if (yEnd >= LCD_YSIZE) yEnd = LCD_YSIZE-1;

    for (y = G_Fb.pos.y; y < yEnd; y++) {
	pixdata = uCHAR(&(assetList[assetId].pixdata[ (y - G_Fb.pos.y) * assetList[assetId].x + seqNum * assetList[assetId].x * assetList[assetId].y]));

	for (x = 0; x < assetList[assetId].x; x++) {
		if ((x + G_Fb.pos.x) >= LCD_XSIZE) continue; /* clip x */

		pixbyte = *pixdata; /* 1 pixel per byte */

		ci = pixbyte;
		if (ci != G_Fb.transIndex) { /* transparent? */
		    cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		    r = cmap[0];
		    g = cmap[1];
		    b = cmap[2];

		    pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		    if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		    else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
		}
		pixdata++;
	}
    }
    G_Fb.changed = 1;
}

void FbImage4bit(unsigned char assetId, unsigned char seqNum)
{
    unsigned char y, yEnd, x;
    unsigned char *pixdata, pixbyte, ci, *cmap, r, g, b;
    unsigned short pixel;

    /* clip to end of LCD buffer */
    yEnd = G_Fb.pos.y + assetList[assetId].y;
    if (yEnd >= LCD_YSIZE) yEnd = LCD_YSIZE-1;

    for (y = G_Fb.pos.y; y < yEnd; y++) {
	pixdata = uCHAR(&(assetList[assetId].pixdata[ (y - G_Fb.pos.y) * (assetList[assetId].x >> 1) + seqNum * (assetList[assetId].x >> 1) * assetList[assetId].y]));

	for (x = 0; x < (assetList[assetId].x); /* manual inc */ ) {
		pixbyte = *pixdata++; /* 2 pixels per byte */

		/* 1st pixel */
		if ((x + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

		ci = ((pixbyte >> 4) & 0xF);
		if (ci != G_Fb.transIndex) { /* transparent? */
		    cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		    r = cmap[0];
		    g = cmap[1];
		    b = cmap[2];

		    pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		    /* G_Fb.pos.x == offset into scan buffer */
		    if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		    else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
		}
		x++;

		/* 2nd pixel */
		if ((x + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

		ci = pixbyte & 0xF;
		if (ci != G_Fb.transIndex) { /* transparent? */
		    cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		    r = cmap[0];
		    g = cmap[1];
		    b = cmap[2];

		    pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		    /* G_Fb.pos.x == offset into scan buffer */
		    if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		    else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
		}
		x++;
	}
    }
    G_Fb.changed = 1;
}

void FbImage2bit(unsigned char assetId, unsigned char seqNum)
{
    unsigned char y, yEnd, x;
    unsigned char *pixdata, pixbyte, ci, *cmap, r, g, b;
    unsigned short pixel;

    /* clip to end of LCD buffer */
    yEnd = G_Fb.pos.y + assetList[assetId].y;
    if (yEnd >= LCD_YSIZE) yEnd = LCD_YSIZE-1;

    for (y = G_Fb.pos.y; y < yEnd; y++) {
	 pixdata = uCHAR(&(assetList[assetId].pixdata[ (y - G_Fb.pos.y) * (assetList[assetId].x >> 2) + seqNum * (assetList[assetId].x >> 2) * assetList[assetId].y]));

	 for (x = 0; x < (assetList[assetId].x); /* manual inc */) {
	    pixbyte = *pixdata++; /* 4 pixels per byte */

		/* ----------- 1st pixel ----------- */
	    if ((x + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

	    ci = ((pixbyte >> 6) & 0x3);
	    if (ci != G_Fb.transIndex) { /* transparent? */
		cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		r = cmap[0];
		g = cmap[1];
		b = cmap[2];

		pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		/* G_Fb.pos.x == offset into scan buffer */
		if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
	    }
	    x++;

		/* ----------- 2nd pixel ----------- */
	    if ((x + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

	    ci = ((pixbyte >> 4) & 0x3);
	    if (ci != G_Fb.transIndex) { /* transparent? */
		cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		r = cmap[0];
		g = cmap[1];
		b = cmap[2];

		pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		/* G_Fb.pos.x == offset into scan buffer */
		if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
	    }
	    x++;

	    /* ----------- 3rd pixel ----------- */
	    if ((x + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

	    ci = ((pixbyte >> 2) & 0x3);
	    if (ci != G_Fb.transIndex) { /* transparent? */
		cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		r = cmap[0];
		g = cmap[1];
		b = cmap[2];

		pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		/* G_Fb.pos.x == offset into scan buffer */
		if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
	    }
	    x++;

	    /* ----------- 4th pixel ----------- */
	    if ((x + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

	    ci = ((pixbyte) & 0x3);
	    if (ci != G_Fb.transIndex) { /* transparent? */
		cmap = uCHAR(&(assetList[assetId].data_cmap[ci * 3]));

		r = cmap[0];
		g = cmap[1];
		b = cmap[2];

		pixel = ((((r >> 3) & 0b11111) << 11 )
			  |  (((g >> 3) & 0b11111) <<  6 )
			  |  (((b >> 3) & 0b11111)       )) ;

		/* G_Fb.pos.x == offset into scan buffer */
		if (G_Fb.transMask > 0)
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = (BUFFER(x + G_Fb.pos.x) & (~G_Fb.transMask)) | (pixel & G_Fb.transMask);
		else
			BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x) = pixel;
	    }
	    x++;
	}
    }
    G_Fb.changed = 1;
}

void FbImage1bit(unsigned char assetId, unsigned char seqNum)
{
    unsigned char y, yEnd, x;
    unsigned char *pixdata, pixbyte, ci, *cmap, r, g, b;
    unsigned short pixel;

    yEnd = G_Fb.pos.y + assetList[assetId].y;
    /* clip to end of LCD buffer */
    if (yEnd >= LCD_YSIZE) yEnd = LCD_YSIZE-1;

    for (y=G_Fb.pos.y; y < yEnd; y++) {
	pixdata = uCHAR(&(assetList[assetId].pixdata[ seqNum * (assetList[assetId].x >> 3) * assetList[assetId].y + (y - G_Fb.pos.y) * (assetList[assetId].x >> 3)]));

	for (x=0; x < (assetList[assetId].x); x += 8) {
	    unsigned char bit;

	    pixbyte = *pixdata++; 

	    for (bit=0; bit < 8; bit++) { /* 8 pixels per byte */
		if ((bit + G_Fb.pos.x) >= (LCD_XSIZE-1)) continue; /* clip x */

		ci = ((pixbyte >> bit) & 0x1); /* ci = color index */
		if (ci != G_Fb.transIndex) { // transparent?
		    if (ci == 0) {
			if (G_Fb.transMask > 0) 
			    BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x + bit) = (BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x + bit) & (~G_Fb.transMask)) | (G_Fb.BGcolor & G_Fb.transMask);
			else
			    BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x + bit) = G_Fb.BGcolor;
		    } else {
			if (G_Fb.transMask > 0) 
			    BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x + bit) = (BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x + bit) & (~G_Fb.transMask)) | (G_Fb.color & G_Fb.transMask);
			else
			    BUFFER(y * LCD_XSIZE + x + G_Fb.pos.x + bit) = G_Fb.color;
		    }
		}
	    }
	}
    }
    G_Fb.changed = 1;
}

/* 
   FbTransparentIndex, also sometimes called key/chromakey color AKA bluescreen/greenscreen
   using this Index in an image means to use what is already in the scanline[] buffer 
   instead of this pixel
*/
void FbTransparentIndex(unsigned short color)
{
    G_Fb.transIndex = color;
}

// Can't decide where to put this...
// Could put null term as the last char, but it should probably be handled separately
// since the LCD will end up printing as the default char (space?)
const unsigned char lcd_font_map[] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ':', '!', '-', '_', ' '
};

unsigned char MapCCharToLCDCharIdx(unsigned char charin){
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
    return charin;
}

void FbCharacter(unsigned char charin)
{
    FbImage1bit(G_Fb.font, MapCCharToLCDCharIdx(charin));

    /* advance x pos, but not y */
    // FbMove(G_Fb.pos.x + assetList[G_Fb.font].x, G_Fb.pos.y);
    G_Fb.changed = 1;
}

void FbFilledRectangle(unsigned char width, unsigned char height)
{
    unsigned int y, x, endX, endY;

    endX = G_Fb.pos.x + width;
    if (endX >= LCD_XSIZE) endX = LCD_XSIZE-1;

    endY = G_Fb.pos.y + height;
    if (endY >= LCD_YSIZE) endY = LCD_YSIZE-1;

    for (y=G_Fb.pos.y; y < endY; y++) {
	for (x=G_Fb.pos.x; x < endX; x++) {
	    BUFFER(y * LCD_XSIZE + x) = G_Fb.color;
	}
    }
    FbMove(endX, endY);
    G_Fb.changed = 1;
}

void FbPoint(unsigned char x, unsigned char y)
{
    if (x >= LCD_XSIZE) x = LCD_XSIZE-1;
    if (y >= LCD_YSIZE) y = LCD_YSIZE-1;

    BUFFER(y * LCD_XSIZE + x) = G_Fb.color;

    FbMove(x, y);
    G_Fb.changed = 1;
}


void FbHorizontalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    unsigned char x;

    FbMove(x1, y1);
    for (x=x1; x<x2; x++) FbPoint(x, y1);

    G_Fb.changed = 1;
}

void FbVerticalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    unsigned char y;

    FbMove(x1, y1);
    for (y=y1; y<y2; y++) FbPoint(x1, y);
    G_Fb.changed = 1;
}

void FbDashedHorizontalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    unsigned char x;
    FbMove(x1, y1);
    for (x = x1; x < x2; x += 2) FbPoint(x, y1);
    G_Fb.changed = 1;
}

void FbDashedVerticalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    unsigned char y;
    for (y=y1; y<y2; y+=2) FbPoint(x1, y);
    G_Fb.changed = 1;
}

void FbLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;

    for(;;) {
	FbPoint(x0, y0); /* optimise this: join multiple y==y points into one segments */

	if (x0==x1 && y0==y1) break;

	e2 = err;
	if (e2 > -dx) { err -= dy; x0 += sx; }
	if (e2 < dy) { err += dx; y0 += sy; }
    }
    G_Fb.changed = 1;
}

void FbWriteLine(unsigned char *string)
{
    unsigned char j, x, y;

    //debug("FbWriteLine ");

    x = G_Fb.pos.x;
    y = G_Fb.pos.y;
    
    for(j=0; string[j] != 0; j++) {
	FbMove(x + j * assetList[G_Fb.font].x, y);
	FbCharacter(string[j]);
    }
    G_Fb.changed = 1;

}

void FbWriteWrappedLine(unsigned char *string)
{
    unsigned char j, orig_x, y;
    unsigned short x;
    //debug("FbWriteLine ");

    x = orig_x = G_Fb.pos.x;
    y = G_Fb.pos.y;

    for(j=0; string[j] != 0; j++) {

        FbMove(x, y);
	FbCharacter(string[j]);

        // TODO: checking at end of loop, so no first char check :(
        x += assetList[G_Fb.font].x;
        if(x > LCD_XSIZE - assetList[G_Fb.font].x){
            y += assetList[G_Fb.font].y +1;
            x = orig_x;
        }
    }
    G_Fb.changed = 1;
}

void FbWriteWrappedLineWithCharHighlight(unsigned char *string,
                                         unsigned char highlight_idx,
                                         unsigned short highlight_color)
{
    unsigned char j, orig_x, y;
    unsigned short x, orig_color = G_Fb.color;
    //debug("FbWriteLine ");

    x = orig_x = G_Fb.pos.x;
    y = G_Fb.pos.y;

    for(j=0; string[j] != 0; j++) {

        FbMove(x, y);
        if(j == highlight_idx){
            //FbMove()
            FbColor(highlight_color);
            if(string[j] == ' '){
                FbFilledRectangle(6,8);
                FbMove(x, y);
            }
            FbCharacter(string[j]);
            FbColor(orig_color);
            
        }
        else
            FbCharacter(string[j]);
	



        // TODO: checking at end of loop, so no first char check :(
        x += assetList[G_Fb.font].x;
        if(x > LCD_XSIZE - assetList[G_Fb.font].x){
            y += assetList[G_Fb.font].y +1;
            x = orig_x;
        }
    }
    G_Fb.changed = 1;

}

void FbRectangle(unsigned char width, unsigned char height)
{
    unsigned char x, y;

    x = G_Fb.pos.x;
    y = G_Fb.pos.y;
    FbVerticalLine(x,	 y, x,	 y + height);
    FbVerticalLine(x + width, y, x + width, y + height);

    FbHorizontalLine(x, y,	  x + width, y	 );
    // Make this 1 pixel longer to slot into bottom right
    FbHorizontalLine(x, y + height, x + width + 1, y + height);
    G_Fb.changed = 1;
}

/*
  doesn't currently swap buffers, just flushes ram->LCD
  could do double buffer with 4,2 or 1 bit per pixel
*/
void FbSwapBuffers()
{
    unsigned int i;

    //debug("FbWriteLine");

    if (G_Fb.changed == 0) return;

    S6B33_rect(0, 0, LCD_XSIZE-1, LCD_YSIZE-1);
    for (i=0; i<(LCD_XSIZE * LCD_YSIZE); i++) {
	S6B33_pixel(BUFFER(i));
	BUFFER(i) = G_Fb.BGcolor; /* clear buffer as we go */
    }
    G_Fb.changed = 0;

    G_Fb.pos.x = 0;
    G_Fb.pos.y = 0;
}

// Move buffer to screen without clearing the buffer
// - More efficient if making small changes to a consistent scene
void FbPushBuffer()
{
    unsigned int i;

    //debug("FbWriteLine");

    if (G_Fb.changed == 0) return;

    S6B33_rect(0, 0, LCD_XSIZE-1, LCD_YSIZE-1);
    for (i=0; i<(LCD_XSIZE * LCD_YSIZE); i++) {
	S6B33_pixel(BUFFER(i));
	//BUFFER(i) = G_Fb.BGcolor; /* clear buffer as we go */
    }
    G_Fb.changed = 0;

    G_Fb.pos.x = 0;
    G_Fb.pos.y = 0;
}
