#ifndef FIREWALL_BOARD_H
#define FIREWALL_BOARD_H

/*
 * Definitions and prototypes dealing with the setup of the firewall board, which can be
 * relatively easily tested without using the actual badge.
 */

/* ============================================================= */
/* Custom Types */

typedef unsigned char byte;

#define TRUE 1
#define FALSE 0

typedef enum {
    NIC = 1, //cruiser
    PROXY, //destroyer
    SWITCH, //submarine
    ROUTER, //battleship
} ship_type;

/* # of bytes to provide 1 bit per cell of the game board */
/* use one array for each option for a cell */
#define GRID_BYTES 8

struct firewall_global_state {
    /*
     * We need an offensive grid to record our hits/misses, and a
     * defensive grid where we place our network components and record
     * opponent's hits.  We show offensive during our turn, and defensive
     * during opponent's.  A Battleship grid is 10 x 10, going to try to
     * get away with making it 8 x 8 to save space. We'll need some
     * room.
     *
     */
    /* all the stored state for a firewall game */
    /* For offensive grid, we need three states: untried, hit, or miss. */
    byte offensive_tries[GRID_BYTES]; // 0 = untried, 1 = tried
    byte offensive_hits[GRID_BYTES]; // 0 = untried/miss, 1 = hit
    /* for defensive, we need opponent's tries, and the location of our network parts */
    byte defensive_tries[GRID_BYTES];

    /* we need to represent each of four ship types, plus the "no ship here" state */
    byte placed_items[GRID_BYTES]; // 1 = ship here, 0 = no ship
    /* for placed_items, we want to know what kind of ship each point
     * represents. With four ship types, we need two bits for each point.
     */
    byte placed_types[GRID_BYTES][2];

    /* we have a concept of a "selected cell", where the user specifies a row
     * and column
     */
    byte curr_row;
    byte curr_column;
};

/* =========================================================== */
/* Prototypes */

// sets position at x,y in grid to 1
#define set_pos(g, x, y) (g)[(y)] = (g)[(y)] | (1 << (x))
// sets position x, y in grid to 0
#define clear_pos(g, x, y) (g)[(y)] = (g)[(y)] & ~(1 << (x))

void clear_grids(void);

byte check_ship(const byte grid[], const ship_type ship, const byte is_vertical, byte x, byte y);

/*
 * attempts to place a rectangle of type ship_type into the defensive grid
 * returns TRUE if a place was found
 */
int set_ship(const ship_type ship);

//inline void clear_grid(byte grid[]) {
//    memset(grid, 0, GRID_BYTES);
//}

/* well, above causes linker errors. "inline" ignored? */
#define clear_grid(g) memset((g), 0, GRID_BYTES)

/* ====================================================== */
/* Global Variables */

// #define FWBOARDEXTERN extern if you don't want to allocate variables
#ifndef FWBOARDEXTERN

struct firewall_global_state fw_state = {0};

#else

extern struct firewall_global_state fw_state;

#endif

#endif
