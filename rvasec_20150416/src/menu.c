#include "badge15.h"
#include "menu.h"
#include "Stages15/game2048.h"
#include "Stages15/connect4.h"
#include "Stages15/bowl.h"
#include "Stages15/firewall.h"
#include "Stages15/update_time.h"
#include "Stages15/splash_screen.h"

/*
   simple menu system
   Author: Paul Bruggeman
   paul@Killercats.com
*/

const struct menu_t breakfast_m[] = {
   {"yummy", GREEN_BG, TEXT, {NULL}}, /* can init union either with or without {} */
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t welcome_m[] = {
   {"yo", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t david_m[] = {
   {"david m", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t break1010_m[] = {
   {"break time", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t nick_m[] = {
   {"nick m", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t pm_m[] = {
   {"paul morgan", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t break1050_m[] = {
   {"break1050", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t lunch_m[] = {
   {"lunch", GREEN_BG, TEXT, {NULL}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t day1_m[] = {
   {" 8:00 Breakfast", GREEN_BG, MENU, {breakfast_m}},
   {" 9:00 Welcome", GREEN_BG, MENU, {welcome_m}},
   {"11:30 Break", GREEN_BG, MENU, {break1050_m}},
   {"11:30 Lunch", GREEN_BG, MENU, {lunch_m}},
   {"back", GREEN_BG, BACK, {NULL}},
};

const struct menu_t day2_m[] = {
   {" 8:00 Breakfast", GREEN_BG, MENU, {breakfast_m}},
   {" 9:00 Welcome", GREEN_BG, MENU, {welcome_m}},
   {"10:00 David", GREEN_BG, MENU, {david_m}},
   {"11:00 Break", GREEN_BG, MENU, {break1010_m}},
   {"11:30 Nick", GREEN_BG, MENU, {nick_m}},
   {"11:45 PM", GREEN_BG, MENU, {pm_m}},
   {"11:30 Break", GREEN_BG, MENU, {break1050_m}},
   {"11:30 Lunch", GREEN_BG, MENU, {lunch_m}},
   {"back", GREEN_BG, BACK, {NULL}},
};


const struct menu_t schedule_m[] = {
   {"day1", GREEN_BG, MENU, {day1_m}},
   {"day2", GREEN_BG, MENU, {day2_m}},
   {"back", GREEN_BG, BACK, {NULL}},
};

#ifdef MAINMENU
void firewall_cb() {};
void connect4_cb() {};
void game_2048_cb() {};
void bowling_cb() {};
void hacker_cb() {};
void aliens_cb() {};
void setNote(unsigned short freq, unsigned short dur) {};
#else

void firewall_cb()
{
    runningApp = firewall_app;
};

void connect4_cb()
{
    runningApp = connect4_Init;
};

void game_2048_cb()
{
    runningApp = game_2048_Run;
};

void bowling_cb() {
	setNote(173, 2048);
	runningApp = bowl_cb;
};

void hacker_cb() {
	setNote(173, 2048);
	runningApp = NULL;
};

void aliens_cb()
{ 
	setNote(173, 2048);
	runningApp = NULL;
};
#endif

const struct menu_t games_m[] = {
   {"Firewall",	GREEN_BG, FUNCTION, {(struct menu_t *)firewall_cb}}, /* coerce/cast to a menu_t data pointer */
   {"Bowling",	GREEN_BG, FUNCTION, {(struct menu_t *)bowling_cb}},
   {"Hacker",	GREEN_BG, FUNCTION, {(struct menu_t *)hacker_cb}},
   {"Aliens",	GREEN_BG, FUNCTION, {(struct menu_t *)aliens_cb}},
   {"2048",	GREEN_BG, FUNCTION, {(struct menu_t *)game_2048_cb}},
   {"Row 4",	GREEN_BG, FUNCTION, {(struct menu_t *)connect4_cb}},
//   {"more",	GREEN_BG, MORE, {NULL}},
   {"back",	GREEN_BG, BACK, {NULL}},
};

void led_cb()
{
	setNote(173, 2048);
	runningApp = NULL;
};

void contrast_cb()
{
	setNote(173, 2048);
	runningApp = NULL;
};

void timedate_cb()
{
	setNote(173, 2048);
	runningApp = update_time_Run;
};

void screensaver_cb()
{
	setNote(173, 2048);
	runningApp = NULL;
};

void backlight_cb()
{
	setNote(173, 2048);
	runningApp = NULL;
};

struct menu_t settings_m[] = {
   {"led",	GREEN_BG, FUNCTION, {(struct menu_t *)led_cb}},  /* coerce/cast to a menu_t data pointer */
   {"contrast",	GREEN_BG, FUNCTION, {(struct menu_t *)contrast_cb}},
   {"time-date",GREEN_BG, FUNCTION, {(struct menu_t *)timedate_cb}},
   {"screensaver",GREEN_BG, FUNCTION, {(struct menu_t *)screensaver_cb}},
   {"backlight",GREEN_BG, FUNCTION, {(struct menu_t *)backlight_cb}},
   {"back",	GREEN_BG, BACK, {NULL}},
};

struct menu_t achievements_m[] = {
   {"back", GREEN_BG, BACK, {NULL}},
};

struct menu_t main_m[] = {
   {"Schedule",    GREEN_BG, MENU, {schedule_m}},
   {"Games",       GREEN_BG, MENU, {games_m}},
   {"Settings",    GREEN_BG, MENU, {settings_m}},
   {"Achievments", GREEN_BG, MENU, {achievements_m}},
   {"", GREEN_BG, BACK, {NULL}}
} ;

#ifdef MAINMENU
void (*runningApp)() = NULL;
#else
//void (*runningApp)() = splash_Init;
void (*runningApp)() = NULL;
#endif

/*
usage case
*/

#define MORE_INC 4

#define MAX_MENU_DEPTH 8
static unsigned char G_menuCnt=0; // index for G_menuStack
struct menu_t *G_menuStack[MAX_MENU_DEPTH] = { 0 }; // track user traversing menus

/*

cc -m32 -g -O0 -I. -c Stages15/bowl.c
cc -m32 -g -O0 -c menu.c -DMAINMENU
cc -m32 -o menu bowl.o menu.o


*/

#ifdef MAINMENU
#include <stdio.h>

void red(unsigned char onPWM) {};
void green(unsigned char onPWM) {};
void blue(unsigned char onPWM) {};
void genericMenu(struct menu_t *L_menu) {};
void clearscreen(unsigned short color) {};

void add_to_display_list(unsigned char ResourceType,
        unsigned short color_picID,                 
        unsigned char x1, unsigned char y1,
        unsigned char x2_width_charVal, unsigned char y2_height) {};

void show_pic(unsigned char picId,
              unsigned char x,
              unsigned char y) {};

void returnToMenus() {};

char spaces[16] = "                ";

struct menu_t *G_menu = NULL ; // current menu item in stack

/* traverse menus */
void main()
{
   void dump_menu(struct menu_t *L_menu);
   extern const struct menu_t instructions_m;

   dump_menu(main_m);
   dump_menu(&instructions_m);
}

void dump_menu(struct menu_t *L_menu) {
   struct menu_t *sub_menu;
   unsigned char depth=0;
   static unsigned char L_menuCnt=0; // index for G_menuStack
   static struct menu_t *L_menuStack[4] = { 0 }; // track user traversing menus


   while(1) {
      printf("%s %s data %x ", &spaces[15-depth], L_menu->name, L_menu);
      fflush(stdout);

      switch (L_menu->type) {
          case MORE: /* display next page of menu */
            printf("more\n");
            fflush(stdout);
            //L_menu += MORE_INC;
            L_menu++;
            break;

          case BACK: /* return from menu */
            /* if depth is already zero then we are at the end of main menu */
            if (depth == 0) return;

            depth--;
            printf("back depth = %d\n", depth);
            fflush(stdout);
            if (L_menuCnt > 0) L_menuCnt--; /* check for menu stack underflow should assert it > 0 */
            L_menu = L_menuStack[L_menuCnt] ;

            break;

          case TEXT: /* display some text (clock perhaps) */
            printf("%s\n", L_menu->name);
            fflush(stdout);
            L_menu++;
            break;

          case MENU: /* drill down into menu */
            printf("menu\n");
            fflush(stdout);
            sub_menu = L_menu->data.menu; /* go into this menu */
            L_menu++;                        /* advance past this item so when we return we are position for next loop */
            L_menuStack[L_menuCnt++] = L_menu; /* push onto stack  */
            if (L_menuCnt == MAX_MENU_DEPTH) L_menuCnt--; /* too deep, undo */

            L_menu = sub_menu; /* go into menu */
            depth++;
            break;

          case FUNCTION: /* call the function pointer */
            printf("function\n");
            fflush(stdout);
            (*L_menu->data.func)();
            L_menu++;
            break;

          default:
            break;
      }
   }
}
#endif

#ifndef MAINMENU


/*
  NOTE- the LCD is running in mirrored mode on the X axis

  When oriented with USB connector at top it looks like this:
  128 .... 64 ... 0

  so everything drawn in increasing order goes right to left

  currently the char routine draws Y in decreasing (up), 
  so 1st Y position has to offset down CHAR_HEIGHT to account for that
*/

extern unsigned char G_font;
#define CHAR_WIDTH assetList[G_font].x
#define CHAR_HEIGHT 8
#define SCAN_BLANK 1 /* blank lines between text entries */
#define TEXTRECT_OFFSET 1 /* text offset within rectangle */

void display_menu(struct menu_t *menu, struct menu_t *selected)
{
	static unsigned char cursor_x, cursor_y;
	unsigned char c;

	DlInit();
	DlBackgroundColor(0);              /* black */
	DlClear(); 

	DlTransparentIndex(256); /* turn off transparency */

	DlMove(0, 0);
	DlPicture(KITTEN, 0);

	DlColor(WHITE);              /* black */

	cursor_x = 5;
	cursor_y = CHAR_HEIGHT;
	DlMove(cursor_x, cursor_y);

	while (1) {
		unsigned char rect_w=0;

		/* if name is NULL leave now */
		if (menu->name[0] == 0) break;

		for (c=0, rect_w=0; (menu->name[c] != 0); c++) rect_w += CHAR_WIDTH;

		DlColor(0b1111100000011111);
		DlMove(cursor_x, cursor_y);
		//DlFilledRectangle(rect_w, CHAR_HEIGHT + 2 * SCAN_BLANK);
		DlRectangle(rect_w, CHAR_HEIGHT + 2 * SCAN_BLANK);

		DlColor(((menu == selected) ? BLUE : GREEN));
		DlMove(cursor_x+1, cursor_y+1);
		DlWriteLine(menu->name);

		cursor_y += (CHAR_HEIGHT + 2 * SCAN_BLANK);

		/* last menu item quit */
		if (menu->type == BACK) break;

		menu++;
	}
	DlSwapBuffers();
}

/* for this increment the units are menu items */
#define PAGESIZE 8

struct menu_t *currMenu = NULL; /* init */
struct menu_t *selectedMenu = NULL; /* item the cursor is on */


/* 
   NOTE-
     apps will call this but since this returns to the callback
     code will execute up the the fuction return()
*/
void returnToMenus()
{
    if (currMenu == NULL) {
        G_menuStack[G_menuCnt] = main_m;
        currMenu = main_m;
        selectedMenu = currMenu;
    }

    display_menu(currMenu, selectedMenu);
    runningApp = NULL;
}

void menus()
{
    if (runningApp != NULL) {
            (*runningApp)();
            return;
    }

    if (currMenu == NULL) {
            G_menuStack[G_menuCnt] = main_m;
            currMenu = main_m;
            selectedMenu = currMenu;
            display_menu(currMenu, selectedMenu);
    }


    /* see if physical button has been clicked */
    //if ((sampleButtonStatus & BUTTON_MASK) && (buttonTimestamp[BUTTON] != last_buttonTimestmap)) {
    //	last_buttonTimestmap = buttonTimestamp[BUTTON];
    if (BUTTON_PRESSED_AND_CONSUME)
    {
            switch (selectedMenu->type) {

            case MORE: /* jump to next page of menu */
                    setNote(173, 2048); /* a */
                    currMenu += PAGESIZE;
                    selectedMenu = currMenu;
                    break;

            case BACK: /* return from menu */
                    setNote(154, 2048); /* b */
                    if (G_menuCnt == 0) return; /* stack is empty, error or main menu */
                    G_menuCnt--;
                    currMenu = G_menuStack[G_menuCnt] ;
                    selectedMenu = currMenu;
                    break;

            case TEXT: /* maybe highlight if clicked?? */
                    setNote(145, 2048); /* c */
                    break;

            case MENU: /* drills down into menu if clicked */
                    setNote(129, 2048); /* d */
                    G_menuStack[G_menuCnt++] = currMenu; /* push onto stack  */
                    if (G_menuCnt == MAX_MENU_DEPTH) G_menuCnt--; /* too deep, undo */
                    currMenu = (struct menu_t *)selectedMenu->data.menu; /* go into this menu */
                    selectedMenu = currMenu;
                    break;

            case FUNCTION: /* call the function pointer if clicked */
                    setNote(115, 2048); /* e */
                    runningApp = selectedMenu->data.func;
                    //(*selectedMenu->data.func)();
                    break;

            default:
                    break;
            }

            display_menu(currMenu, selectedMenu);
    }
    /* *** PEB ***** not convinced this should be an else
       both sliders can be pressed then second will never get handled
    */
    else if (TOP_SLIDE_AND_CONSUME) /* handle slider/soft button clicks */
    {
        setNote(109, 2048); /* f */

        /* make sure not on first menu item */
        if (selectedMenu > currMenu)
        {
            selectedMenu--;
            display_menu(currMenu, selectedMenu);
        }
    }
    /* *** PEB ***** not convinced this should be an else
       both sliders can be pressed then this one will never get handled
    */
    else if (BOTTOM_SLIDE_AND_CONSUME)
    {
        setNote(97, 2048); /* g */

        /* make sure not on last menu item */
        if (selectedMenu->type != BACK)
        {
            selectedMenu++;
            display_menu(currMenu, selectedMenu);
        }
    }
}

/*
  ripped from above for app menus
  this is not meant for persistant menus
  like the main menu
*/
void genericMenu(struct menu_t *L_menu)
{
    static struct menu_t *L_currMenu = NULL; /* LOCAL not to be confused to much with menu()*/
    static struct menu_t *L_selectedMenu = NULL; /* LOCAL ditto   "    "    */
    static unsigned char L_menuCnt=0; // index for G_menuStack
    static struct menu_t *L_menuStack[4] = { 0 }; // track user traversing menus

    if (L_menu == NULL) return; /* no thanks */

    if (L_currMenu == NULL) {
	L_menuCnt = 0;
	L_menuStack[L_menuCnt] = L_menu;
	L_currMenu = L_menu;
	L_selectedMenu = L_menu;

	display_menu(L_currMenu, L_selectedMenu);

	return;
    }

    if (BUTTON_PRESSED_AND_CONSUME)
    {
            switch (L_selectedMenu->type) {

            case MORE: /* jump to next page of menu */
                    setNote(173, 2048); /* a */
                    L_currMenu += PAGESIZE;
                    L_selectedMenu = L_currMenu;
                    break;

            case BACK: /* return from menu */
                    setNote(154, 2048); /* b */
                    if (L_menuCnt == 0) return; /* stack is empty, error or main menu */
                    L_menuCnt--;
                    L_currMenu = L_menuStack[L_menuCnt] ;
                    L_selectedMenu = L_currMenu;
		    display_menu(L_currMenu, L_selectedMenu);
                    break;

            case TEXT: /* maybe highlight if clicked?? */
                    setNote(145, 2048); /* c */
                    break;

            case MENU: /* drills down into menu if clicked */
                    setNote(129, 2048); /* d */
                    L_menuStack[L_menuCnt++] = L_currMenu; /* push onto stack  */
                    if (L_menuCnt == MAX_MENU_DEPTH) L_menuCnt--; /* too deep, undo */
                    L_currMenu = (struct menu_t *)L_selectedMenu->data.menu; /* go into this menu */
                    L_selectedMenu = L_currMenu;
		    display_menu(L_currMenu, L_selectedMenu);
                    break;

            case FUNCTION: /* call the function pointer if clicked */
                    setNote(115, 2048); /* e */
                    (*L_selectedMenu->data.func)();

		    /* clean up for nex call back */
		    L_currMenu = NULL;
		    L_selectedMenu = NULL;

		    L_menuCnt = 0;
		    L_menuStack[L_menuCnt] = NULL;
                    break;

            default:
                    break;
            }
    }
    else if (TOP_SLIDE_AND_CONSUME) /* handle slider/soft button clicks */
    {
        setNote(109, 2048); /* f */

        /* make sure not on first menu item */
        if (L_selectedMenu > L_currMenu)
        {
            L_selectedMenu--;
            display_menu(L_currMenu, L_selectedMenu);
        }
    }
    else if (BOTTOM_SLIDE_AND_CONSUME)
    {
        setNote(97, 2048); /* g */

        /* make sure not on last menu item */
        if (L_selectedMenu->type != BACK)
        {
            L_selectedMenu++;
            display_menu(L_currMenu, L_selectedMenu);
        }
    }
}
#endif
