/** @file
 * Main declarations for "firewall" (battleship) app.
 *
 * @author A. Lloyd Flanagan <a.lloyd.flanagan@gmail.com>
 * @copyright 2016 HackRVA Labs, Inc. <https://www.hackrva.org>
 */
#ifndef FIREWALL_NG_H
#define FIREWALL_NG_H

#include "firewall_common.h"
#include "firewall_comm.h"

typedef enum {
     SCR_TITLE,   //initial display
     SCR_CONNECTING,  //peering instructions
     SCR_PAIRED,
     SCR_FIRST_OFFENDER,  //peered, start game
     SCR_FIRST_DEFENDER,  //peered, start game
     SCR_ITS_A_HIT,   //screen to notify shooter of a hit
     SCR_YOU_MISSED,  // .. or a miss
     SCR_YOUVE_BEEN_HIT, //screens to notify target of a hit
     SCR_OPP_MISSED,     // ... or a miss
     SCR_WAITING,  //tell defender to wait for shot
     SCR_NETWORK_GUIDE,
     SCR_WE_LOSE,  // you sunk my firewall!!
     SCR_WE_WON,  // woohoo!
     SCR_MESSAGE_COUNT // must always be last
} MESSAGE_SCREENS;

/**
 * delay (in loop counts) between successive attempts to send the same
 * message. Determined empirically.
 *
 */
#define RESEND_DELAY 0x6000

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
 * @brief Is it safe to place a ship at x,y?
 *
 * Checks the set of cells in `grid` starting at coords `x`, `y` and
 * extending vertically or horizontally for length of a ship of type
 * `ship` for collision with another object.
 *
 * @param grid The placed items grid to check.
 * @param ship The type (thus length) of ship to check.
 * @param ship_orient Is the ship placed vertically or horizontally?
 * @param x The x coord (left->right) of first checked cell. 0 <= x <= 7.
 * @param y The y coord (top->bottom) of first checked cell. 0 <= y <= 7.
 * @return TRUE if collision detected.
 */
byte check_ship(const byte grid[], const ship_type ship, const orientation ship_orient, byte x, byte y);

/**
 * Draw an 8 x 8 (empty) grid with columns and rows labeled.
 */
void display_grid(void);

/**
 * Draw any ships placed on the grid.
 *
 * @param fgstate Pointer to firewall state structure.
 */
void draw_ships(struct firewall_global_state *fgstate);

/**
 * Display your attempted shots. Hits are indicated with an 'X',
 * misses with an 'O'.
 *
 * @param fgstate Pointer to firewall state structure.
 */
void draw_shots(struct firewall_global_state *fgstate);

/**
 * Display glyphs to indicate the currently selected row, column.
 *
 * @param fgstate Pointer to firewall state structure.
 */
void draw_cursor(struct firewall_global_state *fgstate);

/**
 * Attempts to place a ship of type `ship_type` at some location in
 * global grid. Randomly determines ship orientation and location,
 * with collision avoidance.
 *
 * @param fgstate Pointer to firewall state structure including
 * placement grid.
 *
 * @param ship The type of ship (thus length) to place in the grid.
 *
 * @return TRUE if a place was found.
 */
int set_ship(struct firewall_global_state *fgstate, const ship_type ship);

/**
 * Mark ship as occupying board at a specific location.
 *
 * @param fgstate Pointer to global firewall state
 * @param x X-coordinate of first (top) cell occupied
 * @param y Y-coordinate of first (leftmost) cell occupied
 * @param ship The type/length of ship to place
 * @param ship_orient Layout ship vertically or horizontally
 */
void add_ship(struct firewall_global_state *fgstate,
              const unsigned int x, const unsigned int y,
              const ship_type ship, const orientation ship_orient);

/**
 * Display welcome screen for firewall: description, instructions, etc.
 *
 */
void display_welcome_screen(void);

/**
 * Display a formatted message on the screen.
 *
 * @param msg A MESSAGE_CONSTANTS value denoting which message to display.
 *
 */
void display_message(const MESSAGE_SCREENS msg);

/**
 * Display opponent's badge ID on screen.
 *
 * Clears only the one line, calls FbPushBuffer()
 */
void fw_other_badge_msg(unsigned short other_id);

/**
 * Send initial peering request to all badges.
 */
void initiate_peering();

ship_type get_ship_type(struct firewall_global_state *fgstate,
        const byte x, const byte y);

void set_ship_type(struct firewall_global_state *fgstate,
        const byte x, const byte y, const ship_type ship);

void send_with_delay(fc_command command, unsigned payload);

void draw_cursors(struct firewall_global_state *fgstate);

void draw_hits(struct firewall_global_state *fgstate);

byte get_ship_glyph(struct firewall_global_state *fgstate, byte x, byte y);

/**
 * Draws crosshair graphic in cell with coords fgstate->curr_row,
 * fgstate->curr_column.
 */
void draw_crosshair(struct firewall_global_state *fgstate);

/**
 * Move crosshairs up (delta < 0) or down (delta > 0). If the location
 * that results has already been used for a shot, increases/decreases
 * delta by 1 and tries agin, repeatedly. If no suitable destination
 * is found, beeps and leaves crosshair where it is.
 *
 */
void fw_move_crosshair_vert(struct firewall_global_state *fgstate,
                            signed char delta);

/**
 * Move crosshairs right (delta < 0) or left (delta > 0). If the
 * location that results has already been used for a shot,
 * increases/decreases delta by 1 and tries agin, repeatedly. If no
 * suitable destination is found, beeps and leaves crosshair where it
 * is.
 *
 */
void fw_move_crosshair_horiz(struct firewall_global_state *fgstate,
                             signed char delta);

/**
 * searches game grid right-to-left, top-to-bottom, to find first
 * square where player has not yet taken a shot. Sets
 * fgstate->curr_row, fgstate->curr_column to point to that square.
 *
 */
void find_first_valid_shot(struct firewall_global_state *fgstate);

/**
 * Set up screen so that player can take a shot, go to firing state.
 *
 */
void set_up_to_take_shot(void);

/**
 * Compare opponent's shots to grid of placed ships. If all ship
 * squares have a shot, return TRUE.
 */
int i_am_dead(struct firewall_global_state *fgstate);

#endif

/* Local Variables: */
/* compile-command: "make -k -f ~/Devel/badge2016/software/branches/lloyd/Makefile exec" */
/* compilation-scroll-output: first-error */
/* c-file-style: "k&r" */
/* End: */
