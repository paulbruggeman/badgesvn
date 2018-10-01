const static char BSOD_cmap[2][3] = {
{ 0, 0, 0 },
{ 20, 20, 220 },
};

/* 1 bit deep, no lores version */
#ifdef BSOD_LORES
   #define BSOD_BITS PICTURE1BIT
   #define BSOD_WIDTH BSOD_width
   #define BSOD_HEIGHT BSOD_height
   #define BSOD_CMAP BSOD_cmap
   #define BSOD_DATA BSOD_bits
   #include "assets/hires/BSOD.xbm"
#else
   #define BSOD_BITS PICTURE1BIT
   #define BSOD_WIDTH BSOD_width
   #define BSOD_HEIGHT BSOD_height
   #define BSOD_CMAP BSOD_cmap
   #define BSOD_DATA BSOD_bits
   #include "assets/hires/BSOD.xbm"
#endif

