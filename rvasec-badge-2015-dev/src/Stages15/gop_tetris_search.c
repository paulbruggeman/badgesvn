#include "gop.h"

unsigned char sync_count = 0;
unsigned char sync_thresh[] = {255, 200, 150, 100, 50};
unsigned char sync_time = 0;

void do_sync()
{
    union IRpacket_u pkt;

    pkt.p.command = IR_WRITE;

    pkt.p.address = IR_APP0;

    // Send along this badges ID
    pkt.p.badgeId = 0;
    pkt.p.data    = 0x4000 | G_sysData.badgeId;

    IRqueueSend(pkt);
    setNote(70, 1024);
    sync_time = 1;
}

void end_sync()
{
    gp_state.STAGE = GOP_STAGE_TRANSFER;
    gp_state.next_stage = &gp_state.corridor;
}


struct menu_t sync_main_m[] = {

    {"Need a sync...", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    //{"telephone...", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"Count:", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"Go!", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)do_sync} },
    {"Done", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)(end_sync)} },
};

void update_sync_menu()
{
    sync_main_m[2].name[0] = '0' + ((sync_count/1000)%10);
    sync_main_m[2].name[1] = '0' + ((sync_count/100)%10);
    sync_main_m[2].name[2] = '0' + ((sync_count/10)%10);
    sync_main_m[2].name[3] = '0' + (sync_count%10);
}

void gp_tetris_search_update(GP_UPDATE_PROC stage_proc)
{
    switch (stage_proc)
    {
        case INIT_STAGE:
            gp_state.tetris.run_stage = gp_tetris_search_update;
            gp_state.tetris.window_x  = 0;
            gp_state.tetris.window_y  = 0;
            gp_state.tetris.max_x     = 0;
            gp_state.tetris.max_y     = 0;

            gp_state.tetris.npc_states = 0;
            gp_state.tetris.num_npcs   = 0;

            gp_state.tetris.object_states = 0;
            gp_state.tetris.num_objects   = 0;
            gp_state.tetris.bg_color = 0;
            gp_state.tetris.draw = 0;
            break;
        case  START_STAGE:
            unlock_image_asset(HACKRVA4_UNLOCKABLE);
            unlock_image_asset(JPUNIX_SYSTEM_UNLOCKABLE);
            unlock_image_asset(GAME_OF_PWNS);
            unlock_image_asset(CAKE_IS_LIE_UNLOCKABLE);

            DlInit();
            DlClear();
            //update_menu_count();
            update_sync_menu();
//            animate_door = 0;
//            player_state.x_new_pos = player_state.x_pos = 30;
//            player_state.y_new_pos = player_state.y_pos = 20;
            break;
        case  UPDATE_STAGE:
            gop_interact_popup = sync_main_m;
            popup_style = WHITE_ON_BLACK;
            gp_state.STAGE     = GOP_ACTION_MENU;
            break;
        case  END_STAGE:
            break;
        case DRAW_LAYER_1:
            break;
    }
}
