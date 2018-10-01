#include "lander.h"
#include "HardwareProfile.h"
#include "badge16.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "colors.h"
#include "fb.h"
#include "messenger.h"


enum
{
    MSG_INIT,
    MSG_SHOW_MENU,
    MSG_SENDING,
    MSG_CONFIG_RELAY_AMOUNT,
    MSG_CONFIG_MSG_TEXT
};

char badge_messenger_state = MSG_INIT;
#define MSG_PACKET_DELTA 2000 // lower this number to send packets faster

#define MSG_MAX_RELAY 20
#define MSG_RELAY_DELAY 10

#define MSG_FIRST_M_IDX 1
#define MSG_RELAY_M_IDX 10
struct menu_t messenger_m[] = {
    {"----Message----", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"---------------", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"Send", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)msg_begin_send}},
    {"Edit", HORIZ_ITEM, FUNCTION, {(struct menu_t *)msg_set_edit_msg}},
    {"Clear", HORIZ_ITEM, FUNCTION, {(struct menu_t *)msg_clear_msg}},
    {"Relay:", VERT_ITEM, FUNCTION, {(struct menu_t *)msg_set_relay}},
    {"Exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *) msg_set_exit}},
};


unsigned char max_configurable_relay_amount = 9; 
unsigned char relay_amount = 3;

//char outgoing_message[MAX_MSG_LENGTH] = {0};
// It's a fuckin Badge Tweet - BWEET!
//char outgoing_message[MAX_MSG_LENGTH] = "Bweet!\0";
//char outgoing_message[MAX_MSG_LENGTH] = "Welcome to RVAsec Bweeter!\0";

//Welcome to bwitter!
//unsigned char outgoing_message[MAX_MSG_LENGTH] = {22,4,11,2,14,12,4,41,19,
//                                                  14,41,1,22,8,19,19,4,17,38,
//                                                  MSG_NULL_TERM};

// Welcome to RVAsec 2016! (CENTERED)
unsigned char outgoing_message[MAX_MSG_LENGTH] = {41, 41,41,22,4,11,2,14,12,4,41,19,14,
                        41,41,41,41,17,21,0,18,4,2,41,28,26,27,32,38, MSG_NULL_TERM};
unsigned char char_under_edit = 0;
unsigned char char_highlighter_x = 0;

// Const puts in ROM, right? Do we want that?
const char relay_help_message_l1[] = "Num. times ";
const char relay_help_message_l2[] = "Others will ";
const char relay_help_message_l3[] = "resend msg ";

extern unsigned char new_message;
extern unsigned char msg_relay_remaining;

void msg_set_edit_msg(){
    badge_messenger_state = MSG_CONFIG_MSG_TEXT;
}

void msg_set_relay(){
    badge_messenger_state = MSG_CONFIG_RELAY_AMOUNT;
}

void msg_begin_send(){
    new_message = 0;
    msg_relay_remaining = 0;

    //TODO: Stop any relaying of a recieved message that may be occuring
    badge_messenger_state = MSG_SENDING;
}

void map_to_c_chars(unsigned char lcd_str[],
                    unsigned char to_c_str[],
                    unsigned char len,
                    unsigned char fill){
    // TODO: Wow, so lazy, but is there a better way?
    unsigned char i = 0;
    for(i=0; i < len; i++)
        to_c_str[i] = lcd_font_map[lcd_str[i]];

    // blank the remaining
    for(;i < len-1; i++)
        to_c_str[i] = fill;
}

unsigned char msg_send_msg(unsigned char send_msg[], unsigned char relay_cnt,
                           unsigned reset){

    // so tired of gobals, but have to add param to rest in order to use static...
    static unsigned short count = 0;
    static unsigned char i = 0;

    if(reset){
        i = 0;
        count = 0;
        return 0;
    }

    if(count == 0){
        union IRpacket_u pkt;
        pkt.p.command = IR_WRITE;
        pkt.p.address = IR_LIVETEXT;
        pkt.p.badgeId = 0b111000000 | i;

        if((i < MAX_MSG_LENGTH - 1)){
            pkt.p.data = (send_msg[i] & 0x3F);
            pkt.p.data |= (send_msg[i+1] & 0x3F) << 6;
            pkt.p.data |= (relay_cnt << 12);
            IRqueueSend(pkt);
            // Debug chirp
            setNote(75, 128);
        }

        // If we've sent our NULL term, we're done
        if(send_msg[i] == MSG_NULL_TERM 
                || send_msg[i+1] == MSG_NULL_TERM){
            i = count = 0;
            return 0;
        }
        else
            i += 2;

        count = MSG_PACKET_DELTA;
    }
    else
        count--;

    // Return 1 to indicate more to send (recall)
    return 1;
}

void msg_clear_menu(){
    unsigned char men_idx = MSG_FIRST_M_IDX, men_c_idx=0;
    for(men_idx=MSG_FIRST_M_IDX;
            men_idx < MSG_FIRST_M_IDX + 5;
            men_idx++){
        for(men_c_idx=0; men_c_idx < 15; men_c_idx++)
            messenger_m[men_idx].name[men_c_idx] = ' '; // different fill character?
    }
}

void msg_clear_msg(){
    unsigned char i = 0;

    outgoing_message[i] = MSG_NULL_TERM;
    for(i=1; i < MAX_MSG_LENGTH; i++){
        outgoing_message[i] = MSG_NULL_TERM;
    }
    msg_clear_menu();
    char_under_edit = 0;
}

void msg_set_exit()
{
    badge_messenger_state = MSG_INIT;
    returnToMenus();
}

void msg_populate_menu()
{
    unsigned char men_idx = MSG_FIRST_M_IDX, men_c_idx=0;
    unsigned char msg_c_idx = 0;
    for(msg_c_idx = 0;
            (msg_c_idx < MAX_MSG_LENGTH) && (outgoing_message[msg_c_idx] != MSG_NULL_TERM);
                msg_c_idx++, men_c_idx++){

        // If we've filled the menu item, go to the next one
        if(men_c_idx > 14){
            men_c_idx = 0;
            men_idx++;
        }

        messenger_m[men_idx].name[men_c_idx] = lcd_font_map[outgoing_message[msg_c_idx]];
    }

    messenger_m[MSG_RELAY_M_IDX].name[6] = '0' + (relay_amount/100) % 10;
    messenger_m[MSG_RELAY_M_IDX].name[7] = '0' + (relay_amount/10) % 10;
    messenger_m[MSG_RELAY_M_IDX].name[8] = '0' + (relay_amount) % 10;
    messenger_m[MSG_RELAY_M_IDX].name[9] = 0;
}

void badge_messenger_cb()
{
    switch(badge_messenger_state)
    {
        case MSG_INIT:
            msg_populate_menu();
            badge_messenger_state = MSG_SHOW_MENU;
            break;
        case MSG_SHOW_MENU:
            genericMenu((struct menu_t *)messenger_m, WHITE_ON_BLACK);
            FbSwapBuffers();
            break;
        case MSG_SENDING:
            if(!msg_send_msg(outgoing_message, relay_amount, 0))
                badge_messenger_state = MSG_SHOW_MENU;
            break;
        case MSG_CONFIG_RELAY_AMOUNT:
            if (BUTTON_PRESSED_AND_CONSUME){
                badge_messenger_state = MSG_SHOW_MENU;
            }

            if(TOP_TAP_AND_CONSUME){
                if(relay_amount < max_configurable_relay_amount){
                    relay_amount++;
                    msg_populate_menu();
                }
                else // "unable to config that" chirp
                    setNote(100, 512);
            }

            if(BOTTOM_TAP_AND_CONSUME){
                if(relay_amount > 0){
                    relay_amount--;
                    msg_populate_menu();
                }
                else // "unable to config that" chirp
                    setNote(100, 512);
            }

            FbColor(GREEN);
            FbMove(28, 60);
            FbWriteLine(messenger_m[MSG_RELAY_M_IDX].name);

            FbColor(GREY8);
            FbMove(20, 10);
            FbWriteLine(relay_help_message_l1);
            FbMove(20, 21);
            FbWriteLine(relay_help_message_l2);
            FbMove(20, 32);
            FbWriteLine(relay_help_message_l3);
            FbSwapBuffers();
            break;

        case MSG_CONFIG_MSG_TEXT:
            if (BUTTON_PRESSED_AND_CONSUME){
                badge_messenger_state = MSG_SHOW_MENU;
                msg_populate_menu();
            }
            if(RIGHT_TAP_AND_CONSUME){
                // User can't edit last char - needed for null term
                // (would be LENGTH-1 if editing last char was allowed)
                if(char_under_edit < MAX_MSG_LENGTH - 2)
                    char_under_edit++;
                else
                    setNote(100, 512);
            }
            if(LEFT_TAP_AND_CONSUME){
                if(char_under_edit > 0)
                    char_under_edit--;
                else
                    setNote(100, 512);
            }
            if(TOP_TAP_AND_CONSUME){
                outgoing_message[char_under_edit]++;
            }
            if(BOTTOM_TAP_AND_CONSUME){
                outgoing_message[char_under_edit]--;
            }
            // TODO: Make sure we don't bother to send trailing spaces
            //       by making sure we move the null term back
            // If the user is trying to extend the string, set to space
            if(outgoing_message[char_under_edit] == MSG_NULL_TERM){
                outgoing_message[char_under_edit] = 41;
                // Move the Null terminator back
                if(char_under_edit < MAX_MSG_LENGTH -1)
                    outgoing_message[char_under_edit+1] = MSG_NULL_TERM;
            }

            // Do character wrapping
            if(outgoing_message[char_under_edit] > 41)
                outgoing_message[char_under_edit] = 0;
            else if(outgoing_message[char_under_edit] < 0)
                outgoing_message[char_under_edit] = 41;

            FbMove(5, 32);
            FbColor(GREY16);
            // TODO: Wow, so lazy, but is there a better way?
            unsigned char tmp_str[MAX_MSG_LENGTH+1];
            unsigned char i = 0;
            for(i=0; i < MAX_MSG_LENGTH && outgoing_message[i] != MSG_NULL_TERM; i++)
                tmp_str[i] = lcd_font_map[outgoing_message[i]];
            // blank the remaining
            for(;i < MAX_MSG_LENGTH-1; i++)
                tmp_str[i] = '-';
            tmp_str[MAX_MSG_LENGTH] = 0; //Null terminator for the print functions

            FbWriteWrappedLineWithCharHighlight(tmp_str,//outgoing_message,
                                                char_under_edit,
                                                RED);
            FbSwapBuffers();
            break;
    }
}
