/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */



#ifndef STOPWATCH_H    /* Guard against multiple inclusion */
#define STOPWATCH_H

//global stopwatch variables
extern unsigned int g_blinktimer;

extern volatile int sw_ctf_sound;
extern volatile int sw_ctf_test;

//Prototypes
void process_100ms_stopwatch(void);

#endif 

/* *****************************************************************************
 End of File
 */
