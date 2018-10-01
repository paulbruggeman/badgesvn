#include "gop.h"
#include "telephone.h"
//#include "badge15.h"

unsigned char call_count = 0;
unsigned short last_seen[5] = {0,0,0,0,0};

//sysadmin interaction
struct menu_t telephone_main_m[] = {

    {"You found a", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"telephone...", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"Count:", HORIZ_ITEM|SKIP_ITEM, TEXT, NULL},
    {"", HORIZ_ITEM|SKIP_ITEM, TEXT, NULL},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, NULL},
    {"Call", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)make_call} },
    {"Done", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)(end_telephone)} },
};

void make_call()
{
    union IRpacket_u pkt;

    pkt.p.command = IR_WRITE;

    pkt.p.address = IR_APP0;

    // Send along this badges ID
    pkt.p.badgeId = 0;
    pkt.p.data    = 0x1000 | (0x1FF & G_sysData.badgeId);;

    IRqueueSend(pkt);
    setNote(70, 1024);
}

void update_menu_count()
{
    telephone_main_m[4].name[0] = '0' + ((call_count/1000)%10);
    telephone_main_m[4].name[1] = '0' + ((call_count/100)%10);
    telephone_main_m[4].name[2] = '0' + ((call_count/10)%10);
    telephone_main_m[4].name[3] = '0' + (call_count%10);
}

void end_telephone()
{
    gp_state.STAGE = GOP_STAGE_TRANSFER;
    gp_state.next_stage = &gp_state.corridor;
}

void gp_telephone_update(GP_UPDATE_PROC stage_proc)
{
    switch(stage_proc)
    {
        case INIT_STAGE:

            gp_state.telephone.run_stage = gp_telephone_update;
            gp_state.telephone.window_x  = 0;
            gp_state.telephone.window_y  = 0;
            gp_state.telephone.max_x     = 0;
            gp_state.telephone.max_y     = 0;
            gp_state.telephone.last_player_x = 30;
            gp_state.telephone.last_player_y = 116;

            gp_state.telephone.npc_states = 0;
            gp_state.telephone.num_npcs   = 0;

            gp_state.telephone.object_states = 0;
            gp_state.telephone.num_objects   = 0;
            gp_state.telephone.bg_color = 0b1111100000000000;
            gp_state.telephone.draw = 0;
            // TODO load call count from flash

            break;
        case START_STAGE:
            DlInit();
            DlClear();
            update_menu_count();

            break;
        case UPDATE_STAGE:
            gop_interact_popup = telephone_main_m;
            popup_style = WHITE_ON_BLACK;
            gp_state.STAGE     = GOP_ACTION_MENU;
            break;
        case END_STAGE:
            break;
        case DRAW_LAYER_1:
            break;

    }
}