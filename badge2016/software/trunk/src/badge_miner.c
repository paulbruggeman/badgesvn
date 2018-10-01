#include "badge16.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "colors.h"

enum
{
    INIT,
    //SHOW_MENU,
    RUN
};

char badge_miner_state = INIT;
unsigned int score = 0;
char miner_loc = 0;
char miner_speed = 1;

void init()
{

}

void update()
{
    if (BUTTON_PRESSED_AND_CONSUME)
        miner_speed++;

    if(miner_loc > 120)
    {
        miner_loc = 0;
    }
    else
        miner_loc += miner_speed;
}

void draw()
{
    FbColor(RED);
    FbMove(0, 57);
    FbFilledRectangle(132, 75);

    FbColor(BLUE);
    FbMove(miner_loc, 47);
    FbFilledRectangle(10, 10);

    FbColor(RED);
    FbMove(miner_loc+10, 47);
    FbFilledRectangle(122 - miner_loc, 10);
}

void badge_miner_cb()
{
    switch(badge_miner_state)
    {
        case INIT:
            init();
            badge_miner_state = RUN;
            break;
        case RUN:
            update();
            draw();
            break;
    }
}
