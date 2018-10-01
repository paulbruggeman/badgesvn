#ifdef KITTEN_LORES
#define STUB
#ifdef STUB
   #define KITTEN_BITS PICTURE2BIT
   #define KITTEN_WIDTH lores_width
   #define KITTEN_HEIGHT lores_height
   #define KITTEN_CMAP lores_data_cmap
   #define KITTEN_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define KITTEN_BITS PICTURE2BIT
   #define KITTEN_WIDTH kitten_width
   #define KITTEN_HEIGHT kitten_height
   #define KITTEN_CMAP kitten_data_cmap
   #define KITTEN_DATA kitten_data
   #include "assets/lores/kitten.h"
#endif
#else
   #define KITTEN_BITS PICTURE8BIT
   #define KITTEN_WIDTH kitten_width
   #define KITTEN_HEIGHT kitten_height
   #define KITTEN_CMAP kitten_data_cmap
   #define KITTEN_DATA kitten_data
   #include "assets/hires/kitten.h"
#endif

