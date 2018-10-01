#include <plib.h>
#include "flash.h"
#include "menu.h"
#include "settings.h"
#include "colors.h"
#include "ir.h"
#include "assetList.h"
#include "touchCTMU.h"
#include "fb.h"
#include "apps/conductor.h"
#include "apps/blinkenlights.h"
#include "apps/QC.h"
#include "apps/messenger.h"
#include "apps/lander.h"
#include "apps/firewall_ng.h"
#include "tests.h"
#include "apps/crossHair.h"
#include "apps/badgeinvaders.h"
#include "apps/pong.h"
#include "CTF.h"


/** @file
 *  simple menu system
 *  @author Paul Bruggeman
 *  paul@Killercats.com
 */


#ifdef MAINMENU

void hacker_cb() {
};

void setNote(unsigned short freq, unsigned short dur) {
};
#else

struct menuStack_t {
   struct menu_t *selectedMenu;
   struct menu_t *currMenu;
};

#define MAX_MENU_DEPTH 8
static unsigned char G_menuCnt=0; // index for G_menuStack

struct menuStack_t G_menuStack[MAX_MENU_DEPTH] = { {0,0} }; // track user traversing menus
struct menu_t *G_selectedMenu = NULL; /* item the cursor is on */
struct menu_t *G_currMenu = NULL; /* init */

#define MAIN_MENU_BKG_COLOR GREY2
#define MAIN_MENU_SCRATCH_X 3
#define MAIN_MENU_SCRATCH_Y 42
#define MAIN_MENU_SCRATCH_WIDTH 120
#define MAIN_MENU_SCRATCH_HEIGHT 83

void clear_menu_scratch_area(){
    FbColor(MAIN_MENU_BKG_COLOR);
    FbMove(MAIN_MENU_SCRATCH_X, MAIN_MENU_SCRATCH_Y);
    FbFilledRectangle(MAIN_MENU_SCRATCH_WIDTH, MAIN_MENU_SCRATCH_HEIGHT);
}

#define MENU_ANIM_LINE_COLOR CYAN
void main_menu_animator(){
    static unsigned short anim_cnt = 0;
    static unsigned char iter=0-1;

    // longer delay between draws as animation goes on
    if(anim_cnt++ < (6000))// + (5000 * (iter + 1))))
        return;
    else
        anim_cnt = 0;

    iter++;

    switch(iter){
        case 0:
            clear_menu_scratch_area();
            break;
        case 5:
            FbColor(MENU_ANIM_LINE_COLOR);
            FbHorizontalLine(62, 80,
                             66, 80);
        case 4:
            FbColor(MENU_ANIM_LINE_COLOR);
            FbHorizontalLine(MAIN_MENU_SCRATCH_X+50, 90,
                             MAIN_MENU_SCRATCH_X+MAIN_MENU_SCRATCH_WIDTH-49, 90);
        case 3:
            FbColor(MENU_ANIM_LINE_COLOR);
            FbHorizontalLine(MAIN_MENU_SCRATCH_X+34, 100,
                             MAIN_MENU_SCRATCH_X+MAIN_MENU_SCRATCH_WIDTH-33, 100);
        case 2:
            FbColor(MENU_ANIM_LINE_COLOR);
            FbHorizontalLine(MAIN_MENU_SCRATCH_X+18, 110,
                             MAIN_MENU_SCRATCH_X+MAIN_MENU_SCRATCH_WIDTH-17, 110);
        case 1:
            FbColor(MENU_ANIM_LINE_COLOR);
            FbHorizontalLine(MAIN_MENU_SCRATCH_X+2, 120,
                             MAIN_MENU_SCRATCH_X+MAIN_MENU_SCRATCH_WIDTH-1, 120);
            break;

        default:
            iter = 0 - 1;
    }

}

extern unsigned char new_message;
extern char incoming_message[];
extern unsigned char msg_relay_remaining;

#define RELAY_WAIT 300000
unsigned int long_relay_counter = RELAY_WAIT;

unsigned char redraw_main_menu = 0;

void clear_msg_cb(){
    unsigned char i = 0;
    for(i=0; i < MAX_MSG_LENGTH; i++)
        incoming_message[i] = 0;
    
    new_message = 0;
    msg_relay_remaining = 0;
    // Resets this methods internal static counters
    msg_send_msg(incoming_message, 0, 1);

    G_selectedMenu = NULL;
    returnToMenus();

}

void splash_cb() {
    extern void LCDBars();
    static unsigned short wait = 0;

    if (wait == 0) {
        LCDblack();
    }

    wait++;

    if (wait == 100) LCDBars();
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

void ctf_cb()
{
    runningApp = ctf_app;
}

#endif

const struct menu_t games_m[] = {
    {"Blinkenlights", VERT_ITEM | DEFAULT_ITEM, FUNCTION, {(struct menu_t *) blinkenlights_cb}}, // Set other badges LED
    {"Conductor", VERT_ITEM, FUNCTION, {(struct menu_t *) conductor_cb}}, // Tell other badges to play notes
    {"CTF",	VERT_ITEM, FUNCTION, {(struct menu_t *)ctf_cb}},
    {"Firewall", VERT_ITEM, FUNCTION, {(struct menu_t *) firewall_ng_cb}}, // Battleship clone
    {"Cross hair", VERT_ITEM, FUNCTION, {(struct menu_t *)cross_cb}},
    {"Badge Invaders", VERT_ITEM, FUNCTION, {(struct menu_t *)badgeinvaders_cb}},
    {"Pong", VERT_ITEM, FUNCTION, {(struct menu_t *)pong_game_menu_loop}},
    // Miner is just a fart in the wind right now
    //{"Miner", VERT_ITEM, FUNCTION, {(struct menu_t *)badge_miner_cb}},
    {"Lander", VERT_ITEM, FUNCTION, {(struct menu_t *)badge_lander_cb}},
    {"back", VERT_ITEM | LAST_ITEM, BACK, {NULL}},
};

struct menu_t settings_m[] = {
    {"my badgeId", VERT_ITEM, MENU,
        {(struct menu_t *) myBadgeid_m}},
    {"peer badgeId", VERT_ITEM, MENU,
        {(struct menu_t *) peerBadgeid_m}},
    {"time n date", VERT_ITEM | DEFAULT_ITEM, MENU,
        {(struct menu_t *) timedate_m}},
    {"rotate", VERT_ITEM, MENU,
        {(struct menu_t *) rotate_m}},
    {"backlight", VERT_ITEM, MENU,
        {(struct menu_t *) backlight_m}},
    {"led", VERT_ITEM, MENU,
        {(struct menu_t *) LEDlight_m}}, /* coerce/cast to a menu_t data pointer */
    {"buzzer", VERT_ITEM, MENU,
        {(struct menu_t *) buzzer_m}},
    {"slider", VERT_ITEM, MENU,
        {(struct menu_t *) slider_m}},
    {"tests", VERT_ITEM, MENU,
        {(struct menu_t *) tests_m}},
    {"Back", VERT_ITEM | LAST_ITEM, BACK,
        {NULL}},
};


extern struct menu_t conductor_config_m;

void send_sekrits() {
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_SEKRITS;
    pkt.p.badgeId = 0;
    IRqueueSend(pkt);
    returnToMenus();
}

extern rtccTime G_time;

void send_time() {
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_TIME;
    pkt.p.badgeId = 0; /* all */
    pkt.p.data = (((G_time.hour & 0xFF) << 8) | (G_time.min & 0xFF));

    IRqueueSend(pkt);
    returnToMenus();
}

extern rtccDate G_date;

void send_date() {
    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_DATEYYMM;
    pkt.p.badgeId = 0; /* all */
    pkt.p.data = (G_date.year << 4) | G_date.mon;
    IRqueueSend(pkt);
    returnToMenus();
}

struct menu_t main_m[] = {
   {"Games",       VERT_ITEM|DEFAULT_ITEM, MENU, 
        {games_m}},
   {"Messenger", VERT_ITEM, FUNCTION, 
        {(struct menu_t *)badge_messenger_cb}},
   {"Settings",    VERT_ITEM, MENU, 
        {settings_m}},
   {"Clear Message", VERT_ITEM|LAST_ITEM|HIDDEN_ITEM, FUNCTION, 
        {(struct menu_t *)clear_msg_cb}},
   {"", VERT_ITEM|LAST_ITEM|HIDDEN_ITEM, BACK, 
       {NULL}},
} ;

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

#define MORE_INC 4


struct menu_t *getSelectedMenu() {
    return G_selectedMenu;
}

struct menu_t *getCurrMenu() {
    return G_currMenu;
}

struct menu_t *getMenuStack(unsigned char item) {
    if (item > G_menuCnt) return 0;

    return G_menuStack[G_menuCnt - item].currMenu;
}

struct menu_t *getSelectedMenuStack(unsigned char item) {
    if (item > G_menuCnt) return 0;

    return G_menuStack[G_menuCnt - item].selectedMenu;
}

/*

cc -m32 -g -O0 -I. -c Stages15/bowl.c
cc -m32 -g -O0 -c menu.c -DMAINMENU
cc -m32 -o menu bowl.o menu.o


 */

#ifdef MAINMENU
#include <stdio.h>

void red(unsigned char onPWM) {
};

void green(unsigned char onPWM) {
};

void blue(unsigned char onPWM) {
};

void genericMenu(struct menu_t *L_menu) {
};

void clearscreen(unsigned short color) {
};

void add_to_display_list(unsigned char ResourceType,
        unsigned short color_picID,
        unsigned char x1, unsigned char y1,
        unsigned char x2_width_charVal, unsigned char y2_height) {
};

void show_pic(unsigned char picId,
        unsigned char x,
        unsigned char y) {
};

void returnToMenus() {
};

char spaces[16] = "                ";

struct menu_t *G_menu = NULL; // current menu item in stack

/* traverse menus */
void main() {
    void dump_menu(struct menu_t * L_menu);
    extern const struct menu_t instructions_m;

    dump_menu(main_m);
    dump_menu(&instructions_m);
}

void dump_menu(struct menu_t *L_menu) {
    struct menu_t *sub_menu;
    unsigned char depth = 0;
    static unsigned char L_menuCnt = 0; // index for G_menuStack
    static struct menu_t * L_menuStack[4] = {0}; // track user traversing menus


    while (1) {
        printf("%s %s data %x ", &spaces[15 - depth], L_menu->name, L_menu);
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
                L_menu = L_menuStack[L_menuCnt];

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
                L_menu++; /* advance past this item so when we return we are position for next loop */
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

  currently the char routine draws Y in decreasing (up),
  so 1st Y position has to offset down CHAR_HEIGHT to account for that
 */

unsigned char menu_left = 5;
const unsigned char menu_msg_alert[] = "New Message!";
/* these should all be variables or part of a theme */
#define MENU_LEFT menu_left
#define CHAR_WIDTH assetList[G_Fb.font].x
#define CHAR_HEIGHT 8
#define SCAN_BLANK 1 /* blank lines between text entries */
#define TEXTRECT_OFFSET 1 /* text offset within rectangle */

#define RGBPACKED(R,G,B) ( ((unsigned short)(R)<<11) | ((unsigned short)(G)<<6) | (unsigned short)(B) )

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
            FbClear();
            FbBackgroundColor(MAIN_MENU_BKG_COLOR);

            FbColor(GREEN);
            FbMove(2,5);
            FbRectangle(123, 120);

            FbColor(CYAN);
            FbMove(1,4);
            FbRectangle(125, 122);

            break;
        case WHITE_ON_BLACK:
            FbClear();
            FbBackgroundColor(BLACK);
            FbTransparentIndex(0);
            break;
        case BLANK:
            break;
    }

    cursor_x = MENU_LEFT;
    cursor_y = 2; // CHAR_HEIGHT;
    FbMove(cursor_x, cursor_y);

    while (1) {
        unsigned char rect_w=0;

        if (menu->attrib & HIDDEN_ITEM) {
            // don't jump out of the menu array if this is the last item!
            if(menu->attrib & LAST_ITEM)
                break;
            else
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
            case MAIN_MENU_STYLE:
                break;
            case WHITE_ON_BLACK:
                //FbMove(cursor_x, cursor_y);
                //FbFilledRectangle(rect_w, CHAR_HEIGHT + 2 * SCAN_BLANK);
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
            case MAIN_MENU_STYLE:
                if (menu == selected)
                {
                    FbColor(YELLOW);

                    FbMove(3, cursor_y+1);
                    FbFilledRectangle(2,8);

                    // Set the selected color for the coming writeline
                    FbColor(GREEN);
                }
                else
                    // unselected writeline color
                    FbColor(GREY16);
                break;
            case WHITE_ON_BLACK:
                FbColor((menu == selected) ? GREEN : WHITE);
                break;
            case BLANK:
                break;
        }


        FbMove(cursor_x+1, cursor_y+1);
        FbWriteLine(menu->name);
        cursor_x += (rect_w + CHAR_WIDTH);
        if (menu->attrib & LAST_ITEM) break;
            menu++;
    }// END WHILE

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
        if (menu->attrib & VERT_ITEM)
            cursor_y += (CHAR_HEIGHT + 2 * SCAN_BLANK);

        strcpy(out, time);
        strcat(out, date);


        if (!(menu->attrib & HORIZ_ITEM))
            cursor_x = MENU_LEFT;

    #define FUDGE 4
        FbColor(WHITE);
        FbMove(MENU_LEFT, 128 - (CHAR_HEIGHT + 2 * SCAN_BLANK) - FUDGE);
        FbWriteLine(out);
    }
    // Display the badge ID in the upper right hand corner of the main menu
    // - This condition doesn't pass on boot, which of these isn't True
    if((G_currMenu == main_m) && (runningApp == NULL || runningApp == splash_cb)){
        unsigned char bid[4] = {'0' + G_sysData.badgeId/100 % 10,
                                '0' + G_sysData.badgeId/10 % 10,
                                '0' + G_sysData.badgeId % 10,
                                0};
        FbColor(WHITE);
        FbMove(102,7);
        FbWriteLine(bid);

        FbColor(GREEN);
        FbMove(100,5);
        FbRectangle(25, 11);
    }

    // Display a notification to the user if they have a message
    // But only if they are in the main menu
    if(new_message && G_menuStack[G_menuCnt].currMenu == main_m && runningApp == NULL){
        FbMove(4, 59);
        FbColor(WHITE);
        FbFilledRectangle(95, 10);

        FbMove(5, 60);
        FbColor(BLACK);
        FbWriteWrappedLine(menu_msg_alert);

        FbMove(2, 69);
        FbColor(WHITE);
        FbRectangle(123, 56);

        FbMove(5, 71);
        FbColor(YELLOW);
        unsigned char c_str[MAX_MSG_LENGTH];
        // lol - map back to C chars so buffer code can map back to LCD chars :(
        map_to_c_chars(incoming_message, c_str, MAX_MSG_LENGTH, 0);

        FbWriteWrappedLine(c_str);

        if(msg_relay_remaining){
            unsigned char tmp_relay = '0' + msg_relay_remaining%10;
            FbMove(103, 59);
            FbColor(RED);
            FbFilledRectangle(10, 10);

            FbMove(106, 59);

            FbColor(WHITE);
            FbCharacter(tmp_relay);
        }

        red(20);
        green(100);
        blue(20);
        main_m[3].attrib = VERT_ITEM;
    }
    else if(G_menuStack[G_menuCnt].currMenu == main_m){
        main_m[3].attrib = VERT_ITEM|LAST_ITEM|HIDDEN_ITEM;
        red(0);
        green(0);
        blue(0);
    }

    /* in case last menu item is a skip */
    if (selected == NULL) selected = root_menu;

    return (selected);
}

/* for this increment the units are menu items */
#define PAGESIZE 8

void closeMenuAndReturn() {
    if (G_menuCnt == 0) return; /* stack is empty, error or main menu */
    G_menuCnt--;
    G_currMenu = G_menuStack[G_menuCnt].currMenu;
    G_selectedMenu = G_menuStack[G_menuCnt].selectedMenu;

    G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    runningApp = NULL;
}

/*
   NOTE-
     apps will call this but since this returns to the callback
     code will execute up the the fuction return()
 */
void returnToMenus() {
    if (G_currMenu == NULL) {
        G_currMenu = main_m;
        G_selectedMenu = NULL;
        G_menuStack[G_menuCnt].currMenu = G_currMenu;
        G_menuStack[G_menuCnt].selectedMenu = G_selectedMenu;
    }

    runningApp = NULL; //shouldn't this be set before calling disp menu?
    G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    redraw_main_menu = 1; //hack, sorry (ensures menus come up behind the animation)
}

void menus() {
    if (runningApp != NULL) { /* running app is set by menus() not genericMenus() */
        (*runningApp)();
        return;
    }
    // Only relay if not running an app (else if not really necessary due to return, but w/e)
    // -- App callbacks may take many times longer than reuglar menu updates
    // -- which will make the relay take forever, so just avoid it all together until idle in menus
    else if(new_message && msg_relay_remaining){ // we have relay work to do
        if(!long_relay_counter){ //  need to start/continue relay
            if(!msg_send_msg(incoming_message, msg_relay_remaining - 1, 0)){// returns 0 when message send is done
                redraw_main_menu = 1;
                msg_relay_remaining--;
                long_relay_counter = RELAY_WAIT;
            }
        }
        else
            long_relay_counter--;
    }
    // Only force a redraw under these conditions - other redraws are
    // in user input switch statement
    if (G_currMenu == NULL 
            || (redraw_main_menu && G_menuStack[G_menuCnt].currMenu == main_m)) {
            redraw_main_menu = 0;
        G_menuStack[G_menuCnt].currMenu = main_m;
        G_menuStack[G_menuCnt].selectedMenu = NULL;
        G_currMenu = main_m;
        //selectedMenu = G_currMenu;
        G_selectedMenu = NULL;
        G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
    }

    /* see if physical button has been clicked */
    if (BUTTON_PRESSED_AND_CONSUME) {
        // action happened that will result in menu redraw
        //do_animation = 1;
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
                G_currMenu = G_menuStack[G_menuCnt].currMenu;
                G_selectedMenu = G_menuStack[G_menuCnt].selectedMenu;
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
                G_currMenu = (struct menu_t *) G_selectedMenu->data.menu; /* go into this menu */
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
    }/* *** PEB ***** not convinced this should be an else
       both sliders can be pressed then second will never get handled
    */
    else if (TOP_TAP_AND_CONSUME) /* handle slider/soft button clicks */ {
        setNote(109, 800); /* f */

        /* make sure not on first menu item */
        if (G_selectedMenu > G_currMenu) {
            G_selectedMenu--;

            while (((G_selectedMenu->attrib & SKIP_ITEM) || (G_selectedMenu->attrib & HIDDEN_ITEM))
                    && G_selectedMenu > G_currMenu)
                G_selectedMenu--;

            G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
        }
    }/* *** PEB ***** not convinced this should be an else
       both sliders can be pressed then this one will never get handled
    */
    else if (BOTTOM_TAP_AND_CONSUME) {
        setNote(97, 1024); /* g */

        /* make sure not on last menu item */
        if (!(G_selectedMenu->attrib & LAST_ITEM)) {
            G_selectedMenu++;

            //Last item should never be a skipped item!!
            while (((G_selectedMenu->attrib & SKIP_ITEM) || (G_selectedMenu->attrib & HIDDEN_ITEM))
                    && (!(G_selectedMenu->attrib & LAST_ITEM)))
                G_selectedMenu++;

            // at this point, may be on last item, if it's hidden, back off of it
            if((G_selectedMenu->attrib & LAST_ITEM) && (G_selectedMenu->attrib & HIDDEN_ITEM))
                G_selectedMenu--;

            G_selectedMenu = display_menu(G_currMenu, G_selectedMenu, MAIN_MENU_STYLE);
        }
    }


    // Important: don't do any expensive calls when trying to handle IR crap
    // this animation is only for main menu
    if((G_currMenu == main_m) && !new_message && !msg_relay_remaining){
        main_menu_animator();
    }

    FbPushBuffer();
}

/*
  ripped from above for app menus
  this is not meant for persistent menus
  like the main menu
 */
void genericMenu(struct menu_t *L_menu, MENU_STYLE style) {
    static struct menu_t *L_currMenu = NULL; /* LOCAL not to be confused to much with menu()*/
    static struct menu_t *L_selectedMenu = NULL; /* LOCAL ditto   "    "    */
    static unsigned char L_menuCnt = 0; // index for G_menuStack
    static struct menu_t * L_menuStack[4] = {0}; // track user traversing menus

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

    if (BUTTON_PRESSED_AND_CONSUME) {
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
                L_currMenu = L_menuStack[L_menuCnt];
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
                L_currMenu = (struct menu_t *) L_selectedMenu->data.menu; /* go into this menu */
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
    } else if (TOP_TAP_AND_CONSUME) /* handle slider/soft button clicks */ {
        setNote(109, 2048); /* f */

        /* make sure not on first menu item */
        if (L_selectedMenu > L_currMenu) {
            L_selectedMenu--;

            while ((L_selectedMenu->attrib & SKIP_ITEM)
                    && L_selectedMenu > L_currMenu)
                L_selectedMenu--;

            L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);
        }
    } else if (BOTTOM_TAP_AND_CONSUME) {
        setNote(97, 2048); /* g */

        /* make sure not on last menu item */
        if (!(L_selectedMenu->attrib & LAST_ITEM)) {
            L_selectedMenu++;

            //Last item should never be a skipped item!!
            while (L_selectedMenu->attrib & SKIP_ITEM)
                L_selectedMenu++;



	        L_selectedMenu = display_menu(L_currMenu, L_selectedMenu, style);





        }
    }
}

#endif
