#include "fb.h"
#include "colors.h"
#include "touchCTMU.h"
#include "menu.h"
#include "pong.h"
#include "flash.h"
#include "ir.h"
#include "assets.h"
#include "S6B33.h"
#include "TimeDelay.h"
#include <stdint.h>
#include "../prngs.h"
#include "screensavers.h"

#define SCREEN_SIZE 131
#define PONG_PADDLE_SPEED 4
#define PONG_BALL_SPEED 6

/***********************************************
 *        PONG - Single and Multiplayer
 ***********************************************/

typedef enum {
    OFF,
    INIT_LFG,
    LFG,
    FOUND_LFG,
    INIT_HOST,
    WAIT_HOST,
    FOUND_PLAYER,
    PLAYING,
} searchstate;

typedef enum {
    INIT,
    DRAW_MENU,
    PONG_MENU,
    PONG_GAME,
    SCORE,
    WAIT,
} gamestate;

enum pong_packet_id {
    SEARCH = 0,
    START = 1,
    BOUNCE = 2,
    WIN = 3,     
};

typedef struct __attribute__((packed)){
    unsigned pid:2;
    unsigned ack:1;
    
    unsigned data:13;
} packet_pong;
typedef struct {
    gamestate state;
    short paddleX;
    short ballX;
    short ballY;
    short ballXVel;
    short ballYVel;
    int paddleHit;
    int highScore;
    unsigned ballHeld;
    
} pong_runtime_data;



void pong_select_sp() {
    pong_game_menu_loop(0);
}
void pong_select_mp() {
    pong_game_menu_loop(1);
}


union IRpacket_u searchPacket;
searchstate pong_mp_state = OFF;
pong_runtime_data p_data = {
        .state = INIT,
        .paddleX = 40,
        .ballX = 40,
        .ballY = 110,
        .ballXVel = 3,
        .paddleHit = 0,
        .ballYVel = 2,
        .highScore = 0,
        .ballHeld = 1,
    };

unsigned short pongpeerId;

void pong_mp_search() {
    switch(pong_mp_state) {
        case OFF:
            pong_mp_state = INIT_LFG;
            break;
        case INIT_LFG: {
            searchPacket.p.badgeId = G_sysData.badgeId;
            searchPacket.p.address = IR_PONG;
            searchPacket.p.command = IR_WRITE;
            packet_pong p;
            p.ack = 0;
            p.pid = 0;
            searchPacket.p.data = *(uint16_t*)(&p);
            pong_mp_state = LFG;
        }
            break;
        case LFG:
            IRqueueSend(searchPacket);
            FbSwapBuffers();
            //SLOW DOWN!!!
            
            break;
        case FOUND_LFG: {
            union IRpacket_u startpacket;
            startpacket.p.address = IR_PONG;
            startpacket.p.badgeId = G_sysData.badgeId;
            startpacket.p.command = IR_WRITE;
            packet_pong p;
            p.ack = 0;
            p.pid = START;
            startpacket.p.data = *(uint16_t*)(&p);
            
            }
            FbSwapBuffers();
            break;
        case PLAYING:
        {
            pong_select_mp();
        }
        break;
        default:
            return;
    }
}

void pong_mp_wait() {
    switch(pong_mp_state) {
        case OFF:
            pong_mp_state = INIT_HOST;
            break;
        case INIT_HOST:
            break;
        case WAIT_HOST:
            break;
        case FOUND_PLAYER:
            
            pong_select_mp();
            
            pong_mp_state = PLAYING;
            break;
        case PLAYING:
            pong_select_mp();
            break;
    }
}

const unsigned char pong_text_yourscore[] = "YOUR HIGHSCORE";


int pong_highscore_screen_state = 0; 
void pong_highscore() {
    switch(pong_highscore_screen_state) {
        //Let the menu code clear the screen for us
        case 0:
            p_data.highScore = (p_data.paddleHit > p_data.highScore) ? 
                p_data.paddleHit : p_data.highScore;
            
            setHighScore(SCORE_PONG,p_data.highScore,0);
            
            FbClear();
            FbMove(10,10);
            FbColor(GREEN);
            FbWriteLine(pong_text_yourscore);
            FbMove(50,22);
            unsigned char tmp_str[] = {
                '0' + (p_data.highScore/100) % 10,
                '0' + (p_data.highScore/10) % 10,
                '0' + (p_data.highScore) % 10,
                    0};
            FbColor(WHITE);
            FbWriteLine(tmp_str);
            
            FbSwapBuffers();
            pong_highscore_screen_state++;
            break;
        case 1:
            if(BUTTON_PRESSED_AND_CONSUME) {
                pong_highscore_screen_state = 0;
                returnToMenus();
            }
            
    }

    
}


const struct menu_t pong_multiplayer_menu[] = {
    {"Join Game", DEFAULT_ITEM|VERT_ITEM,FUNCTION, {(void *)pong_mp_search}},
    {"Host Game", DEFAULT_ITEM|VERT_ITEM,FUNCTION, {(void *)pong_mp_wait}},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"Back",VERT_ITEM|LAST_ITEM, BACK, 0},
};

const struct menu_t pong_menu[] = {
    {"Single Player", DEFAULT_ITEM|VERT_ITEM, FUNCTION, {(void *)pong_select_sp}},
    //{"Multi Player", VERT_ITEM, MENU, {pong_multiplayer_menu}},
    {"High Scores", VERT_ITEM, FUNCTION, {(void *)pong_highscore}},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"Back",VERT_ITEM|LAST_ITEM, BACK, 0},
};






typedef struct {
    unsigned mode:1; //1 for ACK 0 for QUERY
    unsigned char startx;
} packet_pong_handsake;


union packet_data{
    
};

//PID 0: Search beacon
ir_pong(struct IRpacket_t p) {
    switch(pong_mp_state) {
        case LFG: {
            uint16_t sillycompilier = p.data;
            packet_pong pong_p = *(packet_pong*)(&sillycompilier);
            if(p.badgeId == G_sysData.badgeId) { return; }
            
            if(pong_p.pid != 0) { return; }
            
            if(pong_p.ack != 1) { return; }
            
            pongpeerId = p.badgeId;
            
            pong_mp_state = FOUND_LFG;
            }
            break;
        case WAIT_HOST: {
            uint16_t sillycompilier = p.data;
            packet_pong pong_p = *(packet_pong*)(&sillycompilier);
            if(p.badgeId == G_sysData.badgeId) { return; }
            
            
            
            if(pong_p.pid == START) {
                pong_mp_state = FOUND_PLAYER;
                p_data.state = INIT;
                return;
            } else if(pong_p.pid != 0) {
                return;
            }
            
            if(pong_p.ack != 0) { return; }
            
            pongpeerId = p.badgeId;
            pong_p.ack = 1;
            p.badgeId = G_sysData.badgeId;
            p.command = IR_WRITE;
            union IRpacket_u p_u;
            p_u.p = p;
            IRqueueSend(p_u);
            }
            break;
        case FOUND_LFG:
            FbClear();
            p_data.state = MENU;
            p_data.ballY = 0;
            p_data.ballYVel = 2;
            p_data.ballXVel = 3;
            p_data.paddleHit = 0;
            p_data.ballHeld = 0;
        case PLAYING:
        {
            pong_mp_state = PLAYING;
            uint16_t sillycompilier = p.data;
            packet_pong pong_p = *(packet_pong*)(&sillycompilier);
            if(p.badgeId != pongpeerId) { return; }
            
            if(pong_p.pid != BOUNCE) { return; }
            
            if(pong_p.ack != 0) { return; }
            
            if(p_data.ballY > 0) { return; }
            
            p_data.ballX = (unsigned char)pong_p.data;
            
            p_data.ballY = 0;
            if(p_data.ballYVel < 0) {
                p_data.ballYVel *= -1;
            }
            
        }
        break;
        default:
            return;
    }
}


/* Use our own draw function for performance */
void drawRectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned short color) {
    S6B33_rect(y, x, height, width);
    unsigned short i = 0;
    for(i = 0; i <= width * height; i++) 
        S6B33_pixel(color);
}


void sendTransferEvent() {
   union IRpacket_u bouncepacket;
   bouncepacket.p.address = IR_PONG;
   bouncepacket.p.badgeId = G_sysData.badgeId;
   bouncepacket.p.command = IR_WRITE;
   packet_pong p;
   p.ack = 0;
   p.pid = START;
   p.data = p_data.ballX;
   bouncepacket.p.data = *(uint16_t*)(&p);
   IRqueueSend(bouncepacket);
}


void pong_game_menu_loop(int multiplayer) {
    
    switch(p_data.state) {
        case INIT:
            FbClear();
            p_data.state = PONG_GAME;
            p_data.ballY = 115;
            p_data.ballYVel = -2;
            p_data.ballXVel = 3;
            p_data.paddleHit = 0;
            p_data.ballHeld = 1;
            break;
        case PONG_GAME:
            if(BUTTON_PRESSED_AND_CONSUME){
                if(p_data.ballHeld) {
                    p_data.ballHeld = 0;
                } else {
                    p_data.highScore = (p_data.paddleHit > p_data.highScore) ? 
                        p_data.paddleHit : p_data.highScore;
            
                    setHighScore(SCORE_PONG,p_data.highScore,0);
                    returnToMenus();
                    return;
                }
            }
            if(TOP_TAP_AND_CONSUME || BOTTOM_TAP_AND_CONSUME) {
                if(p_data.ballHeld) {
                    p_data.ballHeld = 0;
                }
            }
            //If the ball is behind the paddle exit for now
            if((p_data.ballY) > SCREEN_SIZE -5) {
                p_data.state = INIT;
                returnToMenus();
                return;
            }
            
            
            //We want to remove the ball from the screen
            //drawRectangle((p_data.ballX >> 8),(p_data.ballY >> 8) ,2,2,G_Fb.BGcolor);
            
            //If the ball hits the screen edges other than the top: bounce
            if((p_data.ballX) < 0 && p_data.ballXVel < 0)
                p_data.ballXVel = -1 * p_data.ballXVel;
            else if ((p_data.ballX) > SCREEN_SIZE-2 && p_data.ballXVel > 0)
                p_data.ballXVel = -1 * p_data.ballXVel;
            
            if((p_data.ballY) < 2 && p_data.ballYVel < 0) {
                if(multiplayer) {
                    p_data.ballY = -4;
                    
                } else {
                    p_data.ballYVel = -1 * p_data.ballYVel;
                }
            }
            
            //Update ball position
            //TODO: Insert frame counting and finer speed control
            
            
            if(p_data.ballHeld) {
                p_data.ballX = p_data.paddleX + 10;
            } else {
                p_data.ballX += p_data.ballXVel;
                p_data.ballY += p_data.ballYVel;
            }
            
            //Get direction to move the paddle from the bottom slider
            int delta = 0;
            unsigned char button_press = getHorizontalPosition();
            if(button_press > 0 && button_press < 30) {
                delta = -5;
            } else if (button_press > 70) {
                delta = 5;
            }
            
            
            //If we moved update paddle location
            if(delta != 0) {
                //drawRectangle((p_data.paddleX),10,20,5,G_Fb.BGcolor);
                if((p_data.paddleX) + delta < 0) {
                    p_data.paddleX = 0;
                } else if ((p_data.paddleX) + delta + 20 > SCREEN_SIZE-1) {
                    p_data.paddleX = (SCREEN_SIZE-20-1);
                } else {
                    p_data.paddleX += delta;
                }
            }
            
            //Check to see if we have a hit!
            if((p_data.ballY) > SCREEN_SIZE - 15 &&
                    (p_data.ballX)+2 > (p_data.paddleX) &&
                    (p_data.ballX) < (p_data.paddleX ) + 20
                     && !p_data.ballHeld)
            {
                //Bounce the ball
                if(p_data.ballYVel > 0)
                    p_data.ballYVel *= -1;
                
                //Keep score!
                p_data.paddleHit += 1;
                
                //Every other paddle hit accelerate X bounce
                if(p_data.paddleHit % 2 > 0) {
                    p_data.ballXVel = (p_data.ballXVel > 0) ? p_data.ballXVel +1
                            : p_data.ballXVel -1;
                }
                
                //Random bounce
                if(20 > get_rand_char(0,100)) {
                    p_data.ballXVel *= -1;
                }
                
                //Play a note, the freq argument to setNote is inverted
                setNote(-200/p_data.ballYVel,4048);
                
                //Make the game more difficult!
                p_data.ballYVel -= 1;
            }
            //drawRectangle((p_data.paddleX >> 8),10,20,5,BLUE);
            

            //Draw the ball
            //drawRectangle((p_data.ballX >> 8),(p_data.ballY >> 8),2,2,WHITE);
            //Delay1us(10);
             
            FbMove(p_data.paddleX, SCREEN_SIZE-10 );
            FbColor(WHITE);
            FbFilledRectangle(20,5);
            FbColor(WHITE);
            FbMove(p_data.ballX,p_data.ballY);
            FbFilledRectangle(2,2);
            FbSwapBuffers();
            break;

            
    }
    
    
    
}