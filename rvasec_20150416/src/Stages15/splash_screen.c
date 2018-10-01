/*
 * File:   splash_screen.c
 * Author: Sean
 *
 * Created on April 11, 2015, 12:35 PM
 */

#include "splash_screen.h"
#include "badge15.h"

extern badge_state b_state;//badge state structure to reduce variables

#define COUNTER b_state.large_counter

void splash_Init()
{
    COUNTER = 0;
    runningApp = &splash_Run;
}

void splash_Run()
{
    DlInit();
    DlColor(0);
    DlClear();
    DlMove(2, 0);
    DlPicture(SPLASH, 0);
    if(COUNTER==1000)returnToMenus();
    COUNTER++;
}
