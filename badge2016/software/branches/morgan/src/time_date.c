#include <plib.h>
#include "time_date.h"

rtccTime G_time;
rtccDate G_date;

void setupRTCC(void)
{
        SYSKEY = 0x0;
        SYSKEY = 0xAA996655;
        SYSKEY = 0x556699AA;

	OSCCONbits.SOSCEN = 0;
	mOSCEnableSOSC();
	OSCCONbits.SOSCEN = 1;

	RtccInit();			// init the RTCC

        SYSKEY = 0x0;

	while (RtccGetClkStat() != RTCC_CLK_ON);	// wait for the SOSC to be actually running and RTCC to have its clock source

	RtccOpen(G_time.l, G_date.l, 0);	// set time, date and calibration in a single operation
        RtccWrEnable(0);

	return ;
}

void setRTCC() {
   RtccSetTimeDate(G_time.l, G_date.l);
}

void getRTCC() {
   G_time.l = RtccGetTime();
   G_date.l = RtccGetDate();
}
