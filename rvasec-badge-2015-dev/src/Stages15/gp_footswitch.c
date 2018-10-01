#include "gop.h"
#define FOOTSWITCH_0_idx 7
#define FOOTSWITCH_1_idx 8
#define FOOTSWITCH_2_idx 9

extern unsigned char unlocked_stages;

unsigned char matched = 0;
void footswitch_0_interact(GP_StageState *stage_state)
{
    gp_state.footswitch.object_states[FOOTSWITCH_0_idx].seq_id = 1;

    red(50);
    green(0);
    blue(0);

    if(gp_state.footswitch.bg_color == RED)
    {
        matched |= 0b001;
        setNote(50,2048);
    }
    else
        matched = 0;
}

void footswitch_1_interact(GP_StageState *stage_state)
{
    gp_state.footswitch.object_states[FOOTSWITCH_1_idx].seq_id = 1;
    red(0);
    green(50);
    blue(0);


    if(gp_state.footswitch.bg_color == GREEN)
    {
        matched |= 0b010;
        setNote(50,2048);
    }
    else
        matched = 0;
}

void footswitch_2_interact(GP_StageState *stage_state)
{
    gp_state.footswitch.object_states[FOOTSWITCH_2_idx].seq_id = 1;
    red(0);
    green(0);
    blue(50);

    if(gp_state.footswitch.bg_color == BLUE)
    {
        matched |= 0b100;
        setNote(50,2048);
    }
    else
        matched = 0;
}
#define FOOTSWITCH_EXIT_IDX 6
void exit_footswitch(void)
{
    animate_door = FOOTSWITCH_EXIT_IDX;
    
    gp_state.next_stage = &gp_state.corridor;
}

#define NUM_NPCS 0
struct NPCState footswitch_npc_states[] =
{
};


#define NUM_OBJECTS 0
struct ObjectState footswitch_object_states[] =
{
    {1, 106, 32, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {4, 110, 32, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {1, 114, 32, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {4, 118, 32, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {1, 122, 32, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {4, 126, 32, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},

    {8, 106, 18, 26, GP_DOOR_1, 0, exit_footswitch, BTN_INTERACT,-1},

    //x , y, width, height, Sprite, seq_start, interact func
    {20,  60, 16, 8, GP_FOOTSWITCH, 0, footswitch_0_interact, AUTO_INTERACT, -1},
    {60,  40, 16, 8, GP_FOOTSWITCH, 0, footswitch_1_interact, AUTO_INTERACT, -1},
    {100,  60, 16, 8, GP_FOOTSWITCH, 0, footswitch_2_interact, AUTO_INTERACT|LAST_OBJ, -1},
    //{80,  60, 16, 8, GP_FOOTSWITCH, 0, footswitch_2_interact, AUTO_INTERACT|LAST_OBJ, -1},

};
short foot_switch_tick = 0;
void gp_footswitch_update(GP_UPDATE_PROC stage_proc)
{

    switch (stage_proc)
    {
        case INIT_STAGE:
            foot_switch_tick = 0;
            gp_state.footswitch.run_stage = gp_footswitch_update;
            gp_state.footswitch.window_x  = 0;
            gp_state.footswitch.window_y  = 0;
            gp_state.footswitch.max_x     = 0;
            gp_state.footswitch.max_y     = 0;
            gp_state.footswitch.last_player_x = 30;
            gp_state.footswitch.last_player_y = 116;

            gp_state.footswitch.npc_states = footswitch_npc_states;
            gp_state.footswitch.num_npcs   = NUM_NPCS;

            gp_state.footswitch.object_states = footswitch_object_states;
            gp_state.footswitch.num_objects   = NUM_OBJECTS;
            gp_state.footswitch.bg_color = 0b1111100000000000;
            gp_state.footswitch.draw = 1;
            break;
        case  START_STAGE:
            animate_door = 0;
            gp_state.footswitch.object_states[FOOTSWITCH_EXIT_IDX].seq_id = 0;
            player_state.x_new_pos = player_state.x_pos = gp_state.footswitch.last_player_x;
            player_state.y_new_pos = player_state.y_pos = gp_state.footswitch.last_player_y;
            break;
        case  UPDATE_STAGE:
            

            if(matched != 0b111)
            {
                foot_switch_tick++;

                if(foot_switch_tick < 3000)
                    gp_state.footswitch.bg_color = GREEN;
                else if (foot_switch_tick < 6000)
                    gp_state.footswitch.bg_color = BLUE;
                else if (foot_switch_tick < 9000)
                    gp_state.footswitch.bg_color = 0b1111100000000000;
                else
                    foot_switch_tick = 0;
            }
            
            if(matched == 0b111)
            {
                blue(0);
                green(0);
                red(0);
                setNote(100, 8192);
                // unlock the next stage
                unlocked_stages |= 0b10;
                matched = 0;
                unlock_image_asset(BITCOIN_UNLOCKABLE);
                gp_state.next_stage = &gp_state.corridor;
                gp_state.STAGE = GOP_STAGE_TRANSFER;
            }

            // reset the switches
            if(gp_state.actionable == 0)
            {
                gp_state.footswitch.object_states[FOOTSWITCH_0_idx].seq_id = 0;
                gp_state.footswitch.object_states[FOOTSWITCH_1_idx].seq_id = 0;
                gp_state.footswitch.object_states[FOOTSWITCH_2_idx].seq_id = 0;
                blue(0);
                green(0);
                red(0);
            }

            if(!gp_state.ticker)
            {
                if(animate_door )
                {
                    if(gp_state.footswitch.object_states[animate_door].seq_id < 2)
                        gp_state.footswitch.object_states[animate_door].seq_id++;
                    // Once the door is finished animating, load the corridor stage
                    else
                    {
                        animate_door = 0;
                        blue(0);
                        green(0);
                        //gp_state->NEXT_STAGE = GOP_CORRIDOR;
                        gp_state.STAGE = GOP_STAGE_TRANSFER;
                        //gp_state.next_stage = &gp_state.corridor;
                        //player_state.x_new_pos = player_state.x_pos = 130;
                        //player_state.y_new_pos = player_state.y_pos = 20;
                    }
                }
            }
            break;
        case  END_STAGE:
//            gp_state.footswitch.last_player_x = player_state.x_pos;
//            gp_state.footswitch.last_player_y = player_state.y_pos;
            break;
        case DRAW_LAYER_1:
            break;
    }

}