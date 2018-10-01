/** @file
 * Various routines to test board functions.
 *
 * There will be a separate menu of test options, which should have a way to
 * exercise each part of the board.
 */

#include <stddef.h>
#include <string.h>
#include "GenericTypeDefs.h"  // INT32, UNIT32, etc.
#include "menu.h"
#include "tests.h"
#include "flash.h" //G_sysData
#include "fb.h"
#include "TimeDelay.h"
#include "touchCTMU.h"
#include "error.h"

extern const char hextab[];


struct menu_t tests_m[] = {
    {"backlight", VERT_ITEM | DEFAULT_ITEM, FUNCTION,
        testBacklight_cb},
    {"flash", VERT_ITEM, FUNCTION,
        {(struct menu_t *) testFlashRead_cb}},
    {"speaker", VERT_ITEM, FUNCTION,
        {testSpeaker_cb}},
    {"errors", VERT_ITEM, FUNCTION,
        {testErrors_cb}},
    {"Back", VERT_ITEM | LAST_ITEM, BACK,
        {NULL}},
};

struct backlight_state {
    unsigned char orig_brightness;
    unsigned char curr_brightness;
    unsigned char flags;

};

void testBacklight_cb() {
    static struct backlight_state s;
    // ^ initialized to 0

    /* TODO: display what we're doing and when done display
     * "press button to return"
     */

    if (!(s.flags & 1)) {
        FbClear();
        s.curr_brightness = 0;
        FbMove(5, 5);
        FbWriteLine("Testing");
        FbMove(5, 15);
        FbWriteLine("Backlight");
        s.flags |= 1;
    } else {
        if (s.curr_brightness == 0) {
            if (G_sysData.backlight) {
                backlight(G_sysData.backlight);
            }
            memset(&s, 0, sizeof (s));
            returnToMenus();
            return;
        }
    }

    if (!(s.flags & 2)) {
        if (s.curr_brightness < 255) {
            backlight(s.curr_brightness++);
        } else {
            s.flags |= 2;
        }
    } else {
        backlight(--s.curr_brightness);
    }
    DelayMs(20);
}

void h2a(unsigned short value, char *buffer) {
    unsigned char first_digit;
    int i = 0;
    for (i; i < 4; i++) {
        first_digit = (value & 0xF000) >> 12;
        *buffer++ = hextab[first_digit];
        value <<= 4;
    };
    *buffer = '\0';
}

void testFlashRead_cb() {
    //    unsigned char sysDataRead(struct sysData_t *fdata);
    struct sysData_t fdata;
    static int displayed = 0;
    char outbuffer[15];
    int i;

    if (!displayed) {
        sysDataRead(&fdata);
        FbClear();
        h2a(fdata.badgeId, outbuffer);
        FbMove(2, 2);
        if (fdata.badgeId) {
            FbWriteLine(outbuffer);
        } else {
            FbWriteLine("0");
        }
        displayed = 1;
    }
    if (BUTTON_PRESSED_AND_CONSUME) {
        returnToMenus();
        displayed = 0;
    }
    // fdata.badgeId
}

void showSpeakerScreen() {
  FbMove(5, 5);
  FbWriteLine("Speaker");
  FbMove(5, 110);
  FbWriteLine("Press Button");
  FbMove(5, 120);
  FbWriteLine("to end");
}

enum {
    SPKR_INIT,
    SPKR_PLAYING,
    SPKR_PAUSE,
    SPKR_DONE,
    SPKR_RESET
} speakerState;

void testSpeaker_cb() {
  static int displayed = FALSE;
    static unsigned int startCount = 0, currentCount = 0, index = 0;
    const unsigned short NOTE_DUR=8192;

    const static unsigned char notes[] = {
        173 /*a*/, 154 /*b*/, 145 /*c*/, 129 /*d*/, 115 /*e*/,
        109 /*f*/, 97 /*g*/
    };

    if (!displayed) {
        FbClear();
        showSpeakerScreen(index, 0);
        displayed = 1;
        startCount = ReadCoreTimer();
        /* setNote(notes[0], NOTE_DUR); */
        /* index++; */
    }

    currentCount = ReadCoreTimer();
    if ((currentCount - startCount) > NOTE_DUR && index < sizeof (notes)) {
      setNote(notes[index], NOTE_DUR);
      index++;
      startCount = currentCount;
    }

    if (index >= sizeof(notes)) {
      /* This delay function (by Microchip!) uses busy wait, so prevents processing
       * of button presses during the 4 seconds. Replace! */
      DelayMs(4000);
      index = 0;
      startCount = ReadCoreTimer();
      currentCount = startCount;
      displayed=FALSE;
    }

    showSpeakerScreen(index, currentCount - startCount);

    if (BUTTON_PRESSED_AND_CONSUME) {
        displayed = FALSE;
        startCount = currentCount = 0;
        index = 1;
        returnToMenus();
    }
}

void testErrors_cb() {
    static int state = 0;
    if (state == 0) {
        error_display(ERR_FORTH_NO_WORD, NULL);
        state = 1;
    }
    if (state == 1 && BUTTON_PRESSED_AND_CONSUME) {
        state = 2;
    }
    if (state == 2) {
        error_display(ERR_FORTH_STACK_UNDERFLOW, "Extra msg!");
        state = 3;
    }
    if (state == 3 && BUTTON_PRESSED_AND_CONSUME) {
        error_display(ERR_INVALID_ERROR + 5, NULL);
        state = 4;
    }
    if (state == 4 && BUTTON_PRESSED_AND_CONSUME) {
        state = 0;
        returnToMenus();
    }
}