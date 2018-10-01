#ifndef composite_h
#define composite_h

/* 
   LCD item types are stored in displayList.dData.data
*/
enum {
    LCD_UNUSED=0,		/* unused */
    LCD_CLEAR,			/* reset LCD */
    LCD_BACKGROUND_COLOR,	/* 16 bit background fill color */
    LCD_COLOR,			/* 16 bit color */
    LCD_TRANSPARENCY,		/* 8 bit color */
    LCD_TRANSPARENT_COLOR,	/* 16 bit transparent/key/mask color */
    LCD_MOVE, 			/* (x 8bit, y 8bit) where the next stuff goes */
    LCD_SPAN,			/* fill w color from last x -> this.x span  */
    LCD_FONT_SPAN,		/* 8 pixels/byte- fill w font data from last x -> this.x span using aux.x, aux.y  */
    LCD_PICTURE_SPAN,		/* 4 pixel/byte- fill w pic data from last x -> this.x span using aux.x, aux.y  */
};

union displayData_u {
    struct {
	unsigned char ID; /* asset picture ID */
	unsigned char imageNo; /* picture array index */
    } pic;

    unsigned short transparencyMask; /* 16 bit transparency mask: this would reduce bg by 50% 0b01111 011110 01111  */

    unsigned short color; /* 16bit color */

    unsigned short index; /* 16 bit color index */

    struct {
	unsigned char x; /* 8 bit : position */
	unsigned char y; /* 8 bit : position */
    } pos ;
} ;

struct vector_t {
   unsigned char x;
   unsigned char y;
};

/*
  4 bytes per item
*/
struct displayItem_t {
   unsigned char type;

   struct {
	unsigned char scanLine;
   } aux ;

   union displayData_u dData;
};


/* for displayList.buffer */
enum {
    FRONT_BUFFER,
    BACK_BUFFER
};


struct stat_t {
    unsigned short maxItems;
    unsigned short clears;
    unsigned short BGcolor;
    unsigned short color;
    unsigned short transColor;
    unsigned short transparency;
    unsigned short move;
    unsigned short aux;
    unsigned short colorSpan;
    unsigned short fontSpan;
    unsigned short picSpan;
};

/*
   4 bytes per element * MAXDISPLAYLIST
   4 * 4096 = 24k
*/
#define MAXDISPLAYLIST 6000
struct displayList_t {
   unsigned short currItem; /* 16 bit */
   unsigned char buffer;  /* which buffer is active */

   unsigned char sl;  /* next scanline to render */

   struct vector_t pos; /* pos info for building LCD functions */
   struct vector_t aux; /* aux info for doing LCD functions */

   /* persistant stats */
   struct stat_t stats; /* track to report over usb  */

   struct displayItem_t displayList[MAXDISPLAYLIST];
};


void DlInit() ;
int DlFrameDone();
void DlMove(unsigned char x, unsigned char y);
void DlClear();
void DlColor(unsigned short color);
void DlBackgroundColor(unsigned short color);
void DlPicture(unsigned char picID, unsigned char seqNum);
void DlTransparentIndex(unsigned short index);
void DlTransparency(unsigned short transparencyMask);
void DlSprite(unsigned char picId, unsigned char imageNo);
void DlCharacter(unsigned char charin);
void DlFilledRectangle(unsigned char width, unsigned char height);
void DlPoint(unsigned char x, unsigned char y);
void DlPrintChar(unsigned char charin, unsigned char x, unsigned char y);
void DlHorizontalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void DlVerticalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void DlLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void DlWriteLine(unsigned char *string);
void DlRectangle(unsigned char width, unsigned char height);
void DlSwapBuffers();
void testComposite();

#endif
