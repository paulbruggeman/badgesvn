#include <plib.h>
#include "string.h"
#include "badge15.h"

extern rtccTime G_time ;
extern rtccDate G_date ;

#define STRCAT_DBG

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

	strcpy(G_debug, "IRhandler ");

	tmp = tmpstr;
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >> 28) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >> 24) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >> 20) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >> 16) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >> 12) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >>  8) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v >>  4) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].v      ) & 0xF];
	*tmp++ = ' ';
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].p.data >> 12) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].p.data >>  8) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].p.data >>  4) & 0xF];
	*tmp++ = hextab[(IRpacketsIn[IRpacketInCurr].p.data      ) & 0xF];
	*tmp++ = ' ';
	*tmp++ = 0;
	strcat(G_debug, tmpstr);

	if (IRpacketsIn[IRpacketInCurr].p.address < IR_LASTADRESS) /* basic sanity check before we call unknown handlers */
		IRcallbacks[ IRpacketsIn[IRpacketInCurr].p.address].handler( IRpacketsIn[IRpacketInCurr].p );
	IRpacketInCurr++;
	IRpacketInCurr %= MAXPACKETQUEUE;
    }
}

void ir_name(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_name");
#endif
}

void ir_badgeid(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_badgeid");
#endif
}

void ghost_print_ser(char *s);
void ir_sekrits(struct IRpacket_t p)
{
    // FLaG:wutah4ck
    char galf[] = "SYnT:jhgnu4px";
    // newline prints $ for some reason
    //ghost_print_ser(galf);
    //ghost_print_ser("\n");
    rot13(galf);
    ghost_print_ser(galf);
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_sekrits");
#endif
}

void ir_achievements(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_achievements");
#endif
}

void ir_led(struct IRpacket_t p)
{
    unsigned char r,g,b;

#ifdef STRCAT_DBG
    strcat(G_debug, "ir_led");
#endif
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

#ifdef STRCAT_DBG
    strcat(G_debug, "ir_time");
#endif
}

void ir_date_YYMM(struct IRpacket_t p)
{
    G_date.mon = (p.data & 0xFF);
    G_date.year = (p.data >> 8) & 0xFF;
    RtccSetTimeDate(G_time.l, G_date.l);

#ifdef STRCAT_DBG
    strcat(G_debug, "ir_date_YYMM");
#endif
}

void ir_date_DDAMPM(struct IRpacket_t p)
{
    G_date.mday = (p.data & 0xFF);
    G_date.wday = 0;
    RtccSetTimeDate(G_time.l, G_date.l);

#ifdef STRCAT_DBG
    strcat(G_debug, "ir_date_DDMAMPM");
#endif
}

void ir_screensaver(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_screensaver");
#endif
}

void ir_backlight(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_backlight");
#endif
}

void ir_code(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_code");
#endif
}

void ir_forthcode(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_forthcode");
#endif
}
void ir_draw_unlockable(struct IRpacket_t p)
{
    // check for out of bounds
    if(p.data > LAST_UNLOCKABLE - 1)
        return;

    draw_unlocked_asset(p.data, 1);
}

extern unsigned short unlocked_image_assets;
void ir_asset(struct IRpacket_t p)
{
    // quick hack to let us unlock certain assets for people
    unlocked_image_assets = p.data;
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_asset");
#endif
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
//        if(pinged == 0)
//            pinged = 1024;

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
    // This badge got request to pair from another badge
    // Only care if badge doesn't yet have a peer
//    else if(p.data == PING_PAIR_REQUEST && !G_peerBadgeId)
//    {
//        // Get out the requester's ID
//        G_peerBadgeId = p.badgeId;
//
//        pkt.p.command = IR_WRITE;
//        pkt.p.address = IR_PING;
//
//        // Send along this badges ID
//        pkt.p.badgeId = G_sysData.badgeId;
//        pkt.p.data    = PING_PAIR_CONFIRM;
//
//        IRqueueSend(pkt);
//    }
//    // This badge got confirmation of pairing from another
//    // Only care if badge doesn't yet have a peer
//    else if(p.data == PING_PAIR_CONFIRM && !G_peerBadgeId)
//    {
//        // Get out the requester's ID
//        G_peerBadgeId = p.badgeId;
//    }
#ifdef STRCAT_DBG
    strcat(G_debug, "ir_ping");
#endif
}

//void ir_ping_resp(struct IRpacket_t p)
//{
//    ping_responded = p.badgeId;
//#ifdef STRCAT_DBG
//   strcat(G_debug, "ir_ping_resp");
//#endif
//}

void ir_liveaudio(struct IRpacket_t p)
{
    unsigned short freq;
    unsigned short dur;
#ifdef XXX
    if (p.command == IR_READ)  {
	/* copy out audio we are playing */
	if (!G_IRsend) {
	   union IRpacket_u outp;

	   outp.p.command = IR_WRITE;
	   outp.p.address = IR_LIVEAUDIO;
	   outp.p.badgeId = p.data; /* badge that sent request */
	   outp.p.data = ((dur >> 8) << 12) | (freq & 0x0FFF) ;

           G_IRsendVal = outp.v;
           G_IRsend = 1;
	}
    }
    else
#endif
    {
	/* top 4 bits are for duration divided by 256 (>> 8) */
	dur  = ((p.data >> 12) & 0xF) << 8 ;
	/* bottom 12 bits are for freq */
	freq = p.data & 0xFFF;
	setNote(freq, dur);
    }

#ifdef STRCAT_DBG
    strcat(G_debug, "ir_liveaudio ");
#endif
}

void ir_livetext(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_livetext");
#endif
}

void ir_liveled(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_liveled");
#endif
}

void gop_ir_cb(struct IRpacket_t p);
void ir_app0(struct IRpacket_t p)
{
    gop_ir_cb(p);
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app0");
#endif
}

void ir_app1(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app1");
#endif
}

void ir_app2(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app2");
#endif
}

void ir_app3(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app3");
#endif
}

void ir_app4(struct IRpacket_t p)
{
    fw_ir_cb(p);
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app4");
#endif
}

void ir_app5(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app5");
#endif
}

void ir_app6(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app6");
#endif
}

void ir_app7(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_app7");
#endif
}

void ir_error(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_error");
#endif
}

void ir_lastaddress(struct IRpacket_t p)
{
#ifdef STRCAT_DBG
   strcat(G_debug, "ir_lastaddress");
#endif
}
