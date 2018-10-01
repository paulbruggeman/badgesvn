#include "badge15.h"
#include "gop.h"
extern struct GP_State gp_state = {GOP_INIT, 0, 0 };

extern struct PlayerState player_state;

#ifdef ALL_UNLOCKED
unsigned char unlocked_stages = 0xFF;
#else
unsigned char unlocked_stages = 0x1;
#endif
void print_text_box(short x_pos, short y_pos,
                    unsigned char width, unsigned char height,
                    char *txt)
{
    DlMove(x_pos, y_pos);
    //DlTransparency(0b111101111011110);
    //DlColor(0b111101111011110);
    //DlFilledRectangle(width, height);

    DlMove(x_pos + 1, y_pos + 1);
    DlColor(BLUE);
    DlWriteLine(txt);
}

char is_actionable_range(short x_pos,
                         short y_pos,
                         unsigned char  width,
                         unsigned char  height)
{
    // TODO: this should use edges, not centers
    int x_diff = (player_state.x_pos + 8) - (x_pos + width/2);
    int y_diff = (player_state.y_pos + 8) - (y_pos + height/2);

    if( x_diff > -ACTIONABLE_RANGE && x_diff < ACTIONABLE_RANGE
        && y_diff > -ACTIONABLE_RANGE && y_diff < ACTIONABLE_RANGE)
        return 1;
    else
        return 0;
}

void handle_escape()
{
//    if(runningApp != 0)
//        returnToMenus();

    //blue(50);
    menu_escape_cb = 0;
}

void gop_init()
{
    // G_font = FONT35;
    DlInit();
    DlBackgroundColor(BG_COLOR);
    DlClear();
    DlTransparentIndex(3);
    
    //menu_escape_cb = handle_escape;

    popup_style = DRBOB;
    player_state.x_pos = player_state.x_new_pos  = 60;
    player_state.y_pos = player_state.y_new_pos  = 70;
    player_state.redraw = 0;
    player_state.avatar = GP_BLACKHAT_PLAYER;

    // INIT The GP state
    gp_state.actionable = NULL;

    //-------
    //Intro dungeon (really intro test server room for now)
    gp_intro_dungeon_update(INIT_STAGE);

    //-------
    //The corridor
    gp_corridor_update(INIT_STAGE);

    //-------
    //Couples tetris
    gp_tetris_search_update(INIT_STAGE);

    //-------
    //Footswitch
    gp_footswitch_update(INIT_STAGE);

    //-------
    //Telephone
    gp_telephone_update(INIT_STAGE);

    // TODO: READ FLASH FOR SAVE STATE
    gp_state.STAGE = GOP_STAGE_TRANSFER;
    gp_state.running_stage = 0;
    gp_state.next_stage    = &gp_state.corridor;
}

char in_render_window(GP_StageState * stage_state,
                      short x, short y, 
                      unsigned char width, unsigned char height,
                      char allow_x_clip)
{

    unsigned char x_check = (x + width > stage_state->window_x) && (x + width < stage_state->window_x + 132);
    if (allow_x_clip)
        x_check = x_check || (x > stage_state->window_x) && (x < stage_state->window_x + 132);
    else
        x_check = x_check && (x > stage_state->window_x) && (x < stage_state->window_x + 132);
    //= (x + width > stage_state->window_x) && (x + width < stage_state->window_x + 132)  // right side is within window
    //        || (x > stage_state->window_x) && (x < stage_state->window_x + 132); // left side is within window
    //-----------
    // Compositor doesn't like try to paritally render low bit sprites since the divisibility can
    // be off ( i think this is why), so for now only allow render if the x axis of the sprite
    // is entirely in the window. Uncomment above if we fix (use hires sprites or change compositor)
    //= (x + width > stage_state->window_x) && (x + width < stage_state->window_x + 132)  // right side is within window
    //        && (x > stage_state->window_x) && (x < stage_state->window_x + 132); // left side is within window

    unsigned char y_check
        = (y + height > stage_state->window_y) && (y + height < stage_state->window_y + 132)  // bottom side is within window
            || (y > stage_state->window_y) && (y < stage_state->window_y + 132); // top side is within window

    if(x_check && y_check)
        return 1;
    else
        return 0;
}

void draw_objects(GP_StageState *stage_state)
{
    unsigned char repeat_x, repeat_y;
    unsigned char x_check, y_check;
    unsigned short tmp_x, tmp_y;
    unsigned char i = 0, j = 0, k = 0;
    unsigned char is_in_window = 0, is_any_in_window = 0;
    
    // don't do anything if the state has no objects
    if (stage_state->object_states == 0)
        return ;

    // Loops until LAST_OBJ flag
    for(i = 0; ; i++)
    {
        is_any_in_window = 0;

        // how many sub-sprites to draw?
        repeat_x = stage_state->object_states[i].width / assetList[stage_state->object_states[i].sprite].x;
        repeat_y = stage_state->object_states[i].height / assetList[stage_state->object_states[i].sprite].y;

        for(j = 0; j < repeat_x; j++)
        {
            for(k = 0; k < repeat_y; k++)
            {
                // avoid repeating these calculations
                tmp_x = stage_state->object_states[i].x_pos 
                            + (j * assetList[stage_state->object_states[i].sprite].x );
                tmp_y = stage_state->object_states[i].y_pos 
                           + (k * assetList[stage_state->object_states[i].sprite].y );

                // check if this sub-sprite is viewable
                is_in_window = in_render_window(stage_state,
                                                  tmp_x,
                                                  tmp_y,
                                                  assetList[stage_state->object_states[i].sprite].x,
                                                  assetList[stage_state->object_states[i].sprite].y,
                                                  assetList[stage_state->object_states[i].sprite].type == PICTURE8BIT ? 1: 0);

                if(is_in_window)
                {
                    DlMove(tmp_x - stage_state->window_x, tmp_y - stage_state->window_y);

                    if(stage_state->object_states[i].transparency_idx != -1)
                        DlTransparentIndex(stage_state->object_states[i].transparency_idx);
                    
                    DlPicture(stage_state->object_states[i].sprite,
                              stage_state->object_states[i].seq_id);
                }

                // Are any of the sub-sprites (parts of the object) viewable?
                is_any_in_window |= is_in_window;
            }
        }

        // It's not actionable nor is it collidable unless part of it is in the viewable window
        // ... May cause problems, but quick hack for now
        if(is_any_in_window)
        {
            x_check = player_state.x_new_pos + assetList[player_state.avatar].x > stage_state->object_states[i].x_pos;
            x_check = x_check && player_state.x_new_pos < stage_state->object_states[i].x_pos + stage_state->object_states[i].width;

            y_check = player_state.y_new_pos + assetList[player_state.avatar].y > stage_state->object_states[i].y_pos;
            y_check = y_check && player_state.y_new_pos < stage_state->object_states[i].y_pos + stage_state->object_states[i].height;

            // COLLISION
            if(x_check && y_check)
            {
                // Collidable objects reset the player's position
                if(stage_state->object_states[i].FLAGS & COLLIDABLE_OBJ)
                {
                    // reset the positions
                    player_state.y_new_pos = player_state.y_pos;
                    player_state.x_new_pos = player_state.x_pos;
                }

                // set auto interact ( colliding within something e.g. footswitch)
                if(stage_state->object_states[i].FLAGS & AUTO_INTERACT
                        && stage_state->object_states[i].actionable != 0)
                {
                    //stage_state->object_states[i].actionable(stage_state);
                    gp_state.actionable = stage_state->object_states[i].actionable;
                    gp_state.actionable(stage_state);
                }
            }
            // If we are not near this object, but this object has
            // the actionable, reset the actionable (e.g. moving away
            // from auto actionable)
            else if(stage_state->object_states[i].actionable == gp_state.actionable
                    && stage_state->object_states[i].FLAGS & AUTO_INTERACT)
                gp_state.actionable = 0;

            // Object is button interactable
            if(stage_state->object_states[i].FLAGS & BTN_INTERACT)
            {
                if(is_actionable_range(stage_state->object_states[i].x_pos,
                                        stage_state->object_states[i].y_pos,
                                        stage_state->object_states[i].width,
                                        stage_state->object_states[i].height))
                    gp_state.actionable = stage_state->object_states[i].actionable;
                // Only reset due to out of range if  the global actionable was this object
                else if(stage_state->object_states[i].actionable == gp_state.actionable)
                    gp_state.actionable = 0;
            }
        }
        
        if(stage_state->object_states[i].FLAGS & LAST_OBJ)
            break;
    }
}

void draw_npcs(GP_StageState *stage_state)
{
    unsigned char i = 0;
    short x_distance = 0, y_distance = 0;
    unsigned char is_in_window = 0;
    for(i = 0; i < stage_state->num_npcs; i++)
    {
        is_in_window = in_render_window(stage_state, stage_state->npc_states[i].x_pos,
                                          stage_state->npc_states[i].y_pos,
                                          assetList[stage_state->npc_states[i].avatar].x,
                                          assetList[stage_state->npc_states[i].avatar].y,
                assetList[stage_state->npc_states[i].avatar].type == PICTURE8BIT ? 1: 0);

        if(stage_state->npc_states[i].redraw != -1 && is_in_window)
        {
            DlTransparentIndex(5);
            DlMove(stage_state->npc_states[i].x_pos - stage_state->window_x,
                   stage_state->npc_states[i].y_pos - stage_state->window_y);
            
            DlPicture(stage_state->npc_states[i].avatar,
                      stage_state->npc_states[i].redraw);
        }

        // don't bother actionable if it isn't viewable - would be annoying
        if(stage_state->npc_states[i].actionable != NULL && is_in_window)
        {
            x_distance = stage_state->npc_states[i].x_pos - player_state.x_pos;
            y_distance = stage_state->npc_states[i].y_pos - player_state.y_pos;

            // if we are within actionable range, set the actionable call to this 
            if(x_distance < ACTIONABLE_RANGE && x_distance > -ACTIONABLE_RANGE
               && y_distance < ACTIONABLE_RANGE && y_distance > -ACTIONABLE_RANGE)
                gp_state.actionable = stage_state->npc_states[i].actionable;

            // Otherwise reset the global actionable
            else if( stage_state->npc_states[i].actionable == gp_state.actionable)
                gp_state.actionable = NULL;
        }
    }
}

void draw_player(GP_StageState *stage_state)
{
    // Player moving to right edge of window
    if(player_state.x_new_pos > stage_state->window_x + 105)
        if(stage_state->window_x + MOVE_DISTANCE <= stage_state->max_x)
            stage_state->window_x += MOVE_DISTANCE;
        else
            stage_state->window_x = stage_state->max_x;
    
    // Player moving to left edge of window
    else if(player_state.x_new_pos < stage_state->window_x + 15)
        if(stage_state->window_x < MOVE_DISTANCE)
            stage_state->window_x = 0;
        else
            stage_state->window_x -= MOVE_DISTANCE;

    // Player moving to bottom of window
    if(player_state.y_new_pos > stage_state->window_y + 105)
        if(stage_state->window_y + MOVE_DISTANCE <= stage_state->max_y)
            stage_state->window_y += MOVE_DISTANCE;
        else
            stage_state->window_y = stage_state->max_y;

    // Player moving to top of window
    if(player_state.y_new_pos < stage_state->window_y + 15)
        if(stage_state->window_y  < MOVE_DISTANCE)
            stage_state->window_y = 0;
        else
            stage_state->window_y -= MOVE_DISTANCE;

    // Bound player movement to windows viewable space
    // Bound max first
    if(player_state.x_new_pos > stage_state->max_x + LCD_XSIZE - 16)
        player_state.x_new_pos = stage_state->max_x + LCD_XSIZE - 16;
    else if(player_state.x_new_pos < 0)
        player_state.x_new_pos = 0;
    
    if(player_state.y_new_pos > stage_state->max_y + LCD_YSIZE - 16)
        player_state.y_new_pos = stage_state->max_y + LCD_YSIZE - 16;
    else if(player_state.y_new_pos < 0)
        player_state.y_new_pos = 0;
    

    player_state.x_pos = player_state.x_new_pos;
    player_state.y_pos = player_state.y_new_pos;

    DlMove(player_state.x_pos - stage_state->window_x,
           player_state.y_pos - stage_state->window_y);
    DlTransparentIndex(3);
    DlPicture(player_state.avatar, player_state.redraw);
    //player_state.redraw = -1;
}

void draw(GP_StageState *stage_state)
{
    DlInit();

    DlBackgroundColor(stage_state->bg_color);
    DlClear();

    draw_objects(stage_state);

    stage_state->run_stage(DRAW_LAYER_1);
    draw_npcs(stage_state);

    draw_player(stage_state);

    //print_text_box(30, 100, 50, 15, "Nothing...");
}

void moveable(GP_StageState *stage_state)
{
    if (BUTTON_PRESSED_AND_CONSUME)
    {
        if (gp_state.actionable != NULL)
            gp_state.actionable(stage_state);
        else
        {
            red(0);
            green(0);
            blue(0);
        }
    }

    if(TOP_TAP_AND_CONSUME)
    {
        player_state.y_new_pos = player_state.y_pos - MOVE_DISTANCE;
        player_state.redraw = 0;
        GP_RAND ^= (3 ^ gp_state.ticker);
        //setNote(173, 2048);
    }

    if (BOTTOM_TAP_AND_CONSUME)
    {
        player_state.y_new_pos = player_state.y_pos + MOVE_DISTANCE;
        player_state.redraw = 0;
        GP_RAND ^= (5 ^ gp_state.ticker);
        //setNote(173, 2048);
    }

    if (LEFT_TAP_AND_CONSUME)
    {
        player_state.x_new_pos = player_state.x_pos - MOVE_DISTANCE;
        player_state.redraw = 2;
        GP_RAND ^= (7 ^ gp_state.ticker);
        //setNote(173, 2048);
    }

    if (RIGHT_TAP_AND_CONSUME)
    {
        player_state.x_new_pos = player_state.x_pos + MOVE_DISTANCE;
        player_state.redraw = 1;
        GP_RAND ^= (11 ^ gp_state.ticker);
        //setNote(173, 2048);
    }
}

void interact_basic_end(){
    gp_state.STAGE = GOP_RUN_STAGE;
}

extern unsigned char sync_time;
void gop_cb()
{
    // Main GOP loop - init GOP, transfer between GOP stages, run those stages,
    //                and run special methods (e.g. aciton menu)
    switch(gp_state.STAGE) {
        case GOP_INIT:
            gop_init();
            break;
        case GOP_STAGE_TRANSFER:
            // End the running stage
            if(gp_state.running_stage != 0)
                gp_state.running_stage->run_stage(END_STAGE);

            // Start the next stage
            if(gp_state.next_stage != 0)
                gp_state.next_stage->run_stage(START_STAGE);

            gp_state.running_stage = gp_state.next_stage;
            gp_state.next_stage = 0;
            gp_state.STAGE = GOP_RUN_STAGE;
            break;
        case GOP_RUN_STAGE:

            // get IO to move player - may need to abstract later
            // (may not always be moving player with touch pads)
            moveable(gp_state.running_stage);
            
            gp_state.running_stage->run_stage(UPDATE_STAGE);
            // only draw every once in a while
            if(!gp_state.ticker++ && gp_state.running_stage->draw)
                draw(gp_state.running_stage);
            break;
        case GOP_ACTION_MENU:
            genericMenu((struct menu_t *)gop_interact_popup, popup_style);
            if(sync_time)
                sync_time++;
            break;
    }
}

extern struct menu_t telephone_main_m[];
extern unsigned char call_count;
extern unsigned short last_seen[];

extern struct menu_t sync_main_m[];
extern unsigned char sync_count ;
extern unsigned char sync_thresh[];
extern unsigned char sync_time;

void update_menu_count();
void update_sync_menu();

#define TELEPHONE_WIN_AMNT 2

char last = 0;
char search_seen_and_insert(unsigned short id)
{
    unsigned char i;
    for(i = 0; i < 5; i++)
    {
        // already seen
        if(last_seen[i] == id)
            return 1;
        // initial fills
        else if(last_seen[i] == 0)
        {
            last_seen[i] = id;
            return 0;
        }
    }
    // At this point, no open spots, and this is a new call

    // replacement is at the end, go to start
    if(last == 4)
        last = 0;

    // replace an existing one
    last_seen[last] = id;
    last++;
    return 0;

}

void gop_ir_cb(struct IRpacket_t p)
{
    unsigned short id = 0;
    // telephone request
    if(0x1000 & p.data )
    {
        union IRpacket_u pkt;

        pkt.p.command = IR_WRITE;

        pkt.p.address = IR_APP0;

        // Send to all badges
        pkt.p.badgeId = 0;
        pkt.p.data    = 0x2000 | (0x01FF & G_sysData.badgeId);

        IRqueueSend(pkt);
        //setNote(70, 1024);
    }
    // telephone repsonse
    else if(0x2000 & p.data)
    {

        id = p.data & 0xfff;
        if(!search_seen_and_insert(id))
        {
            // new person seen
            call_count++;
            update_menu_count();
            setNote(30,2048);
            if(call_count >= TELEPHONE_WIN_AMNT)
            {
                 strcpy(telephone_main_m[6].name, "Complete");
                 unlocked_stages |= 0b100;
            }
        }
        else
        {
            // already seen person - give brown note
            setNote(200,2048);
        }
    }
    // sync
    else if(0x4000 & p.data)
    {
        // sync time was set and we are under the threshold
        if(sync_time != 0 && sync_time <= sync_thresh[sync_count])
        {
            if(sync_count < 4)
            {
                sync_count++;
                update_sync_menu();
            }
            else
                strcpy(sync_main_m[3].name, "Complete");
        }
        else
        {
            // give brown note
            setNote(200,2048);
        }
    }
    else if(0x8000 & p.data)
    {
        unlocked_stages = p.data & 0xFF;
    }
    else
        setNote(220, 8192);
}
