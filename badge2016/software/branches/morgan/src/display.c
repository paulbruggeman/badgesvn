/* ************************************************************************** */
/** Descriptive File Name

  @Company
    HackRVA

  @File Name
    display.c

  @Summary
    Functions to help display things to the badge screen

  @Description
    Functions to help display things to the badge screen
 */
/* ************************************************************************** */

//Includes
#include "display.h"
#include "fb.h"



//Globals
#define CHAR_HEIGHT 12
#define CHAR_WIDTH  6
#define MIN_Y   2
#define MAX_Y   124
#define MIN_X   1
#define MAX_X   124

/** x cursor position in pixels (not characters) */
unsigned int x_cursor_pos = 0;
/** y cursor position in pixels (not characters) */
unsigned int y_cursor_pos = 0;


/** 
  @Function
    void display_movecursor(unsigned char x, unsigned char y) 

  @Summary
    Places Cursor at desired screen location

  @Remarks
    This function is using its own definitions for character spacing rather than
    the Fb spacing. One spacing is one character width or height. It allows you
    to move the cursor as though you were writing to a character lcd.
 */
/**
 * Places cursor at desired screen location.
 * 
 * This function is using its own definitions for character spacing rather than
 * the Fb spacing. One spacing is one character width or height. It allows you
 * to move the cursor as though you were writing to a character LCD.
 * 
 * @param x x coordinate in # of characters (0-20)
 * @param y y coordinate in # of characters (
 */
void display_movecursor(unsigned char x, unsigned char y){
    y_cursor_pos=y*CHAR_HEIGHT;
    x_cursor_pos=x*CHAR_WIDTH;
    FbMove(x_cursor_pos, y_cursor_pos);
}

/** 
  @Function
    void display_writeline(const char * charin, unsigned short color) 

  @Summary
    Prints string at current cursor position

  @Remarks
    Improved writeline function. Does not need length of string passed to it and
    automatically wraps around the screen. 
 */
/**
 * 
 * @param charin
 * @param color
 */
void display_writeline(const char * charin, unsigned short color)
{
    unsigned int index=0;
    
    //Bound limits
    if(y_cursor_pos < MIN_Y ){
        y_cursor_pos=MIN_Y;
    }
    if(y_cursor_pos > MAX_Y){
        y_cursor_pos=MAX_Y;
    }
    
    if(x_cursor_pos < MIN_X){
        x_cursor_pos=MIN_X;
    }
    if(x_cursor_pos > MAX_X){
        x_cursor_pos=MAX_X;
    }
    
    FbColor(color);
    
    
    while(charin[index] != '\0'){
        
        if(charin[index] != '\n' && charin[index] != '\r'){
            FbMove(x_cursor_pos, y_cursor_pos);
            FbCharacter(charin[index]);
            
            x_cursor_pos=x_cursor_pos + CHAR_WIDTH;
            if(x_cursor_pos >= MAX_X){
                x_cursor_pos = MIN_X;
                y_cursor_pos = y_cursor_pos + CHAR_HEIGHT;
            }
        }else{
            y_cursor_pos = y_cursor_pos + CHAR_HEIGHT;
            x_cursor_pos = MIN_X;
        }
        
        if(y_cursor_pos >= MAX_Y){
            y_cursor_pos = MIN_Y;
        }
        
        
        
        
        index++;
    }
    
}
/* *****************************************************************************
 End of File
 */
