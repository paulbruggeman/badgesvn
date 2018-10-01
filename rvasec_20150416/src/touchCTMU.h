#ifndef TOUCH_CTMU_H

#define TOUCH_CTMU_H

#define TOUCH_BUTTON_THRESHOLD 12000

extern short int sampleButtonStatus;  // Bit field of buttons that are pressed
extern short int CurrentButtonStatus;
extern unsigned int lastHandledTimestamp[];
void initTouch();

/* button numbering */
#define LEFT_SLIDER 0     /* bottom left */
#define RIGHT_SLIDER 1    /* bottom right */
#define TOP_SLIDER 2      /* right side of badge top */
#define BOTTOM_SLIDER 3   /* right side of badge bottom */
#define BUTTON 4   /* physical button */

/* bit masks */
#define LEFT_SLIDER_MASK    (1 << LEFT_SLIDER)   // 0b00001,
#define RIGHT_SLIDER_MASK   (1 << RIGHT_SLIDER)  // 0b00010,
#define TOP_SLIDER_MASK     (1 << TOP_SLIDER)    // 0b00100,
#define BOTTOM_SLIDER_MASK  (1 << BOTTOM_SLIDER) // 0b01000,
#define BUTTON_MASK  (1 << BUTTON)   // 0b10000,

#define BUTTON_PRESSED_AND_CONSUME ((sampleButtonStatus & BUTTON_MASK) && \
    (buttonTimestamp[BUTTON] != lastHandledTimestamp[BUTTON]) && \
    (lastHandledTimestamp[BUTTON] = buttonTimestamp[BUTTON]))

#define TOP_SLIDE_AND_CONSUME ((sampleButtonStatus & TOP_SLIDER_MASK) && \
    (buttonTimestamp[TOP_SLIDER] != lastHandledTimestamp[TOP_SLIDER]) && \
    (lastHandledTimestamp[TOP_SLIDER] = buttonTimestamp[TOP_SLIDER]))

#define BOTTOM_SLIDE_AND_CONSUME ((sampleButtonStatus & BOTTOM_SLIDER_MASK) && \
    (buttonTimestamp[BOTTOM_SLIDER] != lastHandledTimestamp[BOTTOM_SLIDER]) && \
    (lastHandledTimestamp[BOTTOM_SLIDER] = buttonTimestamp[BOTTOM_SLIDER]))

#define LEFT_SLIDE_AND_CONSUME ((sampleButtonStatus & LEFT_SLIDER_MASK) && \
    (buttonTimestamp[LEFT_SLIDER] != lastHandledTimestamp[LEFT_SLIDER]) && \
    (lastHandledTimestamp[LEFT_SLIDER] = buttonTimestamp[LEFT_SLIDER]))

#define RIGHT_SLIDE_AND_CONSUME ((sampleButtonStatus & RIGHT_SLIDER_MASK) && \
    (buttonTimestamp[RIGHT_SLIDER] != lastHandledTimestamp[RIGHT_SLIDER]) && \
    (lastHandledTimestamp[RIGHT_SLIDER] = buttonTimestamp[RIGHT_SLIDER]))

unsigned char getHorizontalPosition();
unsigned char getVerticalPosition();

#endif
