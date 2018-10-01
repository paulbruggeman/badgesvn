/*********************************************************************
 *
 *                  RTCC API Example
 *
 *********************************************************************
 * FileName:        time_date.c
 * Dependencies:	Rtcc.h
 *
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB Cxx
 *                  MPLAB IDE vxx
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the Company) for its PIC32 Microcontroller is intended
 * and supplied to you, the Companys customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * $Id: rtcc_api_example.c 4288 2007-08-22 23:00:56Z aura $
 *
 ********************************************************************/


// Master header file for all peripheral library includes
#include <plib.h>
#include "time_date.h"

unsigned char BcdToByte(unsigned char Bin)
{
    return ((Bin & 0xF0) >> 4) * 10 + (Bin & 0x0F); 
}

unsigned char ByteToBcd(unsigned char Byte)
{
    unsigned char BCD;
    BCD = (Byte / 10) << 4;
    BCD = BCD | (Byte % 10);
    return (BCD);
}

//	local function prototypes

int CheckRtccRunning(int secWait);


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//

#ifdef FOR_REFERENCE_ONLY

//#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
//#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#endif

#define SYS_FREQ 		(40000000L)

rtccTime G_time ;
rtccDate G_date ;


/*********************************************************************
 * Function:        int main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          1 if everything went on ok, 0 if failed
 *
 * Side Effects:    None
 *
 * Overview:        The function is an example of using the RTCC device.

 * Note:            None
 ********************************************************************/
void setupRTCC(void)
{
#define UTTERCRAP
#ifdef UTTERCRAP
//	rtccTime	tm;			// time structure
//	rtccDate	dt;			// date structure

	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..

	//SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

        //mSysUnlockOpLock((OSCCONSET=_OSCCON_SOSCEN_MASK, RTCCONSET=_RTCCON_RTCWREN_MASK));

        SYSKEY = 0x0;
        SYSKEY = 0xAA996655;
        SYSKEY = 0x556699AA;

	OSCCONbits.SOSCEN = 0;
	mOSCEnableSOSC();
	OSCCONbits.SOSCEN = 1;

	RtccInit();			// init the RTCC

        SYSKEY = 0x0;

	while (RtccGetClkStat() != RTCC_CLK_ON);	// wait for the SOSC to be actually running and RTCC to have its clock source
							// could wait here at most 32ms

/*
	G_time.l=0x0;
	G_time.sec=0x0;
	G_time.min=0x07;
	G_time.hour=0x10;

	G_date.wday=2;
	G_date.mday=0x02;
	G_date.mon=0x06;
	G_date.year=0x14;
*/

	RtccOpen(G_time.l, G_date.l, 0);	// set time, date and calibration in a single operation
        RtccWrEnable(0);

//		RtccSetTimeDate(G_time.l, G_date.l);

	//RtccSetTimeDate(0x21140000, 0x14060201);	// time is MSb: hour, min, sec, rsvd. date is MSb: year, mon, mday, wday.
												// please note that the rsvd field has to be 0 in the time field!
	//RtccSetCalibration(0);	// value to calibrate with at each minute

	//tm1.l=RtccGetTime();
	//dt1.l=RtccGetDate();
	//RtccGetTimeDate(&tm1, &dt1);

	// now that we know the RTCC clock is up and running, it's easier to start from fresh:
	//RtccOpen(G_time.l, G_date.l, 0);	// set time, date and calibration in a single operation

	// check that the RTCC is running
	//if(!CheckRtccRunning(3))
	//{
		//return 0;
	//}

	// another way to see the RTCC is tunning: check the SYNC bit
	//while(RtccGetSync());	// wait sync to be low
	//while(!RtccGetSync());	// wait to be high
	//while(RtccGetSync());	// wait sync to be low again


	// other RTCC operations

	// adjust the RTCC timing

	// enabling the RTCC output pin
	//RtccSelectPulseOutput(1);		// select the seconds clock pulse as the function of the RTCC output pin
	//RtccSelectPulseOutput(0);		// select the alarm pulse as the function of the RTCC output pin
	//RtccOutputEnable(1);			// enable the Output pin of the RTCC


	return ;
#else
	// grabbed from rtccOpen, inserted clock select code
        INTEnable(INT_RTCC, INT_DISABLED);      // disable RTCC interrupts

        // be sure the SOSC is enabled, enable RTCC writes
        mSysUnlockOpLock((OSCCONSET=_OSCCON_SOSCEN_MASK, RTCCONSET=_RTCCON_RTCWREN_MASK));

        //_RtccStop();

        RTCALRMCLR=_RTCALRM_ALRMEN_MASK;// disable the alarm
        INTClearFlag(INT_RTCC);         // clear the interrupt flag

        RTCTIME = G_time.l;
        RTCDATE = G_date.l;

	// this doesn't apply to MX devices, just newer one RTCCONSET = 0b0000001000000000; // clk source = ext 32768 

//        RTCCONCLR=_RTCCON_CAL_MASK;
//        RTCCONSET=ulDrift;
//        RTCCONSET=0;

//        RTCCONSET=_RTCCON_ON_MASK;

        RtccWrEnable(0);
        //RtccOutputEnable(0);

	return ;
#endif
}

void setTime_Date(unsigned char *time, unsigned char *date)//time character format 12:00P  date character format MM-DD-YY
{
    unsigned char tensPlace = (time[3] - 48)*10;
    setMinute((tensPlace + (time[4]-48)));
    switch(time[0])
    {
        case '1':
            switch(time[1]){
                case '0':
                    if(time[5] == 'A' || time[5] == 'a')
                        setHour(10);
                    else
                        setHour(22);
                    break;
                case '1':
                    if(time[5] == 'A' || time[5] == 'a')
                        setHour(11);
                    else
                        setHour(23);
                    break;
                case '2':
                    if(time[5] == 'A' || time[5] == 'a')
                        setHour(12);
                    else
                        setHour(24);
                    break;
                default:
                    setHour(0);
                    break;
            }
            break;
        case '0':
            if(time[5] == 'A' || time[5] == 'a')
                setHour((time[1]-48));
            else
                setHour((time[1]-36));
            break;

        default:
            setHour(0);
            break;
    }
    tensPlace = (date[0] - 48)*10;
    setMonth((tensPlace + (date[1]-48)));
    tensPlace = (date[3] - 48)*10;
    setDay((tensPlace + (date[4]-48)));
    tensPlace = (date[6] - 48)*10;
    setYear((tensPlace + (date[7]-48)));

    setRTCC();
}

void setHour(unsigned char hour)
{
    G_time.hour = ByteToBcd(hour);
}

void setMinute(unsigned char min)
{
    G_time.min = ByteToBcd(min);
}

void setMonth(unsigned char month)
{   
    G_date.mon = ByteToBcd(month);
}

void setDay(unsigned char day)
{
    G_date.mday = ByteToBcd(day);
}

void setYear(unsigned char year)
{
    G_date.year = ByteToBcd(year);
}

void getTime_Date(unsigned char *time, unsigned char *date){
    getRTCC();
    unsigned char decimal = BcdToByte(G_date.mon);
    date[0] = (decimal/10) + 48;
    date[1] = (decimal%10) + 48;
    date[2] = '-';
    decimal = BcdToByte(G_date.mday);
    date[3] = (decimal/10) + 48;
    date[4] = (decimal%10) + 48;
    date[5] = '-';
    decimal = BcdToByte(G_date.year);
    date[6] = ((decimal)/10) + 48;
    date[7] = ((decimal)%10) + 48;
    time[2] = ':';
    decimal = BcdToByte(G_time.hour);
    if(decimal > 11){
        time[5] = 'P';
        time[0] = ((decimal-12)/10) + 48;
        time[1] = ((decimal-12)%10) + 48;
    }
    else{
        time[5] = 'A';
        time[0] = (decimal/10) + 48;
        time[1] = (decimal%10) + 48;
        if(time[0] == '0' && time [1] == '0'){
            time[0] = '1';
            time[1] = '2';
        }
    }

    decimal = BcdToByte(G_time.min);
    time[3] = decimal/10 + 48;
    time[4] = decimal%10 + 48;
    //itoa(date, decimal, 2);
}

void setRTCC() {
   RtccSetTimeDate(G_time.l, G_date.l);
}

void getRTCC() {
   G_time.l = RtccGetTime();
   G_date.l = RtccGetDate();
}


/*********************************************************************
 * Function:        int CheckRtccRunning(int secWait)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          1(true) if test succeeded, 0(FALSE) otherwise
 *
 * Side Effects:    None
 *
 * Overview:        The function checks that the RTCC has the clock enabled and counts the time.
 *
 * Note:            None
 ********************************************************************/
int CheckRtccRunning(int secWait)
{
	#define	WAIT_FOR_SEC_TMO	1100			// how many ms to wait for the RTCC seconds count to change

	rtccTime	t0, t1;
	int		fail;
	int		secCnt;
	unsigned int	tStart;


	
	for(secCnt=0, fail=0; secCnt<secWait; secCnt++)
	{
		tStart=ReadCoreTimer();
		t0.l=RtccGetTime();
		do
		{
			t1.l=RtccGetTime();
		}while((t1.sec==t0.sec) && (ReadCoreTimer()-tStart)<(SYS_FREQ/2000)*WAIT_FOR_SEC_TMO);	// wait seconds change

		if(t1.sec==t0.sec)
		{
			fail=1;
			break;	// failed
		}
	}
	
	return !fail;
}

 

