#include "update_time.h"
#include "../badge15.h"
#include "../time_date.h"

extern badge_state b_state;//badge state structure to reduce variables

enum UPDATE_TIME_STATES
{
    UPDATE_TIME_INIT,
    UPDATE_TIME_DRAW_SELECTION,
    UPDATE_TIME_GET_INPUT,
    UPDATE_TIME_DELAY,
    UPDATE_TIME_EXIT
};

#define GREEN 0b0000011111100000
#define RED 0b1111100000000000

#define UPDATE_TIME_STATE b_state.counter1
#define UPDATE_TIME_STRING_COUNTER b_state.counter2
#define UPDATE_TIME_SELECTED b_state.selected_object
#define UPDATE_TIME_WAIT_COUNTER b_state.large_counter
#define UPDATE_TIME_IS_DRAWN b_state.state_drawn

void update_badge_time(void){

    badge_time_last = badge_time[4];

    getTime_Date(badge_time, badge_date);

    if(badge_time_last == badge_time[4])
    {
        return;
    }
    else{
        //do something to draw time and date
    }
}

void update_time_Run(){

    switch(UPDATE_TIME_STATE)
    {
        case UPDATE_TIME_INIT:
	    getTime_Date(badge_time_set, badge_date_set);
            UPDATE_TIME_IS_DRAWN = 0;
	    UPDATE_TIME_SELECTED = 0;
            UPDATE_TIME_WAIT_COUNTER = 0;
            UPDATE_TIME_STRING_COUNTER = 0;
            UPDATE_TIME_STATE++;
            break;

        case UPDATE_TIME_DRAW_SELECTION:
            update_time_draw_selction();
            break;        

        case UPDATE_TIME_GET_INPUT:
            update_time_get_input();
            break;

        case UPDATE_TIME_DELAY:
            UPDATE_TIME_WAIT_COUNTER++;
            if (UPDATE_TIME_WAIT_COUNTER == 250)
            { 
                UPDATE_TIME_WAIT_COUNTER = 0;
                UPDATE_TIME_STATE = UPDATE_TIME_DRAW_SELECTION;
            }
            break;

        case UPDATE_TIME_EXIT:
	    setTime_Date(badge_time_set,badge_date_set);
            UPDATE_TIME_STATE = UPDATE_TIME_INIT;
            returnToMenus();
            break;

        default:
            break;
    }
}

void update_time_draw_selction(){

    unsigned char y;
    unsigned short color = GREEN;

    if(UPDATE_TIME_IS_DRAWN == 0){
        if(UPDATE_TIME_STRING_COUNTER == 0)
        {
            DlInit();
            DlBackgroundColor(BLACK);
            DlClear();
            DlColor(color);
            //Probably need to add more code to
            //initialize your other counters here
        }

        if(UPDATE_TIME_STRING_COUNTER == UPDATE_TIME_SELECTED)
            color = RED;

        if(UPDATE_TIME_STRING_COUNTER < 6)
        {
            y = 10;
            DlColor(color);
            DlMove(10+(UPDATE_TIME_STRING_COUNTER*6), y);
            DlCharacter(badge_time_set[UPDATE_TIME_STRING_COUNTER]);
        }
        else
        {
            y = 30;
            DlColor(color);
            DlMove( 10+(UPDATE_TIME_STRING_COUNTER*6-36), y);
            DlCharacter(badge_date_set[UPDATE_TIME_STRING_COUNTER-6]);
        }

        if(UPDATE_TIME_STRING_COUNTER == 13){
	    DlSwapBuffers();
            UPDATE_TIME_IS_DRAWN = 1;
            UPDATE_TIME_STATE++;
        }

        UPDATE_TIME_STRING_COUNTER++;
    }
}

void update_time_get_input(){

    if (BUTTON_PRESSED_AND_CONSUME){
        update_time_onExit();
    }
    else if(TOP_SLIDE_AND_CONSUME){
        setNote(109, 2048);
        update_time_up();
        UPDATE_TIME_STATE++;
    }
    else if (BOTTOM_SLIDE_AND_CONSUME){
        setNote(97, 2048);
        update_time_down();
        UPDATE_TIME_STATE++;
    }
    else if (LEFT_SLIDE_AND_CONSUME){
        setNote(109, 2048);
        update_time_left();
        UPDATE_TIME_STATE++;
    }
    else if (RIGHT_SLIDE_AND_CONSUME){
        setNote(97, 2048);
        update_time_right();
        UPDATE_TIME_STATE++;
    }
}


void update_time_onExit(){
    UPDATE_TIME_STATE = UPDATE_TIME_EXIT;
}

void update_time_up() {
    if(UPDATE_TIME_SELECTED < 6){
      switch(UPDATE_TIME_SELECTED){
	case 0://hour 1
	  if(badge_time_set[0] == '1')
	    badge_time_set[0] = '0';
	  else
	    badge_time_set[0]++;
	  break;
	case 1://hour 2
	  if(badge_time_set[0] == '0'){
	    if(badge_time_set[1] == '9')
	      badge_time_set[1] = '0';
	    else
	      badge_time_set[1]++;
	  }
	  else{
	    if(badge_time_set[1] == '2')
	      badge_time_set[1] = '0';
	    else
	      badge_time_set[1]++;
	  }
	  break;
	case 3://minute 1
	  if(badge_time_set[3] == '5')
	    badge_time_set[3] = '0';
	  else
	    badge_time_set[3]++;
	  break;
	case 4://minute 2
	  if(badge_time_set[4] == '9')
	    badge_time_set[4] = '0';
	  else
	    badge_time_set[4]++;
	  break;
	case 5://AM/PM
	  if(badge_time_set[5] == 'A')
	    badge_time_set[5] = 'P';
	  else
	    badge_time_set[5] = 'A';
	  break;
      }
    }
    else{
	switch(UPDATE_TIME_SELECTED){
	  case 6://m
	    if(badge_date_set[0] == '1')
	      badge_date_set[0] = '0';
	    else
	      badge_date_set[0]++;
	    break;
	  case 7://m
	    if(badge_date_set[0] == '0'){
	      if(badge_date_set[1] == '9')
		badge_date_set[1] = '0';
	      else
		badge_date_set[1]++;
	    }
	    else{
	      if(badge_date_set[1] == '2')
		badge_date_set[1] = '0';
	      else
		badge_date_set[1]++;
	    }
	    break;
	  case 9://d
	    if(badge_date_set[3] == '3')
	     badge_date_set[3] = '0';
	    else
	     badge_date_set[3]++;
	    break;
	  case 10://d
	    if(badge_date_set[3] < '3'){
	      if(badge_date_set[4] == '9')
		badge_date_set[4] = '0';
	      else
		badge_date_set[4]++;
	    }
	    else{
	      if(badge_date_set[4] == '1')
		badge_date_set[4] = '0';
	      else
		badge_date_set[4]++;
	    }
	    break;
	  case 12://y
	    if(badge_date_set[6] == '9')
	      badge_date_set[6] = '0';
	    else
	      badge_date_set[6]++;
	    break;
	  case 13://y
	    if(badge_date_set[7] == '9')
	      badge_date_set[7] = '0';
	    else
	      badge_date_set[7]++;
	    break;
        }
    }
    UPDATE_TIME_STRING_COUNTER = 0;
    UPDATE_TIME_IS_DRAWN = 0;	
}

void update_time_down() {
    if(UPDATE_TIME_SELECTED < 6){
      switch(UPDATE_TIME_SELECTED){
	case 0://hour 1
	  if(badge_time_set[0] == '0')
	    badge_time_set[0] = '1';
	  else
	    badge_time_set[0]--;
	  break;
	case 1://hour 2
	  if(badge_time_set[0] == '0'){
	    if(badge_time_set[1] == '0')
	      badge_time_set[1] = '9';
	    else
	      badge_time_set[1]--;
	  }
	  else{
	    if(badge_time_set[1] == '0')
	      badge_time_set[1] = '2';
	    else
	      badge_time_set[1]--;
	  }
	  break;
	case 3://minute 1
	  if(badge_time_set[3] == '0')
	    badge_time_set[3] = '5';
	  else
	    badge_time_set[3]--;
	  break;
	case 4://minute 2
	  if(badge_time_set[4] == '0')
	    badge_time_set[4] = '9';
	  else
	    badge_time_set[4]--;
	  break;
	case 5://AM/PM
	  if(badge_time_set[5] == 'A')
	    badge_time_set[5] = 'P';
	  else
	    badge_time_set[5] = 'A';
	  break;
      }
    }
    else{
	switch(UPDATE_TIME_SELECTED){
	  case 6://d
	    if(badge_date_set[0] == '0')
	     badge_date_set[0] = '3'; 
	    else
	     badge_date_set[0]--;
	    break;
	  case 7://d
	    if(badge_date_set[0] < '3'){
	      if(badge_date_set[1] == '0')
		badge_date_set[1] = '9';
	      else
		badge_date_set[1]--;
	    }
	    else{
	      if(badge_date_set[1] == '0')
		badge_date_set[1] = '1';
	      else
		badge_date_set[1]--;
	    }
	    break;
	  case 9://m
	    if(badge_date_set[3] == '0')
	      badge_date_set[3] = '1';
	    else
	      badge_date_set[3]--;
	    break;
	  case 10://m
	    if(badge_date_set[3] == '0'){
	      if(badge_date_set[4] == '0')
		badge_date_set[4] = '9';
	      else
		badge_date_set[4]--;
	    }
	    else{
	      if(badge_date_set[4] == '0')
		badge_date_set[4] = '2';
	      else
		badge_date_set[4]--;
	    }
	    break;
	  case 12://y
	    if(badge_date_set[6] == '0')
	      badge_date_set[6] = '9';
	    else
	      badge_date_set[6]--;
	    break;
	  case 13://y
	    if(badge_date_set[7] == '0')
	      badge_date_set[7] = '9';
	    else
	      badge_date_set[7]--;
	    break;
        }
    }
    UPDATE_TIME_STRING_COUNTER = 0;
    UPDATE_TIME_IS_DRAWN = 0;
}

void update_time_left() {
  if(UPDATE_TIME_SELECTED == 3 ||
     UPDATE_TIME_SELECTED == 9 ||
     UPDATE_TIME_SELECTED == 12){
     UPDATE_TIME_SELECTED -= 2;
  }
  else if(UPDATE_TIME_SELECTED == 0){
     UPDATE_TIME_SELECTED = 13;
  }
  else{UPDATE_TIME_SELECTED--;
  }
  UPDATE_TIME_STRING_COUNTER = 0;
  UPDATE_TIME_IS_DRAWN = 0;
}

void update_time_right() {
  if(UPDATE_TIME_SELECTED == 1 ||
     UPDATE_TIME_SELECTED == 7 ||
     UPDATE_TIME_SELECTED == 10){
     UPDATE_TIME_SELECTED += 2;
  }
  else if(UPDATE_TIME_SELECTED == 13){
     UPDATE_TIME_SELECTED = 0;
  }
  else{UPDATE_TIME_SELECTED++;
  }
  UPDATE_TIME_STRING_COUNTER = 0;
  UPDATE_TIME_IS_DRAWN = 0;
}




