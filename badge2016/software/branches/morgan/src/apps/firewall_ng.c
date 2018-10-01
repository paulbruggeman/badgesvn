#include "badge16.h"
#include "fb.h"
#include "colors.h"
#include "touchCTMU.h"
#include "firewall_ng.h"

/* Various fixed values used to lay out grid on LCD */
#define START_X 19 /* leave room for "current column" icon */
#define START_Y 19 /* leave room for "current row" icon */
#define COL_WIDTH 12 /* (128 - 20) / 9 columns == 12 */
#define ROW_HEIGHT 12
#define LINE_HEIGHT 10 /**< Height of text line */

/* We very frequently have to translate from grid cells (0 - 8) to LCD pixels
 * (0 - 127)
 */
#define GRID_LEFT(x) (START_X + (COL_WIDTH * (x)))
#define GRID_RIGHT(x) (GRID_LEFT((x) + 1) - 1)
#define GRID_TOP(y) (START_Y + (ROW_HEIGHT * (y)))
#define GRID_BOTTOM(y) (GRID_TOP((y) + 1) - 1)

// Global (singleton) firewall state
struct firewall_global_state fw_state = {0};

extern const char hextab[];

/**
 * Resets *all* firewall state to initial values.
 */
inline void reset_firewall_state(void) {
    memset(&fw_state, 0, sizeof (fw_state));
}

/**
 * Get grid value at x,y.
 *
 * @param grid an 8 byte grid array
 * @param x Horizontal position in grid, 0 <= x <= 7 (0 is left)
 * @param y Vertical position in grid, 0 <= x <= 7 (0 is top)
 * @return 0 or 1
 */
inline unsigned short get_pos(const byte const grid[],
        const byte x,
        const byte y) {
    return grid[y] & (1 << x) ? 1 : 0;
}

/* Displays a grid on screen, where we can draw hits/misses/"ships"
 * calls DlInit(), should always be first drawing routine called.
 */
void display_grid(void) {
    char curr_char;
    unsigned char pos;

    /* Top row, vertical lines */
    for (curr_char = 'A'; curr_char <= 'H'; curr_char++) {
        pos = GRID_LEFT(curr_char - 'A' + 1);

        FbColor(B_BLUE2);
        FbMove(pos + 4, START_Y);
        FbCharacter(curr_char);
        FbColor(WHITE);
        FbDashedVerticalLine(pos, GRID_TOP(1), pos, 127);
    } // for curr_col

    /* one extra line for right border */
    FbDashedVerticalLine(127, GRID_TOP(1), 127, 127);

    /* left column, vertical lines */
    for (curr_char = '0'; curr_char <= '7'; curr_char++) {
        pos = GRID_TOP(curr_char - '0' + 1);
        FbColor(B_BLUE2);
        FbMove(START_X, pos + 3);
        FbCharacter(curr_char);
        FbColor(WHITE);
        FbDashedHorizontalLine(GRID_LEFT(1), pos, 127, pos);
    } // for curr_row

    /* extra line for bottom border */
    FbDashedHorizontalLine(GRID_LEFT(1), 127, 127, 127);

} // display_grid();

void firewall_ng_cb(void) {
    switch (fw_state.state) {
        case START:
            display_welcome_screen();
            fw_state.state = WELCOME_SCREEN;
            break;
        case WELCOME_SCREEN:
            if (BUTTON_PRESSED_AND_CONSUME) {
                fw_state.state = INITIAL_DISPLAY;
                set_ship(&fw_state, ROUTER);
                set_ship(&fw_state, SWITCH);
                set_ship(&fw_state, SWITCH);
                set_ship(&fw_state, PROXY);
                set_ship(&fw_state, NIC);
                display_grid();
                draw_ships(&fw_state);
            };
            break;
        case INITIAL_DISPLAY:
            if (BUTTON_PRESSED_AND_CONSUME) {
                fw_state.state = EXIT;
            }
            break;
        case EXIT:
            reset_firewall_state();
            returnToMenus();
            break;
    };

    /* if (!displayed) { */
    /*     display_grid(); */
    /*     clear_grid(fw_state.placed_types[0]); */
    /*     clear_grid(fw_state.placed_types[1]); */
    /*     clear_grid(fw_state.placed_items); */
    /*     draw_ships(&fw_state); */
    /*     displayed = 1; */
    /* }; */
    /* if (BUTTON_PRESSED_AND_CONSUME) { */
    /*     displayed = 0; */
    /*     reset_firewall_state(); */
    /*     returnToMenus(); */
    /* } */
} // firewall_ng_cb()

/*
 * Checks the set of cells in `grid` starting at coords `x`, `y` and
 * extending vertically or horizontally for length of a ship of type
 * `ship` for collision with another object.
 */
byte check_ship(const byte grid[], const ship_type ship, const byte is_vertical, byte x, byte y) {
    int i;
    // ship_type of a ship == its length
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
int set_ship(struct firewall_global_state *fgstate, const ship_type ship) {
    byte x = rand(); // ignore overflow
    byte y = rand(); // ignore overflow
    char i;
    //char is_vert = rand() % 2;

    /*
     * generate random location for ship.
     */

    x %= (GRID_BYTES - ship);
    y %= GRID_BYTES;
    byte orig_x = x;
    byte orig_y = y;

    /* algorithm for handling collision detection could use improvement. */
    while (!check_ship(fgstate->placed_items, ship, TRUE, x, y)) {
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

    add_ship(fgstate, x, y, ship, HORIZONTAL);
    return TRUE;
}

unsigned char get_ship_glyph(
        struct firewall_global_state *fgstate,
        byte x, byte y) {
    ship_type ship = get_pos(fgstate->placed_types[0], x, y) +
            get_pos(fgstate->placed_types[1], x, y) * 2;
    switch (ship) {
        case NIC:
            return 'N';
        case PROXY:
            return 'P';
        case SWITCH:
            return 'S';
        case ROUTER:
            return 'R';
    }
    return '!'; //the oops, I screwed up default
}

void draw_ships(struct firewall_global_state *fgstate) {
    int i, j;
    // grid positions 0 are for labels
    for (i = 1; i < 9; i++) {
        for (j = 1; j < 9; j++) {
            if (get_pos(fgstate->placed_items, i - 1, j - 1)) {
                FbMove(GRID_LEFT(i) + 4, GRID_TOP(j) + 2);
                FbCharacter(get_ship_glyph(fgstate, i - 1, j - 1));
            } else {
                if (get_pos(fgstate->placed_types[0], i - 1, j - 1)
                        && get_pos(fgstate->placed_types[1], i - 1, j - 1)) {
                    // we screwed up somewhere, display error indicator
                    FbMove(GRID_LEFT(i) + 4, GRID_TOP(j) + 2);
                    FbCharacter('E');

                }
            }
        }
    }
}

int add_ship(struct firewall_global_state *fgstate,
        const unsigned int x, const unsigned int y,
        const ship_type ship, const orientation ship_orient) {
    int i;
    for (i = 0; i < ship + 1; i++) {
        // indicate that ship occupies this cell
        if (ship_orient == HORIZONTAL) {
            set_pos(fgstate->placed_items, x + i, y);
            // indicate which type
            switch (ship) {
                case NIC:
                    break;
                case PROXY:
                    set_pos(fgstate->placed_types[0], x + i, y);
                    break;
                case SWITCH:
                    set_pos(fgstate->placed_types[1], x + i, y);
                    break;
                case ROUTER:
                    set_pos(fgstate->placed_types[0], x + i, y);
                    set_pos(fgstate->placed_types[1], x + i, y);
                    break;
            }
        } else {
            set_pos(fgstate->placed_items, x, y + i);
            // indicate which type
            switch (ship) {
                case NIC:
                    break;
                case PROXY:
                    set_pos(fgstate->placed_types[0], x, y + i);
                    break;
                case SWITCH:
                    set_pos(fgstate->placed_types[1], x, y + i);
                    break;
                case ROUTER:
                    set_pos(fgstate->placed_types[0], x, y + i);
                    set_pos(fgstate->placed_types[1], x, y + i);
                    break;
            }
        }
    }
} // add_ship()

void display_welcome_screen(void) {
    const unsigned char LEFT_MARGIN = 5;
    unsigned char pos_y = 5;
    char *msg[] = {
        "   FIREWALL",
        "--------------",
        " DESTROY YOUR",
        "  OPPONENTS",
        "   NETWORK",
        "   BEFORE",
        "  HE OR SHE",
        "   DESTROYS",
        "    YOURS"
    };
    int i;
    for (i = 0; i < 9; i++) {
        FbMove(LEFT_MARGIN, pos_y);
        FbWriteLine(msg[i]);
        pos_y += LINE_HEIGHT;
    };
}

void draw_cursors(struct firewall_global_state *fgstate) {
    FbMove(GRID_LEFT(0), GRID_TOP(fgstate->curr_row));
    FbCharacter('-');
    FbMove(GRID_LEFT(fgstate->curr_column), GRID_TOP(0));
    FbCharacter(':');
}
