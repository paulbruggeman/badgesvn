#include "badge16.h"

char G_debug[80];

/*
   will be save/restored from flash
*/
const unsigned short flashedBadgeId=0xefbe; /* MAGIC value for script. It will be replaced by awk script in final flashing */
struct sysData_t G_sysData;
unsigned short G_peerBadgeId; /* who we are talking to */
