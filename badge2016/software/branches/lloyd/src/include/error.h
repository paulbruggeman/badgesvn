/** @file Declarations for error handling routines.
 *
 * @author A. Lloyd Flanagan <a.lloyd.flanagan@gmail.com>
 * @copyright 2016 HackRVA Labs, Inc. <https://www.hackrva.org>
 */

#ifndef _ERROR_H /* Guard against multiple inclusion */
#define _ERROR_H

/** A numeric code indicating what type of error occurred. */
enum ERROR_CODES {
    // don't change the value of Forth errors, currently they're hard-coded in
    // the assembly code. Probably a way to refer to an enum member in assembly,
    // but I haven't found it yet.
    ERR_FORTH_NO_WORD = 1,
    ERR_FORTH_STACK_UNDERFLOW,
    ERR_INVALID_ERROR   // must be last/highest enum!
};

// indexes are ERROR_CODES constants, MUST keep in sync!
/** A standard message to display for each code in ERROR_CODES. */
extern const char * const error_msgs[];

void error_show_code(enum ERROR_CODES ecode);

/**
 * Display the error on the screen, with an optional extra string from caller.
 *
 * @param ecode The error code, an index into error_msgs array.
 *
 * @param extra Additional string to display after the indicated string from
 * error_msgs.
 *
 */
void error_display(enum ERROR_CODES ecode, const char const *extra);

#endif /* _ERROR_H */