#include "lander.h"
#include "HardwareProfile.h"
#include "badge16.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "colors.h"

enum
{
    INIT,
    INIT_LEVEL,
    RUN
};


lander_level_init_callback_t level_inits[] = {
    { lander_level_1_init, 0, 0 },
    { lander_level_2_init, 0, 0 },
    { lander_level_3_init, 0, 0 },
    { lander_level_4_init, 0, 0 },
    { lander_level_5_init, 0, IS_LAST_LEVEL },
};

char badge_lander_state = INIT;

lander_game_state_t lander_game;

void lander_level_1_init(lander_game_state_t *g_state){
    lander_init();
    lander_game.lander_level_id = 0;
    setNote(173, 1024);
    g_state->pad.pos_x = 40;
    g_state->pad.pos_y = 80;
}

void lander_update_pad(lander_game_state_t *g_state){

    g_state->pad.acc_x += 1;
}

void lander_level_2_init(lander_game_state_t *g_state){
    lander_init();
    lander_game.lander_level_id = 1;
    setNote(143, 1024);
    g_state->pad.pos_x = 10;
    g_state->pad.pos_y = 80;
}

void lander_level_3_init(lander_game_state_t *g_state){
    lander_init();
    lander_game.lander_level_id = 2;
    setNote(113, 1024);
    g_state->pad.pos_x = 90;
    g_state->pad.pos_y = 80;
}

void lander_level_4_init(lander_game_state_t *g_state){
    lander_init();
    lander_game.lander_level_id = 3;
    setNote(90, 1024);
    g_state->pad.pos_x = 40;
    g_state->pad.pos_y = 120;
}

void lander_level_5_init(lander_game_state_t *g_state){
    lander_init();
    lander_game.lander_level_id = 4;
    setNote(50, 1024);
    g_state->pad.pos_x = 10;
    g_state->pad.pos_y = 30;

    lander_game.ship.pos_x = 110;
    lander_game.ship.pos_y = 110;
}

void lander_init()
{
    red(0);
    blue(0);
    green(0);

    // Game state
    lander_game.h_slide = 0;
    lander_game.v_slide = 0;

    lander_game.drag_cnt;
    lander_game.grav_cnt;

    lander_game.ship_on_pad_cnt = 0;

    // Ship state
    lander_game.ship.acc_x = 0;
    lander_game.ship.acc_y = 0;

    lander_game.ship.vel_x = 0;
    lander_game.ship.vel_y = 0;

    lander_game.ship.pos_x = 60;
    lander_game.ship.pos_y = 10;

    lander_game.ship.width = LANDER_WIDTH;
    lander_game.ship.height = LANDER_HEIGHT;

    //lander_game.ship.is_colliding = 0;

    // Lander pad
    lander_game.pad.acc_x = 0;
    lander_game.pad.acc_y = 0;

    lander_game.pad.vel_x = 0;
    lander_game.pad.vel_y = 0;

    lander_game.pad.pos_x = 30;
    lander_game.pad.pos_y = 110;

    lander_game.pad.width = 35;
    lander_game.pad.height = 4;

    lander_game.lander_level_id = 0;
    //lander_game.pad.is_colliding = 0;
}

void lander_update_object_position(object_state *o_state){
     //Apply accel to velocity
    o_state->vel_x += o_state->acc_x;
    o_state->vel_y += o_state->acc_y;

    o_state->pos_x += o_state->vel_x;
    o_state->pos_y += o_state->vel_y;
}

void lander_update()
{
    unsigned char is_col = 0;
    lander_game.v_slide = getVerticalPosition();
    lander_game.h_slide = getHorizontalPosition();
    lander_game.drag_cnt++;
    lander_game.grav_cnt++;

    //reset accel since it shouldn't accumulate
    lander_game.ship.acc_x = 0;
    lander_game.ship.acc_y = 0;

    if(lander_game.v_slide > 50)
        lander_game.ship.acc_y -= 1;

    if(lander_game.h_slide >0 && lander_game.h_slide < 20)
        lander_game.ship.acc_x -= 1;
    else if(lander_game.h_slide > 70)
        lander_game.ship.acc_x += 1;

    // Have to *occasionally* apply gravity and drag, otherwise,
    // it can't be made 'light' enough (maybe possible with floating
    // point, but f that)

    // Apply some gravity?
    if(lander_game.grav_cnt == GRAV_UPDATE_RATE){
        lander_game.grav_cnt = 0;
        lander_game.ship.acc_y++;
    }

    // Add some drag to horizontel
    if(lander_game.drag_cnt == DRAG_UPDATE_RATE){
        lander_game.drag_cnt = 0;
        if(lander_game.ship.vel_x > 0)
            lander_game.ship.acc_x--;
        else if(lander_game.ship.vel_x < 0)
            lander_game.ship.acc_x++;
    }

    //Apply accel to velocity
    lander_game.ship.vel_x += lander_game.ship.acc_x;
    lander_game.ship.vel_y += lander_game.ship.acc_y;

    lander_game.ship.pos_x += lander_game.ship.vel_x;
    lander_game.ship.pos_y += lander_game.ship.vel_y;


    // Check success
    is_col = lander_obj_is_colliding(lander_game.ship,
                                     lander_game.pad);
    if(is_col){

        // Weight the y axis more
        unsigned short tmp_lnd_vel = (lander_game.ship.vel_x+1) 
                                        * (lander_game.ship.vel_y*4+1);
        if(lander_game.ship_on_pad_cnt != -1 && tmp_lnd_vel < 9){
            lander_game.ship_on_pad_cnt++;
            if(!(level_inits[lander_game.lander_level_id].flags & IS_LAST_LEVEL)){
                lander_game.lander_level_id++;
                badge_lander_state = INIT_LEVEL;
            }
            blue(50);
            red(0);
        }
        else
        {
            blue(0);
            red(50);
            lander_game.ship_on_pad_cnt = -1;
            badge_lander_state = INIT_LEVEL;
        }

        lander_game.ship.pos_x -= lander_game.ship.vel_x;
        lander_game.ship.pos_y -= lander_game.ship.vel_y;

        lander_game.ship.vel_x = 0;
        lander_game.ship.vel_y = 0;
    }
    else
    {
        blue(0);
        //red(0);
    }

    lander_bound(&lander_game.ship.pos_x, &lander_game.ship.pos_y,
                 &lander_game.ship.vel_x, &lander_game.ship.vel_y);
}

void lander_draw_ship()
{
    // draw tall and thin top part
    FbColor(GREY16);
    FbMove(lander_game.ship.pos_x+2, lander_game.ship.pos_y);
    FbFilledRectangle(LANDER_BODY_WIDTH-4, 10);

    // draw wide and short base
    FbColor(GREY8);
    FbMove(lander_game.ship.pos_x, lander_game.ship.pos_y+10);
    FbFilledRectangle(LANDER_BODY_WIDTH, 5);

    // draw legs
    FbColor(GREY16);
    FbLine(lander_game.ship.pos_x, lander_game.ship.pos_y+LANDER_BODY_HEIGHT,
           lander_game.ship.pos_x-3, lander_game.ship.pos_y+LANDER_BODY_HEIGHT+5);

    FbLine(lander_game.ship.pos_x+LANDER_BODY_WIDTH, lander_game.ship.pos_y+LANDER_BODY_HEIGHT,
           lander_game.ship.pos_x+LANDER_BODY_WIDTH+3, lander_game.ship.pos_y+LANDER_BODY_HEIGHT+5);

    // TODO: Make particles not suck ass (persistence, better rng, etc.)
    // Bottom booster particle spray
    if(lander_game.ship.acc_y < 0){
        FbColor(YELLOW);
        //unsigned char rnd = 0;//GetInstructionClock()%23
        unsigned char i =0, j=0;
        for(i=0; i < 10; i++, j++){
            FbPoint(lander_game.ship.pos_x+5 + j%3,
                    lander_game.ship.pos_y+LANDER_BODY_HEIGHT + i*(j%3));
        }
    }

    // Only do these when the user is pressing, not when dragging
    // Right booster particle spray
    if(lander_game.h_slide >0 && lander_game.h_slide < 20){
        FbColor(YELLOW);
        //unsigned char rnd = 0;//GetInstructionClock()%23
        unsigned char i =0, j=0;
        for(i=0; i < 4; i++, j++){
            FbPoint(lander_game.ship.pos_x+LANDER_BODY_WIDTH + i*(j%3),
                    lander_game.ship.pos_y + 10 + j%3);
        }
    }

    // Left booster particle spray
    if(lander_game.h_slide > 70){
        FbColor(YELLOW);
        //unsigned char rnd = 0;//GetInstructionClock()%23
        unsigned char i =0, j=0;
        for(i=0; i < 4; i++, j++){
            FbPoint(lander_game.ship.pos_x - i*(j%3),
                    lander_game.ship.pos_y + 10 + j%3);
        }
    }
}

void lander_draw_landing_pad(){
    FbColor(GREEN);
    FbMove(lander_game.pad.pos_x, lander_game.pad.pos_y);
    FbFilledRectangle(lander_game.pad.width, 4);
}

void lander_draw(){
    lander_draw_ship();
    lander_draw_landing_pad();
}

void badge_lander_cb()
{
    switch(badge_lander_state)
    {
        // TODO: INIT and INIT level don't need to be separate
        case INIT:
            lander_init();
            badge_lander_state = INIT_LEVEL;
            break;
        case INIT_LEVEL:
            level_inits[lander_game.lander_level_id].init(&lander_game);
            badge_lander_state = RUN;
            break;
        case RUN:
            lander_update();
            // Check collision before draw so we can correct clipping
            //lander_check_collisions();
            lander_draw();
            FbSwapBuffers();
            break;
    }


    if (BUTTON_PRESSED_AND_CONSUME){
        badge_lander_state = INIT;
        returnToMenus();
    }
}
