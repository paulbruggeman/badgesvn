/** @file
 * Structures and functions for writing to the LCD display.
 *
 */
#ifndef fb_h
#define fb_h

/*
   Frame buffer
*/
enum {
    FB_UNUSED=0,		/**< unused */
    FB_CLEAR,			/**< reset LCD */
    FB_BACKGROUND_COLOR,	/**< 16 bit background fill color */
    FB_COLOR,			/**< 16 bit color */
    FB_TRANSPARENT_MASK,	/**< 16 bit transparent/key/mask color */
    FB_MOVE, 			/**< (x 8bit, y 8bit) where the next stuff goes */
};

/** A single x, y coordinate pair */
struct vector8_t {
   unsigned char x;
   unsigned char y;
};

/** Control structure for an in-memory frame buffer/drawing context. */
struct framebuffer_t {
   unsigned short *buffer;  /**< the actual image bytes */
   struct vector8_t pos;  /**< current x,y position */
   unsigned char font;  /**< current font for text functions */
   unsigned char fontHeight;  /**< current height for text */

   unsigned short color; /**< current foreground drawing color */
   unsigned short BGcolor;  /**< current background drawing color */
   unsigned short transMask;
   unsigned short transIndex;
   unsigned short changed;
};

extern struct framebuffer_t G_Fb;

/** Initialize/reset the single global framebuffer object. (Note: does not clear
 *  buffer contents.)
 */
void FbInit() ;

/** Copies the frame buffer to the LCD (i.e. writes changes to screen). */
void FbSwapBuffers();
/** Moves current position in global buffer to pixels <code>x,y</code> (values
 * 0-131).
 */
void FbMove(unsigned char x, unsigned char y);
/** Resets all pixels in buffer and LCD to current background color. */
void FbClear();
/** Sets current foreground color for the global buffer. */
void FbColor(unsigned short color);
/** Sets current background color for the global buffer. */
void FbBackgroundColor(unsigned short color);
/**
 * Writes an image from the global asset list to the frame buffer.
 *
 * @param [in] assetId An index to the global asset list. Should be value from
 *        first <code>enum</code> defined in assetList.h.
 * @param [in] seqNum If <code>assetId</code> refers to an image array, the
 *        index into that array.
 */
void FbImage(unsigned char assetId, unsigned char seqNum);
/** Sets current transparency mask for the global buffer. */
void FbTransparency(unsigned short transparencyMask);
/* void FbSprite(unsigned char picId, unsigned char imageNo); unimplemented */

/**
 * Writes the character <code>charin</code> to the current position in the
 * global frame buffer. Does not change current position.
 *
 * @param [in] charin A character in the set [A-Za-z0-9.:!-_]. Note lowercase
 *        characters will display as uppercase.
 */
unsigned char MapCCharToLCDCharIdx(unsigned char charin);
void FbCharacter(unsigned char charin);

/**
 * Draws a filled rectangle in the current color to the global frame buffer.
 * Positions upper left corner at the current location. Sets current location
 * to the lower right corner point.
 *
 * @param [in] width width in pixels.
 * @param [in] height height in pixels.
 */
void FbFilledRectangle(unsigned char width, unsigned char height);

/**
 * Sets the pixel at <code>x, y</code> to the current foreground color, and
 * makes that location the current location.
 * @param [in] x x coordinate (0-131)
 * @param [in] y y coordinate (0-131)
 */
void FbPoint(unsigned char x, unsigned char y);

/* void FbPrintChar(unsigned char charin, unsigned char x, unsigned char y); */

/**
 * Draws horizontal line at y coordinate <code>y1<code> from <code>x1</code>
 * to <code>x2</code>.
 *
 * @param [in] x1 x-coordinate where line begins. (0-130, must be <= <code>x2</code>)
 * @param [in] y1 y-coordinate of line. (0-131)
 * @param [in] x2 x-coordinate where line ends. (1-131)
 * @param y2 unused
 */
void FbHorizontalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

/**
 * Draws vertical line at x coordinate <code>x1<code> from <code>y1</code>
 * to <code>y2</code>.
 *
 * @param [in] x1 x-coordinate of line. (0-131)
 * @param [in] y1 y-coordinate where line begins. (0-130, must be <= <code>y2</code>)
 * @param x2 unused
 * @param [in] y2  y-coordinate where line ends. (1-131)
 */
void FbVerticalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

/**
 * Draws vertical line at x coordinate <code>x1<code> from <code>y1</code>
 * to <code>y2</code> by turning on every other pixel. Unlike FbVerticalLine(),
 * long lines do not cause "ghost" artifacts on the screen.
 *
 * @param [in] x1 x-coordinate of line. (0-131)
 * @param [in] y1 y-coordinate where line begins. (0-130, must be <= <code>y2</code>)
 * @param x2 unused
 * @param [in] y2  y-coordinate where line ends. (1-131)
 */
void FbDashedVerticalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

/**
 * Draws horizontal line at y coordinate <code>y1<code> from <code>x1</code>
 * to <code>x2</code> by turning on every other pixel. Unlike FbHorzontalLine(),
 * long lines do not cause "ghost" artifacts on the screen.
 *
 * @param [in] x1 x-coordinate where line begins. (0-130, must be <= <code>x2</code>)
 * @param [in] y1 y-coordinate of line. (0-131)
 * @param [in] x2 x-coordinate where line ends. (1-131)
 * @param y2 unused
 */
void FbDashedHorizontalLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

/**
 * Draws a line from point <code>x1, y1</code> to point <code>x2, y2</code>.
 * @param [in] x0 x-coordinate of starting point (0-130)
 * @param [in] y0 y-coordinate of starting point (0-130)
 * @param [in] x1 x-coordinate of ending point (1-131)
 * @param [in] y1 y-coordinate of ending point. (1-131)
 */
void FbLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
/**
 * Writes zero-terminated string <code>string</code> to buffer starting at
 * current location. Leaves current position at beginning of last character.
 * Strings longer than will fit on display will be clipped, possibly mid-
 * character.
 * @param [in] string A null-terminated string of characters valid for display
 *        (see FbCharacter()).
 */
void FbWriteLine(const unsigned char *string);
/**
 * Draws a rectangle in the current color to the global frame buffer. Rectangle
 * borders are 1 pixel wide. Positions upper left corner at the current
 * location. Sets current location to the lower right corner point (?? check).
 *
 * @param [in] width width in pixels.
 * @param [in] height height in pixels.
 */
void FbWriteWrappedLine(unsigned char *string);
void FbWriteWrappedLineWithCharHighlight(unsigned char *string,
                                         unsigned char highlight_idx,
                                         unsigned short highlight_color);
void FbRectangle(unsigned char width, unsigned char height);
void FbSwapBuffers();
void FbPushBuffer();
const unsigned char lcd_font_map[];
#endif
