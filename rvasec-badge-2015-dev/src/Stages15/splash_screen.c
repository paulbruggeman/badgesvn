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
    DlInit();
    DlTransparentIndex(256);
    DlBackgroundColor(0);
    DlClear();
    DlMove(0, 2);
    DlPicture(SPLASH, 0);
    DlSwapBuffers();
    runningApp = &splash_Run;
}

void splash_Run()
{

    if(COUNTER==2000)returnToMenus();
    COUNTER++;
}
