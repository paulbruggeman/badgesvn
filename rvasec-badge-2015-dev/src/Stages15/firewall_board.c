#ifndef FWBOARDDEBUG
#include "firewall.h"
#endif

#include "firewall_board.h"

#ifndef FWBOARDDEBUG
#include "badge15.h"
#endif

inline unsigned short get_pos(const unsigned char const grid[],
        const unsigned char x,
        const unsigned char y) {
    return grid[y] & (1 << x);
}

/*
 * checks the set of cells starting at grid coords x, y and extending vertically
 * or horizontally for length ship for collision with another
 * object. Returns FALSE (0) if no collision detected, TRUE otherwise
 */
byte check_ship(const byte grid[], const ship_type ship, const byte is_vertical, byte x, byte y) {
    int i;
    if (is_vertical) {
        x %= (GRID_BYTES - ship);
        y %= GRID_BYTES;
        for (i = 0; i < ship; i++) {
            if (get_pos(grid, x + i, y)) {
                return FALSE;
            }
        }
    } else {
        x %= GRID_BYTES;
        y %= (GRID_BYTES - ship);
        for (i = 0; i < ship; i++) {
            if (get_pos(grid, x, y + 1)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/* Place an instance of the given ship on the board at a random location. Returns TRUE if
 * placement was successful, FALSE if there was no room for ship on the board.
 */
int set_ship(const ship_type ship) {
    //char is_vert = rand() % 2;
    byte x = rand(); // ignore overflow
    byte y = rand(); // ignore overflow
    char i;
    /*
     * generate random location for ship.
     */

    x %= (GRID_BYTES - ship);
    y %= GRID_BYTES;
    byte orig_x = x;
    byte orig_y = y;

    /* algorithm for handling collision detection could use improvement. */
    while (!check_ship(fw_state.placed_items, ship, TRUE, x, y)) {
        x++;
        x %= GRID_BYTES;
        if (x == orig_x) {
            y = ++y % GRID_BYTES;
            if (y == orig_y) {
                // well, crap
                return FALSE;
            }
        }
    }

    for (i = 0; i < ship; i++) {
        // indicate that ship occupies this cell
        set_pos(fw_state.placed_items, x + i, y);
        // indicate which type
        switch (ship) {
            case NIC:
                break;
            case PROXY:
                set_pos(fw_state.placed_types[0], x + i, y);
                break;
            case SWITCH:
                set_pos(fw_state.placed_types[1], x + i, y);
                break;
            case ROUTER:
                set_pos(fw_state.placed_types[0], x + i, y);
                set_pos(fw_state.placed_types[1], x + i, y);
                break;
        }
    }
    return TRUE;
}

/*
 * Return the type of the ship which is placed in cell (x, y)
 */
ship_type get_type(const byte x, const byte y) {
    if (get_pos(fw_state.placed_types[0], x, y)) {
        if (get_pos(fw_state.placed_types[1], x, y)) {
            return ROUTER;
        } else {
            return PROXY;
        }
    } else {
        if (get_pos(fw_state.placed_types[1], x, y)) {
            return SWITCH;
        } else {
            return NIC;
        }
    }
}

void clear_grids(void) {
    clear_grid(fw_state.offensive_tries);
    clear_grid(fw_state.offensive_hits);
    clear_grid(fw_state.defensive_tries);
    clear_grid(fw_state.placed_items);
    clear_grid(fw_state.placed_types[0]);
    clear_grid(fw_state.placed_types[1]);
}
