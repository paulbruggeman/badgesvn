#include "HardwareProfile.h"
#include "badge16.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "colors.h"
#include "flash.h"

//#include "badge15.h"
//#include <plib.h>
//#include "time_date.h"

/*
   QC/test the board
   Author: Paul Bruggeman
   paul@Killercats.com
*/
//extern rtccTime G_time;
//extern rtccDate G_date;

//rtccTime G_time_test;
//rtccDate G_date_test;

extern struct sysData_t G_sysData;

// Set high if the badge got an init ping
extern unsigned short pinged;

// Set high if badge got a ping response
extern unsigned short ping_responded;

/* make sure to keep these in the same order as the switch/case */
enum {
	QC_INIT,
	QC_BUTTON,
	QC_RED,
	QC_RED_WAIT,
	QC_GREEN,
	QC_GREEN_WAIT,
	QC_BLUE,
	QC_BLUE_WAIT,
	QC_PIEZO,
	QC_PIEZO_WAIT,
	QC_LCD,
	QC_IR_SEND,
	QC_IR_RECV,
	QC_EXIT
};

static unsigned char pressed_done = 0;
void pressed_cb()
{
    pressed_done = 1;
}

void pinger_cb()
{
    pressed_done = 2;
}

void reset_cb()
{
    pressed_done = 3;
}

void quit_cb()
{
    pressed_done = 4;
}

struct menu_t pressed_m[] = {
    {"PRESSED", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)(pressed_cb)} },
    {"DBG Pinger", VERT_ITEM, FUNCTION, {(struct menu_t *)(pinger_cb)} },
    {"Reset", VERT_ITEM, FUNCTION, {(struct menu_t *)(reset_cb)} },
    {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)(quit_cb)} },
};


#define LED_WAIT_TICKS 50000

void QC_cb()
{
    static unsigned char state = QC_INIT, slider_test_x = 60;
    static unsigned int cnt=0;
    char time_str[7] = {0,0,0,0,0,0, 0};
    char date_str[9] = {0,0,0,0,0,0,0,0,0};
    union IRpacket_u pkt;
    char tmp_out[8] = {'I','D',' ' ,0,0,0,0,0};

    if(pinged && !pressed_done)
    {
        red(0);
        green(50);
        blue(50);

        //DlColor(CYAN);
        //DlMove(35, 100);
        FbColor(CYAN);
        FbMove(35, 100);
        if(pinged == 1024)
        {
            tmp_out[3] = 'C';
            tmp_out[4] = 'A';
            tmp_out[5] = 'S';
            tmp_out[6] = 'T';

        }
        else if(pinged)
        {
            tmp_out[3] = '0' + ((pinged/1000)%10);
            tmp_out[4] = '0' + ((pinged/100)%10);
            tmp_out[5] = '0' + ((pinged/10)%10);
            tmp_out[6] = '0' + (pinged%10);
        }

        pinged = 0;
        //DlWriteLine(tmp_out);
        FbWriteLine(tmp_out);
    }

    switch (state) {
        case QC_INIT:
            setNote(100, 2048);
            red(0);
            green(0);
            blue(0);
            cnt = 0;
            pressed_done = 0;
            //state++;
            state = QC_BUTTON;
            genericMenu((struct menu_t *)pressed_m, MAIN_MENU_STYLE);

                //G_time.min = 0x45;
                //G_time.hour = 0x10;
                //G_date.mon = 0x06;
                //G_date.year = 0x2015;
                //G_date.mday = 0x4;
                //G_date.wday = 0x4;

                //RtccSetTimeDate(G_time.l, G_date.l);

            break;

        case QC_BUTTON:
            genericMenu((struct menu_t *)pressed_m, MAIN_MENU_STYLE);
            if (pressed_done == 1) {
                //state++;
                state = QC_RED;
                green(0);
                red(0);
                blue(0);
                cnt = 0;
            }
            //-----
            // DBG PINGER (Badge will send ping reqs every sec or so)
            else if(pressed_done == 2) {
                cnt++;
                // Send a ping every second or so
                if(cnt == 100000)
                {
                    //Send ping, clear LED
                    pkt.p.command = IR_WRITE;
                    pkt.p.address = IR_PING;
                    pkt.p.badgeId = 0;//G_sysData.badgeId;
                    pkt.p.data = PING_REQUEST;
                    IRqueueSend(pkt);
                    cnt = 0;
                    ping_responded = 0;
                    green(0);
                    red(0);
                    blue(0);
                }
                // set LED to yellow-orange if got a response
                if(ping_responded)
                {
                    green(50);
                    red(50);
                    blue(0);
                    //DlColor(YELLOW);
                    //DlMove(35, 100);
                    FbColor(YELLOW);
                    FbMove(35, 100);

                    tmp_out[3] = '0' + ((ping_responded/1000)%10);
                    tmp_out[4] = '0' + ((ping_responded/100)%10);
                    tmp_out[5] = '0' + ((ping_responded/10)%10);
                    tmp_out[6] = '0' + (ping_responded%10);
                    //DlWriteLine(tmp_out);
                    FbWriteLine(tmp_out);
                    ping_responded = 0;

                    //setNote(173, 8182);
                }
            }
            else if(pressed_done == 3)
            {
                pressed_done = 0;
                cnt = 0;
                pinged = 0;
                ping_responded = 0;
                green(0);
                red(0);
                blue(0);
            }
            else if(pressed_done == 4)
            {
                state=QC_EXIT;
            }
//            else if(cnt++ == 500)
//            {   getRTCC();
//                //getTime_Date(time_str, date_str);
//                //DlMove(1, 80);
//                //DlColor(GREEN);
//                //DlWriteLine(time_str);
//                //DlMove(60, 80);
//                //DlWriteLine(date_str);
//
//                FbMove(1, 80);
//                FbColor(GREEN);
//                FbWriteLine(time_str);
//                FbMove(60, 80);
//                FbWriteLine(date_str);
////                            time_str[0] = '0' + (G_time.sec>>8);
////                            time_str[1] = '0' + (G_time.sec & 0xFF);
//                //time_str[0] = '0' + (((G_time.sec/2)/10)%10);
//                //time_str[1] = '0' + ((G_time.sec/2)%10);
//                time_str[2] = 0;
//                //DlMove(50, 70);
//                //DlWriteLine(time_str);
//                FbMove(50, 70);
//                FbWriteLine(time_str);
//                cnt = 0;
//            }
            break;

        case QC_RED:
            red(50);

            cnt=0;
            //state++;
            state = QC_RED_WAIT;
            break;

        case QC_RED_WAIT:
            cnt++;
            if (cnt == LED_WAIT_TICKS) {
                red(0);
                //state++;
                state = QC_GREEN;
            }
            break;

        case QC_GREEN:
            green(50);
            cnt=0;
            //state++;
            state = QC_GREEN_WAIT;
            break;

        case QC_GREEN_WAIT:
            cnt++;
            if (cnt == LED_WAIT_TICKS) {
                green(0);
                //state++;
                state = QC_BLUE;
            }
            break;

        case QC_BLUE:
            blue(50);
            cnt=0;
            //state++;
            state = QC_BLUE_WAIT;
            break;

        case QC_BLUE_WAIT:
            cnt++;
            if (cnt == LED_WAIT_TICKS) {
                blue(0);
                //state++;
                state=QC_PIEZO;
            }
            break;

        case QC_PIEZO:
            setNote(173, 8182);
            cnt=0;
            //state++;
            state = QC_PIEZO_WAIT;
            break;

        case QC_PIEZO_WAIT:
            cnt++;
            if (cnt == 3000) {
                setNote(0, 2048);
                //state++;
                state = QC_INIT;
            }
            break;

        case QC_LCD:
            state++;
            break;

        case QC_IR_SEND:
            state++;
            break;

        case QC_IR_RECV:
            state++;
            break;

        case QC_EXIT:
            state = QC_INIT;
            returnToMenus(); /* NOTE: the routine returns, so code from here to the end of this function will be execute */
            break;

        default:
            break;
    }
    if(LEFT_SLIDE_AND_CONSUME)
    {
        slider_test_x += 10;
        setNote(173, 4096);
        //DlInit();
        //DlClear();
        //DlClear();

        //DlMove(slider_test_x, 70);
        //DlColor(GREEN);
        //DlFilledRectangle(20,10);

        //FbInit();
        //FbClear();
        //FbClear();

        FbMove(slider_test_x, 70);
        FbColor(GREEN);
        FbFilledRectangle(20,10);
    }

    if(RIGHT_SLIDE_AND_CONSUME)
    {
        slider_test_x -= 10;
        setNote(100, 4096);
        //DlInit();
        //DlClear();
        //DlClear();

        //DlMove(slider_test_x, 70);
        //DlColor(GREEN);
        //DlFilledRectangle(20,10);

        //FbInit();
        //FbClear();
        //FbClear();

        FbMove(slider_test_x, 70);
        FbColor(GREEN);
        FbFilledRectangle(20,10);
    }

    if(slider_test_x > 132)
        slider_test_x = 60;
};
