#include "lander.h"
#include "HardwareProfile.h"
#include "badge16.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "colors.h"
#include "assetList.h"
#include "screensavers.h"

enum
{
    INIT,
    LANDER_SHOW_MENU,
    INIT_LEVEL,
    RUN
};
char badge_lander_state = LANDER_SHOW_MENU;
//unsigned char lander_ship_fuel = 255;
#define STARTING_FUEL 240 // make it even by 30 for niceness
#define FUEL_CAN_AMOUNT
//#define FUEL_DIV STARTING_FUEL/30
#define FUEL_DIV 8
#define FUEL_USAGE 4

lander_game_state_t lander_game;




void lander_menu_play(){
    badge_lander_state = INIT;
}

void lander_menu_fueler(){
    badge_lander_state = INIT;
}

void lander_menu_exit(){
    badge_lander_state = LANDER_SHOW_MENU;
    returnToMenus();
}

struct menu_t lander_help_m[] = {
    {"-Land on green", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"   green pad", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"-Touch to boost", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"-Fuels limited", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"-Gas can refills", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"-Avoid blue", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"   aliens", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"-Levels are", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"   generated", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"-Button to exit", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"back", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM, BACK, 0},

};

struct menu_t lander_main_m[] = {
    {"play", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)lander_menu_play}},
    //{"fueler", VERT_ITEM, FUNCTION, {(struct menu_t *)lander_menu_fueler}},
    {"help", VERT_ITEM, MENU, {(struct menu_t *)lander_help_m}},
    {"Exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *) lander_menu_exit}},
};


void add_fuel(unsigned char fuel_amount){

    if((255 - lander_game.lander_ship_fuel) > fuel_amount 
            && (lander_game.lander_ship_fuel + fuel_amount) < STARTING_FUEL)
        lander_game.lander_ship_fuel += fuel_amount;
    else
        lander_game.lander_ship_fuel = STARTING_FUEL;

}

void lander_update_pad(lander_game_state_t *g_state){
    g_state->pad.acc_x += 1;
}

void generate_level(lander_game_state_t *g_state){
    lander_init();
    if(lander_game.lander_level_id == 0)
        lander_game.lander_ship_fuel = STARTING_FUEL;
    else
        add_fuel(45);

    lander_game.lander_level_id++;
    setNote(120 - lander_game.lander_level_id<<1, 1024);

    lander_game.ship.pos_x = 10 + quick_rand(g_state->lander_ship_fuel)%105;
    lander_game.ship.pos_y = 10 + quick_rand(g_state->lander_ship_fuel)%105;

    unsigned char shift = 0;

    object_state tall_pad;
    // keep generating a pad until it isn't colliding with the ship
    do{
        g_state->pad.pos_x = quick_rand(lander_game.ship.pos_x + shift++)%105;
        g_state->pad.pos_y = 30 + quick_rand(lander_game.ship.pos_y + shift++)%90;

        // Make a tall pad to check for collisions with
        // in order to make sure ship isn't in a super easy landing position
        tall_pad = g_state->pad;
        tall_pad.height += 15;
        tall_pad.pos_y -= 15;

    }while(lander_obj_is_colliding(lander_game.ship,
                                     tall_pad));

    // default to no fuel
    g_state->fuel_can.is_active = 0;

    // only bother giving fuel if it is useful
    if((g_state->lander_ship_fuel != STARTING_FUEL) &&
       (quick_rand(lander_game.ship.pos_x + shift++) %3)){
        g_state->fuel_can.is_active = 1;
        do{
            g_state->fuel_can.pos_x = 15 + quick_rand(lander_game.fuel_can.pos_x + shift++)%115;
            g_state->fuel_can.pos_y = 20 + quick_rand(lander_game.fuel_can.pos_y + shift++)%110;
        }while(lander_obj_is_colliding(lander_game.ship, lander_game.fuel_can));
    }

    g_state->alien.is_active = 0;
    object_state big_alien;
    if(quick_rand(lander_game.ship.pos_x + lander_game.lander_level_id)%3){
        g_state->alien.is_active = 1;
        do{
            g_state->alien.pos_x = 15 + quick_rand(lander_game.alien.pos_x + shift++)%115;
            g_state->alien.pos_y = 20 + quick_rand(lander_game.alien.pos_y + shift++)%110;
            if(g_state->alien.pos_x > 80){
                g_state->alien.vel_x = -1 + (-1 * quick_rand(lander_game.ship.pos_x)%3);
            }
            else{
                g_state->alien.vel_x = 1 + (quick_rand(lander_game.ship.pos_x)%3);
            }

            if(g_state->alien.pos_y > 80){
                g_state->alien.vel_y = -1 + (-1 * quick_rand(lander_game.ship.pos_y)%3);
            }
            else{
                g_state->alien.vel_y = 1 + (quick_rand(lander_game.ship.pos_y)%3);
            }

            big_alien = g_state->alien;
            big_alien.width += 20;
            big_alien.height += 20;
            big_alien.pos_x -=20;
            big_alien.pos_y -=20;
        }while(lander_obj_is_colliding(lander_game.ship, big_alien));
    }

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

    lander_game.ship.is_active = 1; // will never go low

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

    lander_game.pad.is_active = 1; // will never go low

    //lander_game.lander_level_id = 0;
    //lander_game.pad.is_colliding = 0;

    lander_game.fuel_can.acc_x = 0;
    lander_game.fuel_can.acc_y = 0;

    lander_game.fuel_can.vel_x = 0;
    lander_game.fuel_can.vel_y = 0;

    lander_game.fuel_can.pos_x = 0;
    lander_game.fuel_can.pos_y = 0;

    lander_game.fuel_can.width = 10;
    lander_game.fuel_can.height = 13;

    lander_game.fuel_can.is_active = 1;



    lander_game.alien.acc_x = 0;
    lander_game.alien.acc_y = 0;

    lander_game.alien.vel_x = 0;
    lander_game.alien.vel_y = 0;

    lander_game.alien.pos_x = 0;
    lander_game.alien.pos_y = 0;

    lander_game.alien.width = 16;
    lander_game.alien.height = 5;

    lander_game.alien.is_active = 0;



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

    if(lander_game.v_slide > 50 && lander_game.lander_ship_fuel){
        lander_game.ship.acc_y -= 1;
        lander_game.lander_ship_fuel -= FUEL_USAGE;
    }

    if(lander_game.h_slide >0 && lander_game.h_slide < 20 && lander_game.lander_ship_fuel){
        lander_game.ship.acc_x -= 1;
        lander_game.lander_ship_fuel -= FUEL_USAGE;
    }
    else if(lander_game.h_slide > 70 && lander_game.lander_ship_fuel){
        lander_game.ship.acc_x += 1;
        lander_game.lander_ship_fuel -= FUEL_USAGE;
    }

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
            badge_lander_state = INIT_LEVEL;
            blue(50);
            red(0);
        }
        else
        {
            blue(0);
            red(50);
            lander_game.ship_on_pad_cnt = -1;
            //badge_lander_state = INIT_LEVEL;
            badge_lander_state = INIT; //HAHAHA reset!
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


    if(lander_game.fuel_can.is_active){
        unsigned char is_fuel_col = lander_obj_is_colliding(lander_game.ship,
                                                            lander_game.fuel_can);

        if(is_fuel_col){
            add_fuel(75);
            setNote(75, 1024);
            lander_game.fuel_can.is_active = 0;
        }
    }

    if(lander_game.alien.is_active){
        lander_game.alien.pos_x += lander_game.alien.vel_x;
        lander_game.alien.pos_y += lander_game.alien.vel_y;
        if(lander_game.alien.pos_x < 5 
                || lander_game.alien.pos_x > 130
                || lander_game.alien.pos_y < 5
                || lander_game.alien.pos_y > 130)
            lander_game.alien.is_active = 0;
        else{

            unsigned char is_alien_col = lander_obj_is_colliding(lander_game.ship,
                                                                lander_game.alien);

            if(is_alien_col){
                setNote(175, 1024);
                lander_game.alien.is_active = 0;
                blue(0);
                red(50);
                lander_game.ship_on_pad_cnt = -1;
                //badge_lander_state = INIT_LEVEL;
                badge_lander_state = INIT; //HAHAHA reset!
            }
        }
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
    FbColor(WHITE);
    FbMove(lander_game.pad.pos_x, lander_game.pad.pos_y);
    FbRectangle(lander_game.pad.width, 4);
}

#define FUEL_BAR_OUTLINE_COLOR WHITE
#define FUEL_BAR_FILL_COLOR YELLOW
void lander_draw_fuel_bar(){
    FbColor(FUEL_BAR_OUTLINE_COLOR);
    FbMove(115, 9);
    FbRectangle(10, 31);

    FbColor(FUEL_BAR_FILL_COLOR);
    FbMove(116, 10 + (30 - lander_game.lander_ship_fuel/FUEL_DIV));
    FbFilledRectangle(9, lander_game.lander_ship_fuel/FUEL_DIV);
}

void lander_draw_level_number(){
    FbColor(WHITE);
    FbMove(1, 3);
    unsigned char lvl_str[] = {'L', 'V', 'L', ':',
                                '0' + (lander_game.lander_level_id/10) %10,
                                '0' + (lander_game.lander_level_id) %10};
    FbWriteLine(lvl_str);

}

#define FUEL_CAN_COLOR RED
void lander_draw_fuel_can(){
    FbColor(FUEL_CAN_COLOR);
    FbMove(lander_game.fuel_can.pos_x,
           lander_game.fuel_can.pos_y);
    FbFilledRectangle(lander_game.fuel_can.width,
                      lander_game.fuel_can.height);

    FbColor(YELLOW);
    FbMove(lander_game.fuel_can.pos_x+2, lander_game.fuel_can.pos_y - 3);
    FbFilledRectangle(3, 3);

    FbColor(BLACK);
    FbLine(lander_game.fuel_can.pos_x + 1, lander_game.fuel_can.pos_y + 1,
           lander_game.fuel_can.pos_x + lander_game.fuel_can.width - 1,
           lander_game.fuel_can.pos_y + lander_game.fuel_can.height - 1);

    FbLine(lander_game.fuel_can.pos_x + lander_game.fuel_can.width - 1,
           lander_game.fuel_can.pos_y + 1,
           lander_game.fuel_can.pos_x + 1,
           lander_game.fuel_can.pos_y + lander_game.fuel_can.height - 1);

}

void lander_draw_alien(){
    FbColor(CYAN);
    FbMove(lander_game.alien.pos_x,
           lander_game.alien.pos_y);
    FbImage(BI_SPRITE, 0);
}

// TODO: 
//  - Death on hit bottom of screen
//  - moving pad
//  - fuel pickups
//  - badge invaders flyby (one that takes fuel)
void lander_draw(){
    lander_draw_ship();
    lander_draw_fuel_bar();
    lander_draw_landing_pad();
    lander_draw_level_number();

    if(lander_game.fuel_can.is_active)
        lander_draw_fuel_can();

    if(lander_game.alien.is_active)
        lander_draw_alien();
}

void badge_lander_cb()
{
    switch(badge_lander_state)
    {
        // TODO: INIT and INIT level don't need to be separate
        case LANDER_SHOW_MENU:
            genericMenu((struct menu_t *)lander_main_m, WHITE_ON_BLACK);
            FbSwapBuffers();
            break;
        case INIT:
            lander_init();
            lander_game.lander_level_id = 0;
            badge_lander_state = INIT_LEVEL;
            break;
        case INIT_LEVEL:
            //level_inits[lander_game.lander_level_id].init(&lander_game);
            generate_level(&lander_game);
            badge_lander_state = RUN;
            setHighScore(SCORE_LANDER, lander_game.lander_level_id, 0);
            break;
        case RUN:
            lander_update();
            // Check collision before draw so we can correct clipping
            //lander_check_collisions();
            lander_draw();
            if (BUTTON_PRESSED_AND_CONSUME)
                badge_lander_state = LANDER_SHOW_MENU;
            FbSwapBuffers();
            break;
    }
}
