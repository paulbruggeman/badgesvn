#include "badge15.h"
#include "pop_up.h"

// If ordering changed, make sure indices still work
// in the populate function
//TODO: Multiple IMGs so that pop_up hax can
// cycle through them in order sending them out.
struct menu_t pop_up_config_m[] = {
    {"IMG: ", VERT_ITEM, FUNCTION, {(struct menu_t *)set_img}},
    {"", VERT_ITEM|SKIP_ITEM, TEXT, 0},
    {"Go!!", VERT_ITEM|DEFAULT_ITEM, FUNCTION, {(struct menu_t *)set_pu_go} },
    {"Exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *) set_pu_exit}},
};

enum
{
    INIT,
    SHOW_MENU,
    CONFIG_IMG,
    RUN_POP_UP
};

char img_id = -1;
char last_img_id = -1;
char pu_state = 0;
void set_img()
{
    // show *something* on click, find
    // first unlocked
    if(img_id == -1)
    {
        while(img_id++ < LAST_UNLOCKABLE
                && draw_unlocked_asset(img_id, 0));

        if(img_id >= LAST_UNLOCKABLE)
            img_id = last_img_id;

        pu_populate_menu();
    }
    else
        draw_unlocked_asset(img_id, 0);

    pu_state = CONFIG_IMG;
}

void set_pu_go()
{
    union IRpacket_u pkt;
    //if(bl_mode == BCAST_ONLY || bl_mode == LOCAL_AND_BCAST)
    {
        pkt.p.command = IR_WRITE;
        pkt.p.address = IR_DRAW_UNLOCKABLE;
        pkt.p.badgeId = 0;
        pkt.p.data = img_id;
        IRqueueSend(pkt);
    }

    //if(bl_mode == LOCAL_ONLY || bl_mode == LOCAL_AND_BCAST)
    {
    }
}

void set_pu_exit()
{
    pu_state = INIT;
    returnToMenus();
}

void pu_populate_menu()
{
    if(img_id == -1)
    {
        pop_up_config_m[0].name[5] = 'N';
        pop_up_config_m[0].name[6] = 'o';
        pop_up_config_m[0].name[7] = 'n';
        pop_up_config_m[0].name[8] = 'e';
        pop_up_config_m[0].name[9] = 0;
    }
    else
    {
        pop_up_config_m[0].name[5] = '0' + (img_id/100) % 10;
        pop_up_config_m[0].name[6] = '0' + (img_id/10) % 10;
        pop_up_config_m[0].name[7] = '0' + img_id % 10;
        pop_up_config_m[0].name[8] = 0;
    }
}

void pop_up_cb()
{
    last_img_id = img_id;
    switch(pu_state)
    {
        case INIT:
            pu_populate_menu();
            pu_state++;
            break;
        case SHOW_MENU:
            genericMenu((struct menu_t *)pop_up_config_m, MAIN_MENU_STYLE);
            break;
        case CONFIG_IMG:            
            if(BUTTON_PRESSED_AND_CONSUME)
            {
                pu_state = SHOW_MENU;
            }
            else if(TOP_TAP_AND_CONSUME)
            {
                // TODO:
                // check for unlocked
                // check that it is usable (e.g. not chipsprite or GP sprites)
                while(img_id++ < LAST_UNLOCKABLE
                        && draw_unlocked_asset(img_id, 0));

                if(img_id >= LAST_UNLOCKABLE)
                    img_id = last_img_id;

                last_img_id = img_id;
                pu_populate_menu();

            }
            else if(BOTTOM_TAP_AND_CONSUME)
            {
                while(img_id-- > -1
                        && draw_unlocked_asset(img_id, 0));

                if(img_id <= -1)
                    img_id = last_img_id;

                last_img_id = img_id;
                pu_populate_menu();
            }
            break;
    }
}