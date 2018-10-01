#ifndef assetList_h
#define assetList_h


/*
 NOTE
 NOTE   LASTASSET has to be the LAST item in the enum 
 NOTE   insert new enums before it.
 NOTE
*/
enum {
    DRBOB=0,
    HACKRVA,
    RVASEC,
    RVASEC2,
    HACKRVA2,
    MARIO,
    GAMEOTHRONES,
    FONT,
    CHIP,
    BOWLBALL,
    KITTEN,
    SPLASH,
    CHIPSPRITE,
    LASTASSET,
};

enum {
    AUDIO,
    MIDI,
    PICTURE1BIT,
    PICTURE2BIT,
    PICTURE4BIT,
    PICTURE8BIT,
};

struct asset {
    unsigned char assetId;
    unsigned char type;
    unsigned char seqNum; /* image within the asset for animattion, frame no. for font char id */
    unsigned short x;
    unsigned short y;
    const char *data_cmap;
    const char *pixdata;
    void (*datacb)(unsigned char, int);
};
extern const struct asset assetList[];
#endif
