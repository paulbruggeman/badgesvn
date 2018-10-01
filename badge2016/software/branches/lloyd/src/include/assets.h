#ifndef assets_h
#define assets_h

/** Makes assetId active and calls draw routine 
 *
 * @param [in] assetId ID of asset to draw
 */
void drawAsset(unsigned char assetId);
/** Callback for drawing 1-bit-per-pixel (mono) images.
 *
 * @param [in] assetId ID of image asset
 * @param frame unused
 */
void drawLCD1(unsigned char assetId, int frame);
/** Callback for drawing 2-bits-per-pixel (4-color) images.
 *
 * @param [in] assetId ID of image asset
 * @param frame unused
 */
void drawLCD2(unsigned char assetId, int frame);
/** Callback for drawing 4-bits-per-pixel (16-color) images.
 *
 * @param [in] assetId ID of image asset
 * @param frame unused
 */
void drawLCD4(unsigned char assetId, int frame);
/** Callback for drawing 8-bits-per-pixel (256-color) images.
 *
 * @param [in] assetId ID of image asset
 * @param frame unused
 */
void drawLCD8(unsigned char assetId, int frame);
/** Toggles the electric current to speaker if set number of ticks have passed.
 * If the current note is done, calls callback for the current audio asset.
 */
void doAudio();
/**
 * Tells timer interrupt to play a note.
 * @param [in] freq The note frequency (# of timer ticks between pulses)
 *        <needed here> formula for HZ -> freq value
 * @param [in] dur The duration of the note, in timer ticks
 */
void setNote(unsigned short freq, unsigned short dur) ;

/* void setBeep(unsigned short freq); */ /* not implemented */

/** Callback for setting next note when current note is complete. */
void nextNote_cb(unsigned char assetId, int frame);
void nextMIDI_cb(unsigned char assetId, int frame) ;

#endif
