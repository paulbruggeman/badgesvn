#ifdef HACKRVA1BIT_LORES
   #define HACKRVA1_BITS PICTURE2BIT
   #define HACKRVA1_WIDTH lores_width
   #define HACKRVA1_HEIGHT lores_height
   #define HACKRVA1_CMAP lores_data_cmap
   #define HACKRVA1_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define HACKRVA1_BITS PICTURE2BIT
   #define HACKRVA1_WIDTH hackrva_width
   #define HACKRVA1_HEIGHT hackrva_height
   #define HACKRVA1_CMAP BW_cmap
   #define HACKRVA1_DATA hackrva_bits
   #include "assets/hires/hackrva1bit.xbm"
#endif

