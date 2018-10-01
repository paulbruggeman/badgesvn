#ifndef timer1_int_h
#define timer1_int_h
/** @file
 * Functions for dealing with the Timer1 interrupts. Timer1 is an onboard 16-bit timer.
 * It is used for detecting slider inputs and PWM for the LEDs and display backlight.
 */
void touchInterrupt();
/** Toggles red LED with frequency determined by global brightness value. 
 * @param onPWM current timer tick count
 */
void red(unsigned char onPWM);
/** Toggles green LED with frequency determined by global brightness value. 
 * @param onPWM current timer tick count
 */
void green(unsigned char onPWM);
/** Toggles blue LED with frequency determined by global brightness value. 
 * @param onPWM current timer tick count
 */
void blue(unsigned char onPWM);
/**
 * Sets brightness of backlight.
 * @param bright
 */
void backlight(unsigned char bright);
/** Initialize on-chip timers, enable multi-vector interrupts. */
void timerInit();
/** Called each clock tick, toggles LEDs and backlight. */
void doPWM();

#endif

