/** \file
 *  Mid-level functions for updating LCD color display.
 */
#ifndef LCDcolor_h
#define LCDcolor_h

/** Resets LCD to initial state */
void LCDReset(void);
/** Sets LCD display pins -- LCD will do nothing until LCDReset()  (??) */
void LCDInitPins(void);
/** Displays bars of various colors on LCD, for testing. (??) */
void LCDBars();

extern unsigned short scan_bucket[];
#endif

