/** @file Declarations for firewall app which are required by more
 * than one module (separate here to prevent circular includes)
 *
 * @author A. Lloyd Flanagan <a.lloyd.flanagan@gmail.com>
 * @copyright 2016 HackRVA Labs, Inc. <https://www.hackrva.org>
 */
#ifndef FIREWALL_COMMON_H
#define FIREWALL_COMMON_H

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

//timing constants
#define SYS_FREQ (40000000L)
#define TOGGLES_PER_SEC	(38000)

/* Fixed values used to lay out text on LCD */
/** height of a character in our font */
#define CHAR_HEIGHT 8
/** width of a character in our font */
#define CHAR_WIDTH 5
/** pixels of space between successive chars */
#define CHAR_SPACING (CHAR_WIDTH + 1)
/** space between successive baselines */
#define LINE_SPACE (CHAR_HEIGHT + 2)
/** extra space above first line of text */
#define TOP_LINE 8

typedef unsigned char byte;

typedef enum {
    NIC = 1, //cruiser
    PROXY, //destroyer
    SWITCH, //submarine
    ROUTER //battleship
} ship_type;

typedef enum {
    VERTICAL,
    HORIZONTAL
} orientation;

/** All states for our state machine */
typedef enum {
    ST_START, // state 0 will be initial state
    ST_WELCOME_SCREEN,
    ST_INITIAL_DISPLAY,
    ST_WAITING_FOR_CONNECT, // sending SHALL_WE_PLAY_A_GAME
    ST_WAITING_FOR_OTHER_PLAYER, // sending LOVE_TO
    ST_WHO_GOES_FIRST, // sending
    ST_DISPLAY_OFFENSE,
    ST_DISPLAY_DEFENSE,
    ST_TAKE_SHOT,
    ST_SENDING_SHOT,
    ST_WAITING,  // defender waiting on shot
    ST_SEND_HIT,
    ST_SEND_MISS,
    ST_DISPLAY_HIT,
    ST_DISPLAY_MISS,
    ST_SHOW_OFF_GRID, //show offensive grid, before switching to def
    ST_DEAD, // ack! send "you win" message
    ST_BASKING, // in victory, of course
    ST_EXIT
} game_state;

/**
 * Constants to produce varying notes from setNote().
 *
 * Actual frequency not guaranteed.
 */
typedef enum {
    NOTE_A = 173,
    NOTE_B = 154,
    NOTE_C = 145,
    NOTE_D = 129,
    NOTE_E = 115,
    NOTE_F = 109,
    NOTE_G = 97
} note_freqs;

/**
 * Number of bytes to provide 1 bit per cell of the game board. Use
 * one array for each option for a cell.
 */
#define GRID_BYTES 8

/**
 * Number of grid cells across or down.
 */
#define GRID_CELLS 8

/**
 * We need an offensive grid to record our hits/misses, and a
 * defensive grid where we place our network components and record
 * opponent's hits.  We show offensive during our turn, and defensive
 * during opponent's.  Battleship grid is 10 x 10, ours is 8 x 8.
 */
struct firewall_global_state {
    /* all the stored state for a firewall game */
    /* For offensive grid, we need three states: untried, hit, or miss. */
    /** Record whether we took a shot at each grid location */
    byte offensive_tries[GRID_BYTES]; // 0 = untried, 1 = tried
    /** Record when we hit a ship at each grid location */
    byte offensive_hits[GRID_BYTES]; // 0 = untried/miss, 1 = hit
    /** Record our opponent's shots */
    byte defensive_tries[GRID_BYTES];

    /**
     * Bitmap -- set to 1 when ship is placed on that grid cell.
     */
    byte placed_items[GRID_BYTES]; // 1 = ship here, 0 = no ship
    /**
     * For placed_items, we want to know what kind of ship each point
     * represents. With four ship types, we need two bits for each
     * point.
     */
    byte placed_types[2][GRID_BYTES];

    byte curr_row; /**< user-selected row */
    byte curr_column; /**< user-selected column */
    game_state state; /**< The current node of the state machine */
    /**
     * Badge with highest random value goes first.
     */
    unsigned int my_random;
    unsigned int their_random;
    /**
     * badge ID of other player. Required to send message to only that
     * badge.
     */
    unsigned short other_badge_id;
};

/**
 * Master callback for firewall app.
 */
void firewall_ng_cb(void);

/**
 * Called by IR callback when peering request received.
 */
void received_peering(unsigned short other_badge_id, game_state next_state);

/**
 * Called by IR callback when WHOS_FIRST message received.
 */
void received_who_first(unsigned short their_random);

/**
 * Record opponent's shot, display result, and send confirmation
 * messages.
 */
void received_shot(unsigned short location);

/**
 * Record opponent's hit or miss, display result, go to defend.
 */
void received_confirmation(int its_a_hit, unsigned short location);

void received_lets_start(unsigned short their_random);


#endif //FIREWALL_COMMON_H

/* Local Variables: */
/* compile-command: "make -k -f ~/Devel/badge2016/software/branches/lloyd/Makefile exec" */
/* compilation-scroll-output: first-error */
/* c-file-style: "k&r" */
/* End: */
