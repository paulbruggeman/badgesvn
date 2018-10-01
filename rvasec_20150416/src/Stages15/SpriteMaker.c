#include "SpriteMaker.h"

//void* construct_spriteMaker15(struct BadgeState *b_state)
CONSTRUCT_PROTO(spriteMaker)
{
    BASIC_CONSTRUCT(spriteMaker)  
    return 0;
}

HANDLER_PROTO(spriteMaker)
{
    return 0;
}

ON_ENTER_PROTO(spriteMaker)
{
    LCDClear();
    return 0;
}


ON_EXIT_PROTO(spriteMaker)
{
    return 0;
}