#ifdef SPLASH_LORES
#define STUB
#ifdef STUB
   #define SPLASH_BITS PICTURE2BIT
   #define SPLASH_WIDTH lores_width
   #define SPLASH_HEIGHT lores_height
   #define SPLASH_CMAP lores_data_cmap
   #define SPLASH_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define SPLASH_BITS PICTURE4BIT
   #define SPLASH_WIDTH splash_width
   #define SPLASH_HEIGHT splash_height
   #define SPLASH_CMAP splash_data_cmap
   #define SPLASH_DATA splash_data
   #include "assets/lores/splash_lores2.h"
#endif
#else
   #define SPLASH_BITS PICTURE8BIT
   #define SPLASH_WIDTH splash_width
   #define SPLASH_HEIGHT splash_height
   #define SPLASH_CMAP splash_data_cmap
   #define SPLASH_DATA splash_data
   #include "assets/hires/splash.h"
#endif

