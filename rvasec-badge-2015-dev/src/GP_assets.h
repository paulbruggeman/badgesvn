// I'm keeping it granular since its only a minor amount of work but keeps
// us flexible. Example: We want everything lores except for a few sprites
// then we can just comment out their lores defines in the below block.

//TODO: lores and hires are the same. Lores images should be 2BIT, but I'm
//not sure how to produce the output that the 2bit mode uses. 
#ifdef GP_LORES
#define BLACKHAT_PLAYER_LORES
#define GREY_BRICK_LORES
#define SERVER_1_LORES
#define MALE_PLAYER_LORES
#define DOOR_1_LORES
#define FOOTSWITCH_1_LORES
#define CORRIDOR_INDICATOR_LORES
#define SINGLE_BRICK_LORES
#endif

#ifdef SINGLE_BRICK_LORES
    #define SINGLE_BRICK_BITS PICTURE2BIT
    #define SINGLE_BRICK_WIDTH gp_single_brick_width
    #define SINGLE_BRICK_HEIGHT gp_single_brick_height
    #define SINGLE_BRICK_CMAP gp_single_brick_data_cmap
    #define SINGLE_BRICK_DATA gp_single_brick_data
    #include "assets/lores/gp_single_brick.h"
#else
    #define SINGLE_BRICK_BITS PICTURE2BIT
    #define SINGLE_BRICK_WIDTH gp_single_brick_width
    #define SINGLE_BRICK_HEIGHT gp_single_brick_height
    #define SINGLE_BRICK_CMAP gp_single_brick_data_cmap
    #define SINGLE_BRICK_DATA gp_single_brick_data
    #include "assets/lores/gp_single_brick.h"
#endif


#ifdef CORRIDOR_INDICATOR_LORES
    #define CORRIDOR_INDICATOR_BITS PICTURE8BIT
    #define CORRIDOR_INDICATOR_WIDTH gp_corridor_indicator_width
    #define CORRIDOR_INDICATOR_HEIGHT gp_corridor_indicator_height
    #define CORRIDOR_INDICATOR_CMAP gp_corridor_indicator_data_cmap
    #define CORRIDOR_INDICATOR_DATA gp_corridor_indicator_data
    #include "assets/lores/gp_corridor_indicator.h"
#else
    #define CORRIDOR_INDICATOR_BITS PICTURE8BIT
    #define CORRIDOR_INDICATOR_WIDTH gp_corridor_indicator_width
    #define CORRIDOR_INDICATOR_HEIGHT gp_corridor_indicator_height
    #define CORRIDOR_INDICATOR_CMAP gp_corridor_indicator_data_cmap
    #define CORRIDOR_INDICATOR_DATA gp_corridor_indicator_data
    #include "assets/hires/gp_corridor_indicator.h"
#endif


#ifdef FOOTSWITCH_1_LORES
    #define FOOTSWITCH_1_BITS PICTURE2BIT
    #define FOOTSWITCH_1_WIDTH gp_footswitch_width
    #define FOOTSWITCH_1_HEIGHT gp_footswitch_height
    #define FOOTSWITCH_1_CMAP gp_footswitch_data_cmap
    #define FOOTSWITCH_1_DATA gp_footswitch_data
    #include "assets/lores/gp_footswitch.h"
#else
    #define FOOTSWITCH_1_BITS PICTURE8BIT
    #define FOOTSWITCH_1_WIDTH gp_footswitch_width
    #define FOOTSWITCH_1_HEIGHT gp_footswitch_height
    #define FOOTSWITCH_1_CMAP gp_footswitch_data_cmap
    #define FOOTSWITCH_1_DATA gp_footswitch_data
    #include "assets/hires/gp_footswitch.h"
#endif

#ifdef DOOR_1_LORES
    #define DOOR_1_BITS PICTURE2BIT
    #define DOOR_1_WIDTH gp_door_1_width
    #define DOOR_1_HEIGHT gp_door_1_height
    #define DOOR_1_CMAP gp_door_1_data_cmap
    #define DOOR_1_DATA gp_door_1_data
    #include "assets/lores/gp_door_1_2bit.h"
#else
    #define DOOR_1_BITS PICTURE8BIT
    #define DOOR_1_WIDTH gp_door_1_width
    #define DOOR_1_HEIGHT gp_door_1_height
    #define DOOR_1_CMAP gp_door_1_data_cmap
    #define DOOR_1_DATA gp_door_1_data
    #include "assets/hires/gp_door_1.h"
#endif

#ifdef BLACKHAT_PLAYER_LORES
   #define BLACKHAT_PLAYER_BITS PICTURE8BIT
   #define BLACKHAT_PLAYER_WIDTH gp_blackhat_player_width
   #define BLACKHAT_PLAYER_HEIGHT gp_blackhat_player_height
   #define BLACKHAT_PLAYER_CMAP blackhat_player_data_cmap
   #define BLACKHAT_PLAYER_DATA blackhat_player_data
   #include "assets/lores/gp_blackhat_player.h"
#else
   #define BLACKHAT_PLAYER_BITS PICTURE4BIT
   #define BLACKHAT_PLAYER_WIDTH gp_blackhat_player_width
   #define BLACKHAT_PLAYER_HEIGHT gp_blackhat_player_height
   #define BLACKHAT_PLAYER_CMAP blackhat_player_data_cmap
   #define BLACKHAT_PLAYER_DATA blackhat_player_data
   #include "assets/lores/gp_blackhat_player.h"
#endif

#ifdef MALE_PLAYER_LORES
   #define MALE_PLAYER_BITS PICTURE8BIT
   #define MALE_PLAYER_WIDTH gp_male_player_width
   #define MALE_PLAYER_HEIGHT gp_male_player_height
   #define MALE_PLAYER_CMAP male_player_data_cmap
   #define MALE_PLAYER_DATA male_player_data
   #include "assets/lores/gp_male_hero.h"
#else
   #define MALE_PLAYER_BITS PICTURE4BIT
   #define MALE_PLAYER_WIDTH gp_male_player_width
   #define MALE_PLAYER_HEIGHT gp_male_player_height
   #define MALE_PLAYER_CMAP male_player_data_cmap
   #define MALE_PLAYER_DATA male_player_data
   #include "assets/lores/gp_male_hero.h"
#endif



#ifdef SERVER_1_LORES
   #define SERVER_1_BITS PICTURE8BIT
   #define SERVER_1_WIDTH gp_server_1_width
   #define SERVER_1_HEIGHT gp_server_1_height
   #define SERVER_1_CMAP server_1_data_cmap
   #define SERVER_1_DATA server_1_data
   #include "assets/lores/gp_server_1.h"
#else
   #define SERVER_1_BITS PICTURE4BIT
   #define SERVER_1_WIDTH gp_server_1_width
   #define SERVER_1_HEIGHT gp_server_1_height
   #define SERVER_1_CMAP server_1_data_cmap
   #define SERVER_1_DATA server_1_data
   #include "assets/lores/gp_server_1.h"
#endif

#ifdef GREY_BRICK_LORES
   #define GREY_BRICK_BITS PICTURE8BIT
   #define GREY_BRICK_WIDTH gp_grey_brick_width
   #define GREY_BRICK_HEIGHT gp_grey_brick_height
   #define GREY_BRICK_CMAP grey_brick_data_cmap
   #define GREY_BRICK_DATA grey_brick_data
   #include "assets/lores/gp_grey_brick.h"
#else
   #define GREY_BRICK_BITS PICTURE4BIT
   #define GREY_BRICK_WIDTH gp_grey_brick_width
   #define GREY_BRICK_HEIGHT gp_grey_brick_height
   #define GREY_BRICK_CMAP grey_brick_data_cmap
   #define GREY_BRICK_DATA grey_brick_data
   #include "assets/lores/gp_grey_brick.h"
#endif

