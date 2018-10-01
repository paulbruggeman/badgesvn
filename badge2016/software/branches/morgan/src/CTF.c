#include "CTF.h"
#include "plib.h"
#include "stopwatch.h"
#include "menu.h"
#include "colors.h"
#include "touchCTMU.h"

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
    CTF_INIT,
    CTF_MENU,
    CTF_MENU2,
    CTF_HELLO_WORLD,
    CTF_RETURN_TO_MENU,
    CTF_RETURN_TO_MENU2,
    CTF_MYSTORY,
    CTF_MYSTORY_BP,
    CTF_MYSTORY2,
    CTF_MYSTORY2_BP,
    CTF_MYSTORY3,
    CTF_MYSTORY3_BP,
    CTF_MYSTORY4,
    CTF_MYSTORY4_BP,
    CTF_SOUND,
    CTF_SOUND_BP,
    CTF_SOUND2,
    CTF_SOUND2_BP,
    CTF_SOUND3,
    CTF_SOUND3_BP,
    CTF_SOUND_PASSED,
    CTF_SOUND_FAILED,
    CTF_SOUND_FAILED_DELAY,
    CTF_SOUND_S0S,
    CTF_SOUND_SWEEP,
    CTF_SOUND_SWEEP2,
    CTF_SOUND_MANUAL,
    CTF_SOUND_MANUAL_BP,
    CTF_LED,
    CTF_LED_BP,
    CTF_LED2,
    CTF_LED2_BP,
    CTF_LED3,
    CTF_LED3_BP,
    CTF_LED_FAILED,
    CTF_LED_PASSED,
    CTF_ADJUST_RED_LED,
    CTF_ADJUST_RED_LED_BP,
    CTF_ADJUST_GREEN_LED,
    CTF_ADJUST_GREEN_LED_BP,
    CTF_ADJUST_BLUE_LED,
    CTF_ADJUST_BLUE_LED_BP,
    CTF_SERIAL,
    CTF_SERIAL_BP,
    CTF_SERIAL2,
    CTF_SERIAL2_BP,
    CTF_SERIAL3,
    CTF_SERIAL3_BP,
    CTF_IO,
    CTF_IO_BP,
    CTF_IO2,
    CTF_IO2_BP,
    CTF_IO3,
    CTF_IO3_BP,
    CTF_IO4,
    CTF_IO4_BP,
    CTF_IO_MODULATING,
    CTF_IO_SERVER,
    CTF_IO_SERVER_BP,
    CTF_IO_SERVER_PASS,
    CTF_IR,
    CTF_IR_BP,
    CTF_IR2,
    CTF_IR2_BP,
    CTF_IR_MODULATING,
    CTF_IR_SERVER,
    CTF_IR_SERVER_BP,
    CTF_IR_SERVER_PASS,
    CTF_TEMPEST,
    CTF_TEMPEST_BP,
    CTF_TEMPEST2,
    CTF_TEMPEST2_BP,
    CTF_TEMPEST_MODULATING,
    CTF_TEMPEST_SERVER,
    CTF_TEMPEST_SERVER_BP,
    CTF_TEMPEST_SERVER_PASS,
    CTF_START,
    CTF_LEVEL1,
    CTF_EXIT
};

enum {
    CTF_M_LVL_1,
    CTF_M_LVL_2,
    CTF_M_LVL_3,
    CTF_M_LVL_4,
    CTF_M_LVL_5,
    CTF_M_LVL_6,
    CTF_M_LVL_7,
    CTF_M_LVL_8,
    CTF_M_LVL_9,
    CTF_M_LVL_10,
    CTF_M_LVL_11,
    CTF_M_LVL_12,
    CTF_M_LVL_13,
};

//LED PWM ranges
#define CTF_LED_MAX   255
#define CTF_LED_MIN   0

//Sound Ranges
#define CTF_FREQ_MIN 10
#define CTF_FREQ_MAX 255
#define CTF_LEN_MIN 1
#define CTF_LEN_MAX 100

//Ghost game globals
volatile unsigned int g_ctf_server_enable = 0;
volatile unsigned int g_ctf_serial_state = 0;
volatile unsigned int ctf_state = CTF_INIT;
volatile unsigned char ctf_menu_state = CTF_M_LVL_1;
unsigned char ctf_ledstate = 0;
unsigned char ctf_led_button_pushed = 0;
unsigned char ctf_led_button_expected_prev_state = 0;
unsigned char ctf_led_red_val = 0;
unsigned char ctf_led_green_val = 0;
unsigned char ctf_led_blue_val =0;
char ctf_tmp_string[5];
unsigned char ctf_sound_index = 0;

unsigned char ctf_sound_freq = 200;
unsigned char ctf_sound_len = 10;

unsigned char ctf_io_index =0;
unsigned char ctf_io_led_state = 0;
unsigned int ctf_tempest_counter =0;
unsigned int ctf_tempest_beep_length = 0;
unsigned char ctf_tempest_message_index = 0;

//chars for keys
volatile char *ctf_hw[] = {
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

volatile char *ctf_num[]={
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
void ctf_hello_cb(void){
    ctf_state=CTF_HELLO_WORLD;
}

void ctf_quit_cb(void){
    //ctf_state=CTF_INIT;
    returnToMenus();
}

void ctf_mystory_cb(void){
    ctf_state=CTF_MYSTORY;
}

void ctf_sound_cb(void){
    ctf_state=CTF_SOUND;
}

void ctf_sound_sos_cb(void){
    ctf_state=CTF_SOUND_S0S;
    sw_ctf_test = 0;
    ctf_sound_index = 0;
}

void ctf_sound_sweep_cb(void){
    ctf_state=CTF_SOUND_SWEEP;
    sw_ctf_test = 0;
    ctf_sound_index = 0;
}

void ctf_sound_manual_cb(void){
    ctf_state=CTF_SOUND_MANUAL;
    sw_ctf_test = 0;
    ctf_sound_index = 0;
}

void ctf_led_cb(void){
    ctf_state=CTF_LED;
}

void ctf_serial_cb(void){
    ctf_state=CTF_SERIAL;
}

void ctf_led_red_cb(void){
    red(50);
    green(0);
    blue(0);
}

void ctf_led_green_cb(void){
    red(0);
    green(50);
    blue(0);
}

void ctf_led_blue_cb(void){
    red(0);
    green(0);
    blue(50);
}

void ctf_led_white_cb(void){
    red(50);
    green(50);
    blue(50);
}

void ctf_led_red_value_cb(void){
    ctf_state=CTF_ADJUST_RED_LED;
}

void ctf_led_green_value_cb(void){
    ctf_state=CTF_ADJUST_GREEN_LED;
}

void ctf_led_blue_value_cb(void){
    ctf_state=CTF_ADJUST_BLUE_LED;
}

void ctf_io_cb(void){
    ctf_state=CTF_IO;
}

void ctf_io_rerun_cb(void){
    ctf_state=CTF_IO;
}

void ctf_io_server_cb(void){
    ctf_state=CTF_IO_SERVER;
}

void ctf_io_alarm_cb(void){
    
}

void ctf_io_fg_cb(void){
    
}

void ctf_ir_cb(void){
    ctf_state=CTF_IR;
}

void ctf_ir_server_cb(void){
    ctf_state=CTF_IR_SERVER;
}

void ctf_tempest_cb(void){
    ctf_state=CTF_TEMPEST;
}

void ctf_tempest_server_cb(void){
    ctf_state=CTF_TEMPEST_SERVER;
}

struct menu_t ctf_menu[] = {
   {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu2[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu3[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_sound_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_sound_submenu[] = {
   {"SOS", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_sound_sos_cb)},
   {"Sweep", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_sound_sweep_cb)},
   {"Manual", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_sound_manual_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu4[] = {
   {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

/*
struct menu_t ctf_menu5[] = {
   {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_cb)},
   {"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_serial_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};*/

struct menu_t ctf_menu_led_manual[] = {
   {"Red Value", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_red_value_cb)},
   {"Green Value", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_green_value_cb)},
   {"Blue Value", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_blue_value_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu_led[] = {
   {"Red", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_red_cb)},
   {"Green", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_green_cb)},
   {"Blue", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_blue_cb)},
   {"White", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_led_white_cb)},
   {"Manual", VERT_ITEM, MENU, ctf_menu_led_manual},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu6[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   {"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_serial_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu7[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_serial_cb)},
   {"IO", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_io_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu_io[] = {
   {"Rerun IO", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_io_rerun_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_io_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu8[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"Serial", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_serial_cb)},
   {"IO", VERT_ITEM, MENU, ctf_menu_io},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu_io2[] = {
   {"Alarm", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_io_alarm_cb)},
   {"Function Generator", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_io_fg_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu9[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"IO", VERT_ITEM, MENU, ctf_menu_io2},
   {"IR", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_ir_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu_ir_sub[] = {
   {"Rerun", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_ir_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_ir_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu10[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"IO", VERT_ITEM, MENU, ctf_menu_io2},
   {"IR", VERT_ITEM, MENU, ctf_menu_ir_sub},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu11[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"IO", VERT_ITEM, MENU, ctf_menu_io2},
   //{"IR", VERT_ITEM|DEFAULT_ITEM, MENU, ctf_menu_ir_sub},
   {"TEMPEST", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_tempest_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu_tempest_sub[] = {
   {"Rerun", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_tempest_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_tempest_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu12[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"IO", VERT_ITEM, MENU, ctf_menu_io2},
   //{"IR", VERT_ITEM|DEFAULT_ITEM, MENU, ctf_menu_ir_sub},
   {"TEMPEST", VERT_ITEM, MENU, ctf_menu_tempest_sub},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu_tempest_sub2[] = {
   {"Rerun", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_tempest_cb)},
   {"Start Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_tempest_server_cb)},
   {"Back", VERT_ITEM|LAST_ITEM, BACK, {NULL}},
};

struct menu_t ctf_menu13[] = {
   {"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Sound", VERT_ITEM, MENU, ctf_sound_submenu},
   {"LED", VERT_ITEM, MENU, ctf_menu_led},
   //{"IO", VERT_ITEM, MENU, ctf_menu_io2},
   //{"IR", VERT_ITEM|DEFAULT_ITEM, MENU, ctf_menu_ir_sub},
   //{"TEMPEST", VERT_ITEM|DEFAULT_ITEM, MENU, ctf_menu_tempest_sub2},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

//Screen: 21 across by 13 high
//Message: 38 lines long
const char *ctf_mystory[] = {
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
#define CTF_SOUND_LEN     10
const char *ctf_sound[] = {
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

#define CTF_SOUND_PASSED_LEN 5
const char *ctf_sound_passed[] = {
    "Seed found!\r",
    "We now have sound!\r",
    "Return to the sound\r",
    "menu for some fun.\r",
    "CTF Code: ",
    //CTF_SOUND    
};

//message: 2
#define CTF_SOUND_FAILED_LEN  2
const char *ctf_sound_failed[] = {
    "Failure, Sound \r",
    "Not found."   
};

//message: 11 lines
#define CTF_LED_LEN   8
const char *ctf_led[] = {
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
#define CTF_LED_FAIL_LEN  5
const char *ctf_led_fail[] = {
    "Result illogical.\r",
    "Seed generation\r",
    "failed.\r",
    "Press button to\r",
    "return to menu."
};


#define CTF_LED_PASS_LEN  6
const char *ctf_led_pass[] = {
    "Seed discovered!\r",
    "I can now control\r",
    "the light! Return\r",
    "to the LED menu. It\r",
    "has been updated.\r",
    "CTF Code: ",
    
    //CTF_LED
};

//message: 10 lines
#define CTF_SERIAL_LEN    10
const char *ctf_serial[] = {
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

#define CTF_SERIAL2_LEN   3
const char *ctf_serial2[] = {
    "Serial server active\r",
    "Try logging in.\r",
    "Press Button to Exit."
};

#define CTF_SERIAL3_LEN   3
const char *ctf_serial3[] = {
    "Serial server \r",
    "deactivated. \r",
    "Press Button"
};

#define CTF_IO_LEN    6
const char *ctf_io[] = {
    "I think I found a\r",
    "free IO pin! \r",
    "I hacked into a \r",
    "communications \r",
    "subsystem. I have\r",
    "managed to slow it\r"
};

//This one was weird, there was a memory overlap with ctf_io3 so i had
//to add the last space and not call it, hence the -1 value on len.
#define CTF_IO2_LEN   6
const char *ctf_io2[] = {
    "down and link the\r",
    "LED with the clock\r",
    "signal. I need you\r",
    "to find the data \r",
    "on a pin and decode\r",
    "the binary."
};

//Message 3 lines
#define CTF_IO3_LEN   3
const char *ctf_io3[] = {
    "Sending data\r",
    "now. Press button \r",
    "to stop."
};

//Message 5 lines
//This one not used? Check
#define CTF_IO4_LEN   5
const char *ctf_io4[] = {
    "Serial Server \r",
    "Started. Enter \r",
    "IO Seed\r",
    "Press button to\r",
    "end server."
};


//Message 8 lines
#define CTF_IR_LEN    8
const char *ctf_ir[] = {
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
#define CTF_IR2_LEN   3
const char *ctf_ir2[] = {
    "Sending Data\r",
    "Press button to\r",
    "stop."
};


//Message 3 lines
#define CTF_TEMPEST_LEN   3
const char *ctf_tempest[] = {
    "Final stage. Can \r",
    "you figure this\r",
    "one out?"
};

//Message 2 lines
#define CTF_TEMPAST2_LEN  2
const char *ctf_tempest2[] = {
    "Running...\r",
    "Press button to end"
};

//Length 18
// Note: {freq, length(in 0.1 ms)}
// 0 = pause
#define CTF_SOUND_SOS_LENGTH      18
const int ctf_sound_sos[CTF_SOUND_SOS_LENGTH][2]={
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
const int ctf_io_message[]={
    //0,0,1,0,1,0,1,0
    0,1,0,1,1,0,0,0
};

#define CTF_IR_MESSAGE_LEN    21
// Note: {data [1,0], length(in 0.1 ms)}
// start bit 1, stop bit 0
//42 in ascii is 52[00110100], 50[00110010]

//Yo in ascii is 89[01011001], 111[01101111]
const char ctf_ir_message[CTF_IR_MESSAGE_LEN][2]={
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

#define CTF_TEMPEST_MESSAGE_LEN    17
// 'S' = short
// 'L' = long
// number is pause after tone
// data = {L , 1} 
// Freedom (ssls sls s s lss lll ll)
const char ctf_tempest_message[CTF_IR_MESSAGE_LEN][2]={
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

void ctf_app(){
    char i=0;
    
    //Skip ahead for debugging! Remove in final code!
    //#warning remove in final code
    //ctf_menu_state = CTF_M_LVL_9;
    
    switch(ctf_state){
        case CTF_INIT:
            red(50);
            green(50);
            blue(50);
            
            ctf_menu_state = CTF_M_LVL_1;
            ctf_state=CTF_MENU;
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
        break;
        
        case CTF_MENU:
            switch(ctf_menu_state){
                case CTF_M_LVL_1:
                    genericMenu(ctf_menu, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_2:
                    genericMenu(ctf_menu2, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_3:
                    genericMenu(ctf_menu3, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_4:
                    genericMenu(ctf_menu4, MAIN_MENU_STYLE);
                    break;
                //case CTF_M_LVL_5:
                    //genericMenu(ctf_menu5);
                    //break;
                case CTF_M_LVL_6:
                    genericMenu(ctf_menu6, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_7:
                    genericMenu(ctf_menu7, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_8:
                    genericMenu(ctf_menu8, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_9:
                    genericMenu(ctf_menu9, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_10:
                    genericMenu(ctf_menu10, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_11:
                    genericMenu(ctf_menu11, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_12:
                    genericMenu(ctf_menu12, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_13:
                    genericMenu(ctf_menu13, MAIN_MENU_STYLE);
                    break;
                default:
                    ctf_quit_cb();
                    break;
            }
            break;
        
        //case CTF_MENU2:
        //    genericMenu(ctf_menu2);
        //    break;
        
        case CTF_HELLO_WORLD:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("You found me!\rIt worked!\rHelp me.\rI'm trapped!\rI updated my menu.", GREEN);
            ctf_menu_state = CTF_M_LVL_2;
            ctf_state=CTF_RETURN_TO_MENU;
            break;
            
        
        case CTF_MYSTORY:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<12; i++){
                display_writeline(ctf_mystory[i], GREEN);
            }
            ctf_state=CTF_MYSTORY_BP;
            break;
        
        case CTF_MYSTORY_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MYSTORY2;
                //NOTE, add sound for button press
            }
            break;
            
        case CTF_MYSTORY2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=12; i<24; i++){
                display_writeline(ctf_mystory[i], GREEN);
            }
            ctf_state=CTF_MYSTORY2_BP;
            break;
            
        case CTF_MYSTORY2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MYSTORY3;
                //NOTE, add sound for button press
            }
            break;
            
        case CTF_MYSTORY3:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=24; i<36; i++){
                display_writeline(ctf_mystory[i], GREEN);
            }
            ctf_state=CTF_MYSTORY3_BP;
            break;    
        
        case CTF_MYSTORY3_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MYSTORY4;
                //NOTE, add sound for button press
            }
            break;
        
        case CTF_MYSTORY4:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=36; i<38; i++){
                display_writeline(ctf_mystory[i], GREEN);
            }
            ctf_state=CTF_MYSTORY4_BP;
            break;    
        
        case CTF_MYSTORY4_BP:
            ctf_menu_state = CTF_M_LVL_3;
            ctf_state=CTF_RETURN_TO_MENU;
            
            //if (BUTTON_PRESSED_AND_CONSUME){
            //    ctf_menu_state = CTF_M_LVL_2;
            //    
            //    //NOTE, add sound for button press
            //}
            break;    
        case CTF_SOUND:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_SOUND_LEN ; i++){
                display_writeline(ctf_sound[i], GREEN);
            }
            ctf_state=CTF_SOUND_BP;
            break;
            
        case CTF_SOUND_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SOUND2;
                sw_ctf_test = 30;         //3 second wait till audio
                //NOTE, add sound for button press
            }
            break;
            
        case CTF_SOUND2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            red(0);
            green(0);
            blue(0);
            
            //Pressed too early, fail
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SOUND_FAILED;
            }
            
            //Wait to move forward
            if(sw_ctf_test == 0){
                setNote(256, 2000000); //1 second= 200000*10
                //red(255);
                sw_ctf_test = 10;
                ctf_state=CTF_SOUND3;
            }

            break;
            
        case CTF_SOUND3:
            // good press
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SOUND_PASSED;
            }
            if(sw_ctf_test == 0){
                //red(0);
                ctf_state=CTF_SOUND_FAILED_DELAY;
                // one second delay before showing failed
                sw_ctf_test = 10;
            }
            
            break;
        
        case CTF_SOUND_PASSED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_SOUND_PASSED_LEN; i++){
                display_writeline(ctf_sound_passed[i], GREEN);
            }
            
            display_writeline(ctf_hw[0], GREEN);
            display_writeline(ctf_hw[4], GREEN);
            display_writeline(ctf_hw[8], GREEN);
            display_writeline(ctf_num[8], GREEN);
            display_writeline(ctf_num[3], GREEN);
            display_writeline(ctf_num[7], GREEN);
            display_writeline(ctf_num[2], GREEN);
            
            ctf_menu_state = CTF_M_LVL_4;
            ctf_state=CTF_RETURN_TO_MENU;
            break;
        
        case CTF_SOUND_FAILED_DELAY:
            if(sw_ctf_test == 0){
                ctf_state=CTF_SOUND_FAILED;
            }
            break;
            
        case CTF_SOUND_FAILED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_SOUND_FAILED_LEN; i++){
                display_writeline(ctf_sound_failed[i], GREEN);
            }
            ctf_state=CTF_RETURN_TO_MENU;
            break;
        
        case CTF_SOUND_S0S:
            if((sw_ctf_test == 0) && (ctf_sound_index < CTF_SOUND_SOS_LENGTH)){
                //ctf_sound_sos[ctf_sound_index][1]
                //setNote(256, 200000*10); //1 second
                if(ctf_sound_sos[ctf_sound_index][0] != 0){
                    setNote(ctf_sound_sos[ctf_sound_index][0], 200000*ctf_sound_sos[ctf_sound_index][1]);
                }
                sw_ctf_test = ctf_sound_sos[ctf_sound_index][1];
                ctf_sound_index++;
                if(ctf_sound_index == CTF_SOUND_SOS_LENGTH - 1){
                    //ctf_state=CTF_RETURN_TO_MENU;
                    ctf_state=CTF_MENU;
                }
            }
            
            break;
        
        case CTF_SOUND_SWEEP:
            if((sw_ctf_test == 0)){
                //ctf_sound_sos[ctf_sound_index][1]
                if(ctf_sound_index < 200){
                    setNote(ctf_sound_index, 200000*2); //0.2 second
                    sw_ctf_test = 1; //0.1 s
                    ctf_sound_index=ctf_sound_index+5;
                }else{
                    ctf_state=CTF_SOUND_SWEEP2;
                }
            }
            break;
            
        case CTF_SOUND_SWEEP2:
            if((sw_ctf_test == 0)){
                //ctf_sound_sos[ctf_sound_index][1]
                if(ctf_sound_index > 10){
                    setNote(ctf_sound_index, 200000*2); //0.2 second
                    sw_ctf_test = 1;
                    ctf_sound_index=ctf_sound_index-5;;
                }else{
                    ctf_state=CTF_MENU;
                }
            }
            break;
            
        case CTF_SOUND_MANUAL:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(WHITE);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Up Down for Freq\r", BLACK);
            display_writeline("Left Right for Len\r", BLACK);
            display_writeline("Button to Play", BLACK);
            sprintf(ctf_tmp_string,"%03u",ctf_sound_freq);
            display_writeline("\rFreq (rel): ", BLACK);
            display_writeline(ctf_tmp_string, BLACK);
            sprintf(ctf_tmp_string,"%03u",ctf_sound_len);
            display_writeline("\rLen (0.1s): ", BLACK);
            display_writeline(ctf_tmp_string, BLACK);
            
            ctf_state=CTF_SOUND_MANUAL_BP;
            break;
            
        case CTF_SOUND_MANUAL_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                setNote(ctf_sound_freq, 200000*ctf_sound_len);
                ctf_state=CTF_MENU;
                
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                if(ctf_sound_freq <  CTF_FREQ_MAX){
                    ctf_sound_freq++;
                    ctf_state=CTF_SOUND_MANUAL;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                if(ctf_sound_freq >  CTF_FREQ_MIN){
                    ctf_sound_freq--;
                    ctf_state=CTF_SOUND_MANUAL;
                }
            }
            if(LEFT_SLIDE_AND_CONSUME){
                if(ctf_sound_len >  CTF_LEN_MIN){
                    ctf_sound_len--;
                    ctf_state=CTF_SOUND_MANUAL;
                }
                
            }
            
            if(RIGHT_SLIDE_AND_CONSUME){
                if(ctf_sound_len <  CTF_LEN_MAX){
                    ctf_sound_len++;
                    ctf_state=CTF_SOUND_MANUAL;
                }
                
            }
            
            break;
            
        case CTF_LED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_LED_LEN; i++){
                display_writeline(ctf_led[i], GREEN);
            }
            ctf_state=CTF_LED_BP;
            break;
            
        case CTF_LED_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_LED2;
                ctf_ledstate=0; //Reset LED sequence
                ctf_led_button_pushed = 0; //Reset button detection variable
                
                
            }
            break;    
            
        case CTF_LED2:
            if(sw_ctf_test == 0){
                sw_ctf_test = 30; // 3 second delay
                //manage output display
                switch(ctf_ledstate){
                    case 0:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(GREEN);
                        FbClear();
                        red(50);
                        green(0);
                        blue(0);
                        ctf_led_button_expected_prev_state = 0;
                        break;
                    case 1:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(BLUE);
                        FbClear();
                        red(0);
                        green(50);
                        blue(0);
                        ctf_led_button_expected_prev_state = 0;
                        break;
                    case 2:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(GREEN);
                        FbClear();
                        red(0);
                        green(50);
                        blue(0);
                        ctf_led_button_expected_prev_state = 0;
                        break;
                    case 3:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(RED);
                        FbClear();
                        red(0);
                        green(0);
                        blue(50);
                        ctf_led_button_expected_prev_state = 1;
                        break;
                    case 4:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(BLUE);
                        FbClear();
                        red(0);
                        green(50);
                        blue(0);
                        ctf_led_button_expected_prev_state = 0;
                        break;
                    case 5:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(RED);
                        FbClear();
                        red(50);
                        green(0);
                        blue(0);
                        ctf_led_button_expected_prev_state = 0;
                        break;
                    case 6:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(GREEN);
                        FbClear();
                        red(0);
                        green(50);
                        blue(0);
                        ctf_led_button_expected_prev_state = 1;
                        break;
                    case 7:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(BLUE);
                        FbClear();
                        red(0);
                        green(0);
                        blue(50);
                        ctf_led_button_expected_prev_state = 1;
                        break;    
                    case 8:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(WHITE);
                        FbClear();
                        red(0);
                        green(50);
                        blue(0);
                        ctf_led_button_expected_prev_state = 1;
                        break;    
                    case 9:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(WHITE);
                        FbClear();
                        red(50);
                        green(50);
                        blue(50);
                        ctf_led_button_expected_prev_state = 0;
                        break;
                    case 10:
                        FbInit();
                        FbTransparency(0);
                        FbBackgroundColor(BLACK);
                        FbClear();
                        ctf_led_button_expected_prev_state = 1;
                        ctf_state=CTF_LED_PASSED;
                        break;
                }
                ctf_ledstate++;
                if(ctf_led_button_expected_prev_state == ctf_led_button_pushed){
                    //Good pass to next level
                    
                }else{
                    //Fail, exit game
                    ctf_state=CTF_LED_FAILED;
                }
                //reset button push
                ctf_led_button_pushed = 0;
            }
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_led_button_pushed = 1;
                sw_ctf_test = 0;
            }  
            break;
            
        case CTF_LED_PASSED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_LED_PASS_LEN; i++){
                display_writeline(ctf_led_pass[i], GREEN);
            }
            
            display_writeline(ctf_hw[0], GREEN);
            display_writeline(ctf_hw[4], GREEN);
            display_writeline(ctf_hw[8], GREEN);
            display_writeline(ctf_num[6], GREEN);
            display_writeline(ctf_num[2], GREEN);
            display_writeline(ctf_num[5], GREEN);
            display_writeline(ctf_num[3], GREEN);
            
            ctf_menu_state = CTF_M_LVL_6;
            ctf_state=CTF_RETURN_TO_MENU;
            break;
            
        case CTF_LED_FAILED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_LED_FAIL_LEN; i++){
                display_writeline(ctf_led_fail[i], GREEN);
            }
            ctf_state=CTF_RETURN_TO_MENU;
            break;
            
        case CTF_ADJUST_RED_LED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(RED);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Up Down to adjust\r", BLACK);
            display_writeline("Range 0 to 255\r", BLACK);
            display_writeline("Button to exit\r", BLACK);
            sprintf(ctf_tmp_string,"%03u",ctf_led_red_val);
            display_writeline(ctf_tmp_string, BLACK);
            red(ctf_led_red_val);
            ctf_state=CTF_ADJUST_RED_LED_BP;
            break;
            
        case CTF_ADJUST_RED_LED_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                ctf_menu_state = CTF_M_LVL_6;
                ctf_state=CTF_MENU;
                
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                setNote(109, 2048);
                if(ctf_led_red_val <  CTF_LED_MAX){
                    ctf_led_red_val++;
                    ctf_state=CTF_ADJUST_RED_LED;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                setNote(97, 2048);
                if(ctf_led_red_val >  CTF_LED_MIN){
                    ctf_led_red_val--;
                    ctf_state=CTF_ADJUST_RED_LED;
                }
            }
            break;
            
        case CTF_ADJUST_GREEN_LED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(GREEN);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Up Down to adjust\r", BLACK);
            display_writeline("Range 0 to 255\r", BLACK);
            display_writeline("Button to exit\r", BLACK);
            sprintf(ctf_tmp_string,"%03u",ctf_led_green_val);
            display_writeline(ctf_tmp_string, BLACK);
            green(ctf_led_green_val);
            ctf_state=CTF_ADJUST_GREEN_LED_BP;
            break;
            
        case CTF_ADJUST_GREEN_LED_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                ctf_menu_state = CTF_M_LVL_6;
                ctf_state=CTF_MENU;
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                setNote(109, 2048);
                if(ctf_led_green_val <  CTF_LED_MAX){
                    ctf_led_green_val++;
                    ctf_state=CTF_ADJUST_GREEN_LED;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                setNote(97, 2048);
                if(ctf_led_green_val >  CTF_LED_MIN){
                    ctf_led_green_val--;
                    ctf_state=CTF_ADJUST_GREEN_LED;
                }
            }
            break;
            
        case CTF_ADJUST_BLUE_LED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLUE);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Up Down to adjust\r", BLACK);
            display_writeline("Range 0 to 255\r", BLACK);
            display_writeline("Button to exit\r", BLACK);
            sprintf(ctf_tmp_string,"%03u",ctf_led_blue_val);
            display_writeline(ctf_tmp_string, BLACK);
            blue(ctf_led_blue_val);
            ctf_state=CTF_ADJUST_BLUE_LED_BP;
            break;
        
        case CTF_ADJUST_BLUE_LED_BP:
            if(BUTTON_PRESSED_AND_CONSUME){
                ctf_menu_state = CTF_M_LVL_6;
                ctf_state=CTF_MENU;
                break;
            }
            if(TOP_SLIDE_AND_CONSUME){
                setNote(109, 2048);
                if(ctf_led_blue_val <  CTF_LED_MAX){
                    ctf_led_blue_val++;
                    ctf_state=CTF_ADJUST_BLUE_LED;
                }
            }
            if(BOTTOM_SLIDE_AND_CONSUME){
                setNote(97, 2048);
                if(ctf_led_blue_val >  CTF_LED_MIN){
                    ctf_led_blue_val--;
                    ctf_state=CTF_ADJUST_BLUE_LED;
                }
            }
            break;    
            
        case CTF_SERIAL:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_SERIAL_LEN; i++){
                display_writeline(ctf_serial[i], GREEN);
            }
            ctf_state=CTF_SERIAL_BP;
            break;
        
        case CTF_SERIAL_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SERIAL2;
                ctf_set_serial_state(CTF_LOGIN);
                ctf_set_server_state(1);
            }
            break;
            
        case CTF_SERIAL2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_SERIAL2_LEN; i++){
                display_writeline(ctf_serial2[i], GREEN);
            }
            ctf_state=CTF_SERIAL2_BP;
            break;  
        
        case CTF_SERIAL2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SERIAL3;
                ctf_set_server_state(0);   //Turn off the serial server
            }
            break;
            
        case CTF_SERIAL3:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_SERIAL3_LEN; i++){
                display_writeline(ctf_serial3[i], GREEN);
            }
            ctf_state=CTF_SERIAL3_BP;
            break;  
        
        case CTF_SERIAL3_BP:
            if(ctf_get_serial_state() == CTF_DONE){
                ctf_menu_state = CTF_M_LVL_7;
            }else{
                ctf_menu_state = CTF_M_LVL_6;
            }
                
            ctf_state=CTF_RETURN_TO_MENU;
            break;
        
        case CTF_RETURN_TO_MENU:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MENU;
                //NOTE, add sound for button press
            }
            break;
        
        case CTF_IO:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_IO_LEN ; i++){
                display_writeline(ctf_io[i], GREEN);
            }
            ctf_state=CTF_IO_BP;
            ctf_menu_state = CTF_M_LVL_8;
            break;
        
        case CTF_IO_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_IO2;
            }
            break;
            
        case CTF_IO2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_IO2_LEN; i++){
                display_writeline(ctf_io2[i], GREEN);
            }
            ctf_state=CTF_IO2_BP;
            break;
        
        case CTF_IO2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_IO3;
            }
            break;
        
        case CTF_IO3:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_IO3_LEN; i++){
                display_writeline(ctf_io3[i], GREEN);
            }
            ctf_state=CTF_IO3_BP;
            break;
        
        case CTF_IO3_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_IO4;
            }
            if((sw_ctf_test == 0)){
                //ctf_sound_sos[ctf_sound_index][1]
                ctf_state=CTF_IO_MODULATING;
                //ctf_menu_state = CTF_M_LVL_10;
                ctf_io_index = 0;
            }
            break;    
        
        case CTF_IO_MODULATING:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_IO4;
            }
            if((sw_ctf_test == 0)){
                if(ctf_io_index < 8){
                    sw_ctf_test = 10; //1 s
                    LATAbits.LATA7 = ctf_io_message[ctf_io_index];
                    ctf_io_index++;
                    if(ctf_io_led_state != 1){
                        ctf_io_led_state = 1;
                        red(50);
                        green(50);
                        blue(0);
                    }else{
                        ctf_io_led_state = 0;
                        red(0);
                        green(0);
                        blue(50);
                    }
                    
                }else{
                    ctf_state=CTF_IO3_BP;
                    sw_ctf_test = 20; //2 more seconds
                    red(0);
                    green(0);
                    blue(0);
                }
            }
            break;
            
        case CTF_IO4:
            red(0);
            green(0);
            blue(0);
            ctf_state=CTF_MENU;
            break;
        
        case CTF_IO_SERVER:
            ctf_set_serial_state(CTF_IO_LOGIN);
            ctf_set_server_state(1);
            //ctf_set_wrong_tries(0);
            ctf_state=CTF_IO_SERVER_BP;
            break;
            
        case CTF_IO_SERVER_BP:
            if((ctf_get_serial_state() == CTF_DONE ) || (BUTTON_PRESSED_AND_CONSUME)){
                ctf_state=CTF_MENU;
                ctf_set_server_state(0);
            }
            if(ctf_get_serial_state() == CTF_RESET_SYSTEM){
                ctf_state=CTF_INIT;
                ctf_set_server_state(0);
            }
            if(ctf_get_serial_state() == CTF_IO_GOOD){
                ctf_state=CTF_IO_SERVER_PASS;
                ctf_set_server_state(0);
            }
            break;
            
        case CTF_IO_SERVER_PASS:
            ctf_menu_state = CTF_M_LVL_9;
            ctf_state=CTF_MENU;
            break;
        
        case CTF_IR:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_IR_LEN; i++){
                display_writeline(ctf_ir[i], GREEN);
            }
            ctf_state=CTF_IR_BP;
            break;
        
        case CTF_IR_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_IR2;
            }
            break;
        
        case CTF_IR2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_IR2_LEN; i++){
                display_writeline(ctf_ir2[i], GREEN);
            }
            ctf_state=CTF_IR_MODULATING;
            ctf_io_index =0;
            break;
        
        case CTF_IR_MODULATING:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_menu_state = CTF_M_LVL_10;
                ctf_state=CTF_MENU;
            }
            //LATBbits.LATB13 = 0;
            if((sw_ctf_test == 0)){
                if(ctf_io_index < CTF_IR_MESSAGE_LEN){
                    sw_ctf_test = ctf_ir_message[ctf_io_index][1]; //delay
                    LATBbits.LATB13 = ctf_ir_message[ctf_io_index][0];
                    ctf_io_index++;                    
                }else{
                    ctf_io_index = 0;
                }
            }
            break;    
        
        case CTF_IR_SERVER:
            ctf_set_serial_state(CTF_IR_LOGIN);
            ctf_set_server_state(1);
            ctf_state=CTF_IR_SERVER_BP;
            break;
            
        case CTF_IR_SERVER_BP:
            if((ctf_get_serial_state() == CTF_DONE ) || (BUTTON_PRESSED_AND_CONSUME)){
                ctf_state=CTF_MENU;
                ctf_set_server_state(0);
            }
            if(ctf_get_serial_state() == CTF_RESET_SYSTEM){
                ctf_state=CTF_INIT;
                ctf_set_server_state(0);
            }
            if(ctf_get_serial_state() == CTF_IR_GOOD){
                ctf_state=CTF_IR_SERVER_PASS;
                ctf_set_server_state(0);
            }
            break;
            
        case CTF_IR_SERVER_PASS:
            ctf_menu_state = CTF_M_LVL_11;
            ctf_state=CTF_MENU;
            break;
        
        case CTF_TEMPEST:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_TEMPEST_LEN; i++){
                display_writeline(ctf_tempest[i], GREEN);
            }
            ctf_state=CTF_TEMPEST_BP;
            break;
        
        case CTF_TEMPEST_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_TEMPEST2;
            }
            break;
        
        case CTF_TEMPEST2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            for(i=0; i<CTF_TEMPAST2_LEN; i++){
                display_writeline(ctf_tempest2[i], GREEN);
            }
            ctf_state=CTF_TEMPEST_MODULATING;
            ctf_tempest_message_index = 0;
            break;
            
        case CTF_TEMPEST_MODULATING:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_TEMPEST2_BP;
            }
            
            
            if(sw_ctf_test == 0){
                if(ctf_tempest_message_index >= CTF_TEMPEST_MESSAGE_LEN){
                    ctf_tempest_message_index = 0;
                }

                switch(ctf_tempest_message[ctf_tempest_message_index][0]){
                    case 'L':
                        ctf_tempest_beep_length = 600000;
                        break;
                    case 'S':
                        ctf_tempest_beep_length = 200000;
                        break;
                }
                
                        
                TRISAbits.TRISA0 = 0;
                TRISAbits.TRISA1 = 0;

                //Short beep = 200000
                //Long beep = 600000
            
            
                for(ctf_tempest_counter=0; ctf_tempest_counter < ctf_tempest_beep_length; ctf_tempest_counter++){
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
                sw_ctf_test = ctf_tempest_message[ctf_tempest_message_index][1];
                ctf_tempest_message_index++;
            }
            break;
            
        case CTF_TEMPEST2_BP:
            ctf_menu_state = CTF_M_LVL_12;
            ctf_state=CTF_MENU;
            break;
        
        case CTF_TEMPEST_SERVER:
            ctf_set_serial_state(CTF_TEMPEST_LOGIN);
            ctf_set_server_state(1);
            ctf_state=CTF_TEMPEST_SERVER_BP;
            break;
        
        case CTF_TEMPEST_SERVER_BP:
            if((ctf_get_serial_state() == CTF_DONE ) || (BUTTON_PRESSED_AND_CONSUME)){
                ctf_state=CTF_MENU;
                ctf_set_server_state(0);
            }
            if(ctf_get_serial_state() == CTF_RESET_SYSTEM){
                ctf_state=CTF_INIT;
                ctf_set_server_state(0);
            }
            if(ctf_get_serial_state() == CTF_TEMPEST_GOOD){
                ctf_state=CTF_TEMPEST_SERVER_PASS;
                ctf_set_server_state(0);
            }
            break;
            
        case CTF_TEMPEST_SERVER_PASS:
            ctf_menu_state = CTF_M_LVL_13;
            ctf_state=CTF_MENU;
            break;
            
        case CTF_EXIT:
            //ctf_state=CTF_INIT;
            returnToMenus();
        break;
        
    }
}

unsigned int ctf_get_server_state(void){
    return g_ctf_server_enable;
}
void ctf_set_server_state(unsigned int value){
    g_ctf_server_enable = value;
}

unsigned int ctf_get_serial_state(void){
    return g_ctf_serial_state;
}
void ctf_set_serial_state(unsigned int value){
    g_ctf_serial_state = value;
}
