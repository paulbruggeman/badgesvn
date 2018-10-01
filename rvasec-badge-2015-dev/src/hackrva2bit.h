#ifdef HACKRVA2BIT_LORES
   #define HACKRVA2_BITS PICTURE2BIT
   #define HACKRVA2_WIDTH lores_width
   #define HACKRVA2_HEIGHT lores_height
   #define HACKRVA2_CMAP lores_data_cmap
   #define HACKRVA2_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define HACKRVA2_BITS PICTURE2BIT
   #define HACKRVA2_WIDTH hackrva2_width
   #define HACKRVA2_HEIGHT hackrva2_height
   #define HACKRVA2_CMAP hackrva2_data_cmap
   #define HACKRVA2_DATA hackrva2_data
   #include "assets/hires/hackrva2bit.h"
#endif

