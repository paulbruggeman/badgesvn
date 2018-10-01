#include "error.h"
#include "string.h"
#include "touchCTMU.h"
#include "fb.h"
#include "assetList.h"

typedef unsigned char byte;

static const byte LEFT_MARGIN = 5;
static const byte TOP_MARGIN = 12;
static const byte LINE_HEIGHT = 10;

/** width of a character in our font */
#define CHAR_WIDTH (assetList[G_Fb.font].x)
/** pixels of space between successive chars */
#define CHAR_SPACING (CHAR_WIDTH + 1)

const char * const error_msgs[] = {
    NULL,  // no error 0, make life easier for non-programmers
    // for now, error msgs are <= 11 chars
    "F: bad word",
    "F: stk undr",
    "Unknown Err"  // ERR_INVALID_ERROR, must be LAST
};

void error_show_code(enum ERROR_CODES ecode) {

    char buffer[9];
    strcpy(buffer, "ERROR ");
    buffer[6] = '0' + ecode/10 % 10;
    buffer[7] = '0' + ecode % 10;
    buffer[8] = '\0';
    FbWriteLine(buffer);
}

// TODO: Wrap strings in reasonable fashion
void error_display(enum ERROR_CODES ecode, const char const *extra) {
    byte pos_y = TOP_MARGIN;
    int i;

     if (ecode > ERR_INVALID_ERROR) {
        ecode = ERR_INVALID_ERROR;
    }

    FbClear();

    FbMove(LEFT_MARGIN, pos_y);
    error_show_code(ecode);

    pos_y += LINE_HEIGHT;
    FbMove(LEFT_MARGIN, pos_y);
    FbWriteLine(error_msgs[ecode]);
    if (extra) {
        pos_y += LINE_HEIGHT;
        FbMove(LEFT_MARGIN, pos_y);
        FbWriteLine(extra);
    }
    FbPushBuffer();
}
