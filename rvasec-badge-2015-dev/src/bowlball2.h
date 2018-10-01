#ifdef BOWLBALL_LORES
   #define BOWLBALL_BITS PICTURE2BIT
   #define BOWLBALL_WIDTH bowlball2_lores_width
   #define BOWLBALL_HEIGHT bowlball2_lores_height
   #define BOWLBALL_CMAP bowlball2_lores_data_cmap
   #define BOWLBALL_DATA bowlball2_lores_data
   #include "assets/lores/bowlball_lores.h"
#else
   #define BOWLBALL_BITS PICTURE8BIT
   #define BOWLBALL_WIDTH bowlball_width
   #define BOWLBALL_HEIGHT bowlball_height
   #define BOWLBALL_CMAP bowlball_header_data_cmap
   #define BOWLBALL_DATA bowlball_header_data
   #include "assets/hires/bowlball2.h"
#endif

