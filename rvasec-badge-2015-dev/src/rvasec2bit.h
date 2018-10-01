#ifdef RVASEC2BIT_LORES
   #define RVASEC2_BITS PICTURE2BIT
   #define RVASEC2_WIDTH lores_width
   #define RVASEC2_HEIGHT lores_height
   #define RVASEC2_CMAP lores_data_cmap
   #define RVASEC2_DATA lores_data
   #include "assets/hires/rvasec2bit2.h"
#else
   #define RVASEC2_BITS PICTURE2BIT
   #define RVASEC2_WIDTH rvasec2bit2_width
   #define RVASEC2_HEIGHT rvasec2bit2_height
   #define RVASEC2_CMAP rvasec2bit2_data_cmap
   #define RVASEC2_DATA rvasec2bit2_data
   #include "assets/hires/rvasec2bit2.h"
#endif

