#include <plib.h>

#include "badge15.h"
#include "menu.h"
#include "settings.h"
//#include "Stages15/QC.h"
#include "Stages15/game2048.h"
#include "Stages15/connect4.h"
//#include "Stages15/bowl.h"
#include "Stages15/firewall.h"
#include "Stages15/update_time.h"
#include "Stages15/splash_screen.h"
//#include "Stages15/BSOD.h"
#include "Stages15/gop.h"
#include "Stages15/conductor.h"
#include "Stages15/blinkenlights.h"
#include "Stages15/pop_up.h"
#include "gameghost.h"


/*
   simple menu system
   Author: Paul Bruggeman
   paul@Killercats.com
*/
const char duwopydoo[] = "FLAG:H3x0rva";

void to_main_menu_cb();
void yummy_unlock_1();


const struct menu_t breakfast_m[] = {
   {"yummy", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)yummy_unlock_1}}, /* can init union either with or without {} */
   {"back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};


const struct menu_t day2_salons_p1_m[] = {
   {"10:10 Mark P.", VERT_ITEM, TEXT, {NULL}},
   {"11:00 -Break-", VERT_ITEM, TEXT, {NULL}},
   {"11:10 M.S. T.W", VERT_ITEM, TEXT, {NULL}},
   {"12:00 Lunch", VERT_ITEM, TEXT, {NULL}},
   {" 1:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {" 2:00 Elissa", VERT_ITEM, TEXT, {NULL}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};

const struct menu_t day1_salons_p2_m[] = {
   {" 3:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {" 4:00 Robert S.", VERT_ITEM, TEXT, {NULL}},
   {" 4:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};

const struct menu_t day1_salons_p1_m[] = {
   {"11:00 Barry K.", VERT_ITEM, TEXT, {NULL}},
   {"11:50 Lunch", VERT_ITEM, TEXT, {NULL}},
   {" 1:00 Allen H.", VERT_ITEM, TEXT, {NULL}},
   {" 1:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {" 2:00 Schuyler", VERT_ITEM, TEXT, {NULL}},
   {" 2:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {" 3:00 Adam C.", VERT_ITEM, TEXT, {NULL}},
   {"Next", VERT_ITEM|DEFAULT_ITEM, MENU, {day1_salons_p2_m}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};

const struct menu_t day2_ballroom_p2_m[] = {
   {"12:00 Lunch", VERT_ITEM, TEXT, {NULL}},
   {" 1:00 G-Man", VERT_ITEM, TEXT, {NULL}},
   {" 1:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {" 2:00 Borris S.", VERT_ITEM, TEXT, {NULL}},
   {" 2:50 -Break-", VERT_ITEM, TEXT, {NULL}},
   {" 3:00 Debates", VERT_ITEM, TEXT, {NULL}},
   {" 4:00 Closing", VERT_ITEM, TEXT, {NULL}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};

const struct menu_t day2_ballroom_p1_m[] = {
   {" 8:00 Breakfast", VERT_ITEM, MENU, {breakfast_m}}, // breakfast easter egg
   {" 8:50 Welcome", VERT_ITEM, TEXT, {NULL}},
   {" 9:00 Keynote", VERT_ITEM, TEXT, {NULL}},
   {"10:00 -Break-", VERT_ITEM, TEXT, {NULL}},
   {"10:10 David B.", VERT_ITEM, TEXT, {NULL}},
   {"11:00 -Break-", VERT_ITEM, TEXT, {NULL}},
   {"11:10 Bill W.", VERT_ITEM, TEXT, {NULL}},
   {"Next", VERT_ITEM|DEFAULT_ITEM, MENU, {day2_ballroom_p2_m}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};


const struct menu_t day1_ballroom_p3_m[] = {
   {" 4:00 David L.", VERT_ITEM, MENU, {NULL}},
   {" 4:50 -Break-", VERT_ITEM, MENU, {NULL}},
   {" 5:00 Jason S.", VERT_ITEM, MENU, {NULL}},
   {" 5:50 -Closing-", VERT_ITEM, MENU, {NULL}},
   {" 6:30 !Party!", VERT_ITEM, MENU, {NULL}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};


const struct menu_t day1_ballroom_p2_m[] = {
   {"11:50 -lunch-", VERT_ITEM, TEXT, {NULL}},
   {" 1:00 kizz m.", VERT_ITEM, TEXT, {NULL}},
   {" 1:50 -break-", VERT_ITEM, TEXT, {NULL}},
   {" 2:00 chill", VERT_ITEM, TEXT, {NULL}},
   {" 2:50 -break-", VERT_ITEM, TEXT, {NULL}},
   {" 3:00 P.H. D", VERT_ITEM, TEXT, {NULL}},
   {" 3:50 -break-", VERT_ITEM, TEXT, {NULL}},
   {"next", VERT_ITEM|DEFAULT_ITEM, MENU, {day1_ballroom_p3_m}},
   {"back", VERT_ITEM, BACK, {NULL}},
   {"exit", VERT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};


const struct menu_t day1_ballroom_p1_m[] = {
   {" 8:00 Breakfast", VERT_ITEM, MENU, {breakfast_m}},
   {" 9:00 Welcome", VERT_ITEM, TEXT, {NULL}},
   {" 9:10 Keynote", VERT_ITEM, TEXT, {NULL}},
   {"10:10 CTF Intro", VERT_ITEM, TEXT, {NULL}},
   {"10:20 Badges!", VERT_ITEM, TEXT, {NULL}},
   {"10:30 -Break-", VERT_ITEM, TEXT, {NULL}},
   {"11:00 Jason S.", VERT_ITEM, TEXT, {NULL}},
   {"Next", VERT_ITEM|DEFAULT_ITEM, MENU, {day1_ballroom_p2_m}},
   {"exit", VERT_ITEM|LAST_ITEM,FUNCTION, {(struct menu_t *)to_main_menu_cb}},
};


const struct menu_t schedule_m[] = {
    {"-Ballroom-", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
   {"  Day 1", VERT_ITEM|DEFAULT_ITEM, MENU, {day1_ballroom_p1_m}},
   {"  Day 2", VERT_ITEM, MENU, {day2_ballroom_p1_m}},
   {"", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
   {"-Salons-", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
   {"  Day 1", VERT_ITEM, MENU, {day1_salons_p1_m}},
   {"  Day 2", VERT_ITEM, MENU, {day2_salons_p1_m}},
   {"", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
   {"", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
   {"Exit", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

#ifdef MAINMENU
void firewall_cb() {};
void connect4_cb() {};
void game_2048_cb() {};
//void bowling_cb() {};
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
//	runningApp = bowl_cb;
};

void game_of_pwnz_cb()
{
	setNote(173, 2048);
	runningApp = gop_cb;
};

void ghost_cb()
{
    runningApp = ghost_app;
}

void splash_cb()
{
    static unsigned short wait=0;

    if (wait == 0) {
       DlInit();
       DlTransparentIndex(256);
       DlBackgroundColor(0);
       DlClear();
       DlMove(0, 2);
       DlPicture(RVASECBRAIN, 0);
       DlSwapBuffers();
    }

    wait++;

    if (wait == 700) red(100);
    if (wait == 1400) green(100);
    if (wait == 2100) blue(100);

    if (wait % 200 == 1) setNote((3000 - wait) >> 2, 2048);

    if (wait == 3000) {
	red(0);
	green(0);
	blue(0);
	returnToMenus();
    }
}

void audio_conductor_cb()
{
	setNote(173, 2048);
	runningApp = conductor_cb;
}
#endif

const struct menu_t games_m[] = {
   {"Game Of Pwnz",VERT_ITEM, FUNCTION, {(struct menu_t *)game_of_pwnz_cb}},
   //{"Firewall",	VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)firewall_cb}}, /* coerce/cast to a menu_t data pointer */
//   {"Bowling",	VERT_ITEM, FUNCTION, {(struct menu_t *)bowling_cb}},
   {"2048",	VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)game_2048_cb}},
   {"Row 4",	VERT_ITEM, FUNCTION, {(struct menu_t *)connect4_cb}},
   {"Ghost",	VERT_ITEM, FUNCTION, {(struct menu_t *)ghost_cb}},
//   {"more",	VERT_ITEM, MORE, {NULL}},
   {"back",	VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t settings_m[] = {
   {"my badgeId",	VERT_ITEM, MENU, {(struct menu_t *)myBadgeid_m}},
   {"peer badgeId",	VERT_ITEM, MENU, {(struct menu_t *)peerBadgeid_m}},
//   {"time-date",VERT_ITEM, FUNCTION, {(struct menu_t *)timedate_cb}},
   {"time n date",VERT_ITEM|DEFAULT_ITEM, MENU, {(struct menu_t *)timedate_m}},
   {"rotate",VERT_ITEM, MENU, {(struct menu_t *)rotate_m}},
   {"backlight",VERT_ITEM, MENU, {(struct menu_t *)backlight_m}},
   {"led",	VERT_ITEM, MENU, {(struct menu_t *)LEDlight_m}},  /* coerce/cast to a menu_t data pointer */
   {"buzzer",	VERT_ITEM, MENU, {(struct menu_t *)buzzer_m}},
   {"Back",	VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};


extern struct menu_t conductor_config_m;
void send_sekrits()
{
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_SEKRITS;
    pkt.p.badgeId = 0;
    IRqueueSend(pkt);
    returnToMenus();
}

extern rtccTime G_time ;
void send_time()
{
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_TIME;
    pkt.p.badgeId = 0; /* all */
    pkt.p.data = (((G_time.hour & 0xFF) << 8) | (G_time.min & 0xFF));

    IRqueueSend(pkt);
    returnToMenus();
}
extern rtccDate G_date ;
void send_date()
{
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_DATEYYMM;
    pkt.p.badgeId = 0; /* all */
    pkt.p.data = (G_date.year << 4) | G_date.mon;
    IRqueueSend(pkt);
    returnToMenus();
}

struct menu_t hax_m[] = {
#ifdef ALL_UNLOCKED
    {"Blinkenlights", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)blinkenlights_cb}}, // Set other badges LED
    {"Conductor", VERT_ITEM, FUNCTION, {(struct menu_t *)audio_conductor_cb}}, // Tell other badges to play notes
    {"Pop-Up", VERT_ITEM, FUNCTION, {(struct menu_t *)pop_up_cb}}, // Tell other badge to show sprite
#else
    {"Blinkenlights", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)blinkenlights_cb}}, // Set other badges LED
    {"Conductor", VERT_ITEM, FUNCTION, {(struct menu_t *)audio_conductor_cb}}, // Tell other badges to play notes
    {"Pop-Up", VERT_ITEM, FUNCTION, {(struct menu_t *)pop_up_cb}}, // Tell other badge to show sprite
#endif
    
#ifdef HACKRVA_GOON
   {"Sekrits", VERT_ITEM, FUNCTION, {(struct menu_t *)send_sekrits}}, // Tell other badge to show sprite
   {"Send Time", VERT_ITEM, FUNCTION, {(struct menu_t *)send_time}},
   {"Send Date", VERT_ITEM, FUNCTION, {(struct menu_t *)send_date}},
#endif

    {"back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t achievements_m[] = {
   {"back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t main_m[] = {
   {"Schedule",    VERT_ITEM, MENU, {schedule_m}},
   {"Games",       VERT_ITEM|DEFAULT_ITEM, MENU, {games_m}},
#ifdef ALL_UNLOCKED
   {"Settings",    VERT_ITEM, MENU, {settings_m}},
   {"Hax", VERT_ITEM|LAST_ITEM, MENU, {hax_m}}
#else
   {"Settings",    VERT_ITEM|LAST_ITEM, MENU, {settings_m}},
   {"Hax", VERT_ITEM|HIDDEN_ITEM, MENU, {hax_m}}
#endif
} ;

void yummy_unlock_1()
{
    if(main_m[3].attrib & HIDDEN_ITEM)
    {
        // unlock something
        main_m[3].attrib ^= HIDDEN_ITEM;
        main_m[3].attrib ^= LAST_ITEM;
        main_m[2].attrib ^= LAST_ITEM;
        //hax_m[1].attrib ^
    }
    returnToMenus();
}


//#define QC

#ifdef MAINMENU
void (*runningApp)() = NULL;
#else
 #ifdef QC
 void (*runningApp)() = QC_cb;
 #else
 void (*runningApp)() = splash_cb;
 #endif
#endif

/*
usage case
*/

#define MORE_INC 4

struct menuStack_t {
   struct menu_t *selectedMenu;
   struct menu_t *currMenu;
};

#define MAX_MENU_DEPTH 8
static unsigned char G_menuCnt=0; // index for G_menuStack

struct menuStack_t G_menuStack[MAX_MENU_DEPTH] = { {0,0} }; // track user traversing menus

struct menu_t *G_selectedMenu = NULL; /* item the cursor is on */
struct menu_t *G_currMenu = NULL; /* init */


void to_main_menu_cb()
{
    G_currMenu = NULL;
    returnToMenus();
}

struct menu_t *getSelectedMenu()
{
    return G_selectedMenu;
}

struct menu_t *getCurrMenu()
{
    return G_currMenu;
}

struct menu_t *getMenuStack(unsigned char item)
{
   if (item > G_menuCnt) return 0;

   return G_menuStack[G_menuCnt-item].currMenu;
}

struct menu_t *getSelectedMenuStack(unsigned char item)
{
   if (item > G_menuCnt) return 0;

   return G_menuStack[G_menuCnt-item].selectedMenu;
}

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
            (*L_menu->data.func)(L_Menu);
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
unsigned char menu_left=5;

/* these should all be variables or part of a theme */
#define MENU_LEFT menu_left
#define CHAR_WIDTH assetList[G_font].x
#define CHAR_HEIGHT 8
#define SCAN_BLANK 1 /* blank lines between text entries */
#define TEXTRECT_OFFSET 1 /* text offset within rectangle */

#define RGBPACKED(R,G,B) ( ((unsigned short)(R)<<11) | ((unsigned short)(G)<<6) | (unsigned short)(B) )
//char do_animation = 1;
//void main_menu_animator(void)
//{
//    static unsigned short anim_cnt = 0;
//    static unsigned char anim_tick = 0;
//
//    // only animate once in a while
//    if(anim_tick++ < 30)
//        return;
//    anim_tick = 0;
//
//    DlColor(GREEN);
//
//    // Draw growing lines
//    DlLine(42 - anim_cnt, 10, 70 + anim_cnt,10); // top line
//
//    DlLine(2, 50 - anim_cnt, 2, 80 + anim_cnt); // left side line
//    //DlLine(110,10, 120, 25); // connected bevel
//    //DlLine(115,10, 120, 18); // disjoint bevel
//    DlLine(120, 65 - anim_cnt, 120, 80 + anim_cnt); // right line
//    DlLine(42 - anim_cnt, 120, 80 + anim_cnt, 120); // bottom line
//
//    if(anim_cnt < 45)
//    {
//        anim_cnt += 4;
//    }
//    else
//    {
//        // reset the counter for later calls
//        anim_cnt = 0;
//
//        // Animaiton has finished
//        do_animation = 0;
//
//        // force redraw of the final menu
//        G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
//    }
//}

struct menu_t *display_menu(struct menu_t *menu,
                            struct menu_t *selected,
                            MENU_STYLE style)
{
        static unsigned char cursor_x, cursor_y;
	unsigned char c;
	struct menu_t *root_menu; /* keep a copy in case menu has a bad structure */

	root_menu = menu;

        switch (style)
        {
            //case MAIN_MENU_WITH_TIME_DATE_STYLE:
            case MAIN_MENU_STYLE:
                DlInit();
                DlClear();
                DlBackgroundColor(GREY2);
                DlColor(GREEN);

                // if animation done, draw the final product
                //if(!do_animation)
                {
                    DlLine(2,5, 125,5); // top line
                    DlLine(2,5, 2,125); // left side line
//                    DlLine(110,10, 120, 25); // connected bevel
//                    DlLine(115,10, 120, 17); // disjoint bevel
                    DlLine(125,5, 125,125); // right line
                    DlLine(2,125, 125,125); // bottom line
                }
                break;
            case DRBOB_MENU_STYLE:
                DlInit();
                DlClear();
                DlBackgroundColor(GREY4);

                DlTransparency(0);

                DlTransparentIndex(256);

                DlMove(0, 0);
                DlPicture(DRBOB, 0);

                /* this makes the character bg  transparent so the rectangle shows */
                DlTransparentIndex(0);
                DlBackgroundColor(BLUE);
                break;
            case WHITE_ON_BLACK:
                DlInit();
                DlClear();
                DlBackgroundColor(BLACK);
                DlTransparentIndex(0);
                break;
            case BLANK:
                break;
        }

	cursor_x = MENU_LEFT;
	cursor_y = CHAR_HEIGHT;
	DlMove(cursor_x, cursor_y);

	while (1) {
		unsigned char rect_w=0;

		if (menu->attrib & HIDDEN_ITEM) {
		    menu++;
		    continue;
		}

		for (c=0, rect_w=0; (menu->name[c] != 0); c++) rect_w += CHAR_WIDTH;

		if (menu->attrib & VERT_ITEM) 
			cursor_y += (CHAR_HEIGHT + 2 * SCAN_BLANK);

		if (!(menu->attrib & HORIZ_ITEM))
			cursor_x = MENU_LEFT;

                // extra decorations for menu items
                switch(style)
                {
                    //case MAIN_MENU_WITH_TIME_DATE_STYLE:
                    case MAIN_MENU_STYLE:
                        break;
                    case DRBOB_MENU_STYLE:
                        /* draw rectangle behind character */
                        DlTransparency(RGBPACKED(0b01111, 0b01111, 0b01111));
                        //DlColor(GREY4);
                        DlColor(RGBPACKED(0b01111, 0b01111, 0b01111));
                        DlMove(cursor_x, cursor_y);
                        DlFilledRectangle(rect_w, CHAR_HEIGHT + 2 * SCAN_BLANK);
                        //DlRectangle(rect_w, CHAR_HEIGHT + 2 * SCAN_BLANK);

                        /* draw highlighted */
                        DlTransparency(0);
                        break;
                    case WHITE_ON_BLACK:
                        //DlMove(cursor_x, cursor_y);
                        //DlFilledRectangle(rect_w, CHAR_HEIGHT + 2 * SCAN_BLANK);
                        break;
                    case BLANK:
                        break;
                }

		if (selected == menu) {
                    // If we happen to be on a skip ITEM, just increment off it
                    // The menus() method mostly avoids this, except for some cases
                    if (menu->attrib & SKIP_ITEM) selected++;
		}

		if (selected == NULL) {
		    if (menu->attrib & DEFAULT_ITEM) 
			selected = menu;
		}

                // Determine selected item color
                switch(style)
                {
                    //case MAIN_MENU_WITH_TIME_DATE_STYLE:
                    case MAIN_MENU_STYLE:
                        if (menu == selected)
                        {
                            DlColor(YELLOW);

                            DlMove(3, cursor_y+1);
                            DlFilledRectangle(2,8);

                            // Set the selected color for the coming writeline
                            DlColor(GREEN);
                        }
                        else
                            // unselected writeline color
                            DlColor(GREY16);
                        break;
                    case DRBOB_MENU_STYLE:
                        DlColor((menu == selected) ? BLUE : GREY8);
                        break;
                    case WHITE_ON_BLACK:
                        DlColor((menu == selected) ? GREEN : WHITE);
                        break;
                    case BLANK:
                        break;
                }
		
		DlMove(cursor_x+1, cursor_y+1);
		DlWriteLine(menu->name);

		cursor_x += (rect_w + CHAR_WIDTH);
		if (menu->attrib & LAST_ITEM) break;

		menu++;
	}
	if (G_menuStack[G_menuCnt].currMenu == settings_m) { /* need a setting to enable/disable */
	    unsigned char time[16], date[16], out[32];
	    extern rtccTime G_time;
	    extern rtccDate G_date;

	    getRTCC();
            time[0] = ((G_time.hour >> 4) & 0xF) + 48;
            time[1] =  (G_time.hour       & 0xF) + 48;
            time[2] = ':';
            time[3] = ((G_time.min >> 4)  & 0xF) + 48;
            time[4] =  (G_time.min        & 0xF) + 48;
            time[5] = ' ';
            time[6] = 0;

            date[0] = ((G_date.mon >> 4) & 0xF) + 48;
            date[1] =  (G_date.mon       & 0xF) + 48;
            date[2] = '-';
            date[3] = ((G_date.mday >> 4) & 0xF) + 48;
            date[4] =  (G_date.mday       & 0xF) + 48;
            date[5] = '-';
            date[6] = ((G_date.year >> 4) & 0xF) + 48;
            date[7] =  (G_date.year       & 0xF) + 48;
            date[8] = 0;

	    strcpy(out, time);
	    strcat(out, date);

#define FUDGE 4
            DlColor(WHITE);
	    DlMove(MENU_LEFT, 128 - (CHAR_HEIGHT + 2 * SCAN_BLANK) - FUDGE);
	    DlWriteLine(out);
	    //DlWriteLine(time);
	    //DlMove(132/2, 128 - (CHAR_HEIGHT + 2 * SCAN_BLANK));
	    //DlWriteLine(date);
        }
	DlSwapBuffers();

	/* in case last menu item is a skip */
        if (selected == NULL) selected = root_menu;

	return (selected);
}

/* for this increment the units are menu items */
#define PAGESIZE 8

void closeMenuAndReturn()
{
    if (G_menuCnt == 0) return; /* stack is empty, error or main menu */
    G_menuCnt--;
    G_currMenu = G_menuStack[G_menuCnt].currMenu ;
    G_selectedMenu = G_menuStack[G_menuCnt].selectedMenu ;

    G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    runningApp = NULL;
}

/* 
   NOTE-
     apps will call this but since this returns to the callback
     code will execute up the the fuction return()
*/
void returnToMenus()
{
    if (G_currMenu == NULL) {
        G_currMenu = main_m;
        G_selectedMenu = NULL;
        G_menuStack[G_menuCnt].currMenu = G_currMenu;
        G_menuStack[G_menuCnt].selectedMenu = G_selectedMenu;
    }

    G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    runningApp = NULL;
}

void menus()
{
    if (runningApp != NULL) { /* running app is set by menus() not genericMenus() */
            (*runningApp)();
            return;
    }

    if (G_currMenu == NULL) {
            G_menuStack[G_menuCnt].currMenu = main_m;
            G_menuStack[G_menuCnt].selectedMenu = NULL;
            G_currMenu = main_m;
            //selectedMenu = G_currMenu;
            G_selectedMenu = NULL;
            G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    }


    /* see if physical button has been clicked */
    //if ((sampleButtonStatus & BUTTON_MASK) && (buttonTimestamp[BUTTON] != last_buttonTimestmap)) {
    //	last_buttonTimestmap = buttonTimestamp[BUTTON];
    if (BUTTON_PRESSED_AND_CONSUME)
    {
        // action happened that will result in menu redraw
        // do_animation = 1;
            switch (G_selectedMenu->type) {

            case MORE: /* jump to next page of menu */
                    setNote(173, 2048); /* a */
                    G_currMenu += PAGESIZE;
                    G_selectedMenu = G_currMenu;
                    break;

            case BACK: /* return from menu */
		    setNote(154, 2048);
		    if (G_menuCnt == 0) return; /* stack is empty, error or main menu */
		    G_menuCnt--;
		    G_currMenu = G_menuStack[G_menuCnt].currMenu ;
		    G_selectedMenu = G_menuStack[G_menuCnt].selectedMenu ;
		    //G_selectedMenu = G_currMenu;
                    break;

            case TEXT: /* maybe highlight if clicked?? */
                    setNote(145, 2048); /* c */
                    break;

            case MENU: /* drills down into menu if clicked */
                    setNote(129, 2048); /* d */
                    G_menuStack[G_menuCnt].currMenu = G_currMenu; /* push onto stack  */
                    G_menuStack[G_menuCnt].selectedMenu = G_selectedMenu;
		    G_menuCnt++;
                    if (G_menuCnt == MAX_MENU_DEPTH) G_menuCnt--; /* too deep, undo */
                    G_currMenu = (struct menu_t *)G_selectedMenu->data.menu; /* go into this menu */
                    //selectedMenu = G_currMenu;
                    G_selectedMenu = NULL;
                    break;

            case FUNCTION: /* call the function pointer if clicked */
                    setNote(115, 2048); /* e */
                    runningApp = G_selectedMenu->data.func;
                    //(*selectedMenu->data.func)();
                    break;

            default:
                    break;
            }

            G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    }
    /* *** PEB ***** not convinced this should be an else
       both sliders can be pressed then second will never get handled
    */
    else if (TOP_TAP_AND_CONSUME) /* handle slider/soft button clicks */
    {
        setNote(109, 800); /* f */

        /* make sure not on first menu item */
        if (G_selectedMenu > G_currMenu)
        {
            G_selectedMenu--;

            while ( ((G_selectedMenu->attrib & SKIP_ITEM) || (G_selectedMenu->attrib & HIDDEN_ITEM))
                    && G_selectedMenu > G_currMenu)
                G_selectedMenu--;

            G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
        }
    }
    /* *** PEB ***** not convinced this should be an else
       both sliders can be pressed then this one will never get handled
    */
    else if (BOTTOM_TAP_AND_CONSUME)
    {
        setNote(97, 1024); /* g */


        /* make sure not on last menu item */
        if (!(G_selectedMenu->attrib & LAST_ITEM))
        {
            G_selectedMenu++;

            //Last item should never be a skipped item!!
            while ( ((G_selectedMenu->attrib & SKIP_ITEM) || (G_selectedMenu->attrib & HIDDEN_ITEM))
                    && (!(G_selectedMenu->attrib & LAST_ITEM)) ) 
                G_selectedMenu++;

            G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
        }
    }

//    if(do_animation)
//        main_menu_animator();
}

/*
  ripped from above for app menus
  this is not meant for persistant menus
  like the main menu
*/
void genericMenu(struct menu_t *L_menu, MENU_STYLE style)
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
	//L_selectedMenu = L_menu;
	L_selectedMenu = NULL;
	L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);
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
		    L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);
                    break;

            case TEXT: /* maybe highlight if clicked?? */
                    setNote(145, 2048); /* c */
                    break;

            case MENU: /* drills down into menu if clicked */
                    setNote(129, 2048); /* d */
                    L_menuStack[L_menuCnt++] = L_currMenu; /* push onto stack  */
                    if (L_menuCnt == MAX_MENU_DEPTH) L_menuCnt--; /* too deep, undo */
                    L_currMenu = (struct menu_t *)L_selectedMenu->data.menu; /* go into this menu */
                    //L_selectedMenu = L_currMenu;
                    L_selectedMenu = NULL;
		    L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);
                    break;

            case FUNCTION: /* call the function pointer if clicked */
                    setNote(115, 2048); /* e */
                    (*L_selectedMenu->data.func)(L_selectedMenu);

		    /* clean up for nex call back */
		    L_menu = NULL;
		    L_currMenu = NULL;
		    L_selectedMenu = NULL;

		    L_menuCnt = 0;
		    L_menuStack[L_menuCnt] = NULL;
                    break;

            default:
                    break;
            }
	    // L_selectedMenu = display_menu(L_currMenu, L_selectedMenu);
    }
    else if (TOP_TAP_AND_CONSUME) /* handle slider/soft button clicks */
    {
        setNote(109, 2048); /* f */

        /* make sure not on first menu item */
        if (L_selectedMenu > L_currMenu)
        {
            L_selectedMenu--;

            while ((L_selectedMenu->attrib & SKIP_ITEM)
                    && L_selectedMenu > L_currMenu)
                L_selectedMenu--;

	    L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);
        }
    }
    else if (BOTTOM_TAP_AND_CONSUME)
    {
        setNote(97, 2048); /* g */

        /* make sure not on last menu item */
        if (!(L_selectedMenu->attrib & LAST_ITEM))
        {
            L_selectedMenu++;

            //Last item should never be a skipped item!!
            while (L_selectedMenu->attrib & SKIP_ITEM)
                L_selectedMenu++;

	    L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);
        }
    }
}

#endif
