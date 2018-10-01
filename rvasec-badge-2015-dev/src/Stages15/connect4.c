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
#define DIRECTION b_state.current_state
#define CHECK b_state.previous_state

unsigned char grid[7][6];
union IRpacket_u pkt;

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
    DIRECTION = 1;
    CHECK = 0;

    runningApp = &connect4_printBoard;
}

void connect4_wait() {
    if (BUTTON_PRESSED_AND_CONSUME)
    {
        runningApp = &returnToMenus;
    }
    else if(TOP_TAP_AND_CONSUME)
    {
        setNote(109, 2048);
    }
    else if (BOTTOM_TAP_AND_CONSUME)
    {
        if(grid[POS][0] != 0){
            setNote(121, 2048);
        }else{
            setNote(97, 2048);
            runningApp = &connect4_down;
        }
    }
    else if (LEFT_TAP_AND_CONSUME)
    {
        setNote(109, 2048);
        POS--;
        if(POS>6)POS=0;
        runningApp = &connect4_printBoard;
    }
    else if (RIGHT_TAP_AND_CONSUME)
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
    DlPicture(CHIPSPRITE, CHECK ? (1-TURN)+1 : TURN+1);

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
    if(CHECK == 1) runningApp = &connect4_checkBoard;
    else runningApp = &connect4_wait;
}

void connect4_down() {
    for(LEVEL = 0; LEVEL < 6; LEVEL++)
        if(grid[POS][LEVEL] != 0)
            break;
    LEVEL--;

    grid[POS][LEVEL] = TURN + 1;

    CHECK = 1;
    runningApp = &connect4_printBoard;
}

void connect4_checkBoard() {
    unsigned char j;

    switch(DIRECTION)
    {
        case 1:
            for(j = 0; j < 4; j++)
                if(grid[POS+j][LEVEL-j] != TURN+1 || (POS+j > 6) || (LEVEL-j > 5))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;

        case 2:
            for(j = 0; j < 4; j++)
                if(grid[POS+j][LEVEL] != TURN+1 || (POS+j > 6))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;

        case 3:
            for(j = 0; j < 4; j++)
                if(grid[POS+j][LEVEL+j] != TURN+1 || (POS+j > 6) || (LEVEL+j > 5))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;        

        case 4:
            for(j = 0; j < 4; j++)
                if(grid[POS][LEVEL+j] != TURN+1 || (LEVEL+j > 5))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;

        case 5:
            for(j = 0; j < 4; j++)
                if(grid[POS-j][LEVEL+j] != TURN+1 || (POS-j > 6) || (LEVEL+j > 5))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;

        case 6:
            for(j = 0; j < 4; j++)
                if(grid[POS-j][LEVEL] != TURN+1 || (POS-j > 6))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;

        case 7:
            for(j = 0; j < 4; j++)
                if(grid[POS-j][LEVEL-j] != TURN+1  || (POS-j > 6) || (LEVEL-j > 5))
                    break;
            if(j == 4)runningApp = &connect4_win;
            break;

        default:
            DIRECTION = 0;
            TURN = 1 - TURN;
            CHECK = 0;
            runningApp = &connect4_wait;
            break;
    }
    DIRECTION++;
}

void connect4_win() {
    runningApp = &returnToMenus;
}
