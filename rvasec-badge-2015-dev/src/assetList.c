/*
    simple assetList library
    Author: Paul Bruggeman
    paul@killercats.com
    4/2015
*/

#include "assetList.h"
#include "assets.h"
#include "colors.h"
#include "badge15.h"

#define DRBOB_LORES
#include "drbob.h"

//#define HACKRVA4BIT_LORES
#include "hackrva4bit.h"
//#define HACKRVA2BIT_LORES
//#include "hackrva2bit.h"
//#define HACKRVA1BIT_LORES
//#include "hackrva1bit.h"

#define RVASECBUGBIT_LORES
#include "rvasecbug2.h"
#define RVASEC2BIT_LORES
#include "rvasec2bit.h"
                     
#include "mario.h"

//#define GOT_LORES
//#include "got.h"

#include "font_2.xbm"

//#define BOWLBALL_LORES
//#include "bowlball2.h"

//#define KITTEN_LORES
//#include "kitten.h"

//#define SPLASH_LORES
//#include "splash.h"

#define CHIPSPRITE_LORES
#include "chipsprite.h"

#define BSOD_LORES
#include "BSOD.h"

#define GP_LORES
#include "GP_assets.h"

#include "rvasec_brain.h"
#include "extra_assets.h"

/* for 1 bit images */
/* testing color instead of BW */
const static char BW_cmap[2][3] = {
{ 0, 0, 0 },
{ 64, 192, 64 },
};

/*
   all pixels should fill a whole byte and be evenly divided.
   1 bit/pixel image has a min width of 8 pixels 
   2 bits/pixel = 4 pixels wide min. 
   4 bits/pixel = 2 pixels wide min.

 NOTE
 NOTE   if you add or remove an item ALSO add or remove it in the enum in assetList.h 
 NOTE

*/
const struct asset assetList[] = {
    { DRBOB, DRBOB_BITS, 1, DRBOB_WIDTH, DRBOB_HEIGHT, (const char *)DRBOB_CMAP, (const char *)DRBOB_DATA, (drawLCD8) },
    { HACKRVA4, HACKRVA4_BITS, 1, HACKRVA4_WIDTH, HACKRVA4_HEIGHT, (const char *)HACKRVA4_CMAP, (const char *)HACKRVA4_DATA, (drawLCD4) },

    //{ HACKRVA2, HACKRVA2_BITS, 1, HACKRVA2_WIDTH, HACKRVA2_HEIGHT, (const char *)HACKRVA2_CMAP, (const char *)HACKRVA2_DATA, (drawLCD4) },
    //{ HACKRVA1, HACKRVA1_BITS, 1, HACKRVA1_WIDTH, HACKRVA1_HEIGHT, (const char *)HACKRVA1_CMAP, (const char *)HACKRVA1_DATA, (drawLCD1) },

    { RVASECBUG, RVASECBUG_BITS, 1, RVASECBUG_WIDTH, RVASECBUG_HEIGHT, (const char *)RVASECBUG_CMAP, (const char *)RVASECBUG_DATA, (drawLCD2) },
    { RVASECBRAIN,RVASEC_BRAIN_BITS, 1, RVASEC_BRAIN_WIDTH, RVASEC_BRAIN_HEIGHT, (const char *)RVASEC_BRAIN_CMAP, (const char *)RVASEC_BRAIN_DATA, (drawLCD2) },
    //{ RVASEC2, RVASEC2_BITS, 1, RVASEC2_WIDTH, RVASEC2_HEIGHT, (const char *)RVASEC2_CMAP, (const char *)RVASEC2_DATA, (drawLCD2) },

    { MARIO, AUDIO, 1, marioSamples, 0, (const char *)marioDuration, (const char *)marioOfftime, (nextNote_cb) },
//    { GAMEOTHRONES, MIDI, 1, gameOfThronesSamples, 0, (const char *)gameOfThrones, (const char *)0, (nextMIDI_cb) },

    { FONT, PICTURE1BIT, 42, 8, 8, (const char *)BW_cmap, (const char *)font_2_bits, (nextNote_cb) },

    //{ BOWLBALL, BOWLBALL_BITS, 1, BOWLBALL_WIDTH, BOWLBALL_HEIGHT, (const char *)BOWLBALL_CMAP, (const char *)BOWLBALL_DATA, (drawLCD8) },

    //{ KITTEN, KITTEN_BITS, 1, KITTEN_WIDTH, KITTEN_HEIGHT, (const char *)KITTEN_CMAP, (const char *)KITTEN_DATA, (drawLCD8) },
    //{ SPLASH, SPLASH_BITS, 1, SPLASH_WIDTH, SPLASH_HEIGHT, (const char *)SPLASH_CMAP, (const char *)SPLASH_DATA, (drawLCD8) },
    { JPUNIX_SYTEM, JPUNIX_SYSTEM_BITS, 1, JPUNIX_SYSTEM_WIDTH, JPUNIX_SYSTEM_HEIGHT, (const char *)JPUNIX_SYSTEM_CMAP, (const char *)JPUNIX_SYSTEM_DATA, (drawLCD8) },
    { BITCOIN, BITCOIN_BITS, 1, BITCOIN_WIDTH, BITCOIN_HEIGHT, (const char *)BITCOIN_CMAP, (const char *)BITCOIN_DATA, (drawLCD8) },
    { CAKE_IS_LIE, CAKE_BITS, 1, CAKE_WIDTH, CAKE_HEIGHT, (const char *)CAKE_CMAP, (const char *)CAKE_DATA, (drawLCD8) },

    { CHIPSPRITE, CHIPSPRITE_BITS, 3, CHIPSPRITE_WIDTH, CHIPSPRITE_HEIGHT/3, (const char *)CHIPSPRITE_CMAP, (const char *)CHIPSPRITE_DATA, (drawLCD8) },
    { BSOD, BSOD_BITS, 1, BSOD_WIDTH, BSOD_HEIGHT, (const char *)BSOD_CMAP, (const char *)BSOD_DATA, (drawLCD1) },

    { GP_BLACKHAT_PLAYER, BLACKHAT_PLAYER_BITS, 3, BLACKHAT_PLAYER_WIDTH, BLACKHAT_PLAYER_HEIGHT/3, (const char *)BLACKHAT_PLAYER_CMAP, (const char *)BLACKHAT_PLAYER_DATA, (drawLCD4) },
    { GP_MALE_PLAYER, MALE_PLAYER_BITS, 3, MALE_PLAYER_WIDTH, MALE_PLAYER_HEIGHT/3, (const char *)MALE_PLAYER_CMAP, (const char *)MALE_PLAYER_DATA, (drawLCD4) },
    { GP_SERVER_1, SERVER_1_BITS, 5, SERVER_1_WIDTH, SERVER_1_HEIGHT/5, (const char *)SERVER_1_CMAP, (const char *)SERVER_1_DATA, (drawLCD4) },
    //{ GP_GREY_BRICK, GREY_BRICK_BITS, 1, GREY_BRICK_WIDTH, GREY_BRICK_HEIGHT, (const char *)GREY_BRICK_CMAP, (const char *)GREY_BRICK_DATA, (drawLCD4) },
    { GP_SINGLE_BRICK, SINGLE_BRICK_BITS, 1, SINGLE_BRICK_WIDTH, SINGLE_BRICK_HEIGHT, (const char *)SINGLE_BRICK_CMAP, (const char *)SINGLE_BRICK_DATA, (drawLCD4) },
    { GP_DOOR_1, DOOR_1_BITS, 3, DOOR_1_WIDTH, DOOR_1_HEIGHT/3, (const char *)DOOR_1_CMAP, (const char *)DOOR_1_DATA, (drawLCD8) },
    { GP_FOOTSWITCH, FOOTSWITCH_1_BITS, 2, FOOTSWITCH_1_WIDTH, FOOTSWITCH_1_HEIGHT/2, (const char *)FOOTSWITCH_1_CMAP, (const char *)FOOTSWITCH_1_DATA, (drawLCD8) },
    { GP_CORRIDOR_INDICATOR, CORRIDOR_INDICATOR_BITS, 6, CORRIDOR_INDICATOR_WIDTH, CORRIDOR_INDICATOR_HEIGHT/6, (const char *)CORRIDOR_INDICATOR_CMAP, (const char *)CORRIDOR_INDICATOR_DATA, (drawLCD8) }
};

#ifdef ALL_UNLOCKED
unsigned short unlocked_image_assets = 0xFFFF;
#else
unsigned short unlocked_image_assets = 0b1111;
#endif

void unlock_image_asset(char lockable_asset_idx)
{
    unlocked_image_assets |= ((1 << lockable_asset_idx) );
}

// Call to draw the nth unlocked asset
//e.g. draw_unlocked_asset(5) trys to draw the 5th unlockable asset
// if it isn't unlocked, will return 1, else returns 0
char draw_unlocked_asset(char unlocked_idx, char force)
{
    char i = 0, j = 0;
    // check if this images bit flag is unlocked
    if(!force && !((1 << unlocked_idx) & unlocked_image_assets))
        return 1;

    DlInit();
    DlClear();

    switch(unlocked_idx)
    {
/*
        case SPLASH_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(SPLASH, 0);
            break;

        case KITTEN_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(KITTEN, 0);
            break;
*/

        case DRBOB_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(DRBOB, 0);
            break;

        case BSOD_UNLOCKABLE:
            DlTransparentIndex(256);
            /* BSOD was drawn with blue == bit on, white = bits off */
            /* 1 bit image display decode to fg == bit on, bg = bits off */
            DlBackgroundColor(WHITE);
            DlColor(BLUE);
            /* image is 128x128. 1 bit images have to be divisable by 8 for byte boundary */
            DlMove(2, 2);
            DlPicture(BSOD, 0);
            break;

        case RVASECBUG_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(RVASECBUG, 0);
            break;

        case RVASECBRAIN_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(RVASECBRAIN,0);
            break;

        case HACKRVA4_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(HACKRVA4, 0);
            break;
            
        case GAME_OF_PWNS:
            DlTransparentIndex(3);

            for(i = 0; i < 120 ; i+=8)
            {
                for(j = 0; j < 120 ; j+=8)
                {
                    DlMove(i,j);
                    DlPicture(GP_BLACKHAT_PLAYER, i%3);
                }
            }

            break;

        case JPUNIX_SYSTEM_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(JPUNIX_SYTEM, 0);
            break;

        case BITCOIN_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(BITCOIN, 0);
            break;

        case CAKE_IS_LIE_UNLOCKABLE:
            DlMove(0,0);
            DlPicture(CAKE_IS_LIE, 0);
            break;
    }
    
    return 0;
}
