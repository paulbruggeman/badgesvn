/** @file
 * Prototypes and types for functions to test various parts of the board.
 * @author A. Lloyd Flanagan <a.lloyd.flanagan@gmail.com>
 */

#ifndef _TESTS_H    /* Guard against multiple inclusion */
#define _TESTS_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Test hardware operation of backlight -- starts with backlight off, turns it
 * up to max brightness (slowly) then dims it back down.
 */
void testBacklight_cb();
/**
 * Test flash memory. Tests read -- don't want to test write from menu due
 * to limited lifetime of flash.
 */
void testFlashRead_cb();
/**
 * Test hardware speaker.
 */
void testSpeaker_cb();
/**
 * Convert an int into hex digit string
 * @param value Value to convert
 * @param buffer Memory location to receive string, must be at least 5 bytes.
 */
void h2a(unsigned short value, char *buffer);

void showHelpMsg();

extern struct menu_t tests_m[];

#ifdef __cplusplus
}
#endif

#endif /* _TESTS_H */
