#include "badge16.h"
#include "menu.h"
#include "touchCTMU.h"
#include "colors.h"
#include "fb.h"

enum
{
    CROSS_INIT,
    CROSS_RUN,
    CROSS_EXIT
};

char cross_state = CROSS_INIT;

void cross_cb()
{
    short x, y;
    unsigned short x1, x2, y1, y2;
    static unsigned char drawn=0;
    extern struct sample_t sample[NUMSAMPLES];

    switch(cross_state)
    {
        case CROSS_INIT:
	    if (!drawn) {
		FbInit();
		FbTransparentIndex(0);
		FbBackgroundColor(BLACK);
		FbColor(WHITE);
		FbClear();
		FbMove(10, 10);
		FbWriteLine("press button");
		FbMove(10, 10 + G_Fb.fontHeight);
		FbWriteLine("to exit");
		drawn = 1;
	    }

	    if (BUTTON_PRESSED_AND_CONSUME) {
		cross_state++;
	    }

            break;

        case CROSS_RUN:
            // ((ReadCoreTimer() & 0xff) % (max-min)) + min;

//#define WHITECROSS
#ifdef WHITECROSS
	    x1 = sample[0].ButtonVmeasADC[LEFT_SLIDER];
	    x2 = sample[0].ButtonVmeasADC[RIGHT_SLIDER];

	    if (x1 < x2) 
		x = x1 / 256;
	    else
		x = x2 / 256;

	    y1 = sample[0].ButtonVmeasADC[TOP_SLIDER];
	    y2 = sample[0].ButtonVmeasADC[BOTTOM_SLIDER];

	    if (y1 < y2) 
		y = y1 / 256;
	    else
		y = y2 / 256;

	    x -= 64; /* try to center.. */
	    y -= 64; /* try to center.. */

	    if (x<8) x = 8;
	    if (x>123) x = 123;
	    if (y<8) y = 8;
	    if (y>123) y = 123;
            FbColor(WHITE);
	    FbVerticalLine(x, y-8, x, y+8);
	    FbHorizontalLine(x-8, y, x+8, y);
#endif

	    /*
		red cursor
	    */
#define REDCROSS
#ifdef REDCROSS
	    x = getHorizontalPosition();
	    y = getVerticalPosition();
	    if (x<8) x = 8;
	    if (x>123) x = 123;
	    if (y<8) y = 8;
	    if (y>123) y = 123;
            FbColor(RED);
	    FbVerticalLine(x, y-8, x, y+8);
	    FbHorizontalLine(x-8, y, x+8, y);
#endif

            if (BUTTON_PRESSED_AND_CONSUME) cross_state++;
            break;

        case CROSS_EXIT:
	    cross_state = CROSS_INIT;
	    drawn = 0;
            returnToMenus();
            break;
    }
}
