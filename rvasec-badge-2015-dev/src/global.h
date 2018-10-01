#ifndef global_h
#define global_h

#ifdef NOTYET
#include "assetList.h"
#include "composite.h"

/*
   beginings of a global struct
*/

#define LCD_XSIZE 132
#define LCD_YSIZE 132

struct globalDl_t {
   struct displayList_t Dl; /* the display list */
   unsigned short scanLine[LCD_XSIZE]; /* the output buffer */
   unsigned char font; /* default font */
};

struct globalMenu_t {
};

struct globals_t {
   struct globalDl_t Dl;
   struct globalMenu_t Menu;
};

extern struct globals_t G;

#endif

#endif
