/** \file
 * Functions for controlling the IR communication LED and sensor, and changing
 * badge state based on results of IR communication.
 *
 * Actual IR packet receive/transmit is handled by timer interrupt handler
 * 'void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler(void)' in timer1_int.c.
 */

#ifndef ir_h
#define ir_h

/**
 * The data packet transferred by IR communication, 32 bits.
 */
struct IRpacket_t {
    unsigned data : 16; /**< The packet payload. **/
    unsigned badgeId : 9; /**< The badge ID of the badge that created packet */
    unsigned address : 5; /**< Index of the packet handler function */
    unsigned command : 1; /**< IR_READ or IR_WRITE */
    unsigned startBit : 1; /**< currently unused */
};

union IRpacket_u {
    struct IRpacket_t p;
    unsigned int v;
};


#define ADMIN_SILENCE 0x1000
#define ADMIN_GOCRAY 0x2000
#define ADMIN_ANGRY 0x3000
#define ADMIN_SETID 0x4000

#define PING_REQUEST      0x1000
#define PING_RESPONSE     0x2000

#define PING_PAIR_REQUEST 0x2
#define PING_PAIR_CONFIRM 0x3

#define MAX_MSG_LENGTH 64
#define MSG_NULL_TERM 50 // using 6 bits for each char

/* input circular queue */
#define MAXPACKETQUEUE 8
extern unsigned char IRpacketInCurr; /**< current packet in input queue (start of queue) */
extern unsigned char IRpacketInNext; /**< position of first empty slot in input queue. */
extern union IRpacket_u IRpacketsIn[]; /**< Queue of IR packets received (size MAXPACKETQUEUE) */

#define IR_IN_QUEUE_FULL (((IRpacketInNext+1) % MAXPACKETQUEUE) == IRpacketInCurr)
#define IR_IN_QUEUE_EMPTY (IRpacketInNext == IRpacketInCurr)

/* output circular queue */
extern unsigned char IRpacketOutCurr; /**< current packet in input queue (start of queue) */
extern unsigned char IRpacketOutNext; /**< position of first empty slot in input queue. */
extern union IRpacket_u IRpacketsOut[]; /**< Queue of IR packets ready to send (size MAXPACKETQUEUE) */

struct IRcallback_t {
    void (*handler)(struct IRpacket_t p); /**< Pointer to IR received callback function */
};

extern struct IRcallback_t IRcallbacks[];

/** Constants for IR modes read or write. */
enum {
    IR_READ,
    IR_WRITE
};

/**
 * Index constants for address array, to select IR callback function.
 *
 * Don't change the order of this without also changing the functions in ir.c.
 */
enum {
    IR_NAME, /**< length == 32? */
    IR_BADGEID, /**< send to ALL == 0b1111111111/1023/0x3F */
    IR_SEKRITS, /**< not sure, individual per game? */
    IR_ACHIEVEMENTS, /**< ? */

    /* load/save settings */
    IR_LED, /**< led brightness */

    IR_TIME, /**< time */

    IR_DATEYYMM, /**< date */
    IR_DATEDDAMPM, /**< date */

    IR_SCREENSAVER, /**< dunno brightness */
    IR_BACKLIGHT, /**< backlight brightness */

    /* special */
    IR_CODE, /**< code to jump to */
    IR_FORTHCODE, /**< forth code to run */
    IR_DRAW_UNLOCKABLE,
    IR_ASSET, /**< set current asset */
    IR_PING,
    //IR_PING_RESP,

    IR_LIVEAUDIO, /**< stream play to piezo */
    IR_LIVETEXT, /**< stream text screen */
    IR_LIVELED, /**< stream rgb to LED */
    
    /*
       ================================
             handled by apps
       ================================
     */
    IR_DRAW_APP,
    IR_PONG,
    IR_APP2,
    IR_APP3,

    IR_APP4,
    IR_APP5,
    IR_HIGHSCORE,
    IR_APP7,

    IR_ERROR,
    IR_LASTADRESS,
};

/** IR packet received handler.
 *
 * Checks queue for pending packet; if found, executes the callback identified
 * by received packet's address field.
 */
void IRhandler();
/** Adds IR packet `pkt` to the outgoing queue. */
void IRqueueSend(union IRpacket_u pkt);
/** Reset our peer and send out request to peer */
void IRPair();

/** not yet implemented */
void ir_name(struct IRpacket_t p);
/** not yet implemented */
void ir_badgeid(struct IRpacket_t p);
/** not yet implemented */
void ir_sekrits(struct IRpacket_t p);
/** not yet implemented */
void ir_achievements(struct IRpacket_t p);
/**
 * Sets LED color according to the RGB values in p.data.
 *
 * @param p IR communications packet received from other badge.
 */
void ir_led(struct IRpacket_t p);
/**
 * Sets global time according to data in received packet p.
 *
 * @param p An IR data packet with time in the low-order byte and
 *          hours in next-lowest-order byte. Hour and time are BCD
 *          encoded: that is, the first digit in the first 4 bits (a
 *          nibble), the second in the second 4 bits.
 */
void ir_time(struct IRpacket_t p);
/**
 * Sets global month and year according to the data in received packet
 * p.
 *
 * @param p An IR data packet with month in the lowest byte of p.data
 *          and year in the next lowest byte, encoded as BCD.
 */
void ir_date_YYMM(struct IRpacket_t p);
/**
 * Sets global day of the month according to the data in received
 * packet p.
 *
 * @param p An IR data packet with the day of the month (BCD) in the
 *          lowest byte.
 */
void ir_date_DDAMPM(struct IRpacket_t p);
/** not yet implemented */
void ir_screensaver(struct IRpacket_t p);
/** not yet implemented */
void ir_backlight(struct IRpacket_t p);
/** not yet implemented */
void ir_code(struct IRpacket_t p);
/** not yet implemented */
void ir_forthcode(struct IRpacket_t p);
/** not yet implemented */
void ir_draw_unlockable(struct IRpacket_t p);
/** not yet implemented */
void ir_asset(struct IRpacket_t p);
/**
 * Handler for ping request or response sent from other badge.
 *
 * @param p Packet whose data field has the PING_REQUEST or
 *          PING_RESPONSE bit set.
 */
void ir_ping(struct IRpacket_t p);
//void ir_ping_resp(struct IRpacket_t p);
/**
 * Handler for IR_LIVEAUDIO packet sent from other badge.
 *
 * @param p IR packet with data specifying the duration and frequency
 *          of a note to play.
 */
void ir_liveaudio(struct IRpacket_t p);
/** not yet implemented */
void ir_livetext(struct IRpacket_t p);
/** not yet implemented */
void ir_liveled(struct IRpacket_t p);



void ir_draw_app(struct IRpacket_t p);
/** Callback reserved for application 0 */
void ir_app0(struct IRpacket_t p);
/** Callback reserved for application 1 */
void ir_app1(struct IRpacket_t p);

/** Callback reserved for pong */
/** Defined in pong.c */
void ir_pong(struct IRpacket_t p);

/** Callback reserved for HighScore's score population system*/
/** Defined in screensavers.c */
void ir_highscore(struct IRpacket_t p);

/** Callback reserved for application 2 */
void ir_app2(struct IRpacket_t p);
/** Callback reserved for application 3 */
void ir_app3(struct IRpacket_t p);
/** Callback reserved for application 4 */
void ir_app4(struct IRpacket_t p);
/** Callback reserved for application 5 */
void ir_app5(struct IRpacket_t p);
/** Callback reserved for application 6 */
void ir_app6(struct IRpacket_t p);
/** Callback reserved for application 7 */
void ir_app7(struct IRpacket_t p);
/** not yet implemented */
void ir_error(struct IRpacket_t p);
/** not yet implemented */
void ir_lastaddress(struct IRpacket_t p);

#endif
