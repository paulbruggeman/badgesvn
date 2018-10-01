#include "EtchASketch.h"

CONSTRUCT_PROTO(EtchASketch)
{
    BASIC_CONSTRUCT(EtchASketch)
    return 0;
}

HANDLER_PROTO(EtchASketch)
{
    static unsigned char x = 20, y = 20;
    static unsigned char leds = 0b00011000;
    static unsigned char xor = 0x00;

    char putPix = 0;
    b_state->slide_handler(&b_state->slide_states);

    //set_leds(b_state->slide_states.front.lower_loc);
    char lr_swipe = b_state->slide_states.front.lr_swipe;
    char bt_swipe = b_state->slide_states.front.bt_swipe;

    if(lr_swipe > 0)
    {
        struct pix_buff buff;
        buff.height = 4;
        buff.width = 4;
        unsigned char pix[4] = {0x00, 0x00, 0x00, 0x00};
        buff.pixels = pix;

        struct coord loc;
        loc.x = 0;
        loc.y = 0;

        if(leds != 0x03)
        {
            leds >>= 1;
            b_state->counter_1 = 0;
        }

        x += 1;

        putPix = 1;
    }
    else if (lr_swipe < 0)
    {
        if(leds != 0xC0)
        {
            leds <<= 1;

            b_state->counter_1 = 0;
        }
        if(xor)
            putPixel(x,y,0);
        x -= 1;
        putPix = 1;
    }

    if(bt_swipe > 0)
    {
        if(leds != 0x03)
        {
            leds >>= 1;
            b_state->counter_1 = 0;
        }
        if(xor)
            putPixel(x,y,0);
        y -= 1;
        putPix = 1;
    }
    else if (bt_swipe < 0)
    {
        if(leds != 0xC0)
        {
            leds <<= 1;

            b_state->counter_1 = 0;
        }
        if(xor)
            putPixel(x,y,0);
        y += 1;
        putPix = 1;
    }

    if(putPix)
        putPixel(x, y, 1);

    else if ( BUTTON_IS_PRESSED__CONSUME )
        switch_state(b_state, &snake_state);

    if(b_state->slide_states.bottom_hold_count > DEBOUNCE)
    {
        xor = ~xor;
        b_state->slide_states.bottom_hold_count = 0;
    }

    set_leds(leds ^ xor);
    return 0;
}

ON_ENTER_PROTO(EtchASketch)
{
    LCDClear();
    return 0;
}

ON_EXIT_PROTO(EtchASketch)
{
    return 0;
}