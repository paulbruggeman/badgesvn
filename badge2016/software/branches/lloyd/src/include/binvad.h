#ifdef BINVAD_LORES
   #define BI_sprite_width 16
   #define BI_sprite_height 5
   #define BADINVAD_BITS PICTURE1BIT
   #define BADINVAD_WIDTH BI_sprite_width
   #define BADINVAD_HEIGHT BI_sprite_height
   #define BADINVAD_CMAP BW_cmap
   #define BADINVAD_DATA BI_sprite_bits
   #include "assets/lores/BI_sprite.xbm"
#else
   #define BI_sprite_width 16
   #define BI_sprite_height 5
   #define BADINVAD_BITS PICTURE1BIT
   #define BADINVAD_WIDTH BI_sprite_width
   #define BADINVAD_HEIGHT BI_sprite_height
   #define BADINVAD_CMAP BW_cmap
   #define BADINVAD_DATA BI_sprite_bits
   #include "assets/hires/BI_sprite.xbm"
#endif

