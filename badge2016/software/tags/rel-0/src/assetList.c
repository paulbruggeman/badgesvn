/*
    simple assetList library
    Author: Paul Bruggeman
    paul@killercats.com
    4/2015
*/

#include "badge16.h"
#include "assetList.h"
#include "assets.h"
#include "colors.h"

//#define HACKRVA4BIT_LORES
#include "hackrva4bit.h"

//#define RVASECBUGBIT_LORES
#include "rvasecbug2.h"
                     
#include "font_2.xbm"

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
    { HACKRVA4, HACKRVA4_BITS, 1, HACKRVA4_WIDTH, HACKRVA4_HEIGHT, (const char *)HACKRVA4_CMAP, (const char *)HACKRVA4_DATA, (drawLCD4) },
    { RVASECBUG, RVASECBUG_BITS, 1, RVASECBUG_WIDTH, RVASECBUG_HEIGHT, (const char *)RVASECBUG_CMAP, (const char *)RVASECBUG_DATA, (drawLCD2) },
    { FONT, PICTURE1BIT, 42, 8, 8, (const char *)BW_cmap, (const char *)font_2_bits, (nextNote_cb) },
};
