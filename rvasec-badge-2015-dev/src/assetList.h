#ifndef assetList_h
#define assetList_h


/*
 NOTE
 NOTE   LASTASSET has to be the LAST item in the enum 
 NOTE   insert new enums before it.
 NOTE
*/
enum {
    DRBOB=0,
    HACKRVA4,

    //HACKRVA2,
    //HACKRVA1,

    RVASECBUG,
    RVASECBRAIN,
    //RVASEC2,

    MARIO,
    //GAMEOTHRONES,

    FONT,

    //BOWLBALL,
    //KITTEN,
    //SPLASH,
    JPUNIX_SYTEM,
    BITCOIN,
    CAKE_IS_LIE,

    CHIPSPRITE,
    BSOD,
    
    GP_BLACKHAT_PLAYER,
    GP_MALE_PLAYER,
    GP_SERVER_1,
    //GP_GREY_BRICK,
    GP_SINGLE_BRICK,
    GP_DOOR_1,
    GP_FOOTSWITCH,
    GP_CORRIDOR_INDICATOR,

    LASTASSET,
};

enum {
    AUDIO,
    MIDI,
    PICTURE1BIT,
    PICTURE2BIT,
    PICTURE4BIT,
    PICTURE8BIT,
};

struct asset {
    unsigned char assetId;
    unsigned char type;
    unsigned char seqNum; /* image within the asset for animattion, frame no. for font char id */
    unsigned short x;
    unsigned short y;
    const char *data_cmap;
    const char *pixdata;
    void (*datacb)(unsigned char, int);
};
extern const struct asset assetList[];

// too many assets to main asset enum to bother with
enum
{
/*
    SPLASH_UNLOCKABLE,
*/
    RVASECBRAIN_UNLOCKABLE,
    DRBOB_UNLOCKABLE,
    RVASECBUG_UNLOCKABLE,
    BSOD_UNLOCKABLE,
/*
    KITTEN_UNLOCKABLE,
*/
    HACKRVA4_UNLOCKABLE,
    GAME_OF_PWNS,
    JPUNIX_SYSTEM_UNLOCKABLE,
    BITCOIN_UNLOCKABLE,
    CAKE_IS_LIE_UNLOCKABLE,
    LAST_UNLOCKABLE
};

void unlock_image_asset(char lockable_asset_idx);
char draw_unlocked_asset(char unlocked_idx, char force);

#endif
