/* 
 * File:   draw.h
 * Author: jonathan46000.com
 *
 * Created on May 12, 2016, 12:08 PM
 */

#include "badge16.h"
#include "colors.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "draw.h"
#include "fb.h"
#include "assetList.h"

#define IR_DELAY 11000

#define CLICK_TIME_OUT  12000
#define DRAW_WHITE      0b00000000
#define DRAW_BLACK      0b00000001
#define DRAW_CYAN       0b00000010
#define DRAW_RED        0b00000011    
#define DRAW_BLUE       0b00000100
#define DRAW_GREEN      0b00000101
#define DRAW_YELLOW     0b00000110
#define DRAW_MAGENTA    0b00000111

//WHITE, BLACK, CYAN, RED, BLUE, GREEN, YELLOW, MAGENTA, 

struct menu_t draw_config_m[] = {
    {"Draw", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)draw_continue}},
    {"Erase", VERT_ITEM, FUNCTION, {(struct menu_t *)draw_init}},
    {"Color", VERT_ITEM, FUNCTION, {(struct menu_t *)draw_set_color}},
    {"Send Pic", VERT_ITEM, FUNCTION, {(struct menu_t *)draw_send}},
    {"Get Pic", VERT_ITEM, FUNCTION, {(struct menu_t *)draw_receive}},
    {"Exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)set_draw_exit}}
};


enum
{
    DRAW_INIT,
    DRAW_SHOW_INST,
    DRAW_SHOW_MENU,
    DRAW_GET_INPUT,
    DRAW_PROCESS_INPUT,
    DRAW_RENDER,
    DRAW_EXIT,
    DRAW_SELECT_COLOR,
    DRAW_SEND_MESSAGE,
    DRAW_GET_MESSAGE
};

enum
{
    DRAW_OFF,
    DRAW_ON
};

//enum
//{
//    IR=1,
//    LOCAL=2
//};
#define IR 1
#define LOCAL 2


static unsigned char drawn=0;
unsigned char draw_state = DRAW_INIT;//app state
unsigned char draw_image[16][32] = {0};
unsigned char draw_ir_image[16][32] = {0};
unsigned char draw_x = 16;
unsigned char draw_y = 16;
unsigned char draw_toggle = DRAW_OFF;
unsigned char draw_color = DRAW_BLACK;
unsigned char draw_moved = 0;
unsigned char draw_selected_color = DRAW_WHITE;
unsigned char draw_new_message = 1;


void draw_set_color(){
    draw_state = DRAW_SELECT_COLOR;
}


unsigned char draw_send_message(){
    static unsigned char draw_j = 0;
    static unsigned char draw_k = 0;
    static unsigned short send_delay = 0;
    static unsigned char first_row = 0;
    static unsigned char last_row = 0;


    if(draw_j == 0 && draw_k == 0){
        for(draw_k=0;draw_k<32;draw_k++){
            for(draw_j=0;draw_j<16;draw_j++){     
                if(first_row == 0 && draw_image[draw_j][draw_k]!=DRAW_WHITE){       
                        first_row = draw_k;                    
                }
            }
        }
        for(draw_k=31;draw_k > 0;draw_k--){
            for(draw_j=0;draw_j<16;draw_j++){     
                if(last_row == 0 && draw_image[draw_j][draw_k]!=DRAW_WHITE){       
                        last_row = draw_k;                    
                }
            }
        }
        draw_j = 0;
        draw_k = first_row;
    }
    
    
    if(draw_k <= last_row){
        
        if(send_delay == IR_DELAY){
            send_delay = 0;
            union IRpacket_u pkt;
            pkt.p.command = IR_WRITE;
            pkt.p.address = IR_DRAW_APP;
            pkt.p.badgeId = 0;
            pkt.p.badgeId = 0b111000000;
            pkt.p.badgeId |= (0b000011111 & first_row);
       
            pkt.p.data |= draw_image[draw_j][draw_k];
            pkt.p.data |= (draw_image[draw_j+1][draw_k] << 8);
            if(draw_j == 14){
                draw_j = 0;
                draw_k ++;
                if(draw_k == 31){
                    pkt.p.badgeId |= 0b000100000;
                }
            }
            else{
                draw_j += 2;
            }
            
            IRqueueSend(pkt);
            // Debug chirp
            setNote(75, 20);
            
        }
        else{
            send_delay++;
        }
    }
    else{
        draw_j = 0;
        draw_k = 0;
        first_row = 0;
        last_row = 0;
        return 0;
    }        
    // Return 1 to indicate more to send (recall)
    return 1;
}

void draw_select_color(){
    if(TOP_TAP_AND_CONSUME){
        setNote(154, 2048);
        if(draw_selected_color != DRAW_WHITE){
            draw_selected_color--;
        }
    }
    
    if(BOTTOM_TAP_AND_CONSUME){
        setNote(154, 2048);
        if(draw_selected_color != DRAW_MAGENTA){
            draw_selected_color++;
        }
    }
    //WHITE, BLACK, CYAN, RED, BLUE, GREEN, YELLOW, MAGENTA,
    FbMove(1,2);
    FbColor(WHITE);
    FbFilledRectangle(130,14);
    FbMove(1,16);
    FbColor(BLACK);
    FbFilledRectangle(130,14);
    FbMove(1,30);
    FbColor(CYAN);
    FbFilledRectangle(130,14);
    FbMove(1,44);
    FbColor(RED);
    FbFilledRectangle(130,14);
    FbMove(1,58);
    FbColor(BLUE);
    FbFilledRectangle(130,14);
    FbMove(1,72);
    FbColor(GREEN);
    FbFilledRectangle(130,14);
    FbMove(1,86);
    FbColor(YELLOW);
    FbFilledRectangle(130,14);
    FbMove(1,100);
    FbColor(MAGENTA);
    FbFilledRectangle(130,14);
    FbMove(1,114);
    
    unsigned char j = draw_selected_color - DRAW_WHITE;
    
    FbColor(MAGENTA);
    FbMove(1,(14*j)+2);
    FbRectangle(130,14);
    FbMove(2,(14*j)+3);
    FbRectangle(128,12);
    FbMove(3,(14*j)+4);
    FbRectangle(126,10);  
    
    if(BUTTON_PRESSED_AND_CONSUME){
        draw_color = draw_selected_color;
        draw_moved = 1;
        draw_state = DRAW_RENDER;
    }
}

void draw_send(){
    draw_state = DRAW_SEND_MESSAGE;
}

void draw_receive(){
    draw_state = DRAW_GET_MESSAGE;
}


struct button_clicks {
    int click_timer;
    char click_value;
};

struct button_clicks draw_button;

void draw_init(){
    FbClear();
    unsigned char j,k;
    for(j=0;j<16;j++){
        for(k=0;k<32;k++){
            draw_image[j][k] = DRAW_WHITE;
            draw_ir_image[j][k] = DRAW_WHITE;
        }
    }
    draw_moved = 1;
    draw_selected_color = DRAW_WHITE;
    draw_color = DRAW_BLACK;
    draw_toggle = DRAW_ON;
    draw_x = 16;
    draw_y = 16;
    draw_button.click_timer = 0;
    draw_button.click_value = 0;
    if(!drawn){
        draw_state++;
    }
    else{
        draw_state = DRAW_RENDER;
    }
}

void draw_inst(){
    if (!drawn) {
		FbInit();
        FbTransparentIndex(0);
		FbBackgroundColor(BLACK);
		FbColor(WHITE);
		FbClear();
		FbMove(10, 10);
		FbWriteLine("press button");
		FbMove(10, 20);
		FbWriteLine("twice for menu");
        FbMove(10, 30);
        FbWriteLine("once to toggle");
        FbMove(10, 40);
        FbWriteLine("drawing!");
		drawn = 1;
	    }
	    if (BUTTON_PRESSED_AND_CONSUME) {
            draw_state++;
            FbBackgroundColor(BLACK);
	    }
}


void set_draw_exit()
{
    draw_state = DRAW_SHOW_MENU;
    drawn = 0;
    returnToMenus();
}

void draw_get_input(){
    char touch_x = getHorizontalPosition();
	char touch_y = getVerticalPosition();  
    if (BUTTON_PRESSED_AND_CONSUME)
    {
        if(draw_button.click_value <2 ){
            draw_button.click_value ++;
        }
        if(draw_button.click_value == 1){
            draw_button.click_timer = 0;
        }
    }
    if(draw_button.click_value > 0){
        draw_button.click_timer++;
    }
    if(touch_y > 65 && draw_y > 0){
        draw_y--;
        draw_moved =1;
    }
    if(touch_y > 8 && touch_y < 65 && draw_y < 32){
        draw_y++;
        draw_moved =1;
    }
    if(touch_x > 8 && touch_x < 65 && draw_x > 0){
        draw_x--;
        draw_moved = 1;
    }
    if(touch_x > 65 && draw_x < 32){
        draw_x++;
        draw_moved = 1;
    }
    draw_state++;
}


void draw_process_input(){
    
    if(draw_button.click_value == 1 && draw_button.click_timer >= (CLICK_TIME_OUT-1)){
        switch(draw_toggle){
            case DRAW_OFF:
                draw_toggle = DRAW_ON;
                break;
            case DRAW_ON:
                draw_toggle = DRAW_OFF;
                break;
        }
        draw_button.click_value = 0;
        draw_button.click_timer = 0;
    }
    
    if(draw_button.click_value >= 1 && draw_button.click_timer >= (CLICK_TIME_OUT-1)){
        draw_state = DRAW_SHOW_MENU;
        draw_button.click_value = 0;
        draw_button.click_timer = 0;
    }
    
    if(draw_toggle == DRAW_ON && draw_moved == 1){
        if((draw_x%2)==0){
            draw_image[draw_x/2][draw_y] &= 0b11110000;
            draw_image[draw_x/2][draw_y] |= draw_color;
        }
        else{
            draw_image[(draw_x+1)/2][draw_y] &= 0b00001111;
            draw_image[(draw_x+1)/2][draw_y] |= (draw_color << 4);
        }
    }
    
    if(draw_state != DRAW_SHOW_MENU){
        draw_state++;
    }
      
}

void _insert_color(unsigned char _color){
    switch(_color){
                    case DRAW_WHITE:
                        FbColor(WHITE);
                        break;
                    case DRAW_BLACK:
                        FbColor(BLACK);
                        break;
                    case DRAW_CYAN:
                        FbColor(CYAN);
                        break;
                    case DRAW_RED:
                        FbColor(RED);
                        break;
                    case DRAW_BLUE:
                        FbColor(BLUE);
                        break;
                    case DRAW_GREEN:
                        FbColor(GREEN);
                        break;
                    case DRAW_YELLOW:
                        FbColor(YELLOW);
                        break;
                    case DRAW_MAGENTA:
                        FbColor(MAGENTA);
                        break;
                    default:
                        FbColor(WHITE);
                        break;
                }
}

void draw_continue(){
    draw_state = DRAW_GET_INPUT;
    FbBackgroundColor(BLACK);
    draw_moved = 1;
}

void draw_render(char ir_local){
    unsigned char j,k;    
    if(draw_moved == 1){
        for(j=0;j<16;j++){
            for(k=0;k<32;k++){
                FbMove(((j*8)-2),((k*4)+2));
                switch(ir_local){
                    case IR:
                        _insert_color(draw_ir_image[j][k] >> 4);
                        break;
                    case LOCAL:
                        _insert_color(draw_image[j][k] >> 4);
                        break;
                }
                FbFilledRectangle(3,3);
                FbMove(((j*8)+2),((k*4)+2));
                switch(ir_local){
                    case IR:
                        _insert_color(draw_ir_image[j][k] & 0b00001111);
                        break;
                    case LOCAL:
                        _insert_color(draw_image[j][k] & 0b00001111);
                        break;
                }
                FbFilledRectangle(3,3);
            }
        } 
        FbColor(YELLOW);
        FbMove((draw_x*4)+1,(draw_y*4)+1);
        FbRectangle(4,4);
        draw_moved = 0;
    }
    draw_state = DRAW_GET_INPUT;
}

extern unsigned char ir_j;
extern unsigned char ir_k;

void draw_cb(){
    FbSwapBuffers();
    switch (draw_state) {      
        case DRAW_INIT:
            draw_init();
            break;        
        case DRAW_SHOW_INST:
            draw_inst();
            break;
        case DRAW_SHOW_MENU:
            genericMenu((struct menu_t *)draw_config_m, MAIN_MENU_STYLE);
            break;
        case DRAW_GET_INPUT:
            draw_get_input();
            break;
        case DRAW_PROCESS_INPUT:
            draw_process_input();
            break;
        case DRAW_RENDER:
            draw_render(LOCAL);
            break;
        case DRAW_EXIT:
            set_draw_exit();
            break;
        case DRAW_SELECT_COLOR:
            draw_select_color();
            break;
        case DRAW_SEND_MESSAGE:
            if(!draw_send_message())
                draw_state = DRAW_SHOW_MENU;
            break;
        case DRAW_GET_MESSAGE:
            draw_moved = 1;
            draw_render(IR);   
            ir_j = 0;
            ir_k = 0;
            draw_image_received = 0;
            
            if(BUTTON_PRESSED_AND_CONSUME){
                draw_state = DRAW_SHOW_MENU;
            }
            else
                draw_state = DRAW_GET_MESSAGE;
            break;
        default:
            draw_state = DRAW_EXIT;
            break;
    }
}
 
