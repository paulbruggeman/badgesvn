/*
 * File:   connect4.c
 * Author: Sean
 *
 * Created on April 6, 2015, 1:02 PM
 */

#include "connect4.h"
#include "badge15.h"

extern badge_state b_state;//badge state structure to reduce variables

#define POS b_state.counter1
#define TURN b_state.counter2
#define LEVEL b_state.selected_object

unsigned char grid[7][6];


void connect4_Init() {
    unsigned char i;
    unsigned char j;

    //Init Grid
    for(i = 0; i < 7; i++)
        for(j = 0; j < 6; j++)
            grid[i][j] = 0;

    POS = 0;
    TURN = 0;
    LEVEL = 0;

    runningApp = &connect4_printBoard;

}

void connect4_Wait() {
    if (BUTTON_PRESSED_AND_CONSUME)
    {
        runningApp = &returnToMenus;
    }
    else if(TOP_SLIDE_AND_CONSUME)
    {
        setNote(109, 2048);
    }
    else if (BOTTOM_SLIDE_AND_CONSUME)
    {
        if(grid[POS][0] != 0){
            setNote(121, 2048);
        }else{
            setNote(97, 2048);
            runningApp = &connect4_down;
        }
    }
    else if (LEFT_SLIDE_AND_CONSUME)
    {
        setNote(109, 2048);
        POS--;
        if(POS>6)POS=0;
        runningApp = &connect4_printBoard;
    }
    else if (RIGHT_SLIDE_AND_CONSUME)
    {
        setNote(97, 2048);
        POS++;
        if(POS>6)POS=6;
        runningApp = &connect4_printBoard;
    }
}

void connect4_printBoard() {
    unsigned char i, j, x, y, val;

    DlInit();
    DlBackgroundColor(0b0011100111111111);
    DlClear();

    DlColor(0b1111111111100000);
    DlMove(3, 21);
    DlFilledRectangle(122, 105);

    DlMove((POS*16)+POS+5, 4);
    DlTransparentIndex(5);
    DlPicture(CHIPSPRITE, TURN+1);

    for(i = 0; i < 7; i++)
    {
        for(j = 0; j < 6; j++)
        {
            x = (i*16)+i+5;
            y = (j*16)+j+23;
            val = grid[i][j];
            DlMove(x, y);
	    DlPicture(CHIPSPRITE, val);
        }
    }

    DlSwapBuffers();
    runningApp = &connect4_Wait;
}

void connect4_down() {
    unsigned char j;

    for(j = 0; j < 6; j++)
        if(grid[POS][j] != 0)
            break;
    j--;

    grid[POS][j] = TURN + 1;

    TURN = 1 - TURN;
    runningApp = &connect4_printBoard;
}
