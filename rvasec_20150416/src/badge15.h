/* 
 * File:   badge14.h
 * Author: morgan
 *
 * Created on March 28, 2014, 8:24 PM
 */

/* in case this gets included more than once */
#ifndef badge15_h
#define badge15_h

#include <stdlib.h>
#include "touchCTMU.h"
#include "assets.h"
#include "assetList.h"
#include "time_date.h"
#include "menu.h"
#include "timer1_int.h"
#include "LCDcolor.h"
#include "composite.h"
#include "S6B33.h"
#include "colors.h"

#define uCHAR (unsigned char *)

typedef struct badge_state{//structure that controls state flow

     unsigned char current_state;//used by run_states to determine current badge state
     unsigned char previous_state;//used to return to previous state
     unsigned char selected_object;//counter for choosing options in states
     unsigned char state_drawn;//used to redraw state on change in selected object
     unsigned char counter1;
     unsigned char counter2;
     unsigned short large_counter;
}badge_state;


void init_states(void);//initializes states at badge start


#endif
