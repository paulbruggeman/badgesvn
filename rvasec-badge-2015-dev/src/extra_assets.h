//#define JPUNIX_STUB
#ifdef JPUNIX_STUB
   #define JPUNIX_SYSTEM_BITS PICTURE2BIT
   #define JPUNIX_SYSTEM_WIDTH lores_width
   #define JPUNIX_SYSTEM_HEIGHT lores_height
   #define JPUNIX_SYSTEM_CMAP lores_data_cmap
   #define JPUNIX_SYSTEM_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define JPUNIX_SYSTEM_BITS PICTURE8BIT
   #define JPUNIX_SYSTEM_WIDTH jpunix_system_width
   #define JPUNIX_SYSTEM_HEIGHT jpunix_system_height
   #define JPUNIX_SYSTEM_CMAP jpunix_system_data_cmap
   #define JPUNIX_SYSTEM_DATA jpunix_system_data
   #include "assets/hires/JPUnixSystem.h"
#endif

//#define BITCOIN_STUB
#ifdef BITCOIN_STUB
   #define BITCOIN_BITS PICTURE2BIT
   #define BITCOIN_WIDTH lores_width
   #define BITCOIN_HEIGHT lores_height
   #define BITCOIN_CMAP lores_data_cmap
   #define BITCOIN_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define BITCOIN_BITS PICTURE8BIT
   #define BITCOIN_WIDTH bitcoin_width
   #define BITCOIN_HEIGHT bitcoin_height
   #define BITCOIN_CMAP bitcoin_data_cmap
   #define BITCOIN_DATA bitcoin_data
   #include "assets/hires/Bitcoin.h"
#endif

//#define CAKE_STUB
#ifdef CAKE_STUB
   #define CAKE_BITS PICTURE2BIT
   #define CAKE_WIDTH lores_width
   #define CAKE_HEIGHT lores_height
   #define CAKE_CMAP lores_data_cmap
   #define CAKE_DATA lores_data
   #include "assets/lores/lores.h"
#else
   #define CAKE_BITS PICTURE2BIT
   #define CAKE_WIDTH TheCakeIsALie_width
   #define CAKE_HEIGHT TheCakeIsALie_height
   #define CAKE_CMAP TheCakeIsALie_data_cmap
   #define CAKE_DATA TheCakeIsALie_data
   #include "assets/lores/TheCakeIsALie_2bit.h"
#endif
