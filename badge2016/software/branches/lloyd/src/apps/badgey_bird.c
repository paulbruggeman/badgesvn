#include "lander.h"
#include "HardwareProfile.h"
#include "badge16.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "colors.h"
#include "fb.h"
#include "messenger.h"
#include "../prngs.h"
#include "badgey_bird.h"
#include "assetList.h"
#include "flash.h"
#include "screensavers.h"
//#include "lander.h" //using lander utils definitions declared here

unsigned char lander_is_colliding(unsigned char x1,unsigned char y1,unsigned char w1,unsigned char h1,
                         unsigned char x2,unsigned char y2,unsigned char w2,unsigned char h2);


enum{
    BADGEYB_LOAD,
    BADGEYB_INIT,
    BADGEYB_RUN,
    BADGEYB_SHOW_MENU
};

// only set to load here, so we only load once
unsigned char badgey_bird_state = BADGEYB_LOAD;//BADGEYB_SHOW_MENU;//BADGEYB_INIT;

void badgey_bird_menu_play(){
    badgey_bird_state = BADGEYB_INIT;
}

void badgey_bird_menu_exit(){
    FbBackgroundColor(BLACK);
    FbClear();
    badgey_bird_state = BADGEYB_SHOW_MENU;
    setNote(175, 1024);
    returnToMenus();
}


unsigned char badgey_bird_score = 0;
unsigned char badgey_bird_top_score = 0;

unsigned char bird_pos_y = 50;
unsigned char bird_vel_y = 0;
unsigned char bird_acc_y = 0;

#define BADGEY_BIRD_START_HEIGHT 50
#define BADGEY_BIRD_POS_X 25
const unsigned char bird_pos_x = BADGEY_BIRD_POS_X;

#define BADGEY_BIRD_BIRD_WIDTH 20
#define BADGEY_BIRD_BIRD_HEIGHT 14

#define BADGEY_BIRD_PIPE_WIDTH 12
#define BADGEY_BIRD_OPENING_SIZE 55
#define BADGEY_BIRD_PIPE_SPEED 3

#define BADGEY_BIRD_FLAP_ACC -10
#define BADGEY_BIRD_GRAV 2
#define BADGEY_BIRD_MAX_PIPES 4
struct BB_pipe_state badgey_bird_pipes[BADGEY_BIRD_MAX_PIPES];
unsigned char badgey_bird_active_pipe_cnt = 0;

void badgey_bird_init(){
    unsigned char i = 0;
    for(i = 0; i < BADGEY_BIRD_MAX_PIPES; i++){
        badgey_bird_pipes[i].is_active = 0;
    }

    badgey_bird_active_pipe_cnt = 0;
    bird_pos_y = BADGEY_BIRD_START_HEIGHT;
    bird_vel_y = 0;
    bird_acc_y = 0;

    badgey_bird_score = 0;

}

void badgey_bird_add_active_pipe(){
    unsigned char i = 0;
    for(i = 0; i < BADGEY_BIRD_MAX_PIPES 
                && badgey_bird_pipes[i].is_active
                ; i++);

    if(i == BADGEY_BIRD_MAX_PIPES)
        return;

    //setNote(100, 1024);
    badgey_bird_active_pipe_cnt++;
    badgey_bird_pipes[i].is_active = 1;
    badgey_bird_pipes[i].pos_x = 110;
    badgey_bird_pipes[i].opening_height = 15 + quick_rand(bird_vel_y) % 50;

}

#define BADGEY_BIRD_REMOVE_PIPE_X 5
unsigned char bb_most_right_pipe_x = 0;
void badgey_bird_update_pipes(){
    unsigned char i = 0;
    //unsigned char tmp_most_right_x = 0;
    bb_most_right_pipe_x = 0;
    for(i = 0; i < BADGEY_BIRD_MAX_PIPES; i++){
        if(badgey_bird_pipes[i].is_active){
            if(badgey_bird_pipes[i].is_active == 1){ // hasn't crossed pipe, so check
                if(bird_pos_x > (badgey_bird_pipes[i].pos_x + BADGEY_BIRD_PIPE_WIDTH)){
                    badgey_bird_pipes[i].is_active = 2;
                    badgey_bird_score++;
                    setNote(100, 2048);
                }

            }

            if( badgey_bird_pipes[i].pos_x > BADGEY_BIRD_REMOVE_PIPE_X)
                badgey_bird_pipes[i].pos_x -= BADGEY_BIRD_PIPE_SPEED;
            else if(badgey_bird_pipes[i].pos_x <= BADGEY_BIRD_REMOVE_PIPE_X){
                badgey_bird_pipes[i].is_active = 0;
                badgey_bird_active_pipe_cnt--;
            }

            if(badgey_bird_pipes[i].pos_x > bb_most_right_pipe_x)
                bb_most_right_pipe_x = badgey_bird_pipes[i].pos_x;
        }
    }
}


char badgey_bird_is_alive(){
    if(bird_pos_y > 110)
        return 0;

    unsigned char i = 0;
    unsigned char col_check = 0;
    for(i = 0; i < BADGEY_BIRD_MAX_PIPES; i++){
        if(badgey_bird_pipes[i].is_active){
            col_check = lander_is_colliding(badgey_bird_pipes[i].pos_x, 0,
                                                BADGEY_BIRD_PIPE_WIDTH, badgey_bird_pipes[i].opening_height,
                                            bird_pos_x, bird_pos_y,
                                                BADGEY_BIRD_BIRD_WIDTH, BADGEY_BIRD_BIRD_HEIGHT);
            col_check |= lander_is_colliding(badgey_bird_pipes[i].pos_x,
                                             badgey_bird_pipes[i].opening_height + BADGEY_BIRD_OPENING_SIZE,
                                                BADGEY_BIRD_PIPE_WIDTH,
                                                132 - badgey_bird_pipes[i].opening_height + BADGEY_BIRD_OPENING_SIZE,
                                            bird_pos_x, bird_pos_y,
                                                BADGEY_BIRD_BIRD_WIDTH, BADGEY_BIRD_BIRD_HEIGHT);

            if(col_check)
                return 0;
        }
    }

    return 1;
}

void badgey_bird_update(){

    bird_vel_y += (BADGEY_BIRD_GRAV + bird_acc_y);

    bird_pos_y += bird_vel_y;

    bird_acc_y = 0;

    if((badgey_bird_active_pipe_cnt > 0)
            && (badgey_bird_active_pipe_cnt < BADGEY_BIRD_MAX_PIPES)
            && (bb_most_right_pipe_x < 55)){

        unsigned char add_a_pipe = quick_rand(bird_vel_y)%13;
        if(!add_a_pipe){
            badgey_bird_add_active_pipe();
        }
    }
    // always have a pipe on screen ?
    else if (badgey_bird_active_pipe_cnt == 0){
            badgey_bird_add_active_pipe();
    }


    badgey_bird_update_pipes();

    if(!badgey_bird_is_alive()){
        if(badgey_bird_score > badgey_bird_top_score){
            //TODO: Save new top score to flash
            setHighScore(SCORE_BADGYBIRD, badgey_bird_score, 0);
            badgey_bird_top_score = badgey_bird_score;
            NVwrite(3, 0, &badgey_bird_top_score, 1);
        }

        //FbBackgroundColor(BLACK);
        //badgey_bird_state = BADGEYB_INIT;
        //badgey_bird_state = BADGEYB_SHOW_MENU;
        badgey_bird_menu_exit();
    }
}

void badgey_bird_draw_bird(){
//    FbColor(YELLOW);
//    FbMove(bird_pos_x, bird_pos_y);
//    FbFilledRectangle(16, 16);
    FbMove(bird_pos_x, bird_pos_y);
    FbTransparentIndex(3);
    FbImage(BADGEY_BIRD, 0);
}

void badgey_bird_draw_pipe(struct BB_pipe_state pipe_t){

    FbColor(GREEN);
    // Draw top portion
    FbMove(pipe_t.pos_x, 0);
    FbFilledRectangle(BADGEY_BIRD_PIPE_WIDTH,
                      pipe_t.opening_height);
    // easier to just draw over
    FbColor(BLACK);
    FbMove(pipe_t.pos_x, 0);
    FbRectangle(BADGEY_BIRD_PIPE_WIDTH,
                pipe_t.opening_height);


    // Top poertions cap
    FbColor(GREEN);
    FbMove(pipe_t.pos_x-4, pipe_t.opening_height - 10);
    FbFilledRectangle(BADGEY_BIRD_PIPE_WIDTH+8,
                      10);
    FbColor(BLACK);
    FbMove(pipe_t.pos_x-4, pipe_t.opening_height - 10);
    FbRectangle(BADGEY_BIRD_PIPE_WIDTH+8, 10);



    FbColor(GREEN);
    //Draw bottom portion
    FbMove(pipe_t.pos_x, pipe_t.opening_height + BADGEY_BIRD_OPENING_SIZE);
    FbFilledRectangle(BADGEY_BIRD_PIPE_WIDTH,
                      132 - pipe_t.opening_height + BADGEY_BIRD_OPENING_SIZE);

    FbColor(BLACK);
    FbMove(pipe_t.pos_x, pipe_t.opening_height + BADGEY_BIRD_OPENING_SIZE);
    FbRectangle(BADGEY_BIRD_PIPE_WIDTH,
                      132 - pipe_t.opening_height + BADGEY_BIRD_OPENING_SIZE);


    // Draw bottom portion cap
    FbColor(GREEN);
    FbMove(pipe_t.pos_x-4, pipe_t.opening_height + BADGEY_BIRD_OPENING_SIZE);
    FbFilledRectangle(BADGEY_BIRD_PIPE_WIDTH+8, 10);

    FbColor(BLACK);
    FbMove(pipe_t.pos_x-4, pipe_t.opening_height + BADGEY_BIRD_OPENING_SIZE);
    FbRectangle(BADGEY_BIRD_PIPE_WIDTH+8, 10);

}

void badgey_bird_draw_pipes(){
    unsigned char i = 0;
    //unsigned char any_active = 0;
    for(i = 0; i < BADGEY_BIRD_MAX_PIPES; i++){
        if(badgey_bird_pipes[i].is_active){
            badgey_bird_draw_pipe(badgey_bird_pipes[i]);
        }
    }
}

#define BADGEY_BIRD_SCORE_POS_X 100
#define BADGEY_BIRD_SCORE_POS_Y 4
void badgey_bird_draw_current_score(){
    FbMove(BADGEY_BIRD_SCORE_POS_X,
           BADGEY_BIRD_SCORE_POS_Y);

    FbColor(BLACK);

    unsigned char tmp_str[] = {
        '0' + (badgey_bird_score/100) % 10,
        '0' + (badgey_bird_score/10) % 10,
        '0' + (badgey_bird_score) % 10,
            0};
    FbWriteLine(tmp_str);
}

void badgey_bird_render(){
    badgey_bird_draw_bird();
    badgey_bird_draw_pipes();
    badgey_bird_draw_current_score();
}

//#define BADGEYB_REALISTIC_GRAV

void badgey_bird_draw_landing_page(){
    unsigned char score_str[] = {
                '0' + (badgey_bird_top_score/100) % 10,
                '0' + (badgey_bird_top_score/10) % 10,
                '0' + (badgey_bird_top_score) % 10,
                0};

    FbBackgroundColor(CYAN);
    FbColor(BLACK);
    FbMove(10, 10);
    FbWriteLine("BADGEY BIRD");
    FbMove(20, 20);
    FbColor(RED);
    FbWriteLine("in COLOR");

    FbColor(BLACK);
    FbMove(70, 50);
    FbWriteLine(score_str);

    FbMove(30, 80);
    FbWriteLine("Press the");
    FbMove(30, 90);
    FbWriteLine("Button to");
    FbMove(30, 100);
    FbWriteLine("Begin");
    if(BUTTON_PRESSED_AND_CONSUME){
        badgey_bird_menu_play();
    }

    FbMove(BADGEY_BIRD_POS_X, BADGEY_BIRD_START_HEIGHT);
    FbTransparentIndex(3);
    FbImage(BADGEY_BIRD, 0);
    FbSwapBuffers();

}

void badgey_bird_cb(){
    switch(badgey_bird_state){
        case BADGEYB_LOAD:
            NVread(3, 0, &badgey_bird_top_score, 1);
            badgey_bird_state = BADGEYB_SHOW_MENU;
            break;
        case BADGEYB_INIT:

            FbBackgroundColor(CYAN);
            FbClear();

            badgey_bird_init();
            badgey_bird_state = BADGEYB_RUN;
            break;

        case BADGEYB_RUN:
            if(BUTTON_PRESSED_AND_CONSUME){
                #ifndef BADGEYB_REALISTIC_GRAV
                bird_vel_y = 0; // zero out to make it feel more like the orig.
                #endif
                bird_acc_y = BADGEY_BIRD_FLAP_ACC;
            }
            badgey_bird_update();
            // hack to not draw crap over menu on exit
            if(badgey_bird_state == BADGEYB_RUN){
                badgey_bird_render();
                FbSwapBuffers();
            }
            break;
        case BADGEYB_SHOW_MENU:
            //genericMenu((struct menu_t *)badgey_bird_m, WHITE_ON_BLACK);
            badgey_bird_draw_landing_page();
            break;
    }
}
