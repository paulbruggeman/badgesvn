/* 
 * File:   gop.h
 * Author: morgan
 *
 * Created on May 9, 2015, 4:36 PM
 */

#ifndef GOP_H
#define	GOP_H

#include "badge15.h"
#include "touchCTMU.h"
#include "../assetList.h"
#define LCD_XSIZE 132
#define LCD_YSIZE 132

#define MOVE_DISTANCE 4
#define BG_COLOR GREY4
#define ACTIONABLE_RANGE 32
#define GAME_RATE 300

struct menu_t *gop_interact_popup;
MENU_STYLE popup_style;

char GP_RAND, animate_door;
struct GP_State gp_state;
struct PlayerState player_state;
struct GP_StageState;

// Indicate what stage we are in
typedef enum {
   GOP_INIT,
   GOP_STAGE_TRANSFER,
   GOP_RUN_STAGE,
   GOP_ACTION_MENU,
   GOP_NULL_STAGE,
   GOP_EXIT,
} GP_STAGE;

// tell stage update CB what it should be doing
// Using to avoid too many func pointers
typedef enum {
    INIT_STAGE,
    START_STAGE,
    UPDATE_STAGE,
    END_STAGE,
    DRAW_LAYER_1,
} GP_UPDATE_PROC;


typedef enum  {
    NPC_STILL,
    NPC_RAND_EXPLORE,
} NPC_MODE;

//typedef enum {
//    COLLIDES
//} OBJECT_FLAGS;
#define COLLIDABLE_OBJ (1 << 1)
#define INVISIBLE_OBJ (1 << 2)
#define LAST_OBJ (1 << 3)
#define AUTO_INTERACT (1 << 4)
#define BTN_INTERACT (1 << 5)

typedef struct ObjectState
{
    // to pleft of sprite?
    short x_pos;
    short y_pos;

    // for hit box
    unsigned char width, height;

    unsigned char sprite;

    unsigned char seq_id;

    void (*actionable)(struct GP_StageState *stage_state);

    unsigned char FLAGS;
    char transparency_idx;
} ObjectState;

typedef struct PlayerState
{
    //char name[] = "Player";
    short x_pos;
    short y_pos;
    short x_new_pos;
    short y_new_pos;
    char redraw;
    unsigned char avatar;
} PlayerState;

typedef struct NPCState
{
    //char name[] = "Player";
    short x_pos;
    short y_pos;
    short x_new_pos;
    short y_new_pos;
    char redraw;
    unsigned char avatar;
    unsigned char transparency_idx;
    unsigned char updating;
    NPC_MODE mode;
    void (*actionable)(struct GP_StageState *stage_state);
} NPCState;

typedef struct GP_StageState
{
    short window_x, window_y;
    short max_x, max_y;
    // only needed for some stages - those with a few exits/doors
    // could make these static in the stage update method to save space?
    short last_player_x, last_player_y;
    unsigned char num_npcs;
    unsigned char num_objects;

    NPCState *npc_states;
    ObjectState *object_states;

    unsigned short bg_color;
    unsigned char draw;
    void (*run_stage)(GP_UPDATE_PROC stage_state);
} GP_StageState;

typedef struct GP_State
{
    GP_STAGE STAGE, NEXT_STAGE;
    unsigned char ticker;
    void (*actionable)(GP_StageState *stage_state);

    GP_StageState intro_dungeon,
                  corridor,
                  tetris,
                  footswitch,
                  telephone;

    GP_StageState *running_stage, *next_stage;

} GP_State;

char is_actionable_range(short x_pos,
                         short y_pos,
                         unsigned char  width,
                         unsigned char  height);

void print_text_box(short x_pos, short y_pos,
                    unsigned char width, unsigned char height,
                    char *txt);

void gop_cb();

void interact_basic_end();
void interact_door();
void interact_sysadmin(GP_StageState *stage_state);
void interact_server(GP_StageState *stage_state);

void gp_intro_dungeon_update(GP_UPDATE_PROC stage_state);
void gp_corridor_update(GP_UPDATE_PROC stage_state);
void gp_tetris_search_update(GP_UPDATE_PROC stage_proc);
void gp_footswitch_update(GP_UPDATE_PROC stage_proc);
#endif	/* GOP_H */

