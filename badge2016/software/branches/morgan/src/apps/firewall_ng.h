/** @file
 * Main declarations for "firewall" (battleship) app.
 *
 * @author A. Lloyd Flanagan <a.lloyd.flanagan@gmail.com>
 * @copyright 2016 HackRVA Labs, Inc. <https://www.hackrva.org>
 */
#ifndef FIREWALL_NG_H
#define FIREWALL_NG_H

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

/* Fixed values used to lay out text on LCD */
// actual character dimensions from font
#define CHAR_HEIGHT 8
#define CHAR_WIDTH 5
// pixels of space between successive chars
#define CHAR_SPACING (CHAR_WIDTH + 1)
// space between successive baselines
#define LINE_SPACE (CHAR_HEIGHT + 2)
// extra space above first line of text
#define TOP_LINE 8

typedef unsigned char byte;

typedef enum {
    NIC = 0, //cruiser
    PROXY, //destroyer
    SWITCH, //submarine
    ROUTER, //battleship
} ship_type;

typedef enum {
    VERTICAL,
    HORIZONTAL
} orientation;

typedef enum {
  START,
  WELCOME_SCREEN,
  INITIAL_DISPLAY,
  EXIT
} game_state;

/* # of bytes to provide 1 bit per cell of the game board */
/* use one array for each option for a cell */
#define GRID_BYTES 8

/**
 * We need an offensive grid to record our hits/misses, and a
 * defensive grid where we place our network components and record
 * opponent's hits.  We show offensive during our turn, and defensive
 * during opponent's.  Battleship grid is 10 x 10, ours is 8 x 8.
 */
struct firewall_global_state {
    /* all the stored state for a firewall game */
    /* For offensive grid, we need three states: untried, hit, or miss. */
    byte offensive_tries[GRID_BYTES]; // 0 = untried, 1 = tried
    byte offensive_hits[GRID_BYTES]; // 0 = untried/miss, 1 = hit
    /* for defensive, we need opponent's tries, and the location of our network parts */
    byte defensive_tries[GRID_BYTES];

    /** we need to represent each of four ship types, plus the "no ship here" state */
    byte placed_items[GRID_BYTES]; // 1 = ship here, 0 = no ship
    /** for placed_items, we want to know what kind of ship each point
     * represents. With four ship types, we need two bits for each point.
     */
    byte placed_types[GRID_BYTES][2];

    /** we have a concept of a "selected cell", where the user specifies a row
     * and column
     */
    byte curr_row;
    byte curr_column;
  game_state state; /**< The current node of the state machine */
};

/* =========================================================== */
/* Prototypes */

/**
 * sets position at x,y in grid g to 1
 */
#define set_pos(g, x, y) (g)[(y)] = (g)[(y)] | (1 << (x))

/**
 * sets position x, y in grid g to 0
 */
#define clear_pos(g, x, y) (g)[(y)] = (g)[(y)] & ~(1 << (x))

/**
 * Clears all positions in grid g (void *).
 */
#define clear_grid(g) memset((g), 0, GRID_BYTES)

/**
 * Is it safe to place a ship at x,y?
 *
 * Checks the set of cells in `grid` starting at coords `x`, `y` and
 * extending vertically or horizontally for length of a ship of type
 * `ship` for collision with another object.
 *
 * @param grid The placed items grid to check.
 * @param ship The type (thus length) of ship to check.
 * @param is_vertical TRUE if placement is vertical, else horizontal.
 * @param x The x coord (left->right) of first checked cell. 0 <= x <= 7.
 * @param y The y coord (top->bottom) of first checked cell. 0 <= x <= 7.
 * @return TRUE if collision detected.
 */
byte check_ship(const byte grid[], const ship_type ship, const byte is_vertical, byte x, byte y);

/**
 * Master callback for firewall app.
 */
void firewall_ng_cb(void);

/**
 * Draw an 8 x 8 (empty) grid with columns and rows labeled.
 */
void display_grid(void);

/**
 * Draw any ships placed on the grid.
 * @param fgstate Pointer to firewall state structure.
 */
void draw_ships(struct firewall_global_state *fgstate);

/**
 * Display glyphs to indicate the currently selected row, column.
 * @param fgstate Pointer to firewall state structure.
 */
void draw_cursor(struct firewall_global_state *fgstate);

/**
 * Attempts to place a ship of type `ship_type` at some location in
 * global grid. Randomly determines ship orientation and location,
 * with collision avoidance.
 *
 * @param fgstate Pointer to irewall state structure including placement grid.
 * @param ship The type of ship (thus length) to place in the grid.
 * @return TRUE if a place was found.
 */
int set_ship(struct firewall_global_state *fgstate, const ship_type ship);

/**
 * Mark ship as occupying board at a specific location.
 *
 * @param fgstate
 * @param x
 * @param y
 * @param ship
 * @param orientation
 */
int add_ship(struct firewall_global_state *fgstate,
        const unsigned int x, const unsigned int y,
        const ship_type ship, const orientation ship_orient);


/**
 * Display welcome screen for firewall: description, instructions, etc.
 *
 */
void display_welcome_screen(void);

#endif
