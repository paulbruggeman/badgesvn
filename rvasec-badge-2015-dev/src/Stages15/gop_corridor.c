#include "gop.h"

char progress = -1;

//char animate_door = 0;
void interact_door_1();
void interact_door_2();
void interact_door_3();
void interact_door_4();
void interact_door_5();

#define TOTAL_STAGES 4
extern unsigned char unlocked_stages;

#define NUM_CORRIDOR_NPCS 0
struct NPCState corridor_npc_states[] =
{
};

#define NUM_CORRIDOR_OBJECTS 1
struct ObjectState corridor_object_states[] =
{
    //x , y, width, height, Sprite, seq_start, interact func
    {0, 0, 240, 4, GP_SINGLE_BRICK, 0, 0, COLLIDABLE_OBJ, -1},
    {4, 4, 240, 4, GP_SINGLE_BRICK, 0, 0, COLLIDABLE_OBJ, -1},
    {0, 8, 240, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {4, 12, 240, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {0, 16, 240, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},
    {4, 20, 240, 4, GP_SINGLE_BRICK, 0, 0, 0, -1},

    {10, 0, 18, 26, GP_DOOR_1, 0, interact_door_1, BTN_INTERACT, -1},
    {120, 0, 18, 26, GP_DOOR_1, 0, interact_door_2, BTN_INTERACT,-1},
    {180, 0, 18, 26, GP_DOOR_1, 0, interact_door_3, BTN_INTERACT, -1},
    {240, 0, 18, 26, GP_DOOR_1, 0, interact_door_4, BTN_INTERACT, -1},
    {300, 0, 18, 26, GP_DOOR_1, 0, 0, 0, -1},
    {360, 0, 18, 26, GP_DOOR_1, 0, 0, 0, -1},

    
    {105, 4, 18, 13, GP_CORRIDOR_INDICATOR, 0, 0, 0, 10},
    {165, 4, 18, 13, GP_CORRIDOR_INDICATOR, 0, 0, 0, 10},
    {225, 4, 18, 13, GP_CORRIDOR_INDICATOR, 0, 0, LAST_OBJ, 10},

    //{180, 0, 18, 26, GP_DOOR_1, 0, 0, 0},
    //{220, 0, 18, 26, GP_DOOR_1, 0, 0, 0},
    //{260, 0, 18, 26, GP_DOOR_1, 0, 0, LAST_OBJ},
//   {92, 130, 32, 16, GP_SERVER_1, 3, interact_server, COLLIDABLE_OBJ | LAST_OBJ}

};

// door hidden to the left
void interact_door_1(void)
{
    // animate door 1 (animate_door - 1)
    animate_door = 1;

    // reset the door animation sequence
    gp_state.corridor.object_states[animate_door - 1].seq_id = 0;

    // Setup the next stage, but transfer doesn't hadppen
    // until gp_state.STAGE = GOP_STAGE_TRANSFER;
    gp_state.next_stage = &gp_state.intro_dungeon;
}

//footswitch
void interact_door_2(void)
{
    // animate door 1 (animate_door - 1)
    animate_door = 2;

    // reset the door animation sequence
    gp_state.corridor.object_states[animate_door - 1].seq_id = 0;

    // Setup the next stage, but transfer doesn't hadppen
    // until gp_state.STAGE = GOP_STAGE_TRANSFER;
    gp_state.next_stage = &gp_state.footswitch;
}

// telephone
void interact_door_3(void)
{
    if(unlocked_stages & 0b10)
    {
        // animate door 0 (animate_door - 1)
        animate_door = 3;

        // reset the door animation sequence
        gp_state.corridor.object_states[animate_door - 1].seq_id = 0;

        // Setup the next stage, but transfer doesn't hadppen
        // until gp_state.STAGE = GOP_STAGE_TRANSFER;
        gp_state.next_stage = &gp_state.telephone;
    }
    else
    {
        setNote(110, 2048);
    }

}

void interact_door_4(void)
{
    if(unlocked_stages & 0b100)
    {
        // animate door 0 (animate_door - 1)
        animate_door = 4;

        // reset the door animation sequence
        gp_state.corridor.object_states[animate_door - 1].seq_id = 0;

        // Setup the next stage, but transfer doesn't hadppen
        // until gp_state.STAGE = GOP_STAGE_TRANSFER;
        gp_state.next_stage = &gp_state.tetris;
    }
    else
    {
        setNote(110, 2048);
    }
}

void gp_corridor_update(GP_UPDATE_PROC stage_proc)
{
    
    unsigned char i = 0, j = 0;
    switch (stage_proc)
    {
        case INIT_STAGE:

            // The progress is unset - read from FLASH?
            if(progress == -1)
            {
                progress = 1;
            }

            gp_state.corridor.run_stage = gp_corridor_update;
            gp_state.corridor.window_x = 99;
            gp_state.corridor.window_y = 0;
            gp_state.corridor.max_x    = 500;
            gp_state.corridor.max_y    = 0;
            gp_state.corridor.last_player_x = 130;
            gp_state.corridor.last_player_y = 20;

            gp_state.corridor.npc_states = corridor_npc_states;
            gp_state.corridor.num_npcs   = NUM_CORRIDOR_NPCS;

            gp_state.corridor.object_states = corridor_object_states;
            gp_state.corridor.num_objects   = NUM_CORRIDOR_OBJECTS;
            gp_state.corridor.bg_color = BG_COLOR;
            gp_state.corridor.draw = 1;
            break;
        case  START_STAGE:
            animate_door = 0;
            player_state.x_new_pos = player_state.x_pos = gp_state.corridor.last_player_x;
            player_state.y_new_pos = player_state.y_pos = gp_state.corridor.last_player_y;
            do
                gp_state.corridor.object_states[i].seq_id = 0;
            while(!(gp_state.corridor.object_states[i++].FLAGS & LAST_OBJ));
            break;
        case  UPDATE_STAGE:
            if(player_state.y_pos > 70)
            {
                player_state.y_pos = 20;
                player_state.x_pos = 130;
                gp_state.STAGE = GOP_INIT;
                returnToMenus();
            }
            
            if(!gp_state.ticker)
            {
                if(animate_door )
                {
                    if(gp_state.corridor.object_states[animate_door + 5].seq_id < 2)
                        gp_state.corridor.object_states[animate_door + 5].seq_id++;
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

                // animate indicators
                for(i = 0; i < 3; i++)
                {
                    // Animate locked doors
                    //if( i >= progress)
                    if(!(unlocked_stages & (1 << i)))
                    {
                        if(gp_state.corridor.object_states[i + 12].seq_id < 2)
                            gp_state.corridor.object_states[i + 12].seq_id++;
                        else
                            gp_state.corridor.object_states[i + 12].seq_id = 0;

                    }
                    // Animate unlocked doors
                    else
                    {
                        if(gp_state.corridor.object_states[i + 12].seq_id < 5)
                            gp_state.corridor.object_states[i + 12].seq_id++;
                        else
                            gp_state.corridor.object_states[i + 12].seq_id = 3;
                                        
                    }
                }
            }
            break;
        case  END_STAGE:
            gp_state.corridor.last_player_x = player_state.x_pos;
            gp_state.corridor.last_player_y = player_state.y_pos;
            break;
        case DRAW_LAYER_1:
            DlMove(30,70);
            DlColor(WHITE);
            DlWriteLine("Walk Down");
            DlMove(35,80);
            DlWriteLine("To Exit");

            break;
    }
}
