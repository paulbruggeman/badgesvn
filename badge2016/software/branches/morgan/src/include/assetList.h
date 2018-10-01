/** \file
 * Data structure for large binary data assets.
 */
#ifndef assetList_h
#define assetList_h

/*
 NOTE
 NOTE   LASTASSET has to be the LAST item in the enum 
 NOTE   insert new enums before it.
 NOTE
*/
enum {
//    DRBOB=0,
    HACKRVA4=0,
//    RVASECBUG,
    FONT,
    BI_SPRITE,
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

/**
 * A managed asset (binary data)
 */
struct asset {
    unsigned char assetId; /**< number (enum) used to reference object */
    unsigned char type;    /**< image/audio/midi/private */
    unsigned char seqNum; /**< \# images in the asset for animation, frame # for font char id */
    unsigned short x;	/**< array x */
    unsigned short y;	/**< array y */
    const char *data_cmap; /**< color map lookup table for image data */
    const char *pixdata;   /**< color pixel data */
    void (*datacb)(unsigned char, int); /**< routine that can display or play asset */
};

/** global array of static assets */
extern const struct asset assetList[];

#endif
