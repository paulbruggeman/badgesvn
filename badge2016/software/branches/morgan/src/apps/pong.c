#include "fb.h"
#include "colors.h"
#include "touchCTMU.h"
#include "menu.h"
#include "pong.h"
#include "assets.h"
#include "S6B33.h"
#include "TimeDelay.h"

#define SCREEN_SIZE 132

/***********************************************
 *        PONG - Single and Multiplayer
 ***********************************************/

void pong_select_sp() {
    
}


const struct menu_t pong_menu[] = {
    {"Single Player", DEFAULT_ITEM|VERT_ITEM, FUNCTION, {(void *)pong_game_menu_loop}},
    {"Multi Player", VERT_ITEM, FUNCTION, {(void *)pong_select_sp}},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"Back",VERT_ITEM|LAST_ITEM, BACK, 0},
};


typedef enum {
    INIT,
    DRAW_MENU,
    PONG_MENU,
    S_PLAYER,
    M_PLAYER,
    SCORE,
} gamestate;

typedef struct {
    gamestate state;
    short paddleX;
    short ballX;
    short ballY;
    short ballXVel;
    short ballYVel;
    int paddleHit;
    int highScore;
    
} pong_runtime_data;


/* Use our own draw function for performance */
void drawRectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned short color) {
    S6B33_rect(y, x, height, width);
    unsigned short i = 0;
    for(i = 0; i <= width * height; i++) 
        S6B33_pixel(color);
}





void pong_game_menu_loop() {
    static pong_runtime_data p_data = {
        .state = INIT,
        .paddleX = 40,
        .ballX = 40,
        .ballY = 120,
        .ballXVel = 3,
        .paddleHit = 0,
        .ballYVel = 2,
        .highScore = 0,
    };
    
    switch(p_data.state) {
        case INIT:
            FbClear();
            p_data.state = MENU;
            p_data.ballY = 120;
            p_data.ballYVel = 2;
            p_data.ballXVel = 3;
            p_data.paddleHit = 0;
            break;
        case DRAW_MENU:
            FbColor(RED);
            FbMove(0,0);
            FbRectangle(SCREEN_SIZE-1,SCREEN_SIZE-1);
            p_data.state = MENU;
            break;
        case MENU:
            if(BUTTON_PRESSED_AND_CONSUME){
                returnToMenus();
                return;                
            }
            //If the ball is behind the paddle exit for now
            if((p_data.ballY) < 10) {
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
            
            if((p_data.ballY) > SCREEN_SIZE-2 && p_data.ballYVel > 0)
                p_data.ballYVel = -1 * p_data.ballYVel;
            
            //Update ball position
            //TODO: Insert frame counting and finer speed control
            
            p_data.ballX += p_data.ballXVel;
            p_data.ballY += p_data.ballYVel;
            
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
                drawRectangle((p_data.paddleX),10,20,5,G_Fb.BGcolor);
                if((p_data.paddleX) + delta < 0) {
                    p_data.paddleX = 0;
                } else if ((p_data.paddleX) + delta + 20 > SCREEN_SIZE-1) {
                    p_data.paddleX = (SCREEN_SIZE-20-1);
                } else {
                    p_data.paddleX += delta;
                }
            }
            
            //Check to see if we have a hit!
            if((p_data.ballY) < 15 &&
                    (p_data.ballX)+2 > (p_data.paddleX) &&
                    (p_data.ballX) < (p_data.paddleX ) + 20)
            {
                //Bounce the ball
                if(p_data.ballYVel < 0)
                    p_data.ballYVel *= -1;
                
                //Keep score!
                p_data.paddleHit += 1;
                
                //Every other paddle hit accelerate X bounce
                if(p_data.paddleHit % 2 > 0) {
                    p_data.ballXVel = (p_data.ballXVel > 0) ? p_data.ballXVel +1
                            : p_data.ballXVel -1;
                }
                
                //Play a note, the freq argument to setNote is inverted
                setNote(200/p_data.ballYVel,4048);
                
                //Make the game more difficult!
                p_data.ballYVel += 1;
            }
            //drawRectangle((p_data.paddleX >> 8),10,20,5,BLUE);
            

            //Draw the ball
            //drawRectangle((p_data.ballX >> 8),(p_data.ballY >> 8),2,2,WHITE);
            //Delay1us(10);
             
            FbMove(p_data.paddleX,10);
            FbColor(BLUE);
            FbFilledRectangle(20,5);
            FbColor(WHITE);
            FbMove(p_data.ballX,p_data.ballY);
            FbFilledRectangle(2,2);
            
            break;

            
    }
    
    
    
}