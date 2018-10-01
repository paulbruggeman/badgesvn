#include "stopwatch.h"


//Global stopwatch variables
//volatile int sw_ghost_sound=0;
volatile int sw_ghost_test=0;

/*
 * Used the variables directly since i was having trouble with the function scope
 * working in multiple files. Not sure why.
//decrements are handled by the timer isr function process_stopwatch()
void sw_decrement(int *counter){
    if(*counter != 0){
        *counter--;
    }
}

char sw_running(int *counter){
    if(*counter != 0){
        return 1;
    }else{
        return 0;
    }
}

char sw_finished(int *counter){
    if(*counter == 0){
        return 1;
    }else{
        return 0;
    }
}
*/