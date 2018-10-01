#include "badge15.h"

/*
   blue screen of death
   Author: Paul Bruggeman
   paul@Killercats.com
*/

/* make sure to keep these in the same order as the switch/case */
enum {
   BSOD_INIT,
   BSOD_DRAW,
   BSOD_WAIT_CLICK,
   BSOD_EXIT,
};

void BSOD_cb()
{
	static unsigned char state = BSOD_INIT;
	static unsigned int cnt=0;

	cnt++;
	switch (state) {
		case BSOD_INIT:
			blue(100);
			cnt = 0;
			state++;
			break;

		case BSOD_DRAW:
			DlInit();
			DlClear();
			DlTransparentIndex(256);
			/* BSOD was drawn with blue == bit on, white = bits off */
			/* 1 bit image display decode to fg == bit on, bg = bits off */
			DlBackgroundColor(WHITE);
			DlColor(BLUE);
			/* image is 128x128. 1 bit images have to be divisable by 8 for byte boundary */
			DlMove(2, 2); 
			DlPicture(BSOD, 0);
			state++;
			break;

		case BSOD_WAIT_CLICK:
			if (BUTTON_PRESSED_AND_CONSUME) state++;
			break;

		case BSOD_EXIT:
			state = BSOD_INIT;
			returnToMenus(); /* NOTE: the routine returns, so code from here to the end of this function will be execute */
			break;

		default:
			break;
	}
};
