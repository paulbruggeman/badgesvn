#ifndef TOUCH_CTMU_H

#define TOUCH_CTMU_H

#include <stdbool.h>

extern unsigned char G_button; // physical button on/off
void (*menu_escape_cb)(void);

#define NNOPS 3

#define TOUCH_BUTTON_THRESHOLD 12000
#define NUMSAMPLES 3
#define NUM_DIRECT_KEYS  4

/*
   a sample collection
*/
struct sample_t {
    unsigned int timestamp; // incremented every 1/120 sec. it will wrap around in 414 days
                            // the SAMPLES are not necessarily updated every 1/120
    unsigned short int buttonStatus; // updated when all sampling and averaging for all 4 buttons is done
    unsigned short int ButtonVmeasADC[NUM_DIRECT_KEYS]; // updated when all sampling and average for all 4 button is done
} ;

extern unsigned short int sampleButtonStatus;  // Bit field of buttons that are pressed
extern unsigned short int CurrentButtonStatus;
extern unsigned int lastHandledTimestamp[];

extern struct sample_t sample[];
void initTouch();

bool IsSlideUp();
bool IsSlideDown();
bool IsSlideLeft();
bool IsSlideRight();

bool IsTapUp();
bool IsTapDown();
bool IsTapLeft();
bool IsTapRight();

/* button numbering */
#define LEFT_SLIDER 0     /* bottom left */
#define RIGHT_SLIDER 1    /* bottom right */
#define TOP_SLIDER 2      /* right side of badge top */
#define BOTTOM_SLIDER 3   /* right side of badge bottom */
#define BUTTON 4   /* physical button */
#define SLIDE_UP 5
#define SLIDE_DOWN 6
#define SLIDE_LEFT 7
#define SLIDE_RIGHT 8 

/* bit masks */
#define LEFT_SLIDER_MASK    (1 << LEFT_SLIDER)   // 0b00001,
#define RIGHT_SLIDER_MASK   (1 << RIGHT_SLIDER)  // 0b00010,
#define TOP_SLIDER_MASK     (1 << TOP_SLIDER)    // 0b00100,
#define BOTTOM_SLIDER_MASK  (1 << BOTTOM_SLIDER) // 0b01000,
#define BUTTON_MASK  (1 << BUTTON)   // 0b10000,
#define SLIDE_LEFT_MASK    (1 << SLIDE_LEFT)
#define SLIDE_RIGHT_MASK   (1 << SLIDE_RIGHT)
#define SLIDE_UP_MASK     (1 << SLIDE_UP)
#define SLIDE_DOWN_MASK  (1 << SLIDE_DOWN)


#define BUTTON_PRESSED_AND_CONSUME ((sampleButtonStatus & BUTTON_MASK) && \
    (buttonTimestamp[BUTTON] != lastHandledTimestamp[BUTTON]) && \
    (lastHandledTimestamp[BUTTON] = buttonTimestamp[BUTTON]))

#define TOP_SLIDE_AND_CONSUME IsSlideUp()
#define BOTTOM_SLIDE_AND_CONSUME IsSlideDown()
#define LEFT_SLIDE_AND_CONSUME IsSlideLeft()
#define RIGHT_SLIDE_AND_CONSUME IsSlideRight()

#define TOP_TAP_AND_CONSUME IsTapUp()
#define BOTTOM_TAP_AND_CONSUME IsTapDown()
#define LEFT_TAP_AND_CONSUME IsTapLeft()
#define RIGHT_TAP_AND_CONSUME IsTapRight()

extern unsigned char G_outputMode;
extern unsigned char G_entry;

#define PORTRAIT_MODE ((G_outputMode == 0b00000110) && (G_entry == 0b10000000))
#define LANDSCAPE_MODE ((G_outputMode == 0b00000111) && (G_entry = 0b10000010))

unsigned char getHorizontalPosition();
unsigned char getVerticalPosition();

#endif
