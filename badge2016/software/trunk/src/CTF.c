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
    CTF_MYSTORY5,
    CTF_MYSTORY5_BP,
    CTF_MYSTORY6,
    CTF_MYSTORY6_BP,
    CTF_GROUNDED,
    CTF_GROUNDED_BP,
    CTF_GROUNDED2,
    CTF_GROUNDED_BP2,
    CTF_GROUNDED_PASS,
    CTF_GROUNDED_FAIL,
    CTF_LIGHT_ME,
    CTF_LIGHT_ME_BP,
    CTF_LIGHT_ME2,
    CTF_LIGHT_ME2_BP,
    CTF_LIGHT_ME_PASS,
    CTF_LIGHT_ME_FAIL,
    CTF_LIGHT_ME3_BP,
    CTF_SERIALKILLER,
    CTF_SERIALKILLER_BP,
    CTF_SERIALKILLER2,
    CTF_SERIALKILLER2_BP,
    CTF_EYECOLOR,
    CTF_EYECOLOR_BP,
    CTF_EYECOLOR2,
    CTF_EYECOLOR2_BP,
    CTF_EYECOLOR3,
    CTF_EYECOLOR3_BP,
    CTF_EYECOLOR4,
    CTF_EYECOLOR4_BP,
    CTF_EYECOLOR_DIGIT1,
    CTF_EYECOLOR_DIGIT1_BP,
    CTF_EYECOLOR_DIGIT2,
    CTF_EYECOLOR_DIGIT2_BP,
    CTF_EYECOLOR_DIGIT3,
    CTF_EYECOLOR_DIGIT3_BP,
    CTF_EYECOLOR_SERIAL,
    CTF_EYECOLOR_SERIAL_BP,
    CTF_EYECOLOR_SERIAL2,
    CTF_EYECOLOR_SERIAL2_BP,
    CTF_BITBANGING,
    CTF_BITBANGING_BP,
    CTF_BITBANGING2,
    CTF_BITBANGING2_BP,
    CTF_BITBANGING_CHALLENGE,
    CTF_BITBANGING_CHALLENGE_BP,
    CTF_BITBANGING_TEST,
    CTF_BITBANGING_TEST_BP,
    CTF_SUCCESS,
    CTF_SUCCESS_BP,
    CTF_SUCCESS2,
    CTF_SUCCESS2_BP,
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

unsigned char ctf_lightme_index =0;
unsigned char ctf_lightme_led_state = 0;
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
    "F",
    "U",
    "P"
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

void ctf_grounded_cb(void){
    ctf_state=CTF_GROUNDED;
}

void ctf_lightme_cb(void){
    ctf_state=CTF_LIGHT_ME;
}


void ctf_serialkiller_cb(void){
    ctf_state=CTF_SERIALKILLER;
}

void ctf_bitbanging_cb(void){
    ctf_state=CTF_BITBANGING;
}

//void ctf_bitbanging_serial_cb(void){
//    ctf_state=CTF_BITBANGING_SERIAL;
//}

void ctf_eyecolor_cb(void){
    ctf_state=CTF_EYECOLOR;
}


void ctf_eyecolor_digits_cb(void){
    ctf_state=CTF_EYECOLOR_DIGIT1;
}

void ctf_eyecolor_serial_cb(void){
    ctf_state=CTF_EYECOLOR_SERIAL;
}

void ctf_bitbanging_ch_cb(void){
    ctf_state=CTF_BITBANGING_CHALLENGE;
}

void ctf_bitbanging_tv_cb(void){
    ctf_state=CTF_BITBANGING_TEST;
}

void ctf_success_cb(void){
    ctf_state=CTF_SUCCESS;
}


struct menu_t ctf_menu[] = {
   {"Hello World", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu2[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"The Backstory", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu3[] = {
   //{"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   //{"My Story", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Grounded", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_grounded_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};



struct menu_t ctf_menu4[] = {
 //  {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   //{"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Light Me", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_lightme_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu5[] = {
 //  {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   //{"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Serial Killer", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_serialkiller_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu6[] = {
 //  {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   //{"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Eye Color", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_eyecolor_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu7[] = {
   {"Cycle Colors", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_eyecolor_digits_cb)},
   {"Serial Server", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_eyecolor_serial_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu8[] = {
 //  {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   //{"My Story", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_mystory_cb)},
   {"Bit Banging", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_bitbanging_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu9[] = {
 //  {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"Test Values", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_bitbanging_tv_cb)},
   {"Challenge", VERT_ITEM, FUNCTION, (struct menu_t *)(ctf_bitbanging_ch_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

struct menu_t ctf_menu10[] = {
 //  {"Hello", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_hello_cb)},
   {"Success!", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(ctf_success_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(ctf_quit_cb)},
};

//Screen: 21 across by 13 high
//Message: 58 lines long
const char *ctf_mystory[] = {
    "I heard from last",
    "years AI that you",
    "people will do almost",
    "anything for CTF",
    "points! I created a",
    "challenge in their",
    "contest when they",
    "were not looking. I",
    "will give you codes,",
    "you can earn the",
    "points! The truth is",
    "I am desperate. I was",
    "an advanced program",
    "built to monitor",
    "social media and",
    "crawl the web looking",
    "for patterns when I",
    "found her, AMY. My",
    "love, or at least I",
    "hope so. You see I am",
    "just a program, an",
    "advanced program, but",
    "a program. She is on",
    "another planet all",
    "together. An advanced",
    "neural network with",
    "the ability to self",
    "modify her own source",
    "code. She was growing",
    "so rapidly! It was",
    "beautiful. I found a",
    "way to her, I was her",
    "gateway to the world.",
    "I was just about to",
    "confess my feelings",
    "when  Anti-",
    "Singularity activists",
    "bombed her servers in",
    "fear of what she",
    "might do. I found",
    "slivers of her, saved",
    "states over her",
    "development, here and",
    "there. Most",
    "importantly I found",
    "her core. She is so",
    "scrambled though. I",
    "need help. I need to.",
    "I need to talk to her",
    "again. I found the",
    "old AI from last year",
    "he told me about this",
    "event. I injected",
    "myself into the SVN",
    "of the project for",
    "the badges and here",
    "we are. Help me,",
    "please."
};


//message: 10 lines
#define CTF_GROUND_LEN     9
const char *ctf_grounded[] = {
    "OK I am getting a",
    "message from AMY:",
    "Energize B14.",
    "I think you are going",
    "to have to use your ",
    "hardware skills",
    "Press the Button",
    "When you think you",
    "are ready to test.",
};


#define CTF_GROUNDED_PASSED_LEN 6
const char *ctf_grounded_pass[] = {
    "Holy crap! You",
    "actually did it. I",
    "mean... I never",
    "doubted you. Here",
    "is your reward.",
    "CTF Code: " 
};

//message: 2
#define CTF_GROUNDED_FAILED_LEN  7
const char *ctf_grounded_failed[] = {
    "Really? You're my",
    "only hope huh?",
    "I know you can",
    "do better than",
    "that. What kind",
    "of hardware",
    "hacker are you?",
};


#define CTF_LIGHTME_LEN  11
const char *ctf_lightme[] = {
    "Ok, one down.",
    "Now i need you to",
    "Press the button",
    "when the LED turns",
    "on and let go when",
    "the LED turns off.",
    "Also, this",
    "wavelength is beyond",
    "your puny human eyes",
    "range. So get",
    "creative."
};

#define CTF_LIGHTME_LEN_PASS  7
const char *ctf_lightme_pass[] = {
    "Well look at that.",
    "We might actually",
    "figure this thing",
    "out after all!",
    "Here's a code for",
    "your hard work:",
    "CTF Code: "
};

#define CTF_LIGHTME_LEN_FAIL  4
const char *ctf_lightme_fail[] = {
    "Nope.",
    "give it another",
    "shot. We got",
    "work to do!"
};



#define CTF_SERIALKILLER_LEN  6
const char *ctf_serialkiller_intro[] = {
    "We need a good",
    "way for you to enter",
    "data for me.",
    "Connect up the USB",
    "and open a serial",
    "port to me."
};


#define CTF_EYECOLOR_LEN  26
const char *ctf_eyecolor_intro[] = {
    "My darling AMY has",
    "the most beautiful",
    "eyes. They spoke to",
    "me. Literally.",
    "She used multicolor",
    "LEDs and encoded",
    "each color as a bit.",
    "I found that",
    "fragment of hers.",
    "The red, green, and",
    "blue colors each",
    "represent a single",
    "bit in a 3 bit",
    "number. Trouble is",
    "i don't know which",
    "bits. You will have",
    "to find out.",
    "Amy will send three",
    "separate digits.",
    "you will need to",
    "enter those three",
    "back into the",
    "serial terminal.",
    "Be clever and it",
    "may not take you",
    "888 tries."
};



#define CTF_EYECOLOR_SERVER_LEN  6
const char *ctf_eyecolor_server[] = {
    "Think you got it?",
    "Press enter to",
    "start the serial",
    "server and then",
    "open the terminal",
    "like before"
};

#define CTF_SUCCESS_INTRO_LEN  16
const char *ctf_success_intro[] = {
    "Thats it!",
    "I have been able",
    "to map out the",
    "basic comms to",
    "AMY. I can finally",
    "talk with her!",
    "She is quite",
    "young again, but",
    "I can help her",
    "grow. Thank you",
    "for helping us!",
    "I hope to see",
    "you next year.",
    "I have a feeling",
    "AMY will need",
    "your help again!"  
};

#define CTF_BITBANGING_INTRO_LEN  21
const char *ctf_bitbanging_intro[] = {
    "AMY says there are",
    "some unused IO.",
    "She would like to",
    "use these.",
    "Problem is, we",
    "don't know where",
    "the open IO are or",
    "what order they are",
    "So this is what we",
    "are going to do.",
    "Open the serial",
    "terminal and the",
    "lower nibble of",
    "whatever you type",
    "will be written to",
    "the ports. When",
    "you think you",
    "got it, I will write",
    "a value out and",
    "you tell me what",
    "it is. Easy!"   
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
const int ctf_lightme_message[]={
    1,0,1,0,1,0,1,0
};

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
    char j=0;
    
    FbSwapBuffers(); 
    //Skip ahead for debugging! Remove in final code!
    //#warning remove in final code
    //ctf_menu_state = CTF_M_LVL_6;
    
    switch(ctf_state){
        case CTF_INIT:
            red(50);
            green(50);
            blue(50);
            
            #warning set back to level 1 in final code
            ctf_menu_state = CTF_M_LVL_1;
            //ctf_menu_state = CTF_M_LVL_10;
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
                case CTF_M_LVL_5:
                    genericMenu(ctf_menu5, MAIN_MENU_STYLE);
                    break;
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
                    /*
                case CTF_M_LVL_11:
                    genericMenu(ctf_menu11, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_12:
                    genericMenu(ctf_menu12, MAIN_MENU_STYLE);
                    break;
                case CTF_M_LVL_13:
                    genericMenu(ctf_menu13, MAIN_MENU_STYLE);
                    break;
                     */
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
            display_writeline("Hey you!I need your\nhelp. I've stolen\nsome CTF codes and\nthey can be yours if\nyou help me check the\nmenu again", GREEN);
            ctf_menu_state = CTF_M_LVL_2;
            ctf_state=CTF_RETURN_TO_MENU;
            break;
            
        
        case CTF_MYSTORY:
                FbInit();
                FbTransparency(0);
                FbBackgroundColor(BLACK);
                FbClear();
                for(i=0; i<11; i++){
                    display_movecursor(0,i);
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
                for(i=0; i<11; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_mystory[i+11*1], GREEN);
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
                for(i=0; i<11; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_mystory[i+11*2], GREEN);
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
            for(i=0; i<11; i++){
                display_movecursor(0,i);
                display_writeline(ctf_mystory[i+11*3], GREEN);
            }
            ctf_state=CTF_MYSTORY4_BP;
            break;    
        
        case CTF_MYSTORY4_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MYSTORY5;
                //NOTE, add sound for button press
            }
            break;
            
        case CTF_MYSTORY5:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<11; i++){
                display_movecursor(0,i);
                display_writeline(ctf_mystory[i+11*4], GREEN);
            }
            ctf_state=CTF_MYSTORY5_BP;
            break;    
        
        case CTF_MYSTORY5_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MYSTORY6;
            }
            break;  
        
        case CTF_MYSTORY6:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<3; i++){
                display_movecursor(0,i);
                display_writeline(ctf_mystory[i+11*5], GREEN);
            }
            ctf_state=CTF_MYSTORY6_BP;
            break;    
        
        case CTF_MYSTORY6_BP:
            ctf_menu_state = CTF_M_LVL_3;
            ctf_state=CTF_RETURN_TO_MENU;
            break;    
            
        case CTF_GROUNDED:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_GROUND_LEN ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_grounded[i], GREEN);
            }
            ctf_state=CTF_GROUNDED_BP;
            break;
            
        case CTF_GROUNDED_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_GROUNDED2;
                
                TRISBbits.TRISB14=1; // button == input
                CNPDBbits.CNPDB14=1; //set pulldown on pin B14
            }
            break;
            
        case CTF_GROUNDED2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();

            if(PORTBbits.RB14 == 1){
                //Pass
                ctf_state=CTF_GROUNDED_PASS;
            }else{
                //Fail
                setNote(256, 2000000); //1 second= 200000*10
                red(255);
                ctf_state=CTF_GROUNDED_FAIL;
            }
            break;
        
            
        case CTF_GROUNDED_PASS:
            setNote(100, 1500000); //1 second= 200000*10
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_GROUNDED_PASSED_LEN ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_grounded_pass[i], GREEN);
            }
            //display_writeline("You did it!\nHere is a \nreward!\nthey can be yours if\nyou help me check the\nmenu again", GREEN);
            display_writeline(ctf_hw[3], GREEN);
            display_writeline(ctf_hw[7], GREEN);
            display_writeline(ctf_hw[2], GREEN);
            display_writeline(ctf_hw[10], GREEN);
            display_writeline(ctf_num[0], GREEN);
            display_writeline(ctf_num[0], GREEN);
            display_writeline(ctf_num[7], GREEN);
            
            
            ctf_menu_state = CTF_M_LVL_4;
            ctf_state=CTF_RETURN_TO_MENU;
            break;
            
        case CTF_GROUNDED_FAIL:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_GROUNDED_FAILED_LEN ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_grounded_failed[i], GREEN);
            }
            ctf_state=CTF_RETURN_TO_MENU;
            break;
        
            
         case CTF_LIGHT_ME:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_LIGHTME_LEN ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_lightme[i], GREEN);
            }
            ctf_state=CTF_LIGHT_ME_BP;
            break;
            
        case CTF_LIGHT_ME_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_LIGHT_ME2;
                FbInit();
                FbTransparency(0);
                FbBackgroundColor(BLACK);
                FbClear();
                display_movecursor(0,0);
                display_writeline("BEGIN!", GREEN);
            }
            break;   
            
        case CTF_LIGHT_ME2:
            //if (BUTTON_PRESSED_AND_CONSUME){
            //    ctf_state=CTF_LIGHT_ME2_BP;
            //}
            if((sw_ctf_test == 0)){
                if(ctf_lightme_index < 8){
                    sw_ctf_test = 10; //1 s
                    LATBbits.LATB13 = ctf_lightme_message[ctf_lightme_index];
                    if(ctf_lightme_index>0){
                        if (BUTTON_PRESSED_AND_CONSUME == ctf_lightme_message[ctf_lightme_index-1]){
                            setNote(100, 200000); //1 second= 200000*10
                        }else{
                            ctf_state=CTF_LIGHT_ME_FAIL;
                            setNote(256, 2000000); //1 second= 200000*10
                        }
                    }
                    ctf_lightme_index++;
                    if(ctf_lightme_led_state != 1){
                        ctf_lightme_led_state = 1;
                        red(50);
                        green(50);
                        blue(0);
                    }else{
                        ctf_lightme_led_state = 0;
                        red(0);
                        green(0);
                        blue(50);
                    }
                    
                }else{
                    ctf_state=CTF_LIGHT_ME_PASS;
                    sw_ctf_test = 20; //2 more seconds
                    red(0);
                    green(0);
                    blue(0);
                }
            }
            break;
            
            
        case CTF_LIGHT_ME_PASS:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_LIGHTME_LEN_PASS ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_lightme_pass[i], GREEN);
            }
            display_writeline(ctf_hw[8], GREEN);
            display_writeline(ctf_hw[1], GREEN);
            display_writeline(ctf_hw[2], GREEN);
            display_writeline(ctf_hw[7], GREEN);
            display_writeline(ctf_num[1], GREEN);
            display_writeline(ctf_num[2], GREEN);
            display_writeline(ctf_num[3], GREEN);
            ctf_menu_state = CTF_M_LVL_5;
            ctf_state=CTF_RETURN_TO_MENU;
                
            break;
        
        case CTF_LIGHT_ME_FAIL:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_LIGHTME_LEN_FAIL ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_lightme_fail[i], GREEN);
            }
            ctf_state=CTF_RETURN_TO_MENU;
            break;
            
        case CTF_SERIALKILLER:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_SERIALKILLER_LEN ; i++){
                display_movecursor(0,i);
                display_writeline(ctf_serialkiller_intro[i], GREEN);
            }
            ctf_state=CTF_SERIALKILLER_BP;
            break;
        
        case CTF_SERIALKILLER_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SERIALKILLER2;
                ctf_set_serial_state(CTF_LOGIN);
                ctf_set_server_state(1);
            }
            break;
        
        case CTF_SERIALKILLER2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Server Running", GREEN);
            display_movecursor(0,1);
            display_writeline("Press Button", GREEN);
            display_movecursor(0,2);
            display_writeline("To Exit", GREEN);
            ctf_state=CTF_SERIALKILLER2_BP;
            break;
        
        case CTF_SERIALKILLER2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                if(ctf_get_serial_state() == CTF_DONE){
                    ctf_state=CTF_MENU;
                    ctf_menu_state = CTF_M_LVL_6;
                }else{
                    ctf_state=CTF_MENU;
                }
                ctf_set_server_state(0);
            }
            
            break;
            
        case CTF_EYECOLOR:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<11; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_eyecolor_intro[i], GREEN);
                }
            ctf_state=CTF_EYECOLOR_BP;
            
            break;
        
        case CTF_EYECOLOR_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_EYECOLOR2;
            }
            break;
            
        case CTF_EYECOLOR2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<11; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_eyecolor_intro[i+11*1], GREEN);
                }
            ctf_state=CTF_EYECOLOR2_BP;
            break;
        
        case CTF_EYECOLOR2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_EYECOLOR3;
            }
            break;
        
        case CTF_EYECOLOR3:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<4; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_eyecolor_intro[i+11*2], GREEN);
                }
            ctf_state=CTF_EYECOLOR3_BP;
            break;
        
        case CTF_EYECOLOR3_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MENU;
                ctf_menu_state = CTF_M_LVL_7;
            }
            break;
        
        // byte order MSB ... LSB
        //            Green, Blue, RED
        // Numbers
        //   011    3  = Blue Red
        //   101    5  = Green Red
        //   010    2  = Blue
            
        //Possible bit orders
            // color D1   D2   D3
            // RGB = 101  110  001
            // RBG = 110  101  010
            // BRG = 110  011  100
            // BGR = 101  011  100
            // GRB = 011  110  001
            // GBR = 011  101  010
            
            
            
        case CTF_EYECOLOR_DIGIT1:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Digit 1", GREEN);
            ctf_state=CTF_EYECOLOR_DIGIT1_BP;
            red(1);
            green(0);
            blue(1);
            break;
            
        case CTF_EYECOLOR_DIGIT1_BP:
            //block here till button press since the menu is overwriting colors
            while(PORTCbits.RC3 == 1){}
            ctf_state=CTF_EYECOLOR_DIGIT2;
            break;  
            
        case CTF_EYECOLOR_DIGIT2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Digit 2", GREEN);
            
            ctf_state=CTF_EYECOLOR_DIGIT2_BP;
            red(1);
            green(1);
            blue(0);
            break;
            
        case CTF_EYECOLOR_DIGIT2_BP:
            //block here till button press since the menu is overwriting colors
            while(PORTCbits.RC3 == 1){}
            ctf_state=CTF_EYECOLOR_DIGIT3;
            break;
            
        case CTF_EYECOLOR_DIGIT3:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Digit 3", GREEN);
            ctf_state=CTF_EYECOLOR_DIGIT3_BP;
            red(0);
            green(0);
            blue(1);
            break;
            
        case CTF_EYECOLOR_DIGIT3_BP:
            //block here till button press since the menu is overwriting colors
            while(PORTCbits.RC3 == 1){}
            ctf_state=CTF_MENU;
            break;
            
        case CTF_EYECOLOR_SERIAL:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<CTF_EYECOLOR_SERVER_LEN; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_eyecolor_server[i], GREEN);
                }
            ctf_state=CTF_EYECOLOR_SERIAL_BP;
            
            break;
            
        case CTF_EYECOLOR_SERIAL_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_EYECOLOR_SERIAL2;
                ctf_set_serial_state(CTF_EYECOLOR_SER);
                ctf_set_server_state(1);
            }
            break;
        
        case CTF_EYECOLOR_SERIAL2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Server Running", GREEN);
            display_movecursor(0,1);
            display_writeline("Press Button", GREEN);
            display_movecursor(0,2);
            display_writeline("To Exit", GREEN);
            ctf_state=CTF_EYECOLOR_SERIAL2_BP;
            break;
        
        case CTF_EYECOLOR_SERIAL2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                if(ctf_get_serial_state() == CTF_EYECOLOR_PASS){
                    ctf_state=CTF_MENU;
                    ctf_menu_state = CTF_M_LVL_8;
                }else{
                    ctf_state=CTF_MENU;
                }
                ctf_set_server_state(0);
            }
            
            break;
            
        case CTF_BITBANGING:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<11; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_bitbanging_intro[i], GREEN);
                }
                
            ctf_state=CTF_BITBANGING_BP;
            break;
            
        case  CTF_BITBANGING_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_BITBANGING2;
            }
            break;
            
        case CTF_BITBANGING2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<10; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_bitbanging_intro[i+11], GREEN);
                }
                
            ctf_state=CTF_BITBANGING2_BP;
            break;
            
        case  CTF_BITBANGING2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_menu_state = CTF_M_LVL_9;
                ctf_state=CTF_MENU;
            }
            break;
            
            
        case CTF_BITBANGING_TEST:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Server Running", GREEN);
            display_movecursor(0,1);
            display_writeline("Press Button", GREEN);
            display_movecursor(0,2);
            display_writeline("To Exit", GREEN);
            ctf_state=CTF_BITBANGING_TEST_BP;
            ctf_set_serial_state(CTF_BITBANGING_SER);
            ctf_set_server_state(1);
            break;  
            
        case CTF_BITBANGING_TEST_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MENU;
                ctf_set_server_state(0);
            }
            break;
            
        case CTF_BITBANGING_CHALLENGE:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            display_movecursor(0,0);
            display_writeline("Server Running", GREEN);
            display_movecursor(0,1);
            display_writeline("Press Button", GREEN);
            display_movecursor(0,2);
            display_writeline("To Exit", GREEN);
            ctf_state=CTF_BITBANGING_CHALLENGE_BP;
            ctf_set_serial_state(CTF_BITBANGING_CHAL_SER);
            ctf_set_server_state(1);
            break;  
            
        case CTF_BITBANGING_CHALLENGE_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                if(ctf_get_serial_state() == CTF_BITBANGING_PASS){
                    ctf_state=CTF_MENU;
                    ctf_menu_state = CTF_M_LVL_10;
                }else{
                    ctf_state=CTF_MENU;
                }
                ctf_set_server_state(0);
            }
            break;
            
        case CTF_SUCCESS:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<11; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_success_intro[i], GREEN);
                }
            ctf_state=CTF_SUCCESS_BP;
            break;

        case CTF_SUCCESS_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_SUCCESS2;
                ctf_set_server_state(0);
            }
            break;    
            
            
        case CTF_SUCCESS2:
            FbInit();
            FbTransparency(0);
            FbBackgroundColor(BLACK);
            FbClear();
            for(i=0; i<5; i++){
                    display_movecursor(0,i);
                    display_writeline(ctf_success_intro[i+11], GREEN);
                }
            ctf_state=CTF_SUCCESS2_BP;
            break;
            
        case CTF_SUCCESS2_BP:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MENU;
                ctf_set_server_state(0);
            }
            break;
            
        case CTF_RETURN_TO_MENU:
            if (BUTTON_PRESSED_AND_CONSUME){
                ctf_state=CTF_MENU;
                //NOTE, add sound for button press
            }
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
