#include <plib.h>
#include "string.h"
#include "colors.h"
#include "flash.h"
#include "ir.h"
#include "debug.h"
#include "fb.h"

extern rtccTime G_time ;
extern rtccDate G_date ;

unsigned char IRpacketInCurr = 0;
unsigned char IRpacketInNext = 0;
union IRpacket_u IRpacketsIn[MAXPACKETQUEUE];

unsigned char IRpacketOutCurr = 0;
unsigned char IRpacketOutNext = 0;
union IRpacket_u IRpacketsOut[MAXPACKETQUEUE];

// Set to a badge id if the badge got an init ping
unsigned short pinged = 0 ;

// Set to a badge id if badge got a ping response
unsigned short ping_responded = 0;

unsigned char new_message = 0;

unsigned char msg_relay_remaining = 0;

unsigned char incoming_message[MAX_MSG_LENGTH] = {0};//"What is up!\0";

/*
   Dont change the order of this without also changing the enum's in ir.h
*/
struct IRcallback_t IRcallbacks[] = {
	{ ir_name },
	{ ir_badgeid },
	{ ir_sekrits },
	{ ir_achievements },

	/* load/save settings */
	{ ir_led },
	{ ir_time },
	{ ir_date_YYMM },
	{ ir_date_DDAMPM },
	{ ir_screensaver },
	{ ir_backlight },

	/* special */
	{ ir_code },
	{ ir_forthcode },
        { ir_draw_unlockable },
	{ ir_asset },
        { ir_ping},

	{ ir_liveaudio },	/* stream play to piezo */
	{ ir_livetext },	/* stream text screen */
	{ ir_liveled },	/* stream rgb to LED */

	{ ir_app0 },
	{ ir_app1 },
	{ ir_app2 },
	{ ir_app3 },

	{ ir_app4 },
	{ ir_app5 },
	{ ir_app6 },
	{ ir_app7 },

	{ ir_error },

	{ ir_lastaddress },
};

void IRqueueSend(union IRpacket_u pkt) 
{
    /* make sure not full */
    if ( ((IRpacketOutNext+1) % MAXPACKETQUEUE) != IRpacketOutCurr) {
	IRpacketsOut[IRpacketOutNext].v = pkt.v;
	IRpacketOutNext++;
	IRpacketOutNext %= MAXPACKETQUEUE;
    }
}

void IRPair()
{
    union IRpacket_u pkt;
    
    // reset our peer and send out request to peer
    G_peerBadgeId = 0;
    
    //Send ping
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_PING;
    pkt.p.badgeId = G_sysData.badgeId;
    pkt.p.data    = PING_PAIR_REQUEST;
    IRqueueSend(pkt);
}

/* handler for IR recv in main thread */
void IRhandler() 
{
    /* 
       should we lock variable IRpacketInCurr incase of interupts? 
       No: this routine modifies InCurr, where the insert routine modfies InNext
     */

    /* curr == next == empty */
    if (IRpacketInCurr != IRpacketInNext) {
	unsigned char *tmp, tmpstr[16];
	extern unsigned char hextab[];

	if (IRpacketsIn[IRpacketInCurr].p.address < IR_LASTADRESS) /* basic sanity check before we call unknown handlers */
		IRcallbacks[ IRpacketsIn[IRpacketInCurr].p.address].handler( IRpacketsIn[IRpacketInCurr].p );

	IRpacketInCurr++;
	IRpacketInCurr %= MAXPACKETQUEUE;
    }
}

void ir_name(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_name");
}

void ir_badgeid(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_badgeid");
}

void ir_sekrits(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_sekrits");
}

void ir_achievements(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_achievements");
}

void ir_led(struct IRpacket_t p)
{
    unsigned char r,g,b;

    DEBUGSTRLVL(DNOISE, "ir_led");

    r = UNPACKR(p.data);
    red(r);

    g = UNPACKG(p.data);
    green(g);

    b =  UNPACKB(p.data);
    blue(b);
}

void ir_time(struct IRpacket_t p)
{
    G_time.min = (p.data & 0xFF);
    G_time.hour = (p.data >> 8) & 0xFF;
    RtccSetTimeDate(G_time.l, G_date.l);

    DEBUGSTRLVL(DNOISE, "ir_time");
}

void ir_date_YYMM(struct IRpacket_t p)
{
    G_date.mon = (p.data & 0xFF);
    G_date.year = (p.data >> 8) & 0xFF;
    RtccSetTimeDate(G_time.l, G_date.l);

    DEBUGSTRLVL(DNOISE, "ir_date_YYMM");
}

void ir_date_DDAMPM(struct IRpacket_t p)
{
    G_date.mday = (p.data & 0xFF);
    G_date.wday = 0;
    RtccSetTimeDate(G_time.l, G_date.l);

    DEBUGSTRLVL(DNOISE, "ir_date_DDMAMPM");
}

void ir_screensaver(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_screensaver");
}

void ir_backlight(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_backlight");
}

void ir_code(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_code");
}

void ir_forthcode(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_forthcode");
}

void ir_draw_unlockable(struct IRpacket_t p)
{
    return;
}

void ir_asset(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_asset");
}

// This is the initial ping ( person initiating the pinging sent this)
void ir_ping(struct IRpacket_t p)
{
    //respond to ping
    union IRpacket_u pkt;

    // Badge got a Ping from another badge
    if(p.data & PING_REQUEST)
    {
        // Get out the requester's ID
        pinged = p.data & 0x1FF;
        // If broadcast, just set to some high number
        if(pinged == 0)
            pinged = 1024;

        pkt.p.command = IR_WRITE;

        pkt.p.address = IR_PING;

        // Send along this badges ID
        pkt.p.badgeId = 0;//G_sysData.badgeId;
        pkt.p.data    = PING_RESPONSE | G_sysData.badgeId;

        IRqueueSend(pkt);
    }
    // badge got it's (or someone's) response to a PING_REQUEST
    else if(p.data & PING_RESPONSE)
    {
        //ping_responded = p.badgeId;
        ping_responded = p.data & 0x1FF;
    }

    DEBUGSTRLVL(DNOISE, "ir_ping");
}

void ir_liveaudio(struct IRpacket_t p)
{
    unsigned short freq;
    unsigned short dur;

    /* top 4 bits are for duration divided by 256 (>> 8) */
    dur  = ((p.data >> 12) & 0xF) << 8 ;
    /* bottom 12 bits are for freq */
    freq = p.data & 0xFFF;
    setNote(freq, dur);


    DEBUGSTRLVL(DNOISE, "ir_liveaudio ");
}

extern char redraw_main_menu;
//TODO: May be better to move some of the processing out of this function
//     but then again, IR so slow, it probably is not a big deal?

//
void ir_livetext(struct IRpacket_t p)
{
   unsigned char recv_char = 0, recv_char2 = 0;
   unsigned char recv_idx = 0;
   char null_idx = -1;

   // If we are still relaying a message, don't listen to new messages
   if(msg_relay_remaining && new_message)
       return;
   else
       new_message = 0; //only set high if null term reached and not relaying

   //TODO: shouldn't send relay in every packet - maybe only in last packet?
   //    But bothering to do this is only worthwhile if we can do something
   //    useful with the extra bits. Combine with only sending i/2, gives one
   //    more bit...
   msg_relay_remaining = p.data >> 12;
   
   // Get the ordering of the character (index)
   recv_idx = (p.badgeId & 0b000111111);

   // Get out the two characters
   recv_char  =  p.data & 0b0000000000111111;
   recv_char2 = (p.data & 0b0000111111000000)>>6;

   // Map them back to standard C characters
   // But explicitly handle the NULL term, so let it pass through
   if(recv_char != MSG_NULL_TERM)
        recv_char = recv_char;
   else{
        recv_char = MSG_NULL_TERM;
        null_idx = recv_idx;
        new_message = redraw_main_menu = 1; //Message completed!
   }

   if(recv_char2 != MSG_NULL_TERM)
        recv_char2 = recv_char2;
   else{
        recv_char2 = MSG_NULL_TERM;
        null_idx = recv_idx+1;
        new_message = redraw_main_menu = 1;
   }

   // sanity check on the indices
   if(recv_idx < MAX_MSG_LENGTH)
        incoming_message[recv_idx] = recv_char;

   if((recv_idx+1) < MAX_MSG_LENGTH)
        incoming_message[recv_idx+1] = recv_char2;

   // Probably not necessary, but null the remainder out
   // (Removes whatever is leftover from old message)
   if(new_message)
       for(; null_idx > 0 && null_idx < MAX_MSG_LENGTH; null_idx++)
           incoming_message[null_idx] = MSG_NULL_TERM;
   
}

void ir_liveled(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_liveled");
}

void ir_app0(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app0");
}

void ir_app1(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app1");
}

void ir_app2(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app2");
}

void ir_app3(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app3");
}

void ir_app4(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app4");
}

void ir_app5(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app5");
}

void ir_app6(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app6");
}

void ir_app7(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_app7");
}

void ir_error(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_error");
}

void ir_lastaddress(struct IRpacket_t p)
{
   DEBUGSTRLVL(DNOISE, "ir_lastaddress");
}
