#include <plib.h>
#include "string.h"
#include "flash.h"
#include "menu.h"
#include "touchCTMU.h"
#include "stopwatch.h"
#include "display.h"

extern rtccTime G_time;
extern rtccDate G_date;


/*
   Time And Date
   Author: Paul Bruggeman
   paul@Killercats.com
*/

void hours_cb(struct menu_t *h) {
   void setHour(unsigned char hour);
   struct menu_t *selectedMenu;
   unsigned char hours;

   selectedMenu = getSelectedMenu();
   hours = ((selectedMenu->name[0] - 48) << 4) | (selectedMenu->name[1] - 48) ;
   G_time.hour = hours;
   setRTCC();

   closeMenuAndReturn();
}

const struct menu_t hours_m[] = {
    {"01", 1|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"02", 2|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"03", 3|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"04", 4|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"05", 5|VERT_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"06", 6|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"07", 7|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"08", 8|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"09", 9|VERT_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"10",10|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"11",11|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"12",12|HORIZ_ITEM, FUNCTION, {(struct menu_t *)hours_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

void minsTens_cb(struct menu_t *m) {
   struct menu_t *minsTensMenu, *minsOnesMenu, *selectedMenu;
   unsigned char mins;
   void setMinute(unsigned char min);

   minsTensMenu = minsOnesMenu = getSelectedMenuStack(1); /* minsOnes_m[] */
   minsOnesMenu++; /* mins are after ones in the array */
   selectedMenu = getSelectedMenu(); /*  minsOnes_m[selected] */

   /* update calling menu's name field */
   strcpy(minsTensMenu->name, selectedMenu->name);

   mins = ((minsTensMenu->name[0] - 48) << 4) | (minsOnesMenu->name[0] - 48) ;
   G_time.min = mins;
   setRTCC();

   closeMenuAndReturn();
}


void minsOnes_cb(struct menu_t *m) {
   struct menu_t *minsTensMenu, *minsOnesMenu, *selectedMenu;
   unsigned char mins;
   void setMinute(unsigned char min);

   minsTensMenu = minsOnesMenu = getSelectedMenuStack(1); /* minsOnes_m[] */
   minsTensMenu--; /* tens are before ones in the array */
   selectedMenu = getSelectedMenu(); /*  minsOnes_m[selected] */

   /* update calling menu's name field */
   strcpy(minsOnesMenu->name, selectedMenu->name);

   mins = ( ((minsTensMenu->name[0] - 48) & 0xF) << 4) | ((minsOnesMenu->name[0] - 48) & 0xF) ;
   G_time.min = mins;
   setRTCC();

   closeMenuAndReturn();
}


const struct menu_t minsTens_m[] = {
    {"0", 0|VERT_ITEM, FUNCTION, {(struct menu_t *)minsTens_cb} },
    {"1", 1|VERT_ITEM, FUNCTION, {(struct menu_t *)minsTens_cb} },
    {"2", 2|VERT_ITEM, FUNCTION, {(struct menu_t *)minsTens_cb} },
    {"3", 3|VERT_ITEM, FUNCTION, {(struct menu_t *)minsTens_cb} },
    {"4", 4|VERT_ITEM, FUNCTION, {(struct menu_t *)minsTens_cb} },
    {"5", 5|VERT_ITEM, FUNCTION, {(struct menu_t *)minsTens_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

const struct menu_t minsOnes_m[] = {
    {"0", 0|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"1", 1|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"2", 2|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"3", 3|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"4", 4|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"5", 5|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"6", 6|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"7", 7|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"8", 8|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"9", 9|VERT_ITEM, FUNCTION, {(struct menu_t *)minsOnes_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

struct menu_t mins_m[] = {
    {"0", 0|VERT_ITEM, MENU, {(struct menu_t *)minsTens_m} },
    {"0", 1|HORIZ_ITEM, MENU, {(struct menu_t *)minsOnes_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

/*
void AMPM_cb(struct menu_t *ampm) {
   struct menu_t *dstMenu, *selectedMenu;

   dstMenu = getSelectedMenuStack(1);
   selectedMenu = getSelectedMenu();

   // update calling menu's name field

   strcpy(dstMenu->name, selectedMenu->name);

   closeMenuAndReturn();
}


const struct menu_t AMPM_m[] = {
    {"AM", 0|VERT_ITEM, FUNCTION, {(struct menu_t *)AMPM_cb} },
    {"PM", 1|VERT_ITEM, FUNCTION, {(struct menu_t *)AMPM_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};
*/

struct menu_t time_m[] = {
    {"Hour",   VERT_ITEM|DEFAULT_ITEM,     MENU, {(struct menu_t *)hours_m} },
    {"Min",   HORIZ_ITEM,     MENU, {(struct menu_t *)mins_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};


void year_cb(struct menu_t *year_m) {
   closeMenuAndReturn();
}

struct menu_t year_m[] = {
    {"2015", VERT_ITEM,  FUNCTION, {(struct menu_t *)year_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

void month_cb(struct menu_t *month_m) {
   struct menu_t *monthMenu, *selectedMenu;
   unsigned char month;

   monthMenu = getSelectedMenuStack(1);
   selectedMenu = getSelectedMenu(); /*  minsOnes_m[selected] */

   /* update calling menu's name field */
   strcpy(monthMenu->name, selectedMenu->name);

   month = ( ((selectedMenu->name[0] - 48) & 0xF) << 4) | ((selectedMenu->name[1] - 48) & 0xF) ;

   G_date.mon = month;
   setRTCC();

   closeMenuAndReturn();
}

struct menu_t month_m[] = {
    {"01",   1|VERT_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"02",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"03",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"04",   1|VERT_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"05",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"06",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"07",   1|VERT_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"08",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"09",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"10",   1|VERT_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"11",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"12",   1|HORIZ_ITEM,   FUNCTION, {(struct menu_t *)month_cb} },
    {"Back", 1|VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

void day_cb(struct menu_t *day_m) {
   struct menu_t *dayMenu, *selectedMenu;
   unsigned char day;

   dayMenu = getSelectedMenuStack(1);
   selectedMenu = getSelectedMenu(); /*  minsOnes_m[selected] */

   /* update calling menu's name field */
   strcpy(dayMenu->name, selectedMenu->name);

   day = ( ((selectedMenu->name[0] - 48) & 0xF) << 4) | ((selectedMenu->name[1] - 48) & 0xF) ;

   G_date.mday = day;
   setRTCC();

   closeMenuAndReturn();

}

struct menu_t day_m[] = {
    {"01",    1|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"02",    2|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"03",    3|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"04",    4|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"05",    5|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"06",    6|VERT_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"07",    7|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"08",    8|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"09",    9|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"10",   10|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"11",   11|VERT_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"12",   12|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"13",   13|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"14",   14|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"15",   15|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"16",   16|VERT_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"17",   17|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"18",   18|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"19",   19|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"20",   20|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"21",   21|VERT_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"22",   22|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"23",   23|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"24",   24|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"25",   25|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"26",   26|VERT_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"27",   27|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"28",   28|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"29",   29|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"30",   30|HORIZ_ITEM, FUNCTION, {(struct menu_t *)day_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

struct menu_t date_m[] = {
    {"06",   VERT_ITEM,      		MENU, {month_m} },
    {"-",    HORIZ_ITEM|SKIP_ITEM,      TEXT, {0} },
    {"05",   HORIZ_ITEM,     		MENU, {day_m} },
    {"-",    HORIZ_ITEM|SKIP_ITEM,     TEXT, {0} },
    {"2015", HORIZ_ITEM,     		MENU, {year_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

const struct menu_t timedate_m[] = {
    {"Set Time", VERT_ITEM, MENU, {time_m} },
    {"Set Date", VERT_ITEM, MENU, {date_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

void touchdebug_cb(struct menu_t *m) {
    char ghost_tmp_string[10];
    char temp_var = 0;

    if(g_blinktimer==0){
        FbInit();
        FbTransparentIndex(0);
        FbBackgroundColor(BLACK);
        FbClear();
        display_movecursor(0,0);
        display_writeline("This is a test.", GREEN);

        display_movecursor(0,1);
        sprintf(ghost_tmp_string,"%03u",sample[0].ButtonVmeasADC[0]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(7,1);
        sprintf(ghost_tmp_string,"%03u",sample[1].ButtonVmeasADC[0]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(14,1);
        sprintf(ghost_tmp_string,"%03u",sample[2].ButtonVmeasADC[0]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(0,2);
        sprintf(ghost_tmp_string,"%03u",sample[0].ButtonVmeasADC[1]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(7,2);
        sprintf(ghost_tmp_string,"%03u",sample[1].ButtonVmeasADC[1]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(14,2);
        sprintf(ghost_tmp_string,"%03u",sample[2].ButtonVmeasADC[1]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(0,3);
        sprintf(ghost_tmp_string,"%03u",sample[0].ButtonVmeasADC[2]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(7,3);
        sprintf(ghost_tmp_string,"%03u",sample[1].ButtonVmeasADC[2]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(14,3);
        sprintf(ghost_tmp_string,"%03u",sample[2].ButtonVmeasADC[2]);
        display_writeline(ghost_tmp_string, GREEN);



        display_movecursor(0,4);
        sprintf(ghost_tmp_string,"%03u",sample[0].ButtonVmeasADC[3]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(7,4);
        sprintf(ghost_tmp_string,"%03u",sample[1].ButtonVmeasADC[3]);
        display_writeline(ghost_tmp_string, GREEN);

        display_movecursor(14,4);
        sprintf(ghost_tmp_string,"%03u",sample[2].ButtonVmeasADC[3]);
        display_writeline(ghost_tmp_string, GREEN);

        g_blinktimer = 5; //Half a second
    }

    if(BUTTON_PRESSED_AND_CONSUME){
        closeMenuAndReturn();
    }
}


const struct menu_t slider_m[] = {
    //{"Debug",    1|HORIZ_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)touchdebug_cb} },
    {"Debug",    1|VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)touchdebug_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },

};



/*
    set badge Id
*/
void myBadgeid_cb(struct menu_t *h) {
   struct menu_t *selectedMenu;
   extern const char hextab[];

   //dstMenu = getSelectedMenuStack(1);
   selectedMenu = getSelectedMenu();

   selectedMenu->name[0] = hextab[((G_sysData.badgeId >> 12) & 0xF)];
   selectedMenu->name[1] = hextab[((G_sysData.badgeId >>  8) & 0xF)];
   selectedMenu->name[2] = hextab[((G_sysData.badgeId >>  4) & 0xF)];
   selectedMenu->name[3] = hextab[((G_sysData.badgeId      ) & 0xF)];
   selectedMenu->name[4] = 0;
   //strcpy(dstMenu->name, selectedMenu->name);

   returnToMenus();
}

struct menu_t myBadgeid_m[] = {
    {"check",   VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)myBadgeid_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

/*
    peer we want to talk to
*/
void peerBadgeid_cb(struct menu_t *h) {
   struct menu_t *dstMenu, *selectedMenu;

   dstMenu = getSelectedMenuStack(1);
   selectedMenu = getSelectedMenu();

   /* update calling menu's name field */
   G_peerBadgeId = selectedMenu->attrib & 0xFF;

   strcpy(dstMenu->name, selectedMenu->name);

   closeMenuAndReturn();
}


const struct menu_t peerBadgeNum_m[] = {
    {"ALL", 0|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  1", 1|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  2", 2|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  3", 3|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  4", 4|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  5", 5|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  6", 6|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  7", 7|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"  8", 8|VERT_ITEM, FUNCTION, {(struct menu_t *)peerBadgeid_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

struct menu_t peerBadgeid_m[] = {
    {"0",   VERT_ITEM|DEFAULT_ITEM, MENU, {peerBadgeNum_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};


/*
    backlight
*/
void backlight_cb(struct menu_t *h) {
   struct menu_t *dstMenu, *selectedMenu;

   dstMenu = getSelectedMenuStack(1);
   selectedMenu = getSelectedMenu();

   /* update calling menu's name field */
   strcpy(dstMenu->name, selectedMenu->name);

   G_sysData.backlight = selectedMenu->attrib & 0x1FF;
   backlight(G_sysData.backlight);

   closeMenuAndReturn();
}

const struct menu_t backlightList_m[] = {
//    {"       ", 0|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} }, oh support... why is my screen black?
    {"      -",  16|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },
    {"     --",  32|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },
    {"    ---",  64|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },
    {"   ----", 128|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },
    {"  -----", 192|VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },
    {" ------", 224|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },
    {"-------", 255|VERT_ITEM, FUNCTION, {(struct menu_t *)backlight_cb} },

    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

struct menu_t backlight_m[] = {
    {"-------",   VERT_ITEM,     MENU, {backlightList_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};


/*
   rotate screen
*/
void rotate_cb(struct menu_t *h) {
    unsigned char rotated=0;
    struct menu_t *selectedMenu;
    extern unsigned char G_outputMode;
    extern unsigned char G_entry; 

    selectedMenu = getSelectedMenu();

    rotated = selectedMenu->attrib & 0x1FF;
    if (rotated) {
	G_outputMode = 0b00000111; /* CDIR=1 */
	G_entry = 0b10000010; /* Y=Yend -> X incremented */
    }
    else {
	/* old way */
	G_outputMode = 0b00000110; /* CDIR=0 */
	G_entry = 0b10000000; /* X=Xend -> Y incremented */
    }

    LCDReset();

    closeMenuAndReturn();
};

const struct menu_t rotate_m[] = {
    {" default",   0|VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)rotate_cb} },
    {" rotated",   1|VERT_ITEM, FUNCTION, {(struct menu_t *)rotate_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};



/*
    LED brightness
*/
void LEDlight_cb(struct menu_t *h) {
    void led_brightness(unsigned char bright) ;
    struct menu_t *dstMenu, *selectedMenu;
    extern unsigned char G_red_pwm, G_green_pwm, G_blue_pwm;

    dstMenu = getSelectedMenuStack(1);
    selectedMenu = getSelectedMenu();

    strcpy(dstMenu->name, selectedMenu->name);

    G_sysData.ledBrightness = selectedMenu->attrib & 0x1FF;
    led_brightness(G_sysData.ledBrightness);
    /* because of calcs done on pwm, 
       have to reload the values for
       it to take effect. because of division, 
       info is lost in this process
    */
    red(G_red_pwm);
    green(G_green_pwm);
    blue(G_blue_pwm);

    closeMenuAndReturn();
}


const struct menu_t LEDlightList_m[] = {
    {"       ", 7|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"      -", 6|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"     --", 5|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"    ---", 4|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"   ----", 3|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"  -----", 2|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {" ------", 1|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"-------", 0|VERT_ITEM, FUNCTION, {(struct menu_t *)LEDlight_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

struct menu_t LEDlight_m[] = {
    {"--------",   VERT_ITEM,     MENU, {LEDlightList_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

extern unsigned short G_mute;
void buzzer_config_cb()
{
    struct menu_t *dstMenu, *selectedMenu;
    extern unsigned char G_red_pwm, G_green_pwm, G_blue_pwm;

    dstMenu = getSelectedMenuStack(1);
    selectedMenu = getSelectedMenu();

    strcpy(dstMenu->name, selectedMenu->name);

    G_mute = selectedMenu->attrib & 0x1;
    closeMenuAndReturn();
}

struct menu_t buzzer_config_m[] = {
    {"Buzzer: On",   0|VERT_ITEM,     FUNCTION, {(struct menu_t *)buzzer_config_cb} },
    {"Buzzer: Off",  1|VERT_ITEM|DEFAULT_ITEM,     FUNCTION, {(struct menu_t *)buzzer_config_cb} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};

struct menu_t buzzer_m[] = {
    {"Buzzer: On",   VERT_ITEM|DEFAULT_ITEM,     MENU, {buzzer_config_m} },
    {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL} },
};
