#include "gop.h"

//char animate_door = 0;

#define NUM_NPCS 1
struct NPCState intro_npc_states[] =
{
    {20, 70, 20, 70, 0, GP_MALE_PLAYER, 0, 0, NPC_RAND_EXPLORE, interact_sysadmin}
};

#define NUM_OBJECTS 7
struct ObjectState intro_object_states[] =
{
    //x , y, width, height, Sprite, seq_start, interact func
    {1,  50, 48, 16, GP_SERVER_1, 0, 0, COLLIDABLE_OBJ, -1},
    {1, 20, 48, 16, GP_SERVER_1, 0, 0, COLLIDABLE_OBJ, -1},
    {92, 20, 32, 16, GP_SERVER_1, 1, 0, COLLIDABLE_OBJ, -1},

    {92, 40, 32, 16, GP_SERVER_1, 2, 0, COLLIDABLE_OBJ, -1},
    {92, 60, 32, 16, GP_SERVER_1, 3, interact_server, COLLIDABLE_OBJ|BTN_INTERACT, -1},

    {60, 0, 18, 26, GP_DOOR_1, 0, interact_door, BTN_INTERACT, -1},
    {92, 130, 32, 16, GP_SERVER_1, 3, 0, COLLIDABLE_OBJ, -1},
    {92, 180, 16, 32, GP_SERVER_1, 3, 0, COLLIDABLE_OBJ, -1},
    {1, 230, 16, 32, GP_SERVER_1, 3, 0, COLLIDABLE_OBJ | LAST_OBJ, -1}
};

void gp_intro_dungeon_update(GP_UPDATE_PROC stage_proc)
{
    unsigned char i = 0;
    if(stage_proc == INIT_STAGE)
    {
        gp_state.intro_dungeon.run_stage = gp_intro_dungeon_update;
        gp_state.intro_dungeon.window_x  = 0;
        gp_state.intro_dungeon.window_y  = 0;
        gp_state.intro_dungeon.max_x     = 100;
        gp_state.intro_dungeon.max_y     = 200;

        gp_state.intro_dungeon.npc_states = intro_npc_states;
        gp_state.intro_dungeon.num_npcs   = NUM_NPCS;

        gp_state.intro_dungeon.object_states = intro_object_states;
        gp_state.intro_dungeon.num_objects   = NUM_OBJECTS;
        gp_state.intro_dungeon.bg_color = BG_COLOR;
        gp_state.intro_dungeon.draw = 1;
    }
    else if (stage_proc == START_STAGE )
    {
        animate_door = 0;
        // set where the player starts in the stage
        player_state.x_pos = player_state.x_new_pos  = 60;
        player_state.y_pos = player_state.y_new_pos  = 30;
        return;
    }
    else if (stage_proc == END_STAGE)
        return;
    else if(stage_proc != UPDATE_STAGE)
        return;

    if(!gp_state.ticker)
    {
        // make some servers blink
        for(i = 0; i < 5; i++)
        {
            if(gp_state.intro_dungeon.object_states[i].seq_id == 4)
                gp_state.intro_dungeon.object_states[i].seq_id = 3;
            else if(gp_state.intro_dungeon.object_states[i].seq_id == 3)
                gp_state.intro_dungeon.object_states[i].seq_id = 4;
            else if (gp_state.intro_dungeon.object_states[i].seq_id == 2)
                gp_state.intro_dungeon.object_states[i].seq_id = 0;
            else
                gp_state.intro_dungeon.object_states[i].seq_id++;
        }

        if(animate_door)
        {
            if(gp_state.intro_dungeon.object_states[5].seq_id < 2)
                gp_state.intro_dungeon.object_states[5].seq_id++;
            // Once the door is finished animating, load the corridor stage
            else
            {
                animate_door = 0;
                blue(0);
                green(0);
                //gp_state->NEXT_STAGE = GOP_CORRIDOR;
                gp_state.STAGE = GOP_STAGE_TRANSFER;
                gp_state.next_stage = &gp_state.corridor;
                //player_state.x_new_pos = player_state.x_pos = 130;
                //player_state.y_new_pos = player_state.y_pos = 20;
            }
        }
    }
}

// door rinteraction
void interact_door()
{
    animate_door = 1;

    // reset the door animation sequence
    gp_state.intro_dungeon.object_states[5].seq_id = 0;
}


//sysadmin interaction
struct menu_t sysadmin_interact_popup[] = {
    {"You're late,", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"again! There's", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {" a machine down", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"...go fix it.", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"", VERT_ITEM, TEXT, {0} },
    {"OK", VERT_ITEM|DEFAULT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)(interact_basic_end)} },
};


void interact_sysadmin(GP_StageState *stage_state)
{
    gop_interact_popup = sysadmin_interact_popup;
    popup_style = DRBOB_MENU_STYLE;
    gp_state.STAGE     = GOP_ACTION_MENU;
}


// SERVER interaction
void use_debug_skills()
{
    red(50);
    green(0);
    blue(0);
    //gp_state.STAGE = GOP_INTRO_DUNGEON;
    gp_state.STAGE = GOP_RUN_STAGE;
    unlock_image_asset(DRBOB_UNLOCKABLE);
}

void reboot_intro_server()
{
    red(0);
    green(50);
    blue(0);
    //gp_state.STAGE = GOP_INTRO_DUNGEON;
    gp_state.STAGE = GOP_RUN_STAGE;
    unlock_image_asset(BSOD_UNLOCKABLE);
}


struct menu_t server_interact_popup[] = {
    {"It's broken...", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"It's Windows...", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"---------", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"Use dbg skill", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)(use_debug_skills)} },
    {"Reboot", VERT_ITEM|LAST_ITEM, FUNCTION,{(struct menu_t *)(reboot_intro_server)}},
};

void interact_server(GP_StageState *stage_state)
{
    gop_interact_popup = server_interact_popup;
    popup_style = WHITE_ON_BLACK;
    gp_state.STAGE     = GOP_ACTION_MENU;
}


