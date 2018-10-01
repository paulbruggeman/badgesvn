#ifdef BOWL_LORES
   #define BOWL_BITS PICTURE2BIT
   #define BOWL_WIDTH bowl_width
   #define BOWL_HEIGHT bowl_height
   #define BOWL_CMAP bowl_data_cmap
   #define BOWL_DATA bowl_data
   #include "assets/lores/bowl.h"
#else
   #define BOWL_BITS PICTURE2BIT
   #define BOWL_WIDTH bowl_width
   #define BOWL_HEIGHT bowl_height
   #define BOWL_CMAP bowl_data_cmap
   #define BOWL_DATA bowl_data
   #include "assets/hires/bowl.h"
#endif

