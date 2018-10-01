#include "badge15.h"

char G_debug[80];


char rot13b(char c, char basis){
  c = (((c-basis)+13)%26)+basis;
  return c;
}


void rot13(char in_rot[]){
    unsigned char i = 0;
    while(in_rot[i] != '\0')
    {
        if('a' <= in_rot[i] && in_rot[i] <= 'z'){
            in_rot[i] = rot13b(in_rot[i],'a');
        } else if ('A' <= in_rot[i] && in_rot[i] <= 'Z') {
            in_rot[i] = rot13b(in_rot[i], 'A');
        }
        i++;
    }
}

/*
   will be save/restored from flash
*/
const unsigned short flashedBadgeId=0xefbe; /* MAGIC value for script. It will be replaced by awk script in final flashing */
struct sysData_t G_sysData;
unsigned short G_peerBadgeId; /* who we are talking to */


badge_state b_state;//badge state structure

/**************************[STATE INITIALIZATION]*****************************
 *    Called only once during start up to provide entry point                *
 *****************************************************************************/

void init_states(void){
   b_state.previous_state = 0;
   b_state.current_state = 0;
   b_state.state_drawn = 0;
   b_state.selected_object = 0;
   b_state.counter1 = 0;
   b_state.counter2 = 0;

}
