/** \file
 * Macros defining basic colors, and to pack/unpack colors into a 16-bit word.
 *
 * Blue and Red values are 0 - 31, Green values are 0 - 63.
 *
 * (Fun fact: human eye is more sensitive to shades of green. See
 * https://en.wikipedia.org/wiki/RGB_color_model).
 */
#ifndef colors_h
#define colors_h

/** Packs colors R, G, B into one 16-bit word. */
#define RGBPACKED(R,G,B) ( ((unsigned short)(R)<<11) | ((unsigned short)(G)<<6) | (unsigned short)(B) )
/** Packs colors R, G, B into one 16-bit word. */
#define PACKRGB(R,G,B) ( ((unsigned short)(R)<<11) | ((unsigned short)(G)<<6) | (unsigned short)(B) )

/** Returns Red portion of 16-bit word in PACKED. */
#define UNPACKR(PACKED) (((PACKED & 0b1111100000000000) >> 11) &  0b11111)
/** Returns Green portion of 16-bit word in PACKED. */
#define UNPACKG(PACKED) (((PACKED & 0b0000011111100000) >>  5) & 0b111111)
/** Returns Blue portion of 16-bit word in PACKED. */
#define UNPACKB(PACKED)   (PACKED & 0b0000000000011111)

#define D_BLUE 0b0000000000000111
#define B_BLUE 0b1000010000011111

#define BLUE    0b0000000000011111
#define GREEN   0b0000011111100000
#define RED     0b1111100000000000

#define BLACK   0b0000000000000000
#define GREY1   0b0000100001000001
#define GREY2   0b0001000010000010
#define GREY4   0b0010000100000100
#define GREY8   0b0100001000001000
#define GREY16  0b1000010000010000
#define WHITE   0b1111111111111111

#define CYAN    0b0000011111111111
#define YELLOW  0b1111111111100000
#define MAGENTA 0b1111100000011111

#endif
