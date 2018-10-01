#ifndef TIME_DATE_H
#define	TIME_DATE_H

int CheckRtccRunning(int secWait);

void setTime_Date(unsigned char *time, unsigned char *date);

void getTime_Date(unsigned char *time, unsigned char *date);

void setupRTCC(void);

void setRTCC();

void getRTCC();

int CheckRtccRunning(int secWait);

#endif	/* TIME_DATE_H */
