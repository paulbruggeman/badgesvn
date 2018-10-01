#ifdef CHIPSPRITE_LORES
   #define CHIPSPRITE_BITS PICTURE4BIT
   #define CHIPSPRITE_WIDTH chipsprite_lores_width
   #define CHIPSPRITE_HEIGHT chipsprite_lores_height
   #define CHIPSPRITE_CMAP chipsprite_lores_data_cmap
   #define CHIPSPRITE_DATA chipsprite_lores_data
   #include "assets/lores/chipsprite_lores.h"
#else
   #define CHIPSPRITE_BITS PICTURE8BIT
   #define CHIPSPRITE_WIDTH chipsprite_width
   #define CHIPSPRITE_HEIGHT chipsprite_height
   #define CHIPSPRITE_CMAP chipsprite_data_cmap
   #define CHIPSPRITE_DATA chipsprite_data
   #include "assets/hires/chipsprite.h"
#endif

