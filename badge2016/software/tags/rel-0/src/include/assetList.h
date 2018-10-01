#ifndef assetList_h
#define assetList_h


/*
 NOTE
 NOTE   LASTASSET has to be the LAST item in the enum 
 NOTE   insert new enums before it.
 NOTE
*/
enum {
    HACKRVA4=0,
    RVASECBUG,
    FONT,

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
