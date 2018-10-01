/** \file
 * Structures and functions for reading/writing on-board Flash memory.
 * 
 */
#ifndef flash_h
#define flash_h

#ifdef MAIN
extern unsigned char G_flashstart[];
#else
extern const unsigned char G_flashstart[];
#endif

/*
   one per badge
*/
/**
 * Structure for all global data stored in flash memory.
 */
struct sysData_t {
   char name[32];  /**< a string name for the badge */
   unsigned short badgeId; /**< 2 bytes == our unique badge Id */
   char sekrits[8]; /**< secret (?) data */
   char achievements[8]; /**< achievements unlocked for this badge */

   /* 
      prefs 
   */
   char ledBrightness;  /**< current brightness of LED(s) */
   char backlight;      /**< current LCD backlight level */
};

extern struct sysData_t G_sysData;
extern const unsigned short flashedBadgeId; /* overrides what is stored in sysData*/
extern unsigned short G_peerBadgeId; /* who we are talking to */

#define FLASHSIZE 1024

/**
 * Erases flash memory page at current location.
 */
void flashErasePage();
/** Reads global system data from flash. (Should be significantly faster to
 * read into memory once, then read from RAM after).
 * @param [out] fdata sysData_t structure to receive copy.
 */
unsigned char sysDataRead(struct sysData_t *fdata);
/** Writes global system data to flash.
 * @param [in] fdata New data to be written.
 */
unsigned char sysDataWrite(struct sysData_t *fdata);

/**
 * Write <code>datasize</code> bytes with appId and dataId identifiers.
 * @param [in] appId unique ID => badge menu position
 * @param [in] dataId unique app assigned ID so it can access data for read/write
 * @param [in] data pointer to data to be written
 * @param [in] datasize size of <code>data</code>, in bytes.
 * @return # of bytes written, either <code>0</code> or <code>datasize</code>.
 */
unsigned char NVwrite(unsigned char appId, unsigned char dataId, 
                      unsigned char *data, unsigned char datasize);
/**
 * Read at most <code>datasize</code> bytes from flash memory identified by 
 * <code>appId</code> and <code>dataId.
 * @param [in] appId unique < 255 Id => badge menu position
 * @param [in] dataId unique app assigned id
 * @param [out] data buffer of at least size <code>datasize</code> to hold
 *              read data
 * @param [in] datasize size of <code>data</code> buffer, maximum # of bytes
 *             to be read
 * @return # of bytes actually read, rounded up to 4-byte boundary (?).
 */
unsigned char NVread(unsigned char appId, unsigned char dataId, 
                     unsigned char *data, unsigned char datasize);

/**
 * format of a record header in the flash block
 */
struct flashHeader_t {
    unsigned char appId; /**< Application that wrote record */
    unsigned char dataId; /**< Unique ID assigned by application */
    unsigned char dataSize; /**< size of data written (not incl. header or padding) */
    unsigned char pad; /**< to fit struct into 32 bits */
};

/**
 * union for easier access as header, 4 bytes, or int
 */
union flashWord_u {
    struct flashHeader_t h;
    char b[4];		/* byte access */
    unsigned int l;	/* int word access */
};


#endif

