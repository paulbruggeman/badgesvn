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

enum{
    IOMON_INIT,
    IOMON_RUN,
    IOMON_WAIT
};
unsigned short iomon_cnter = 0;
extern unsigned char num_packets_seen;

#define IOMON_HISTORY_LEN 50
unsigned char packet_history[IOMON_HISTORY_LEN] = {0};

#define IOMON_GRAPH_VERTEX_X 10
#define IOMON_GRAPH_VERTEX_Y 115
#define IOMON_GRAPH_X_LEN 110
#define IOMON_GRAPH_Y_LEN 110
#define IOMON_AXIS_COLOR WHITE

#define IOMON_GRAPH_Y_ZERO 109
void iomon_draw_axis(){

    FbColor(IOMON_AXIS_COLOR);

    // draw X axis
    FbLine(IOMON_GRAPH_VERTEX_X, IOMON_GRAPH_VERTEX_Y, 
                     IOMON_GRAPH_VERTEX_X + IOMON_GRAPH_X_LEN, IOMON_GRAPH_VERTEX_Y);

    // draw Y axis
    FbLine(IOMON_GRAPH_VERTEX_X, IOMON_GRAPH_VERTEX_Y, 
                     IOMON_GRAPH_VERTEX_X, IOMON_GRAPH_VERTEX_Y - IOMON_GRAPH_Y_LEN);

    // Y Label
    FbMove(1, 30);
    FbCharacter('P');
    FbMove(1, 40);
    FbCharacter('k');
    FbMove(1, 50);
    FbCharacter('t');
    FbMove(1, 60);
    FbCharacter('s');

    // X Label
    unsigned char tmp[] = "time";
    FbMove(45, IOMON_GRAPH_VERTEX_Y + 2);
    FbWriteLine(tmp);
}

void iomon_shift_in_packet_amount(unsigned char packet_cnt){
    unsigned char i = 0;
    for(i = 1; i < IOMON_HISTORY_LEN; i++){
        packet_history[i-1] = packet_history[i];
    }

    packet_history[IOMON_HISTORY_LEN - 1] = packet_cnt;
}

void iomon_plot_packet_history(){
    unsigned char i = 0;
    unsigned char x_pt = IOMON_GRAPH_VERTEX_X+2, y_pt = 0;
    FbColor(YELLOW);
    for(i = 1; i < IOMON_HISTORY_LEN; i++, x_pt+=2){
        FbLine(x_pt-2, IOMON_GRAPH_Y_ZERO - packet_history[i-1],
                x_pt, IOMON_GRAPH_Y_ZERO - packet_history[i]);
    }
}

void iomon_init_packet_cnts(){
    unsigned char i = 0;
    for(i=0; i<IOMON_HISTORY_LEN; i++)
        packet_history[i] = 0; //quick_rand(i)%80;
}

unsigned char iomon_state = IOMON_INIT;
void iomon_cb(){
    switch(iomon_state){
        case IOMON_INIT:
            iomon_init_packet_cnts();
            iomon_state = IOMON_RUN;
            break;
        case IOMON_RUN:
            FbClear();
            iomon_draw_axis();
            iomon_plot_packet_history();
            FbSwapBuffers();
            iomon_state = IOMON_WAIT;
            break;
        case IOMON_WAIT:
            iomon_cnter++;
            if(iomon_cnter > 30000){
                iomon_cnter = 0;
                iomon_shift_in_packet_amount(num_packets_seen);
                iomon_state = IOMON_RUN;
                if(num_packets_seen){
                    num_packets_seen >>= 1;
                    setNote(130, 256);
                }
            }
            break;
    }

    PowerSaveIdle();
    if(BUTTON_PRESSED_AND_CONSUME){
        iomon_state = IOMON_RUN;
        returnToMenus();
    }

}
