/* in case this gets included more than once */
#ifndef badge16_h
#define badge16_h

#define HACKRVA_GOON
#define ALL_UNLOCKED

/*
   one per badge
*/
struct sysData_t {
   char name[32];
   unsigned short badgeId; /* 2 bytes == our badge Id */
   char sekrits[8];
   char achievements[8];

   /* 
      prefs 
   */
   char ledBrightness;  /* 1 byte */
   char backlight;      /* 1 byte */
};

extern char G_debug[];
extern struct sysData_t G_sysData;
extern const unsigned short flashedBadgeId; /* overrides what is stored in sysData*/
extern unsigned short G_peerBadgeId; /* who we are talking to */

#endif
