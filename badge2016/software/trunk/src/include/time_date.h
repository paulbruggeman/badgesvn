#ifndef TIME_DATE_H
#define	TIME_DATE_H

/**
 * Enables the on-board Real Time Clock and Calendar and sets the current time
 * and date.
 */
void setupRTCC(void);
/**
 * Sets RTCC from current global time and date.
 */
void setRTCC();
/**
 * Sets current global time and date from the RTCC.
 */
void getRTCC();

#endif	/* TIME_DATE_H */
