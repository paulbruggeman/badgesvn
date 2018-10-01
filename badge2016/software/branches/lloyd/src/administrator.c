#include <plib.h>
#include "string.h"
#include "flash.h"
#include "menu.h"
#include "touchCTMU.h"
#include "stopwatch.h"
#include "display.h"
#include "ir.h"
#include "fb.h"
#include "prngs.h"
extern unsigned int G_currentstamp;
void silence_cb(){
    static unsigned char num_pinged = 0;

    union IRpacket_u pkt;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_APP7;
    pkt.p.badgeId = 0;
    pkt.p.data = ADMIN_SILENCE;
    IRqueueSend(pkt);

    returnToMenus();
}

void note_crazy_cb(){
    static unsigned short cnter = 0;

    if(cnter> 700){
        union IRpacket_u pkt;
        pkt.p.command = IR_WRITE;
        pkt.p.address = IR_LIVEAUDIO;
        pkt.p.badgeId = 0;
        unsigned short freq = 50 + (quick_rand(cnter + G_currentstamp) %200);
        pkt.p.data = (((2048 >> 8) & 0xF) << 12) | (freq & 0x0FFF) ;
        IRqueueSend(pkt);
        cnter = 0;
    }
    cnter++;
    PowerSaveIdle();

    if(BUTTON_PRESSED_AND_CONSUME){
        cnter = 0;
        returnToMenus();
    }
}