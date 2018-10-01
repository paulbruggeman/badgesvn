#include "badge16.h"
#include "assets.h"
#include "fb.h"
#include "colors.h"
#include "peripheral/timer.h"
#include "touchCTMU.h"
#include "menu.h"
#include "flash.h"
#include "firewall_ng.h"
#include "firewall_comm.h"
#include "assetList.h"

/* Various fixed values used to lay out grid on LCD */
#define START_X 19 /* leave room for "current column" icon */
#define START_Y 19 /* leave room for "current row" icon */
#define COL_WIDTH 12 /* (128 - 20) / 9 columns == 12 */
#define ROW_HEIGHT 12

#define LINE_HEIGHT 10 /**< Height of text line */

/* We very frequently have to translate from grid cells (0 - 7) to LCD pixels
 * (0 - 127)
 */
#define GRID_LEFT(x) (START_X + (COL_WIDTH * (x))) /**< leftmost pixel of grid cell in column `x` */
#define GRID_RIGHT(x) (GRID_LEFT((x) + 1) - 1) /**< rightmost pixel of grid cell in column `x` */
#define GRID_TOP(y) (START_Y + (ROW_HEIGHT * (y))) /**< top pixel of grid cell in row `y` */
#define GRID_BOTTOM(y) (GRID_TOP((y) + 1) - 1)  /**< bottom pixel of grid cell in row `y` */

extern char hextab[];

// Global (singleton) firewall state
struct firewall_global_state fw_state = {0};

// Text contents of various instruction screens

// screen will display about 11 lines with decent format
typedef const char const *screen_text[11];

screen_text SCREEN_MESSAGES[] = {
    //SCR_TITLE
    {"   FIREWALL",
     "---------------",
     " DESTROY YOUR",
     " OPPONENTS",
     " NETWORK",
     " BEFORE HE",
     " OR SHE",
     " DESTROYS",
     " YOURS",
     "",
     ""},
    //SCR_CONNECTING
    {"  CONNECTING",
     "--------------",
     " HAVE YOUR",
     " OPPONENT",
     " HOLD HIS OR",
     " HER BADGE",
     " NEAR YOURS",
     " UNTIL THIS",
     " SCREEN",
     " CHANGES",
     ""},
    //SCR_PAIRED
    {"  PAIRED",
     "--------------",
     " YOU HAVE",
     " BEEN PAIRED",
     " WITH YOUR",
     " OPPONENT",
     "",
     "",
     " WAIT TO",
     " SEE WHO",
     " GOES FIRST"},
    //SCR_FIRST_OFFENDER
    {"    READY",
     "--------------",
     " YOU HAVE",
     " BEEN RANDOMLY",
     " CHOSEN",
     " TO TAKE THE",
     " FIRST SHOT",
     " NEXT SCREEN:",
     " OFFENSE",
     "",
     ""},
    //SCR_FIRST_DEFENDER
    {"    READY",
     "--------------",
     " YOU HAVE",
     " BEEN RANDOMLY",
     " CHOSEN",
     " AS THE",
     " DEFENDER",
     " NEXT SCREEN:",
     " DEFENSE",
     "",
     ""},
    //SCR_ITS_A_HIT
    {"   CONGRATS",
     "--------------",
     " YOU FRY",
     " A JUNCTION",
     "",
     " SMOKE",
     " RISES",
     "",
     " ADMINS",
     " PANIC",
     ""},
    //SCR_YOU_MISSED
    {"   TOO_BAD",
     "--------------",
     " YOUR ATTACK",
     " MISSED",
     " VITAL",
     " PARTS",
     "",
     " ADMINS",
     " GET FAT",
     " BONUS",
     ""},
    //SCR_YOUVE_BEEN_HIT
    {"  OH NOES",
     "--------------",
     " YOUR",
     " INFRA",
     " STRUCTURE",
     " TAKES A",
     " HIT",
     "",
     " YOUR",
     " ULCER",
     " WORSENS"},
    //SCR_OPP_MISSED
    {" HA HA HA",
     "--------------",
     " YOU WATCH",
     " AS YOUR",
     " FIREWALL",
     " BLOCKS",
     " AN",
     " ATTACK",
     "",
     "",
     ""},
    //SCR_WAITING
    {" WAITING",
     "--------------",
     " YOU ARE",
     " SITTING",
     " IN A SMALL",
     " CUBICLE",
     " AS ATTACKS",
     " COMMENCE",
     "",
     "",
     ""},
    //SCR_NETWORK_GUIDE
    {" NETWORK",
     "--------------",
     " PARTS OF",
     " YOUR NETWORK",
     "",
     " N NIC 1",
     " P PROXY 2"
     " S SWITCH 3",
     " R ROUTER 4",
     "",
     ""},
    // SCR_WE_LOSE
    {" DEFEAT",
     "--------------",
     " YOUR FIRE",
     " WALL IS",
     " PWND YOUR",
     " DATA IS",
     " GONE",
     " FILE FOR",
     " UNEMPLOY",
     " MENT",
     ""},
    // SCR_WE_WON
    {" VICTORY",
     "--------------",
     " YOU PLUNDER",
     " YOUR VICTIMS",
     " NETWORK FOR",
     " WAREZ AND",
     " EMBARRASSING",
     " EMAILS",
     "",
     " YARRR",
     ""}
};

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
inline unsigned short get_pos(const byte grid[], const byte x, const byte y) {
    return grid[y] & (1 << x) ? 1 : 0;
}

/* Displays a grid on screen, where we can draw hits/misses/"ships"
 *
 */
void display_grid(void) {
    char curr_char;
    byte pos;

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


void display_our_ships() {
    FbClear();
    set_ship(&fw_state, ROUTER);
    set_ship(&fw_state, SWITCH);
    set_ship(&fw_state, SWITCH);
    set_ship(&fw_state, PROXY);
    set_ship(&fw_state, NIC);
    display_grid();
    draw_ships(&fw_state);
    FbSwapBuffers();
}


void fw_status_msg(const char const *msg) {
    if (strlen(msg) > 15) {
        return;
    }
    FbMove(GRID_LEFT(0), GRID_TOP(8));
    unsigned short color = G_Fb.color;
    G_Fb.color = G_Fb.BGcolor;
    FbFilledRectangle(127 - GRID_LEFT(0), GRID_TOP(9) - GRID_TOP(8));
    G_Fb.color = color;
    FbMove(GRID_LEFT(0) + 4, GRID_TOP(8) + 2);
    FbWriteLine(msg);
    FbPushBuffer();
}


void fw_other_badge_msg(unsigned short other_id) {
    char buffer[10];
    strcpy(buffer, " OPP:   ");
    buffer[6] = '0' + other_id/100 % 10;
    buffer[7] = '0' + other_id/10 % 10;
    buffer[8] = '0' + other_id % 10;
    buffer[9] = '\0';
    FbMove(0, 12 + LINE_HEIGHT * 6);
    unsigned short color = G_Fb.color;
    G_Fb.color = G_Fb.BGcolor;
    FbFilledRectangle(127, LINE_HEIGHT);
    G_Fb.color = color;
    FbMove(5, 12 + LINE_HEIGHT * 6);
    FbWriteLine(buffer);
    FbPushBuffer();
};
    
// Packets sent to "everybody" must have badgeID == 0 || badgeId > 447
// Packets sent to other badge must have badgeID == other badge's ID
// _All_ received packets will be ignored if !(badgeId == 0 || badgeId > 447 || badgeId == G_sysData.badgeId)
// Nobody is using IRPair(). Does it work???
void send_with_delay(fc_command command, unsigned payload) {
    static int delayCounter = RESEND_DELAY;
    if (delayCounter > 0) {
        delayCounter--;
    } else {
        fw_send_cmd(command, payload);
        delayCounter = RESEND_DELAY;
    }
} // send_with_delay()


void initiate_peering() {
    static int delayCounter = RESEND_DELAY;

    if (delayCounter > 0) {
        delayCounter--;
    } else {
        fw_send_cmd_all(MSG_SHALL_WE_PLAY_A_GAME, G_sysData.badgeId);
        delayCounter = RESEND_DELAY;
    }
}


void received_peering(unsigned short other_badge_id, game_state next_state) {
    fw_state.other_badge_id = other_badge_id;
    display_message(SCR_PAIRED);
    fw_other_badge_msg(fw_state.other_badge_id);
    fw_state.state = next_state;
}


/**
 * Show random numbers to prove they were transmitted OK
 */
/*
void fw_show_randoms() {
    const byte LEFT_MARGIN = 5;
    const byte TOP_MARGIN = 12;

    byte pos_y = TOP_MARGIN;
    FbClear();
    FbMove(LEFT_MARGIN, pos_y);
    FbWriteLine("OUR RANDOM");
    pos_y += LINE_HEIGHT;
    FbMove(LEFT_MARGIN, pos_y);
    FbWriteHexLine(fw_state.my_random);
    pos_y += LINE_HEIGHT;
    FbMove(LEFT_MARGIN, pos_y);
    FbWriteLine("THEIR RANDOM");
    pos_y += LINE_HEIGHT;
    FbMove(LEFT_MARGIN, pos_y);
    FbWriteHexLine(fw_state.their_random);
    FbSwapBuffers();
}
*/


void received_who_first(unsigned short their_random) {
    if (their_random == fw_state.my_random) {
        srand(ReadCoreTimer());
        fw_state.my_random = rand() & 0x7FF;
        fw_state.state = ST_WHO_GOES_FIRST;
    } else {
        fw_state.their_random = their_random;
        if (fw_state.my_random > fw_state.their_random) {
            display_message(SCR_FIRST_OFFENDER);
            fw_state.state = ST_DISPLAY_OFFENSE;
        } else {
            display_message(SCR_FIRST_DEFENDER);
            fw_state.state = ST_DISPLAY_DEFENSE;
        }
     }
} // received_who_first()


void received_shot(unsigned short location) {
    //unsigned short data = fw_state.curr_row << 4 | fw_state.curr_column;
    //and again we have row-column reversed.
    fw_state.curr_row = location & 0b1111;
    fw_state.curr_column = location >> 4;
    unsigned short hit = get_pos(fw_state.placed_items, fw_state.curr_row, fw_state.curr_column);
    set_pos(fw_state.defensive_tries, fw_state.curr_row, fw_state.curr_column);
    if (hit) {
        if (i_am_dead(&fw_state)) {
            fw_state.state = ST_DEAD;
            display_message(SCR_WE_LOSE);
        } else {
            fw_state.state = ST_SEND_HIT;
            display_message(SCR_YOUVE_BEEN_HIT);
        }
        //show_row_col(fw_state.curr_row, fw_state.curr_column);
    } else {
        fw_state.state = ST_SEND_MISS;
        display_message(SCR_OPP_MISSED);
        //show_row_col(fw_state.curr_row, fw_state.curr_column);
    }
} // received_shot()

/*
void show_row_col(unsigned short row, unsigned short col) {
    FbMove(5, 5);
    FbCharacter('A' + col);
    FbMove(15, 5);
    FbCharacter('0' + row);
    FbPushBuffer();
}
*/

void received_confirmation(int its_a_hit, unsigned short location) {
   byte hit_row = location & 0b1111;
   byte hit_column = location >> 4;
   if (its_a_hit) {
       set_pos(fw_state.offensive_hits, hit_row, hit_column);
       display_message(SCR_ITS_A_HIT);
       fw_state.state = ST_DISPLAY_HIT;
   } else {
       display_message(SCR_YOU_MISSED);
       fw_state.state = ST_DISPLAY_MISS;
   }
} // received_confirmation()

void received_lets_start(unsigned short their_random) {
    fw_state.their_random = their_random;
    if (fw_state.my_random > fw_state.their_random) {
        display_message(SCR_FIRST_OFFENDER);
        fw_state.state = ST_DISPLAY_OFFENSE;
    } else {
        display_message(SCR_FIRST_DEFENDER);
        fw_state.state = ST_DISPLAY_DEFENSE;
    }
}  //void received_lets_start()

void received_victory() {
    //oh, that sweet, sweet, smell...
    display_message(SCR_WE_WON);
    fw_state.state = ST_BASKING;
}

void firewall_ng_cb(void) {
    static unsigned long startTime = 0;

    if (startTime == 0) {
        startTime = ReadCoreTimer();
        /* if we don't do this, the players may get same pseudo-random
         * sequence
         */
        srand(startTime);
         /* mask down to 11 bits, because that's the space in message */
        fw_state.my_random = rand() & 0x7FF;
    }

    switch (fw_state.state) {
    case ST_START:
        display_message(SCR_TITLE);
        fw_state.state = ST_WELCOME_SCREEN;
        break;
    case ST_WELCOME_SCREEN:
        if (BUTTON_PRESSED_AND_CONSUME) {
            fw_state.state = ST_INITIAL_DISPLAY;
            display_our_ships();
        };
        break;
    case ST_INITIAL_DISPLAY:
        if (BUTTON_PRESSED_AND_CONSUME) {
            fw_state.state = ST_WAITING_FOR_CONNECT;
            display_message(SCR_CONNECTING);
        }
        break;
    case ST_WAITING_FOR_CONNECT:
        initiate_peering();
        if (BUTTON_PRESSED_AND_CONSUME) {
            // to get past this state requires interaction with other
            // badge, state should be set by firewall_ir_cb()
            fw_state.state = ST_EXIT;
        }
        break;
    case ST_WAITING_FOR_OTHER_PLAYER:
        send_with_delay(MSG_LOVE_TO, G_sysData.badgeId);
        if (BUTTON_PRESSED_AND_CONSUME) {
            // to get past this state requires interaction with other
            // badge, state should be set by firewall_ir_cb()
            fw_state.state = ST_WHO_GOES_FIRST;
        }
        break;
    case ST_WHO_GOES_FIRST:
        // send our random to other  badge
        send_with_delay(MSG_WHO_STARTS, fw_state.my_random);
        if (BUTTON_PRESSED_AND_CONSUME) {
            fw_state.state = ST_EXIT;
        }
        break;
    case ST_DISPLAY_OFFENSE:
        // we have to acknowledge that we got the msg so sending badge can transition
        send_with_delay(MSG_LETS_START, fw_state.my_random);
        if (BUTTON_PRESSED_AND_CONSUME) {
            set_up_to_take_shot();
        }
        break;
    case ST_DISPLAY_DEFENSE:
        send_with_delay(MSG_LETS_START, 0);
        if (BUTTON_PRESSED_AND_CONSUME) {
            display_message(SCR_WAITING);
            fw_state.state = ST_WAITING;
        }
        break;
    case ST_WAITING:
        if (BUTTON_PRESSED_AND_CONSUME) {
            fw_state.state = ST_EXIT;
        }
        break;
    case ST_TAKE_SHOT:
        // TODO: switch row/column handling to reflect actual layout
        if (TOP_TAP_AND_CONSUME) {
            fw_move_crosshair_horiz(&fw_state, -1);
        } else if (BOTTOM_TAP_AND_CONSUME) {
            fw_move_crosshair_horiz(&fw_state, 1);
        } else if (LEFT_TAP_AND_CONSUME) {
            fw_move_crosshair_vert(&fw_state, -1);
        } else if (RIGHT_TAP_AND_CONSUME) {
            fw_move_crosshair_vert(&fw_state, 1);
        };
        if (BUTTON_PRESSED_AND_CONSUME) {
            set_pos(fw_state.offensive_tries, fw_state.curr_row, fw_state.curr_column);
            fw_state.state = ST_SENDING_SHOT;
        };
        break;
    case ST_SENDING_SHOT:
        // send message about shot
        send_with_delay(MSG_SHOT_TAKEN, (fw_state.curr_row << 4) | fw_state.curr_column);
        if (BUTTON_PRESSED_AND_CONSUME) {
            fw_state.state = ST_EXIT;
        };
        break;
    case ST_SEND_HIT:
        // we've been hit, showing ships on screen, but need to send result to opp
        send_with_delay(MSG_IM_HIT, (fw_state.curr_row << 4) | fw_state.curr_column);
        if (BUTTON_PRESSED_AND_CONSUME) {
            set_up_to_take_shot();
        }
        break;
    case ST_SEND_MISS:
        // opponent missed, we're showing "You were missed" screen
        send_with_delay(MSG_YOU_MISSED, (fw_state.curr_row << 4) | fw_state.curr_column);
        if (BUTTON_PRESSED_AND_CONSUME) {
            set_up_to_take_shot();
        }
        break;
    case ST_DISPLAY_HIT:
        if (BUTTON_PRESSED_AND_CONSUME) {
            FbClear();
            display_grid();
            draw_shots(&fw_state);
            FbSwapBuffers();
            fw_state.state = ST_SHOW_OFF_GRID;
        }
        break;
    case ST_DISPLAY_MISS:
        if (BUTTON_PRESSED_AND_CONSUME) {
            FbClear();
            display_grid();
            draw_shots(&fw_state);
            FbSwapBuffers();
            fw_state.state = ST_SHOW_OFF_GRID;
        }
        break;
    case ST_SHOW_OFF_GRID:
        if (BUTTON_PRESSED_AND_CONSUME) {
            display_message(SCR_WAITING);
            fw_state.state = ST_WAITING;
        }
        break;
    case ST_DEAD:
        send_with_delay(MSG_IM_DEAD, 0);
        if (BUTTON_PRESSED_AND_CONSUME) {
            fw_state.state = ST_EXIT;
        }
        break;
    case ST_EXIT:
        reset_firewall_state();
        startTime = 0;
        returnToMenus();
        break;
    };

} // firewall_ng_cb()


void set_up_to_take_shot(void) {
    FbClear();
    display_grid();
    draw_shots(&fw_state);
    draw_cursors(&fw_state);
    // make sure crosshairs in first available square
    find_first_valid_shot(&fw_state);
    draw_crosshair(&fw_state);
    FbPushBuffer();
    fw_state.state = ST_TAKE_SHOT;
}

/*
n * Checks the set of cells in `grid` starting at coords `x`, `y` and
 * extending vertically or horizontally for length of a ship of type
 * `ship` for collision with another object.
 */
byte check_ship(const byte grid[], const ship_type ship,
        const orientation ship_orient, byte x, byte y) {
    int i;
    // ship_type of a ship == its length
    if (ship_orient == HORIZONTAL) {
        // minimum ship is 1, which can go in pos 7
        if (x > (GRID_CELLS - 1) || y > (GRID_CELLS - ship)) {
            return FALSE;
        }
        for (i = 0; i < ship; i++) {
            if (get_pos(grid, x + i, y)) {
                return FALSE;
            }
        }
    } else {
        if (x > (GRID_CELLS - ship) || y > (GRID_CELLS - 1)) {
            return FALSE;
        }
        for (i = 0; i < ship; i++) {
            if (get_pos(grid, x, y + i)) {
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
    const orientation orient = rand() % 2 == 0 ? VERTICAL : HORIZONTAL;

    /*
     * generate random location for ship.
     */

    if (orient == VERTICAL) {
        x %= GRID_CELLS;
        y %= (GRID_CELLS - (ship - 1));
    } else {
        x %= (GRID_CELLS - (ship - 1));
        y %= GRID_CELLS;
    }
    //save first try coords for retry
    byte orig_x = x;
    byte orig_y = y;

    /* algorithm for handling collision detection could use improvement. */
    while (!check_ship(fgstate->placed_items, ship, orient, x, y)) {
        if (orient == VERTICAL) {
            // try next column
            ++x;
            x %= GRID_CELLS;
            if (x == orig_x) {
                // try next row
                ++y;
                y = y % (GRID_CELLS - (ship - 1));
                if (y == orig_y) {
                    // tried all rows, columns. crap.
                    return FALSE;
                }
            }
        } else {
            ++y;
            y %= GRID_CELLS;
            if (y == orig_y) {
                ++x;
                x = x % (GRID_CELLS - (ship - 1));
                if (x == orig_x) {
                    return FALSE;
                }
            }
        } // if (orient = ...
    } //while

    add_ship(fgstate, x, y, ship, orient);
    return TRUE;
} // set_ship()


ship_type get_ship_type(struct firewall_global_state *fgstate,
        const byte x, const byte y) {
    return (ship_type) (get_pos(fgstate->placed_types[0], x, y) * 2 +
            get_pos(fgstate->placed_types[1], x, y) + 1);
} // get_ship_type()


void set_ship_type(struct firewall_global_state *fgstate,
        const byte x, const byte y, const ship_type ship) {
    switch (ship) {
        case NIC:
            clear_pos(fgstate->placed_types[0], x, y);
            clear_pos(fgstate->placed_types[1], x, y);
            break;
        case PROXY:
            clear_pos(fgstate->placed_types[0], x, y);
            set_pos(fgstate->placed_types[1], x, y);
            break;
        case SWITCH:
            set_pos(fgstate->placed_types[0], x, y);
            clear_pos(fgstate->placed_types[1], x, y);
            break;
        case ROUTER:
            set_pos(fgstate->placed_types[0], x, y);
            set_pos(fgstate->placed_types[1], x, y);
    }
} // set_ship_type()


byte get_ship_glyph(struct firewall_global_state *fgstate, byte x, byte y) {
    switch (get_ship_type(fgstate, x, y)) {
        case NIC:
            return 'N';
        case PROXY:
            return 'P';
        case SWITCH:
            return 'S';
        case ROUTER:
            return 'R';
    }
    return '!'; //shouldn't happen
}


void draw_ships(struct firewall_global_state *fgstate) {
    int i, j;
    // like "padding" in CSS: empty pixels between character and border
    static const int X_PADDING = 4;
    static const int Y_PADDING = 2;

    for (i = 0; i < GRID_CELLS; i++) {
        for (j = 0; j < GRID_CELLS; j++) {
            if (get_pos(fgstate->placed_items, i, j)) {
                // GRID_LEFT, GRID_TOP have 0, 0 for label columns
                FbMove(GRID_LEFT(i + 1) + X_PADDING, GRID_TOP(j + 1) + Y_PADDING);
                FbCharacter(get_ship_glyph(fgstate, i, j));
            } else {
                //error check: no bit for "ship is here" but bit(s) for "ship is type x"
                if (get_ship_type(fgstate, i, j) != NIC) {
                    // we screwed up somewhere, display error indicator
                    FbMove(GRID_LEFT(i + 1) + X_PADDING, GRID_TOP(j + 1) + Y_PADDING);
                    FbCharacter('E');
                }
            }
        } // for j
    } // for i
}


void draw_miss(struct firewall_global_state *fgstate, byte row, byte col) {
    FbMove(GRID_LEFT(col + 1) + 4, GRID_TOP(row + 1) + 2);
    FbCharacter('O');
}


void draw_shots(struct firewall_global_state *fgstate) {
    int i, j;
    // like "padding" in CSS: empty pixels between character and border
    static const int X_PADDING = 4;
    static const int Y_PADDING = 2;

    for (i = 0; i < GRID_CELLS; i++) {
        for (j = 0; j < GRID_CELLS; j++) {
            if (get_pos(fgstate->offensive_tries, i, j)) {
                // GRID_LEFT, GRID_TOP have 0, 0 for label columns
                FbMove(GRID_LEFT(i + 1) + X_PADDING, GRID_TOP(j + 1) + Y_PADDING);
                if (get_pos(fgstate->offensive_hits, i, j)) {
                    FbCharacter('X');
                } else {
                    FbCharacter('O');
                }
            } 
        } // for j
    } // for i
}


void add_ship(struct firewall_global_state *fgstate,
              const unsigned int x, const unsigned int y,
              const ship_type ship, const orientation ship_orient) {
    int i;
    for (i = 0; i < ship; i++) {
        // indicate that ship occupies this cell
        if (ship_orient == HORIZONTAL) {
            set_pos(fgstate->placed_items, x + i, y);
            set_ship_type(fgstate, x + i, y, ship);
        } else {
            set_pos(fgstate->placed_items, x, y + i);
            set_ship_type(fgstate, x, y + i, ship);
        }
    } // for
} // add_ship()


void display_message(const MESSAGE_SCREENS msg) {
    const byte LEFT_MARGIN = 5;
    const byte TOP_MARGIN = 12;

    byte pos_y = TOP_MARGIN;
    int i;
    const char const **lines = SCREEN_MESSAGES[msg];
    FbClear();
    for (i = 0; i < 11; i++) {
        FbMove(LEFT_MARGIN, pos_y);
        FbWriteLine(lines[i]);
        pos_y += LINE_HEIGHT;
    };
    FbPushBuffer();
}


void draw_cursors(struct firewall_global_state *fgstate) {
    FbMove(START_X - COL_WIDTH, GRID_TOP(fgstate->curr_column + 1) + 3);
    FbCharacter('-');
    FbMove(GRID_LEFT(fgstate->curr_row + 1) + 3, START_Y - LINE_HEIGHT);
    FbCharacter(':');
}


void draw_crosshair(struct firewall_global_state *fgstate) {
    FbHorizontalLine(GRID_LEFT(fgstate->curr_row + 1) + 1,
                     GRID_TOP(fgstate->curr_column + 1) + 6,
                     GRID_LEFT(fgstate->curr_row + 2) - 1,
                     0);
   FbVerticalLine(GRID_LEFT(fgstate->curr_row + 1) + 6,
                  GRID_TOP(fgstate->curr_column + 1) + 1,
                  0,
                  GRID_TOP(fgstate->curr_column + 2) - 1);
};
    

void draw_hits(struct firewall_global_state *fgstate) {
    int x, y;
    for (x = 0; x < GRID_CELLS; x++) {
        for (y = 0; y < GRID_CELLS; y++) {
            if (get_pos(fgstate->offensive_tries, x, y)) {
                FbMove(GRID_LEFT(x) + 4, GRID_TOP(y) + 2);
                if (get_pos(fgstate->offensive_hits, x, y)) {
                    FbCharacter('X');
                } else {
                    FbCharacter('O');
                }
            } // if
        } // for (y
    } // for (x
} // draw_hits()


void fw_move_crosshair_vert(struct firewall_global_state *fgstate,
                            signed char delta) {
    signed char new_row = fgstate->curr_row + delta;

    if (new_row > 7 || new_row < 0 || delta == 0) {
        setNote(NOTE_G, 7800); //chirp
        return;
    }
    
    while (get_pos(fgstate->offensive_tries, new_row, fgstate->curr_column)) {
        if (delta > 0) {
            new_row += 1;
        } else {
            new_row -= 1;
        };
        if (new_row > 7 || new_row < 0) {
            setNote(NOTE_G, 7800); //chirp
            return;
        }
    } //while

    // if we're here, (new_row, fgstate->curr_column) is valid and
    // hasn't been tried
    
    // erase existing markers
    FbColor(G_Fb.BGcolor);
    draw_cursors(fgstate);
    draw_crosshair(fgstate);
    fgstate->curr_row = new_row;
    FbColor(0xFFFF);
    draw_cursors(fgstate);
    draw_crosshair(fgstate);
    FbPushBuffer();
} // fw_move_crosshair_vert()


void fw_move_crosshair_horiz(struct firewall_global_state *fgstate,
                            signed char delta) {
    signed char new_col = fgstate->curr_column + delta;

    if (new_col > 7 || new_col < 0 || delta == 0) {
        setNote(NOTE_G, 7800); //chirp
        return;
    }
    
    while (get_pos(fgstate->offensive_tries, fgstate->curr_row, new_col)) {
        if (delta > 0) {
            new_col += 1;
        } else {
            new_col -= 1;
        };
        if (new_col > 7 || new_col < 0) {
            setNote(NOTE_G, 7800); //chirp
            return;
        }
    } //while

    // if we're here, (fgstate->curr_row, new_col) is valid and hasn't
    // been tried
    
    // erase existing markers
    FbColor(G_Fb.BGcolor);
    draw_cursors(fgstate);
    draw_crosshair(fgstate);
    fgstate->curr_column = new_col;
    FbColor(0xFFFF);
    draw_cursors(fgstate);
    draw_crosshair(fgstate);
    FbPushBuffer();
} // fw_move_crosshair_vert()

void find_first_valid_shot(struct firewall_global_state *fgstate) {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (!get_pos(fgstate->offensive_tries, i, j)) {
                //search right-to-left, then top-to-bottom
                fgstate->curr_column = j;
                fgstate->curr_row = i;
                return;
            }
        }  //for (j
    } //for (i
} //find_first_valid_shot()


int i_am_dead(struct firewall_global_state *fgstate) {
    int i, check;
    for (i = 0; i < GRID_BYTES; i++) {
        check = fgstate->defensive_tries[i] & fgstate->placed_items[i];
        // now check has a 1 for each shot that hit
        if (check != fgstate->placed_items[i]) {
            // at least one bit in placed_items is unhit
            return FALSE;
        }
    }
    // all ship positions have been hit.
    return TRUE;
}

/**
 * Display a value on screen in hex.
 *
 */
/*
void FbWriteHexLine(unsigned int value)
{
    //debug("FbWriteHexLine ");
    char this_digit = 0, hexdigits = sizeof(value) * 2;
    int x, y, x_inc;

    x = G_Fb.pos.x;
    y = G_Fb.pos.y;
    x_inc = assetList[G_Fb.font].x;

    while (hexdigits) {
	FbMove(x + hexdigits * x_inc, y);
        this_digit = value & 0xF;
        value >>= 4;
	FbCharacter(hextab[this_digit]);
        hexdigits--;
    }
    G_Fb.changed = 1;

}
*/

/* Local Variables: */
/* compile-command: "make -k -f ~/Devel/badge2016/software/branches/lloyd/Makefile exec" */
/* compilation-scroll-output: first-error */
/* c-file-style: "k&r" */
/* c-basic-offset: 4 */
/* End: */
