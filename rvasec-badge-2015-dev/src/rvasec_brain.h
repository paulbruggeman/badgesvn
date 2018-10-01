#ifdef RVASEC_BRAIN_LORES
   #define RVASEC_BRAIN_BITS PICTURE2BIT
   #define RVASEC_BRAIN_WIDTH lores_width
   #define RVASEC_BRAIN_HEIGHT lores_height
   #define RVASEC_BRAIN_CMAP lores_data_cmap
   #define RVASEC_BRAIN_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define RVASEC_BRAIN_BITS PICTURE8BIT
   #define RVASEC_BRAIN_WIDTH rvasec_brain_width
   #define RVASEC_BRAIN_HEIGHT rvasec_brain_height
   #define RVASEC_BRAIN_CMAP rvasec_brain_data_cmap
   #define RVASEC_BRAIN_DATA rvasec_brain_data
   #include "assets/hires/rv4sec_brain.h"
#endif

