#include "badge15.h"
#include "firewall.h"
#define FWBOARDEXTERN
#include "firewall_board.h"
#include "firewall_comm.h"

#include "touchCTMU.h"

extern badge_state b_state; //badge state structure to reduce variables
extern const char hextab[];

#define FIREWALL_STATE b_state.counter1

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

/* Various fixed values used to lay out grid on LCD */
#define START_X 19 /* leave room for "current column" icon */
#define START_Y 19 /* leave room for "current row" icon */
#define COL_WIDTH 12
#define ROW_HEIGHT 12

/* We very frequently have to translate from grid cells (0 - 8) to LCD pixels
 * (0 - 127)
 */
#define GRID_LEFT(x) (START_X + (COL_WIDTH * (x)))
#define GRID_RIGHT(x) (GRID_LEFT((x) + 1) - 1)
#define GRID_TOP(y) (START_Y + (ROW_HEIGHT * (y)))
#define GRID_BOTTOM(y) (GRID_TOP((y) + 1) - 1)

inline unsigned char grid_top(const unsigned char y) {
    return START_Y + (ROW_HEIGHT * y);
}

inline unsigned char grid_bottom(const unsigned char y) {
    return grid_top(y + 1) - 1;
}

void show_selected(const byte row, const byte column) {
    /* chipsprite is oversized for my purposes, need custom */
    DlMove(2, GRID_TOP(row + 1) - 1);
    DlTransparentIndex(5);
    /* 1 -> DARK RED
       2 -> GREY
       3 ->
       4 ->
     */
    DlPicture(CHIPSPRITE, 2);
    DlMove(GRID_LEFT(column + 1) - 1, 3);
    DlPicture(CHIPSPRITE, 1); //RED
}

/*
 * GAME PLAY
 * welcome screen
 * instruction screen
 * show our ship layout
 * badges negotiate who goes first -- random?
 * while both players have hardware
 *     first player chooses shot
 *     second player sees hit/miss
 *     second player chooses shot
 *     first player sees hit/miss
 * loser badge detects loss, notifies winner
 * winner sees victory page
 * loser sees loss page
 */

/** Write out zero-terminated string beginning at pixels (x, y) in color
 * color. Terminates when '\0' reached or there is no space on display for
 * another character.
 */
void writelinez(char * charin, unsigned char x,
        const unsigned char y, const unsigned short color) {
    DlColor(color);
    while (*charin && x < (127 - CHAR_WIDTH)) {
        DlMove(x, y);
        DlCharacter(*charin);
        x += 6;
        ++charin;
    }
}

#define center_text(str, len, y, color) \
       writelinez(str, (127 - (len * CHAR_SPACING)) / 2, y, color)

/* Display our hits/misses */
void show_shots(void) {
    int x, y;
    for (x = 1; x < 9; x++) {
        for (y = 1; y < 9; y++) {
            DlMove(GRID_LEFT(x) + 3, GRID_TOP(y) + 2);
            if (get_pos(fw_state.offensive_tries, x - 1, y - 1)) {
                if (get_pos(fw_state.offensive_hits, x - 1, y - 1)) {
                    DlCharacter('X'); //hit
                } else {
                    DlCharacter('0'); //miss
                }
            } else {
                DlCharacter('-'); //untried
            } // if
        } // for (y = 1; ...
    } // for (x = 1; ...
}

/* Assuming a grid has been drawn, place 'ship' icons as needed */
void show_ships(void) {
    // for each x, y
    // is there a ship in this cell?
    // did opponent shoot this cell?
    int x, y;
    for (x = 0; x < GRID_BYTES; x++) {
        for (y = 0; y < GRID_BYTES; y++) {
            // skip one row and one column for labels
            DlMove(GRID_LEFT(x + 1) + 3, GRID_TOP(y + 1) + 2);
            if (get_pos(fw_state.placed_items, x, y)) {
                //ship
                if (get_pos(fw_state.defensive_tries, x, y)) {
                    DlColor(RED);
                    DlCharacter('X');
                } else {
                    DlColor(YELLOW);
                    switch (get_type(x, y)) {
                        case NIC:
                            DlCharacter('N');
                            break;
                        case SWITCH:
                            DlCharacter('S');
                            break;
                        case PROXY:
                            DlCharacter('P');
                            break;
                        case ROUTER:
                            DlCharacter('R');
                    }
                }
            } else {
                // no ship
                if (get_pos(fw_state.defensive_tries, x, y)) {
                    DlColor(GREEN);
                    DlCharacter('-');
                }
            } // if (get_pos...
        } // for (y = 0;...
    } // for (x = 0; ...
}

/* Displays our initial splash screen */
void display_welcome(void) {

    if (b_state.state_drawn == 0) {
        unsigned char i = 0;
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        center_text("FIREWALL", 8, TOP_LINE + (LINE_SPACE * i), GREEN);
        i += 2;
        center_text("YOU", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        center_text("PWNED", 4, TOP_LINE + (LINE_SPACE * i++), GREEN);
        center_text("MY", 2, TOP_LINE + (LINE_SPACE * i++), GREEN);
        center_text("NETWORK!", 8, TOP_LINE + (LINE_SPACE * i++), GREEN);

        center_text("Press Button", 12, 128 - LINE_SPACE - 5, RED);

        b_state.state_drawn = 1;
        DlSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        FIREWALL_STATE = FIREWALL_INSTR;
    }
}

/* Displays page (or 2) of instructions for user */
void display_help(void) {

    if (b_state.state_drawn == 0) {
        unsigned char i = 0;
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        center_text("FIREWALL", 8, TOP_LINE + (LINE_SPACE * i++), GREEN);
        i++;
        writelinez("PWN your opponents", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("network -- destroy", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("hardware. Use scroll", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("to place your shot,", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("button to fire.", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        center_text("Press Button", 12, 128 - LINE_SPACE - 5, RED);
        b_state.state_drawn = 1;
        DlSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        FIREWALL_STATE = FIREWALL_INSTR2;
    }
}

/* Displays list of network components */
void display_help2(void) {

    if (b_state.state_drawn == 0) {
        unsigned char i = 0;
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        writelinez("YOUR NETWORK:", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        i++;
        writelinez("NIC: N", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("Proxy: PP", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("Switch: SSS", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("Router: RRRR", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        i++;
        writelinez("Layout of your", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("network is random.", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("Press button to see", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);
        writelinez("it.", 3, TOP_LINE + (LINE_SPACE * i++), GREEN);

        b_state.state_drawn = 1;
        DlSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        FIREWALL_STATE = FIREWALL_LAYOUT;
    }
}

void display_waiting(void) {

    if (b_state.state_drawn == 0) {
        unsigned char i = 0;
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        writelinez("PAIRING", 3, TOP_LINE, GREEN);
        writelinez("Searching for", 3, TOP_LINE + LINE_SPACE * 2, GREEN);
        writelinez("other badge to", 3, TOP_LINE + LINE_SPACE * 3, GREEN);
        writelinez("play with. Other", 3, TOP_LINE + LINE_SPACE * 4, GREEN);
        writelinez("badge must be", 3, TOP_LINE + LINE_SPACE * 5, GREEN);
        writelinez("showing this", 3, TOP_LINE + LINE_SPACE * 6, GREEN);
        writelinez("screen.", 3, TOP_LINE + LINE_SPACE * 7, GREEN);
        writelinez("Button: cancel game", 3, 127 - LINE_SPACE, RED);

        b_state.state_drawn = 1;
        DlSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        FIREWALL_STATE = FIREWALL_EXIT;
    }

    if (G_peerBadgeId) { // successfully paired?
        b_state.state_drawn = 0;
        //TODO: have one badge determine randomly, will require co-ordination msg on IR
        if (G_peerBadgeId > G_sysData.badgeId) {
            FIREWALL_STATE = FIREWALL_DEFENSE;
        } else {
            FIREWALL_STATE = FIREWALL_OFFENSE;
        }
    } else {
        IRPair();
    }

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
        DlColor(RED);
        DlMove(pos + 4, START_Y);
        DlCharacter(curr_char);
        DlColor(WHITE);
        DlLine(pos, GRID_TOP(1), pos, 127);
    } // for curr_col

    /* one extra line for right border */
    DlLine(127, GRID_TOP(1), 127, 127);

    /* left column, vertical lines */
    for (curr_char = '0'; curr_char <= '7'; curr_char++) {
        pos = GRID_TOP(curr_char - '0' + 1);
        DlColor(RED);
        DlMove(START_X, pos + 3);
        DlCharacter(curr_char);
        DlColor(WHITE);
        DlLine(GRID_LEFT(1), pos, 127, pos);
    } // for curr_row

    /* extra line for bottom border */
    DlLine(GRID_LEFT(1), 127, 127, 127);

} // display_grid();

/* Displays ships */
void display_layout(void) {
    if (b_state.state_drawn == 0) {
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        display_grid();
        center_text("YOUR NETWORK", 12, 5, WHITE);
        set_ship(NIC);
        set_ship(NIC);
        set_ship(PROXY);
        set_ship(SWITCH);
        set_ship(ROUTER);
        show_ships();
        DlSwapBuffers();
        b_state.state_drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        /* temporary. need way for user to exit game, may need to go to won/lost state */
        FIREWALL_STATE = FIREWALL_CONNECT;
    }
}

void send_shot_to_other_board(byte row, byte column) {
    fw_send_shot(row, column);
}

/* Displays our hits/misses, allows us to pick shot */
void attack_firewall(void) {

    if (b_state.state_drawn == 0) {
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        display_grid();
        writelinez("YOUR", 1, 3, WHITE);
        writelinez("TURN", 1, LINE_SPACE + 3, WHITE);
        show_shots();
        show_selected(fw_state.curr_row, fw_state.curr_column);
        DlSwapBuffers();
        b_state.state_drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        if (!get_pos(fw_state.offensive_tries, fw_state.curr_column, fw_state.curr_row)) {
            set_pos(fw_state.offensive_tries, fw_state.curr_column, fw_state.curr_row);
            send_shot_to_other_board(fw_state.curr_column, fw_state.curr_row);
            b_state.state_drawn = 0;
            FIREWALL_STATE = FIREWALL_OFFENSE_RESULT;
        } else {
            /* beep? print warning message? */
        }
    } else if (TOP_TAP_AND_CONSUME) {
        if (fw_state.curr_row > 0) {
            fw_state.curr_row--;
            b_state.state_drawn = 0;
        }
    } else if (BOTTOM_TAP_AND_CONSUME) {
        if (fw_state.curr_row < 7) {
            fw_state.curr_row++;
            b_state.state_drawn = 0;
        }
    } else if (LEFT_TAP_AND_CONSUME) {
        if (fw_state.curr_column > 0) {
            fw_state.curr_column--;
            b_state.state_drawn = 0;
        }
    } else if (RIGHT_TAP_AND_CONSUME) {
        if (fw_state.curr_column < 7) {
            fw_state.curr_column++;
            b_state.state_drawn = 0;
        }
    } /* if */
} /* attack_firewall() */

/* Displays our hits/misses */
void show_offense(void) {

    if (b_state.state_drawn == 0) {
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        display_grid();
        show_shots();
        DlSwapBuffers();
        b_state.state_drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        FIREWALL_STATE = FIREWALL_DEFENSE;
    } /* if */
}

/* Displays ships, opponents hits/misses. */
void defend_firewall(void) {
    if (b_state.state_drawn == 0) {
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        display_grid();
        center_text("OPPONENTS TURN", 14, 5, WHITE);
        show_ships();
        DlSwapBuffers();
        b_state.state_drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        /* temporary. need way for user to exit game, may need to go to won/lost state */
        FIREWALL_STATE = FIREWALL_OFFENSE;
    }
}

/* Display winner's screen */
void show_victory(void) {

    if (b_state.state_drawn == 0) {
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        center_text("YOU WIN!", 8, TOP_LINE, RED);
        writelinez("Your opponents net-", 3, TOP_LINE + (LINE_SPACE * 2), GREEN);
        writelinez("work is toast. You", 3, TOP_LINE + (LINE_SPACE * 3), GREEN);
        writelinez("are now downloading:", 3, TOP_LINE + (LINE_SPACE * 4), GREEN);
        writelinez("financial info", 3, TOP_LINE + (LINE_SPACE * 5), GREEN);
        writelinez("passwords", 3, TOP_LINE + (LINE_SPACE * 6), GREEN);
        writelinez("blackmail material", 3, TOP_LINE + (LINE_SPACE * 7), GREEN);
        writelinez("sex tapes", 3, TOP_LINE + (LINE_SPACE * 8), GREEN);
        center_text("Congratulations!", 16, 127 - LINE_SPACE, BLUE);

        b_state.state_drawn = 1;
        DlSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME) {
        b_state.state_drawn = 0;
        FIREWALL_STATE = FIREWALL_LOST;
    }
}

/* Display sarcastic message about our bitter defeat */
void show_loss(void) {
    if (b_state.state_drawn == 0) {
        DlInit();
        DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        center_text("YOU LOSE!", 9, TOP_LINE, RED);
        writelinez("Your network is", 3, TOP_LINE + (LINE_SPACE * 2), GREEN);
        writelinez("toast. All your bank", 3, TOP_LINE + (LINE_SPACE * 3), GREEN);
        writelinez("accounts are empty.", 3, TOP_LINE + (LINE_SPACE * 4), GREEN);
        writelinez("Your most embarrass-", 3, TOP_LINE + (LINE_SPACE * 5), GREEN);
        writelinez("sing secrets are on", 3, TOP_LINE + (LINE_SPACE * 6), GREEN);
        writelinez("Facebook and your", 3, TOP_LINE + (LINE_SPACE * 7), GREEN);
        writelinez("spouse wants a div-", 3, TOP_LINE + (LINE_SPACE * 8), GREEN);
        writelinez("orce.", 3, TOP_LINE + (LINE_SPACE * 9), GREEN);
        center_text("SORRY", 5, 127 - LINE_SPACE, BLUE);

        b_state.state_drawn = 1;
        DlSwapBuffers();
    }
    if (BUTTON_PRESSED_AND_CONSUME) {
        FIREWALL_STATE = FIREWALL_EXIT;
    }
}

void initialize(void) {
    FIREWALL_STATE = FIREWALL_WELCOME;
    clear_grids();
    fw_state.curr_column = 0;
    fw_state.curr_row = 0;
    b_state.state_drawn = 0;
}

void showhex(unsigned int value) {
    char buff[5];
    buff[0] = hextab[(value >> 12) & 0xF];
    buff[1] = hextab[(value >> 8) & 0xF];
    buff[2] = hextab[(value >> 4) & 0xF];
    buff[3] = hextab[value & 0xF];
    buff[4] = 0;
    writelinez(buff, 1, 1, WHITE);
}

void firewall_app(void) {
    static int done_init = FALSE;
    if (!done_init) {
        initialize();
        done_init = TRUE;
    }
    switch (FIREWALL_STATE) {
        case FIREWALL_WELCOME:
            display_welcome();
            break;
        case FIREWALL_INSTR:
            display_help();
            break;
        case FIREWALL_INSTR2:
            display_help2();
            break;
        case FIREWALL_CONNECT:
            display_waiting();
            break;
        case FIREWALL_LAYOUT:
            display_layout();
            break;
        case FIREWALL_DEFENSE:
            defend_firewall();
            break;
        case FIREWALL_OFFENSE:
            attack_firewall();
            break;
        case FIREWALL_OFFENSE_RESULT:
            show_offense();
            break;
        case FIREWALL_WON:
            show_victory();
            break;
        case FIREWALL_LOST:
            show_loss();
            break;
        case FIREWALL_EXIT:
            done_init = FALSE;
            returnToMenus();
    }
}

void shot_received(byte row, byte column) {
    set_pos(fw_state.defensive_tries, row, column);
    if (get_pos(fw_state.placed_items, row, column)) {
        fw_send_im_hit(row, column);
    } else {
        fw_send_you_missed(row, column);
    }
    FIREWALL_STATE = FIREWALL_DEFENSE;
}

void record_hit(char row, char column) {
    set_pos(fw_state.offensive_hits, row, column);
}

void record_miss(char row, char column) {
    clear_pos(fw_state.offensive_hits, row, column);
}
