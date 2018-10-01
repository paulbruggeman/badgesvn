#ifndef flash_h
#define flash_h

#include "badge15.h"

#ifdef MAIN
extern unsigned char G_flashstart[];
#else
extern const unsigned char G_flashstart[];
#endif


#define FLASHSIZE 1024

void flashErasePage();
unsigned char sysDataRead(struct sysData_t *fdata);
unsigned char sysDataWrite(struct sysData_t *fdata);

unsigned char NVwrite(unsigned char appId, unsigned char dataId, unsigned char *data, unsigned char datasize);
unsigned char NVread(unsigned char appId, unsigned char dataId, unsigned char *data, unsigned char datasize);

#endif

