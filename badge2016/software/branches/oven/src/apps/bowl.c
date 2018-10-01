#include "menu.h"
#include "colors.h"
#include "touchCTMU.h"
#include "assetList.h"
#include "fb.h"

/*
   simple menu example app
   Author: Paul Bruggeman
   paul@Killercats.com
*/

/* make sure to keep these in the same order as the switch/case */
enum {
   BOWL_INIT,
   BOWL_DRAW_INSTRUCTIONS,
   BOWL_WAIT_INSTRUCTIONS,
   BOWL_DRAW_INIT,
   BOWL_START,
   BOWL_DRAW_BG,
   BOWL_DRAW_WAIT,
   BOWL_SLEEPLOOP,
   BOWL_MATH,
   BOWL_STATUS,
   BOWL_SCORE,
   BOWL_POPUP,
   BOWL_EXIT,
};

static unsigned char popup_done = 0;
void popup_cb()
{
    popup_done = 1;
}

/* 
  NOTE: this menu is writeable since it is not const
  so a high score could be put in it limited to 16 chars

popup_m[1].name[LEN + 0]] = 48 +  (unsigned char)score / 100;          // hundreds
popup_m[1].name[LEN + 1]] = 48 + ((unsigned char)score % 100) / 10;    // tens
popup_m[1].name[LEN + 2]] = 48 + ((unsigned char)score % 100) % 10;    // ones

*/
struct menu_t popup_m[] = {
//   {"You win", VERT_ITEM, TEXT, 0},
    {"HIGH SCORE", VERT_ITEM|DEFAULT_ITEM, TEXT, {0} },
    {"", VERT_ITEM, TEXT, {0} }, /* empty string but still has space for 16 chars */
//   {"0123456789", VERT_ITEM, TEXT, {0} }, /* empty string but still has space for 16 chars */
    {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)(popup_cb)} },
};

static unsigned char instructions_done = 0;
void instructions_cb()
{
    instructions_done = 1;
}

const struct menu_t extra_m[] = {
    {"Extra", VERT_ITEM, TEXT, {0}},
    {"Back", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM, BACK, {0}},
};

const struct menu_t instructions_m[] = {
   {"Button: Roll", VERT_ITEM|SKIP_ITEM, TEXT, {0}},
   {"Lower slider:", VERT_ITEM|SKIP_ITEM, TEXT, {0}},
   {"positions ", VERT_ITEM|SKIP_ITEM, TEXT, {0}},
   {"Right slider:", VERT_ITEM|SKIP_ITEM, TEXT, {0}},
   {"force", VERT_ITEM|SKIP_ITEM, TEXT, {0}},
   {"Test menu", VERT_ITEM, MENU, {extra_m}},
   {"Ok", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)(instructions_cb)} },
};

struct vector {
   unsigned char x;
   unsigned char y;
};

void bowl_cb()
{
	static unsigned char state = BOWL_INIT;
	static unsigned int cnt=0;
	static struct vector cursor;
	static int score=0;

	cnt++;
	switch (state) {
		case BOWL_INIT:
			blue(100);
			cursor.x = 64;
			cursor.y = 128;
			cnt = 0;
			score = 0;
			instructions_done = 0;
			popup_done = 0;
			state++;
			break;

		/* first time inits some */
		case BOWL_DRAW_INSTRUCTIONS:
			genericMenu((struct menu_t *)instructions_m, MAIN_MENU_STYLE);
			state++;
			break;

		case BOWL_WAIT_INSTRUCTIONS:
			/* stay in state until user presses button to exit menu  */
			genericMenu((struct menu_t *)instructions_m, MAIN_MENU_STYLE);
			if (instructions_done != 0) {
			    state++;
			    green(0);
			    red(100);
			    cnt = 0;
			}
			score++; /* some random score thing for demo purposes */
			break;

		case BOWL_DRAW_INIT:
			FbInit();
//			FbClear();
			FbTransparency(0);
			FbBackgroundColor(BLACK);
			FbColor(GREEN);
			FbMove(32, 0);
			FbFilledRectangle(64, 131);
			state++;
			break;

		case BOWL_START:
			if (BUTTON_PRESSED_AND_CONSUME) state++;
			// state++;
			break;

		case BOWL_DRAW_BG:
			blue(0);

			FbInit();
//			FbClear();
			FbTransparency(0);
			FbBackgroundColor(BLACK);
			FbTransparentIndex(0);
			FbColor(GREEN);
			FbMove(32, 0);
			FbFilledRectangle(64, 131);
        		FbMove(cursor.x, cursor.y);
			FbImage(BOWLBALL, 0);

			// draw cross hairs that move with image
			FbColor(RED);
			FbLine(64, 48+cursor.y-16, 64, 80+cursor.y-16);
			FbLine(48, 64+cursor.y-16, 80, 64+cursor.y-16);

			FbLine(48, 48+cursor.y-16, 80, 80+cursor.y-16);
			FbLine(48, 80+cursor.y-16, 80, 48+cursor.y-16);

			state++;
			break;

		case BOWL_DRAW_WAIT:
			if (G_Fb.changed == 0) { // wait until framebuffer has been written to LCD 
			    cnt = 0;
			    state++;
			}
			break;

		case BOWL_SLEEPLOOP:
			if (cnt == 200) { /* kill time so ball doesnt move too fast */
				state++;

				blue(100);
			}
			break;

		case BOWL_MATH:
			/* ha. put a flappy bird random cursor.x that seems to react to the user */
			//cursor.x = 0;
			cursor.y -= 5;
			if (cursor.y >= 5) {
				state = BOWL_DRAW_BG;
			}
			else {
				state++;
			}
			break;

		case BOWL_STATUS:
			green(0);
			blue(0);
			red(255);
			state++;
			break;

		case BOWL_SCORE:
			popup_m[1].name[0] = 48 + (score % 1000000) / 100000;          // hundred 
			popup_m[1].name[1] = 48 + (score % 100000) / 10000 ; // ten 
			popup_m[1].name[2] = 48 + (score % 10000) / 1000;    // thousand
			popup_m[1].name[3] = 48 + (score % 1000) / 100;      // hundreds
			popup_m[1].name[4] = 48 + (score % 100) / 10;        // tens
			popup_m[1].name[5] = 48 + (score % 10);        // ones
			popup_m[1].name[6] = 0; // null term
			state++;
			break;

		case BOWL_POPUP:
			genericMenu((struct menu_t *)popup_m, MAIN_MENU_STYLE);
			if (popup_done != 0) {
			    state++;
			    cnt = 0;
			    red(0);
			}
			break;

		case BOWL_EXIT:
			state = BOWL_INIT;
			returnToMenus(); /* NOTE: the routine returns, so code from here to the end of this function will be execute */
			break;

		default:
			break;
	}
};
