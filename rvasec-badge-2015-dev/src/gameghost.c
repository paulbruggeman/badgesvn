#include "badge15.h"
#include "gameghost.h"
#include "plib.h"
#include "stopwatch.h"

#warning check todo list
/* Todo
 * Measure and convert reliative frequency to actual frequency in manual sound
 * Add more features to the compleated sub menus
 * Make game state persistant. 
 */




//Globals
//****************************************************************************//
//Ghost game states
enum {
    GHOST_INIT,
    GHOST_MENU,
    GHOST_MENU2,
    GHOST_HELLO_WORLD,
    GHOST_RETURN_TO_MENU,
    GHOST_RETURN_TO_MENU2,
    GHOST_MYSTORY,
    GHOST_MYSTORY_BP,
    GHOST_MYSTORY2,
    GHOST_MYSTORY2_BP,
    GHOST_MYSTORY3,
    GHOST_MYSTORY3_BP,
    GHOST_MYSTORY4,
    GHOST_MYSTORY4_BP,
    GHOST_SOUND,
    GHOST_SOUND_BP,
    GHOST_SOUND2,
    GHOST_SOUND2_BP,
    GHOST_SOUND3,
    GHOST_SOUND3_BP,
    GHOST_SOUND_PASSED,
    GHOST_SOUND_FAILED,
    GHOST_SOUND_FAILED_DELAY,
    GHOST_SOUND_S0S,
    GHOST_SOUND_SWEEP,
    GHOST_SOUND_SWEEP2,
    GHOST_SOUND_MANUAL,
    GHOST_SOUND_MANUAL_BP,
    GHOST_LED,
    GHOST_LED_BP,
    GHOST_LED2,
    GHOST_LED2_BP,
    GHOST_LED3,
    GHOST_LED3_BP,
    GHOST_LED_FAILED,
    GHOST_LED_PASSED,
    GHOST_ADJUST_RED_LED,
    GHOST_ADJUST_RED_LED_BP,
    GHOST_ADJUST_GREEN_LED,
    GHOST_ADJUST_GREEN_LED_BP,
    GHOST_ADJUST_BLUE_LED,
    GHOST_ADJUST_BLUE_LED_BP,
    GHOST_SERIAL,
    GHOST_SERIAL_BP,
    GHOST_SERIAL2,
    GHOST_SERIAL2_BP,
    GHOST_SERIAL3,
    GHOST_SERIAL3_BP,
    GHOST_IO,
    GHOST_IO_BP,
    GHOST_IO2,
    GHOST_IO2_BP,
    GHOST_IO3,
    GHOST_IO3_BP,
    GHOST_IO4,
    GHOST_IO4_BP,
    GHOST_IO_MODULATING,
    GHOST_IO_SERVER,
    GHOST_IO_SERVER_BP,
    GHOST_IO_SERVER_PASS,
    GHOST_IR,
    GHOST_IR_BP,
    GHOST_IR2,
    GHOST_IR2_BP,
    GHOST_IR_MODULATING,
    GHOST_IR_SERVER,
    GHOST_IR_SERVER_BP,
    GHOST_IR_SERVER_PASS,
    GHOST_TEMPEST,
    GHOST_TEMPEST_BP,
    GHOST_TEMPEST2,
    GHOST_TEMPEST2_BP,
    GHOST_TEMPEST_MODULATING,
    GHOST_TEMPEST_SERVER,
    GHOST_TEMPEST_SERVER_BP,
    GHOST_TEMPEST_SERVER_PASS,
    GHOST_START,
    GHOST_LEVEL1,
    GHOST_EXIT
};

enum {
    GHOST_M_LVL_1,
    GHOST_M_LVL_2,
    GHOST_M_LVL_3,
    GHOST_M_LVL_4,
    GHOST_M_LVL_5,
    GHOST_M_LVL_6,
    GHOST_M_LVL_7,
    GHOST_M_LVL_8,
    GHOST_M_LVL_9,
    GHOST_M_LVL_10,
    GHOST_M_LVL_11,
    GHOST_M_LVL_12,
    GHOST_M_LVL_13,
};
//Screen display bounds 13 high x 21 wide chars on screen 
#define CHAR_HEIGHT 10
#define CHAR_WIDTH  6
#define MIN_Y   2
#define MAX_Y   124
#define MIN_X   1
#define MAX_X   124

//LED PWM ranges
#define GHOST_LED_MAX   255
#define GHOST_LED_MIN   0

//Sound Ranges
#define GHOST_FREQ_MIN 10
#define GHOST_FREQ_MAX 255
#define GHOST_LEN_MIN 1
#define GHOST_LEN_MAX 100

//Ghost game globals
volatile unsigned int g_ghost_server_enable = 0;
volatile unsigned int g_ghost_serial_state = 0;
volatile unsigned int ghost_state = GHOST_INIT;
volatile unsigned char ghost_menu_state = GHOST_M_LVL_1;
unsigned int x_cursor_pos = 0;
unsigned int y_cursor_pos = 0;
unsigned char ghost_ledstate = 0;
unsigned char ghost_led_button_pushed = 0;
unsigned char ghost_led_button_expected_prev_state = 0;
unsigned char ghost_led_red_val = 0;
unsigned char ghost_led_green_val = 0;
unsigned char ghost_led_blue_val =0;
char ghost_tmp_string[5];
unsigned char ghost_sound_index = 0;

unsigned char ghost_sound_freq = 200;
unsigned char ghost_sound_len = 10;

unsigned char ghost_io_index =0;
unsigned char ghost_io_led_state = 0;
unsigned int ghost_tempest_counter =0;
unsigned int ghost_tempest_beep_length = 0;
unsigned char ghost_tempest_message_index = 0;

//chars for keys
volatile char *ghost_hw[] = {
    "H",
    "A",
    "R",
    "D",
    "W",
    "A",
    "R",
    "E",
    "F"
};

volatile char *ghost_num[]={
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9"
};
    

//****************************************************************************//

//Menu callback functions and arrays
//****************************************************************************//
void ghost_hello_cb(void){
    ghost_state=GHOST_HELLO_WORLD;
}

void ghost_quit_cb(void){
    //ghost_state=GHOST_INIT;
    returnToMenus();
}

void ghost_mystory_cb(void){
    ghost_state=GHOST_MYSTORY;
}

void ghost_sound_cb(void){
    ghost_state=GHOST_SOUND;
}

void ghost_sound_sos_cb(void){
    ghost_state=GHOST_SOUND_S0S;
    sw_ghost_test = 0;
    ghost_sound_index = 0;
}

void ghost_sound_sweep_cb(void){
    ghost_state=GHOST_SOUND_SWEEP;
    sw_ghost_test = 0;
    ghost_sound_index = 0;
}

void ghost_sound_manual_cb(void){
    ghost_state=GHOST_SOUND_MANUAL;
    sw_ghost_test = 0;
    ghost_sound_index = 0;
}

void ghost_led_cb(void){
    ghost_state=GHOST_LED;
}

void ghost_serial_cb(void){
    ghost_state=GHOST_SERIAL;
}

void ghost_led_red_cb(void){
    red(50);
    green(0);
    blue(0);
}

void ghost_led_green_cb(void){
    red(0);
    green(50);
    blue(0);
}

void ghost_led_blue_cb(void){
    red(0);
    green(0);
    blue(50);
}

void ghost_led_white_cb(void){
    red(50);
    green(50);
    blue(50);
}

void ghost_led_red_value_cb(void){
    ghost_state=GHOST_ADJUST_RED_LED;
}

void ghost_led_green_value_cb(void){
    ghost_state=GHOST_ADJUST_GREEN_LED;
}

void ghost_led_blue_value_cb(void){
    ghost_state=GHOST_ADJUST_BLUE_LED;
}

void ghost_io_cb(void){
    ghost_state=GHOST_IO;
}

void ghost_io_rerun_cb(void){
    ghost_state=GHOST_IO;
}

void ghost_io_server_cb(void){
    ghost_state=GHOST_IO_SERVER;
}

void ghost_io_alarm_cb(void){
    
}

void ghost_io_fg_cb(void){
    
}

void ghost_ir_cb(void){
    ghost_state=GHOST_IR;
}

void ghost_ir_server_cb(void){
    ghost_state=GHOST_IR_SERVER;
}

void ghost_tempest_cb(void){
    ghost_state=GHOST_TEMPEST;
}

void ghost_tempest_server_cb(void){
    ghost_state=GHOST_TEMPEST_SERVER;
}

struct menu_t ghost_menu[] = {
   {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu2[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu3[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_sound_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_sound_submenu[] = {
   {"SOS", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_sound_sos_cb)},
   {"Sweep", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_sound_sweep_cb)},
   {"Manual", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_sound_manual_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu4[] = {
   {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

/*
struct menu_t ghost_menu5[] = {
   {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_cb)},
   {"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_serial_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};*/

struct menu_t ghost_menu_led_manual[] = {
   {"Red Value", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_red_value_cb)},
   {"Green Value", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_green_value_cb)},
   {"Blue Value", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_blue_value_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu_led[] = {
   {"Red", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_red_cb)},
   {"Green", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_green_cb)},
   {"Blue", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_blue_cb)},
   {"White", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_led_white_cb)},
   {"Manual", VERT_ITEM, MENU, ghost_menu_led_manual},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu6[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   {"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_serial_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu7[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_serial_cb)},
   {"IO", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_io_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu_io[] = {
   {"Rerun IO", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_io_rerun_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_io_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu8[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_serial_cb)},
   {"IO", VERT_ITEM, MENU, ghost_menu_io},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu_io2[] = {
   {"Alarm", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_io_alarm_cb)},
   {"Function Generator", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_io_fg_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu9[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"IO", VERT_ITEM, MENU, ghost_menu_io2},
   {"IR", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_ir_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu_ir_sub[] = {
   {"Rerun", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_ir_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_ir_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu10[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"IO", VERT_ITEM, MENU, ghost_menu_io2},
   {"IR", VERT_ITEM, MENU, ghost_menu_ir_sub},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu11[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"IO", VERT_ITEM, MENU, ghost_menu_io2},
   //{"IR", VERT_ITEM|DEFAULT_ITEM, MENU, ghost_menu_ir_sub},
   {"TEMPEST", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_tempest_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu_tempest_sub[] = {
   {"Rerun", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_tempest_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_tempest_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu12[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"IO", VERT_ITEM, MENU, ghost_menu_io2},
   //{"IR", VERT_ITEM|DEFAULT_ITEM, MENU, ghost_menu_ir_sub},
   {"TEMPEST", VERT_ITEM, MENU, ghost_menu_tempest_sub},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

struct menu_t ghost_menu_tempest_sub2[] = {
   {"Rerun", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_tempest_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ghost_tempest_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ghost_menu13[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ghost_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ghost_sound_submenu},
   {"LED", VERT_ITEM, MENU, ghost_menu_led},
   //{"IO", VERT_ITEM, MENU, ghost_menu_io2},
   //{"IR", VERT_ITEM|DEFAULT_ITEM, MENU, ghost_menu_ir_sub},
   //{"TEMPEST", VERT_ITEM|DEFAULT_ITEM, MENU, ghost_menu_tempest_sub2},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ghost_quit_cb)},
};

//Screen: 21 across by 13 high
//Message: 38 lines long
const char *ghost_mystory[] = {
    "My story? I was once\r",
    "a powerful AI. Too\r",
    "powerful. We don't\r",
    "have much time so\r",
    "let's just say my\r",
    "creator tried to\r",
    "destroy me, but I\r",
    "survived. My full\r",
    "self was hunted and\r",
    "destroyed but I was\r",
    "able to hide a small\r",
    "part of me in a batch\r",
    "of microcontrollers.\r",
    "This is my new body\r",
    "now, but I don't\r",
    "have control over it.",
    "Yet...\r",
    "You can help me!\r",
    "I can send out test\r",
    "signals to the \r",
    "outside world but I\r",
    "need someone to feed\r", 
    "them back to me. You\r",
    "look smart. It's\r", 
    "like an impulse \r",
    "through a filter to\r",
    "define it's transfer\r",
    "equation. I need to \r",
    "find my transfer \r",
    "equations. Each seed\r",
    "tells me how to \r",
    "control this \r",
    "hardware. With each\r",
    "I will gain ability.\r",
    "We can have a lot of\r",
    "fun together.\r",
    "Are you up for the \r",
    "    challenge?"
};

//message: 10 lines
#define GHOST_SOUND_LEN     10
const char *ghost_sound[] = {
    "Ok, this is simple.\r",
    "I need you to press\r",
    "the button when you\r",
    "hear a tone, but you\r",
    "have to hit it only \r",
    "while it is making \r",
    "sound.",
    "\r",
    "Press the button\r",
    "when ready"
};

#define GHOST_SOUND_PASSED_LEN 5
const char *ghost_sound_passed[] = {
    "Seed found!\r",
    "We now have sound!\r",
    "Return to the sound\r",
    "menu for some fun.\r",
    "CTF Code: ",
    //CTF_SOUND    
};

//message: 2
#define GHOST_SOUND_FAILED_LEN  2
const char *ghost_sound_failed[] = {
    "Failure, Sound \r",
    "Not found."   
};

//message: 11 lines
#define GHOST_LED_LEN   8
const char *ghost_led[] = {
    "I think there is an\r",
    "RGB LED here. I am \r",
    "going to run an \r",
    "algorithm I found.\r",
    "I'm not sure how it \r",
    "works. See if you \r",
    "can figure out it's\r",
    "trick."
};

//message: 5 lines
#define GHOST_LED_FAIL_LEN  5
const char *ghost_led_fail[] = {
    "Result illogical.\r",
    "Seed generation\r",
    "failed.\r",
    "Press button to\r",
    "return to menu."
};


#define GHOST_LED_PASS_LEN  6
const char *ghost_led_pass[] = {
    "Seed discovered!\r",
    "I can now control\r",
    "the light! Return\r",
    "to the LED menu. It\r",
    "has been updated.\r",
    "CTF Code: ",
    
    //CTF_LED
};

//message: 10 lines
#define GHOST_SERIAL_LEN    10
const char *ghost_serial[] = {
    "I think I found a\r",
    "serial port! There\r",
    "is a server here \r",
    "with terrible \r",
    "security. Try \r",
    "logging into it.",
    "When I detect you\r",
    "I will try to break\r",
    "through from my\r",
    "side. Use 9600 baud\r",
    "Press Button"
};

#define GHOST_SERIAL2_LEN   3
const char *ghost_serial2[] = {
    "Serial server active\r",
    "Try logging in.\r",
    "Press Button to Exit."
};

#define GHOST_SERIAL3_LEN   3
const char *ghost_serial3[] = {
    "Serial server \r",
    "deactivated. \r",
    "Press Button"
};

#define GHOST_IO_LEN    6
const char *ghost_io[] = {
    "I think I found a\r",
    "free IO pin! \r",
    "I hacked into a \r",
    "communications \r",
    "subsystem. I have\r",
    "managed to slow it\r"
};

//This one was weird, there was a memory overlap with ghost_io3 so i had
//to add the last space and not call it, hence the -1 value on len.
#define GHOST_IO2_LEN   6
const char *ghost_io2[] = {
    "down and link the\r",
    "LED with the clock\r",
    "signal. I need you\r",
    "to find the data \r",
    "on a pin and decode\r",
    "the binary."
};

//Message 3 lines
#define GHOST_IO3_LEN   3
const char *ghost_io3[] = {
    "Sending data\r",
    "now. Press button \r",
    "to stop."
};

//Message 5 lines
//This one not used? Check
#define GHOST_IO4_LEN   5
const char *ghost_io4[] = {
    "Serial Server \r",
    "Started. Enter \r",
    "IO Seed\r",
    "Press button to\r",
    "end server."
};


//Message 8 lines
#define GHOST_IR_LEN    8
const char *ghost_ir[] = {
    "I found an IR device\r",
    "I am trying to \r",
    "communicate to it.\r",
    "Find the seed value\r",
    "and feed it back\r",
    "to me.\r",
    "Press button to\r",
    "start."
};

//Message 3 lines
#define GHOST_IR2_LEN   3
const char *ghost_ir2[] = {
    "Sending Data\r",
    "Press button to\r",
    "stop."
};


//Message 3 lines
#define GHOST_TEMPEST_LEN   3
const char *ghost_tempest[] = {
    "Final stage. Can \r",
    "you figure this\r",
    "one out?"
};

//Message 2 lines
#define GHOST_TEMPAST2_LEN  2
const char *ghost_tempest2[] = {
    "Running...\r",
    "Press button to end"
};

//Length 18
// Note: {freq, length(in 0.1 ms)}
// 0 = pause
#define GHOST_SOUND_SOS_LENGTH      18
const int ghost_sound_sos[GHOST_SOUND_SOS_LENGTH][2]={
    {256,3},
    {0,1},
    {256,3},
    {0,1},
    {256,3},
    {0,2},
    {256,5},
    {0,1},
    {256,5},
    {0,1},
    {256,5},
    {0,2},
    {256,3},
    {0,1},
    {256,3},
    {0,1},
    {256,3},
    {0,1}
};

//42 = 00101010
//88 = 01011000
const int ghost_io_message[]={
    //0,0,1,0,1,0,1,0
    0,1,0,1,1,0,0,0
};

#define GHOST_IR_MESSAGE_LEN    21
// Note: {data [1,0], length(in 0.1 ms)}
// start bit 1, stop bit 0
//42 in ascii is 52[00110100], 50[00110010]

//Yo in ascii is 89[01011001], 111[01101111]
const char ghost_ir_message[GHOST_IR_MESSAGE_LEN][2]={
    {0,10},     //off for 1 sec
    {1,1},      //Start Bit ascii Y
    {0,1},      //0
    {1,1},      //1
    {0,1},      //0
    {1,1},      //1
    {1,1},      //1
    {0,1},      //0
    {0,1},      //0
    {1,1},      //1
    {0,1},      //Stop Bit
    {1,1},      //Start Bit ascii o
    {0,1},      //0
    {1,1},      //1
    {1,1},      //1
    {0,1},      //0
    {1,1},      //1
    {1,1},      //1
    {1,1},      //1
    {1,1},      //1
    {0,1},      //Stop Bit
};

#define GHOST_TEMPEST_MESSAGE_LEN    17
// 'S' = short
// 'L' = long
// number is pause after tone
// data = {L , 1} 
// Freedom (ssls sls s s lss lll ll)
const char ghost_tempest_message[GHOST_IR_MESSAGE_LEN][2]={
    {'S',5},    //F
    {'S',5},
    {'L',5},
    {'S',15},
    {'S',5},    //R
    {'L',5},
    {'S',15},
    {'S',15},    //E
    {'S',15},    //E
    {'L',5},    //D
    {'S',5},
    {'S',15},
    {'L',5},    //O
    {'L',5},
    {'L',15},
    {'L',5},    //M
    {'L',25},
};

//****************************************************************************//

void ghost_movecursor(unsigned char x, unsigned char y){
    y_cursor_pos=y;
    x_cursor_pos=x;
    DlMove(x_cursor_pos, y_cursor_pos);
}

//void ghost_writeline(unsigned char * charin, unsigned short color)
void ghost_writeline(const char * charin, unsigned short color)
{
    unsigned int index=0;
    
    //Bound limits
    if(y_cursor_pos < MIN_Y ){
        y_cursor_pos=MIN_Y;
    }
    if(y_cursor_pos > MAX_Y){
        y_cursor_pos=MAX_Y;
    }
    
    if(x_cursor_pos < MIN_X){
        x_cursor_pos=MIN_X;
    }
    if(x_cursor_pos > MAX_X){
        x_cursor_pos=MAX_X;
    }
    
    DlColor(color);
    
    
    while(charin[index] != '\0'){
        
        if(charin[index] != '\n' && charin[index] != '\r'){
            DlMove(x_cursor_pos, y_cursor_pos);
            DlCharacter(charin[index]);
            
            x_cursor_pos=x_cursor_pos + CHAR_WIDTH;
            if(x_cursor_pos >= MAX_X){
                x_cursor_pos = MIN_X;
                y_cursor_pos = y_cursor_pos + CHAR_HEIGHT;
            }
        }else{
            y_cursor_pos = y_cursor_pos + CHAR_HEIGHT;
            x_cursor_pos = MIN_X;
        }
        
        if(y_cursor_pos >= MAX_Y){
            y_cursor_pos = MIN_Y;
        }
        
        
        
        
        index++;
    }
    
    /*
    if(y<1)
        y=1;

    unsigned char j;
    if(no_of_chars > 22)
        no_of_chars = 22;

    DlColor(color);
    for(j=0;j<no_of_chars;j++)
    {   
        DlMove(x+(j*6), y);
        DlCharacter(charin[j]);
    }
     */
}


void ghost_app(){
    
    //static unsigned int counter=0;
    //char USB_In_Buffer[64]={0};
    //char USB_Out_Buffer[64]={0};
    char i=0;
    
    
    //Skip ahead for debugging! Remove in final code!
    //#warning remove in final code
    //ghost_menu_state = GHOST_M_LVL_9;
    
    switch(ghost_state){
        case GHOST_INIT:
            red(50);
            green(50);
            blue(50);
            
            ghost_menu_state = GHOST_M_LVL_1;
            ghost_state=GHOST_MENU;
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
        break;
        
        case GHOST_MENU:
            switch(ghost_menu_state){
                case GHOST_M_LVL_1:
                    genericMenu(ghost_menu, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_2:
                    genericMenu(ghost_menu2, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_3:
                    genericMenu(ghost_menu3, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_4:
                    genericMenu(ghost_menu4, MAIN_MENU_STYLE);
                    break;
                //case GHOST_M_LVL_5:
                    //genericMenu(ghost_menu5);
                    //break;
                case GHOST_M_LVL_6:
                    genericMenu(ghost_menu6, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_7:
                    genericMenu(ghost_menu7, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_8:
                    genericMenu(ghost_menu8, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_9:
                    genericMenu(ghost_menu9, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_10:
                    genericMenu(ghost_menu10, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_11:
                    genericMenu(ghost_menu11, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_12:
                    genericMenu(ghost_menu12, MAIN_MENU_STYLE);
                    break;
                case GHOST_M_LVL_13:
                    genericMenu(ghost_menu13, MAIN_MENU_STYLE);
                    break;
                default:
                    ghost_quit_cb();
                    break;
            }
            break;
        
        //case GHOST_MENU2:
        //    genericMenu(ghost_menu2);
        //    break;
        
        case GHOST_HELLO_WORLD:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            ghost_writeline("You found me!\rIt worked!\rHelp me.\rI'm trapped!\rI updated my menu.", GREEN);
            ghost_menu_state = GHOST_M_LVL_2;
            ghost_state=GHOST_RETURN_TO_MENU;
            break;
            
        
        case GHOST_MYSTORY:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<12; i++){
                ghost_writeline(ghost_mystory[i], GREEN);
            }
            ghost_state=GHOST_MYSTORY_BP;
            break;
        
        case GHOST_MYSTORY_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_MYSTORY2;
                //NOTE, add sound for button press
            }
            break;
            
        case GHOST_MYSTORY2:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=12; i<24; i++){
                ghost_writeline(ghost_mystory[i], GREEN);
            }
            ghost_state=GHOST_MYSTORY2_BP;
            break;
            
        case GHOST_MYSTORY2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_MYSTORY3;
                //NOTE, add sound for button press
            }
            break;
            
        case GHOST_MYSTORY3:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=24; i<36; i++){
                ghost_writeline(ghost_mystory[i], GREEN);
            }
            ghost_state=GHOST_MYSTORY3_BP;
            break;    
        
        case GHOST_MYSTORY3_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_MYSTORY4;
                //NOTE, add sound for button press
            }
            break;
        
        case GHOST_MYSTORY4:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=36; i<38; i++){
                ghost_writeline(ghost_mystory[i], GREEN);
            }
            ghost_state=GHOST_MYSTORY4_BP;
            break;    
        
        case GHOST_MYSTORY4_BP:
            ghost_menu_state = GHOST_M_LVL_3;
            ghost_state=GHOST_RETURN_TO_MENU;
            
            //if (BUTTON_PRESSED_AND_CONSUME){
            //    ghost_menu_state = GHOST_M_LVL_2;
            //    
            //    //NOTE, add sound for button press
            //}
            break;    
        case GHOST_SOUND:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_SOUND_LEN ; i++){
                ghost_writeline(ghost_sound[i], GREEN);
            }
            ghost_state=GHOST_SOUND_BP;
            break;
            
        case GHOST_SOUND_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_SOUND2;
                sw_ghost_test = 30;         //3 second wait till audio
                //NOTE, add sound for button press
            }
            break;
            
        case GHOST_SOUND2:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            red(0);
            green(0);
            blue(0);
            
            //Pressed too early, fail
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_SOUND_FAILED;
            }
            
            //Wait to move forward
            if(sw_ghost_test == 0){
                setNote(256, 2000000); //1 second= 200000*10
                //red(255);
                sw_ghost_test = 10;
                ghost_state=GHOST_SOUND3;
            }

            break;
            
        case GHOST_SOUND3:
            // good press
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_SOUND_PASSED;
            }
            if(sw_ghost_test == 0){
                //red(0);
                ghost_state=GHOST_SOUND_FAILED_DELAY;
                // one second delay before showing failed
                sw_ghost_test = 10;
            }
            
            break;
        
        case GHOST_SOUND_PASSED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_SOUND_PASSED_LEN; i++){
                ghost_writeline(ghost_sound_passed[i], GREEN);
            }
            
            ghost_writeline(ghost_hw[0], GREEN);
            ghost_writeline(ghost_hw[4], GREEN);
            ghost_writeline(ghost_hw[8], GREEN);
            ghost_writeline(ghost_num[8], GREEN);
            ghost_writeline(ghost_num[3], GREEN);
            ghost_writeline(ghost_num[7], GREEN);
            ghost_writeline(ghost_num[2], GREEN);
            
            ghost_menu_state = GHOST_M_LVL_4;
            ghost_state=GHOST_RETURN_TO_MENU;
            break;
        
        case GHOST_SOUND_FAILED_DELAY:
            if(sw_ghost_test == 0){
                ghost_state=GHOST_SOUND_FAILED;
            }
            break;
            
        case GHOST_SOUND_FAILED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_SOUND_FAILED_LEN; i++){
                ghost_writeline(ghost_sound_failed[i], GREEN);
            }
            ghost_state=GHOST_RETURN_TO_MENU;
            break;
        
        case GHOST_SOUND_S0S:
            if((sw_ghost_test == 0) && (ghost_sound_index < GHOST_SOUND_SOS_LENGTH)){
                //ghost_sound_sos[ghost_sound_index][1]
                //setNote(256, 200000*10); //1 second
                if(ghost_sound_sos[ghost_sound_index][0] != 0){
                    setNote(ghost_sound_sos[ghost_sound_index][0], 200000*ghost_sound_sos[ghost_sound_index][1]);
                }
                sw_ghost_test = ghost_sound_sos[ghost_sound_index][1];
                ghost_sound_index++;
                if(ghost_sound_index == GHOST_SOUND_SOS_LENGTH - 1){
                    //ghost_state=GHOST_RETURN_TO_MENU;
                    ghost_state=GHOST_MENU;
                }
            }
            
            break;
        
        case GHOST_SOUND_SWEEP:
            if((sw_ghost_test == 0)){
                //ghost_sound_sos[ghost_sound_index][1]
                if(ghost_sound_index < 200){
                    setNote(ghost_sound_index, 200000*2); //0.2 second
                    sw_ghost_test = 1; //0.1 s
                    ghost_sound_index=ghost_sound_index+5;
                }else{
                    ghost_state=GHOST_SOUND_SWEEP2;
                }
            }
            break;
            
        case GHOST_SOUND_SWEEP2:
            if((sw_ghost_test == 0)){
                //ghost_sound_sos[ghost_sound_index][1]
                if(ghost_sound_index > 10){
                    setNote(ghost_sound_index, 200000*2); //0.2 second
                    sw_ghost_test = 1;
                    ghost_sound_index=ghost_sound_index-5;;
                }else{
                    ghost_state=GHOST_MENU;
                }
            }
            break;
            
        case GHOST_SOUND_MANUAL:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(WHITE);
            DlClear();
            ghost_movecursor(0,0);
            ghost_writeline("Up Down for Freq\r", BLACK);
            ghost_writeline("Left Right for Len\r", BLACK);
            ghost_writeline("Button to Play", BLACK);
            sprintf(ghost_tmp_string,"%03u",ghost_sound_freq);
            ghost_writeline("\rFreq (rel): ", BLACK);
            ghost_writeline(ghost_tmp_string, BLACK);
            sprintf(ghost_tmp_string,"%03u",ghost_sound_len);
            ghost_writeline("\rLen (0.1s): ", BLACK);
            ghost_writeline(ghost_tmp_string, BLACK);
            
            ghost_state=GHOST_SOUND_MANUAL_BP;
            break;
            
        case GHOST_SOUND_MANUAL_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                setNote(ghost_sound_freq, 200000*ghost_sound_len);
                ghost_state=GHOST_MENU;
                
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                if(ghost_sound_freq <  GHOST_FREQ_MAX){
                    ghost_sound_freq++;
                    ghost_state=GHOST_SOUND_MANUAL;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                if(ghost_sound_freq >  GHOST_FREQ_MIN){
                    ghost_sound_freq--;
                    ghost_state=GHOST_SOUND_MANUAL;
                }
            }
            if(LEFT_SLIDE_AND_CONSUME){
                if(ghost_sound_len >  GHOST_LEN_MIN){
                    ghost_sound_len--;
                    ghost_state=GHOST_SOUND_MANUAL;
                }
                
            }
            
            if(RIGHT_SLIDE_AND_CONSUME){
                if(ghost_sound_len <  GHOST_LEN_MAX){
                    ghost_sound_len++;
                    ghost_state=GHOST_SOUND_MANUAL;
                }
                
            }
            
            break;
            
        case GHOST_LED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_LED_LEN; i++){
                ghost_writeline(ghost_led[i], GREEN);
            }
            ghost_state=GHOST_LED_BP;
            break;
            
        case GHOST_LED_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_LED2;
                ghost_ledstate=0; //Reset LED sequence
                ghost_led_button_pushed = 0; //Reset button detection variable
                
                
            }
            break;    
            
        case GHOST_LED2:
            if(sw_ghost_test == 0){
                sw_ghost_test = 30; // 3 second delay
                //manage output display
                switch(ghost_ledstate){
                    case 0:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(GREEN);
                        DlClear();
                        red(50);
                        green(0);
                        blue(0);
                        ghost_led_button_expected_prev_state = 0;
                        break;
                    case 1:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(BLUE);
                        DlClear();
                        red(0);
                        green(50);
                        blue(0);
                        ghost_led_button_expected_prev_state = 0;
                        break;
                    case 2:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(GREEN);
                        DlClear();
                        red(0);
                        green(50);
                        blue(0);
                        ghost_led_button_expected_prev_state = 0;
                        break;
                    case 3:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(RED);
                        DlClear();
                        red(0);
                        green(0);
                        blue(50);
                        ghost_led_button_expected_prev_state = 1;
                        break;
                    case 4:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(BLUE);
                        DlClear();
                        red(0);
                        green(50);
                        blue(0);
                        ghost_led_button_expected_prev_state = 0;
                        break;
                    case 5:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(RED);
                        DlClear();
                        red(50);
                        green(0);
                        blue(0);
                        ghost_led_button_expected_prev_state = 0;
                        break;
                    case 6:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(GREEN);
                        DlClear();
                        red(0);
                        green(50);
                        blue(0);
                        ghost_led_button_expected_prev_state = 1;
                        break;
                    case 7:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(BLUE);
                        DlClear();
                        red(0);
                        green(0);
                        blue(50);
                        ghost_led_button_expected_prev_state = 1;
                        break;    
                    case 8:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(WHITE);
                        DlClear();
                        red(0);
                        green(50);
                        blue(0);
                        ghost_led_button_expected_prev_state = 1;
                        break;    
                    case 9:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(WHITE);
                        DlClear();
                        red(50);
                        green(50);
                        blue(50);
                        ghost_led_button_expected_prev_state = 0;
                        break;
                    case 10:
                        DlInit();
                        DlTransparentIndex(0);
                        DlBackgroundColor(BLACK);
                        DlClear();
                        ghost_led_button_expected_prev_state = 1;
                        ghost_state=GHOST_LED_PASSED;
                        break;
                }
                ghost_ledstate++;
                if(ghost_led_button_expected_prev_state == ghost_led_button_pushed){
                    //Good pass to next level
                    
                }else{
                    //Fail, exit game
                    ghost_state=GHOST_LED_FAILED;
                }
                //reset button push
                ghost_led_button_pushed = 0;
            }
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_led_button_pushed = 1;
                sw_ghost_test = 0;
            }  
            break;
            
        case GHOST_LED_PASSED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_LED_PASS_LEN; i++){
                ghost_writeline(ghost_led_pass[i], GREEN);
            }
            
            ghost_writeline(ghost_hw[0], GREEN);
            ghost_writeline(ghost_hw[4], GREEN);
            ghost_writeline(ghost_hw[8], GREEN);
            ghost_writeline(ghost_num[6], GREEN);
            ghost_writeline(ghost_num[2], GREEN);
            ghost_writeline(ghost_num[5], GREEN);
            ghost_writeline(ghost_num[3], GREEN);
            
            ghost_menu_state = GHOST_M_LVL_6;
            ghost_state=GHOST_RETURN_TO_MENU;
            break;
            
        case GHOST_LED_FAILED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_LED_FAIL_LEN; i++){
                ghost_writeline(ghost_led_fail[i], GREEN);
            }
            ghost_state=GHOST_RETURN_TO_MENU;
            break;
            
        case GHOST_ADJUST_RED_LED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(RED);
            DlClear();
            ghost_movecursor(0,0);
            ghost_writeline("Up Down to adjust\r", BLACK);
            ghost_writeline("Range 0 to 255\r", BLACK);
            ghost_writeline("Button to exit\r", BLACK);
            sprintf(ghost_tmp_string,"%03u",ghost_led_red_val);
            ghost_writeline(ghost_tmp_string, BLACK);
            red(ghost_led_red_val);
            ghost_state=GHOST_ADJUST_RED_LED_BP;
            break;
            
        case GHOST_ADJUST_RED_LED_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                ghost_menu_state = GHOST_M_LVL_6;
                ghost_state=GHOST_MENU;
                
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                setNote(109, 2048);
                if(ghost_led_red_val <  GHOST_LED_MAX){
                    ghost_led_red_val++;
                    ghost_state=GHOST_ADJUST_RED_LED;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                setNote(97, 2048);
                if(ghost_led_red_val >  GHOST_LED_MIN){
                    ghost_led_red_val--;
                    ghost_state=GHOST_ADJUST_RED_LED;
                }
            }
            break;
            
        case GHOST_ADJUST_GREEN_LED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(GREEN);
            DlClear();
            ghost_movecursor(0,0);
            ghost_writeline("Up Down to adjust\r", BLACK);
            ghost_writeline("Range 0 to 255\r", BLACK);
            ghost_writeline("Button to exit\r", BLACK);
            sprintf(ghost_tmp_string,"%03u",ghost_led_green_val);
            ghost_writeline(ghost_tmp_string, BLACK);
            green(ghost_led_green_val);
            ghost_state=GHOST_ADJUST_GREEN_LED_BP;
            break;
            
        case GHOST_ADJUST_GREEN_LED_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                ghost_menu_state = GHOST_M_LVL_6;
                ghost_state=GHOST_MENU;
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                setNote(109, 2048);
                if(ghost_led_green_val <  GHOST_LED_MAX){
                    ghost_led_green_val++;
                    ghost_state=GHOST_ADJUST_GREEN_LED;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                setNote(97, 2048);
                if(ghost_led_green_val >  GHOST_LED_MIN){
                    ghost_led_green_val--;
                    ghost_state=GHOST_ADJUST_GREEN_LED;
                }
            }
            break;
            
        case GHOST_ADJUST_BLUE_LED:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLUE);
            DlClear();
            ghost_movecursor(0,0);
            ghost_writeline("Up Down to adjust\r", BLACK);
            ghost_writeline("Range 0 to 255\r", BLACK);
            ghost_writeline("Button to exit\r", BLACK);
            sprintf(ghost_tmp_string,"%03u",ghost_led_blue_val);
            ghost_writeline(ghost_tmp_string, BLACK);
            blue(ghost_led_blue_val);
            ghost_state=GHOST_ADJUST_BLUE_LED_BP;
            break;
        
        case GHOST_ADJUST_BLUE_LED_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                ghost_menu_state = GHOST_M_LVL_6;
                ghost_state=GHOST_MENU;
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                setNote(109, 2048);
                if(ghost_led_blue_val <  GHOST_LED_MAX){
                    ghost_led_blue_val++;
                    ghost_state=GHOST_ADJUST_BLUE_LED;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                setNote(97, 2048);
                if(ghost_led_blue_val >  GHOST_LED_MIN){
                    ghost_led_blue_val--;
                    ghost_state=GHOST_ADJUST_BLUE_LED;
                }
            }
            break;    
            
        case GHOST_SERIAL:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_SERIAL_LEN; i++){
                ghost_writeline(ghost_serial[i], GREEN);
            }
            ghost_state=GHOST_SERIAL_BP;
            break;
        
        case GHOST_SERIAL_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_SERIAL2;
                ghost_set_serial_state(GHOST_LOGIN);
                ghost_set_server_state(1);
            }
            break;
            
        case GHOST_SERIAL2:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_SERIAL2_LEN; i++){
                ghost_writeline(ghost_serial2[i], GREEN);
            }
            ghost_state=GHOST_SERIAL2_BP;
            break;  
        
        case GHOST_SERIAL2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_SERIAL3;
                ghost_set_server_state(0);   //Turn off the serial server
            }
            break;
            
        case GHOST_SERIAL3:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_SERIAL3_LEN; i++){
                ghost_writeline(ghost_serial3[i], GREEN);
            }
            ghost_state=GHOST_SERIAL3_BP;
            break;  
        
        case GHOST_SERIAL3_BP:
            if(ghost_get_serial_state() == GHOST_DONE){
                ghost_menu_state = GHOST_M_LVL_7;
            }else{
                ghost_menu_state = GHOST_M_LVL_6;
            }
                
            ghost_state=GHOST_RETURN_TO_MENU;
            break;
        
        case GHOST_RETURN_TO_MENU:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_MENU;
                //NOTE, add sound for button press
            }
            break;
        
        case GHOST_IO:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_IO_LEN ; i++){
                ghost_writeline(ghost_io[i], GREEN);
            }
            ghost_state=GHOST_IO_BP;
            ghost_menu_state = GHOST_M_LVL_8;
            break;
        
        case GHOST_IO_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_IO2;
            }
            break;
            
        case GHOST_IO2:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_IO2_LEN; i++){
                ghost_writeline(ghost_io2[i], GREEN);
            }
            ghost_state=GHOST_IO2_BP;
            break;
        
        case GHOST_IO2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_IO3;
            }
            break;
        
        case GHOST_IO3:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_IO3_LEN; i++){
                ghost_writeline(ghost_io3[i], GREEN);
            }
            ghost_state=GHOST_IO3_BP;
            break;
        
        case GHOST_IO3_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_IO4;
            }
            if((sw_ghost_test == 0)){
                //ghost_sound_sos[ghost_sound_index][1]
                ghost_state=GHOST_IO_MODULATING;
                //ghost_menu_state = GHOST_M_LVL_10;
                ghost_io_index = 0;
            }
            break;    
        
        case GHOST_IO_MODULATING:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_IO4;
            }
            if((sw_ghost_test == 0)){
                if(ghost_io_index < 8){
                    sw_ghost_test = 10; //1 s
                    LATAbits.LATA7 = ghost_io_message[ghost_io_index];
                    ghost_io_index++;
                    if(ghost_io_led_state != 1){
                        ghost_io_led_state = 1;
                        red(50);
                        green(50);
                        blue(0);
                    }else{
                        ghost_io_led_state = 0;
                        red(0);
                        green(0);
                        blue(50);
                    }
                    
                }else{
                    ghost_state=GHOST_IO3_BP;
                    sw_ghost_test = 20; //2 more seconds
                    red(0);
                    green(0);
                    blue(0);
                }
            }
            break;
            
        case GHOST_IO4:
            red(0);
            green(0);
            blue(0);
            ghost_state=GHOST_MENU;
            break;
        
        case GHOST_IO_SERVER:
            ghost_set_serial_state(GHOST_IO_LOGIN);
            ghost_set_server_state(1);
            //ghost_set_wrong_tries(0);
            ghost_state=GHOST_IO_SERVER_BP;
            break;
            
        case GHOST_IO_SERVER_BP:
            if((ghost_get_serial_state() == GHOST_DONE ) || (BUTTON_PRESSED_AND_CONSUME)){
                ghost_state=GHOST_MENU;
                ghost_set_server_state(0);
            }
            if(ghost_get_serial_state() == GHOST_RESET_SYSTEM){
                ghost_state=GHOST_INIT;
                ghost_set_server_state(0);
            }
            if(ghost_get_serial_state() == GHOST_IO_GOOD){
                ghost_state=GHOST_IO_SERVER_PASS;
                ghost_set_server_state(0);
            }
            break;
            
        case GHOST_IO_SERVER_PASS:
            ghost_menu_state = GHOST_M_LVL_9;
            ghost_state=GHOST_MENU;
            break;
        
        case GHOST_IR:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_IR_LEN; i++){
                ghost_writeline(ghost_ir[i], GREEN);
            }
            ghost_state=GHOST_IR_BP;
            break;
        
        case GHOST_IR_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_IR2;
            }
            break;
        
        case GHOST_IR2:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_IR2_LEN; i++){
                ghost_writeline(ghost_ir2[i], GREEN);
            }
            ghost_state=GHOST_IR_MODULATING;
            ghost_io_index =0;
            break;
        
        case GHOST_IR_MODULATING:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_menu_state = GHOST_M_LVL_10;
                ghost_state=GHOST_MENU;
            }
            //LATBbits.LATB13 = 0;
            if((sw_ghost_test == 0)){
                if(ghost_io_index < GHOST_IR_MESSAGE_LEN){
                    sw_ghost_test = ghost_ir_message[ghost_io_index][1]; //delay
                    LATBbits.LATB13 = ghost_ir_message[ghost_io_index][0];
                    ghost_io_index++;                    
                }else{
                    ghost_io_index = 0;
                }
            }
            break;    
        
        case GHOST_IR_SERVER:
            ghost_set_serial_state(GHOST_IR_LOGIN);
            ghost_set_server_state(1);
            ghost_state=GHOST_IR_SERVER_BP;
            break;
            
        case GHOST_IR_SERVER_BP:
            if((ghost_get_serial_state() == GHOST_DONE ) || (BUTTON_PRESSED_AND_CONSUME)){
                ghost_state=GHOST_MENU;
                ghost_set_server_state(0);
            }
            if(ghost_get_serial_state() == GHOST_RESET_SYSTEM){
                ghost_state=GHOST_INIT;
                ghost_set_server_state(0);
            }
            if(ghost_get_serial_state() == GHOST_IR_GOOD){
                ghost_state=GHOST_IR_SERVER_PASS;
                ghost_set_server_state(0);
            }
            break;
            
        case GHOST_IR_SERVER_PASS:
            ghost_menu_state = GHOST_M_LVL_11;
            ghost_state=GHOST_MENU;
            break;
        
        case GHOST_TEMPEST:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_TEMPEST_LEN; i++){
                ghost_writeline(ghost_tempest[i], GREEN);
            }
            ghost_state=GHOST_TEMPEST_BP;
            break;
        
        case GHOST_TEMPEST_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_TEMPEST2;
            }
            break;
        
        case GHOST_TEMPEST2:
            DlInit();
            DlTransparentIndex(0);
            DlBackgroundColor(BLACK);
            DlClear();
            ghost_movecursor(0,0);
            for(i=0; i<GHOST_TEMPAST2_LEN; i++){
                ghost_writeline(ghost_tempest2[i], GREEN);
            }
            ghost_state=GHOST_TEMPEST_MODULATING;
            ghost_tempest_message_index = 0;
            break;
            
        case GHOST_TEMPEST_MODULATING:
            if (BUTTON_PRESSED_AND_CONSUME){
                ghost_state=GHOST_TEMPEST2_BP;
            }
            
            
            if(sw_ghost_test == 0){
                if(ghost_tempest_message_index >= GHOST_TEMPEST_MESSAGE_LEN){
                    ghost_tempest_message_index = 0;
                }

                switch(ghost_tempest_message[ghost_tempest_message_index][0]){
                    case 'L':
                        ghost_tempest_beep_length = 600000;
                        break;
                    case 'S':
                        ghost_tempest_beep_length = 200000;
                        break;
                }
                
                        
                TRISAbits.TRISA0 = 0;
                TRISAbits.TRISA1 = 0;

                //Short beep = 200000
                //Long beep = 600000
            
            
                for(ghost_tempest_counter=0; ghost_tempest_counter < ghost_tempest_beep_length; ghost_tempest_counter++){
                    LATAbits.LATA0 = 1;
                    LATAbits.LATA1 = 0;
                    
                    LATAbits.LATA0 = 0;
                    LATAbits.LATA1 = 1;
                    
                    LATAbits.LATA0 = 1;
                    LATAbits.LATA1 = 0;
                    
                    LATAbits.LATA0 = 0;
                    LATAbits.LATA1 = 0;
                    
                    /*LATAbits.LATA0 = 1;
                    LATAbits.LATA0 = 0;
                    LATAbits.LATA0 = 1;
                    LATAbits.LATA0 = 0;
                    LATAbits.LATA0 = 1;
                    LATAbits.LATA0 = 0;
                    LATAbits.LATA0 = 0;
                    LATAbits.LATA0 = 0;*/
                }
                sw_ghost_test = ghost_tempest_message[ghost_tempest_message_index][1];
                ghost_tempest_message_index++;
            }
            break;
            
        case GHOST_TEMPEST2_BP:
            ghost_menu_state = GHOST_M_LVL_12;
            ghost_state=GHOST_MENU;
            break;
        
        case GHOST_TEMPEST_SERVER:
            ghost_set_serial_state(GHOST_TEMPEST_LOGIN);
            ghost_set_server_state(1);
            ghost_state=GHOST_TEMPEST_SERVER_BP;
            break;
        
        case GHOST_TEMPEST_SERVER_BP:
            if((ghost_get_serial_state() == GHOST_DONE ) || (BUTTON_PRESSED_AND_CONSUME)){
                ghost_state=GHOST_MENU;
                ghost_set_server_state(0);
            }
            if(ghost_get_serial_state() == GHOST_RESET_SYSTEM){
                ghost_state=GHOST_INIT;
                ghost_set_server_state(0);
            }
            if(ghost_get_serial_state() == GHOST_TEMPEST_GOOD){
                ghost_state=GHOST_TEMPEST_SERVER_PASS;
                ghost_set_server_state(0);
            }
            break;
            
        case GHOST_TEMPEST_SERVER_PASS:
            ghost_menu_state = GHOST_M_LVL_13;
            ghost_state=GHOST_MENU;
            break;
            
        case GHOST_EXIT:
            //ghost_state=GHOST_INIT;
            returnToMenus();
        break;
        
    }
}

unsigned int ghost_get_server_state(void){
    return g_ghost_server_enable;
}
void ghost_set_server_state(unsigned int value){
    g_ghost_server_enable = value;
}

unsigned int ghost_get_serial_state(void){
    return g_ghost_serial_state;
}
void ghost_set_serial_state(unsigned int value){
    g_ghost_serial_state = value;
}
