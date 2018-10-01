/* ************************************************************************** */
/** Descriptive File Name

  @Company
    HackRVA

  @File Name
    display.h

  @Summary
    Include file for display.c

  @Description
    Include file for display.c
 */
/* ************************************************************************** */

#ifndef DISPLAY_H   /* Guard against multiple inclusion */
#define DISPLAY_H

#include "colors.h"

//Prototypes
void display_movecursor(unsigned char x, unsigned char y);
void display_writeline(const char * charin, unsigned short color);


#endif 

/* *****************************************************************************
 End of File
 */
