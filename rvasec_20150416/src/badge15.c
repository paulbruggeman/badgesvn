#include "badge_common.h"
#include "badge15.h"
#include "touchCTMU.h"
#include "time_date.h"
#include "assetList.h"

//------------------------------LCD COLORS-------------------------------------

#define BLUE 0b0000000000011111
#define GREEN 0b0000011111100000
#define RED 0b1111100000000000
#define WHITE 0b1111111111111111
#define BLACK 0b0000000000000000

//-------------------------------END COLORS------------------------------------

//-------------------------------END STATES------------------------------------

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
