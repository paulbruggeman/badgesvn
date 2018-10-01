#ifdef RVASECBUG_LORES
   #define RVASECBUG_BITS PICTURE2BIT
   #define RVASECBUG_WIDTH rvasecbug2_width
   #define RVASECBUG_HEIGHT rvasecbug2_height
   #define RVASECBUG_CMAP rvasecbug2_data_cmap
   #define RVASECBUG_DATA rvasecbug2_data
   #include "assets/hires/rvasecbug2.h"
#else
   #define RVASECBUG_BITS PICTURE2BIT
   #define RVASECBUG_WIDTH rvasecbug2_width
   #define RVASECBUG_HEIGHT rvasecbug2_height
   #define RVASECBUG_CMAP rvasecbug2_data_cmap
   #define RVASECBUG_DATA rvasecbug2_data
   #include "assets/hires/rvasecbug2.h"
#endif

