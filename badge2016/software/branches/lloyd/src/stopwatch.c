/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "stopwatch.h"


//Global stopwatch variables
unsigned int g_blinktimer=0;
volatile int sw_ctf_test=0;

void process_100ms_stopwatch(void){
    
    if(g_blinktimer != 0){
        g_blinktimer--;
    }
    
    if(sw_ctf_test != 0){
        sw_ctf_test--;
    }
}
