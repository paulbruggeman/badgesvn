#include "TouchCalibrate.h"

CONSTRUCT_PROTO(TouchCalibrate)
{
    BASIC_CONSTRUCT(TouchCalibrate)
    return 0;
}

HANDLER_PROTO(TouchCalibrate)
{
    if(!b_state->big_counter)
    {
        b_state->big_counter += TouchCalibrate15_calibrateBottom(b_state);

        if(BUTTON_IS_PRESSED__CONSUME)
        {
            b_state->counter_1 = 0;
            b_state->counter_2 = 0;
            b_state->big_counter = 1;
        }
    }
    else if(b_state->big_counter == 1)
    {
        if(!++b_state->counter_1)
        {
            if(!++b_state->counter_2)
                b_state->big_counter = 2;

            LCDClear();
            unsigned char str[btm_size*5 + 1];//*str = "DONE!";
            unsigned char j, k;
            for(j = 0; j < btm_size; j++)
            {
                k = 5*j;
                str[k] = '[';
                str[k+1] = 48 +  (unsigned char)bottom_left_pad[j];
                str[k+2] = ',';
                str[k+3] = 48 +  (unsigned char)bottom_right_pad[j];
                str[k+4] = ']';
            }
            str[k+5] = 0;
            LCDString(str);
        }

        if(BUTTON_IS_PRESSED__CONSUME)
        {
            b_state->big_counter = 2;
            b_state->counter_2 = 0;
            b_state->counter_1 = 0;
        }

    }
    else if (b_state->big_counter == 2)
    {
        b_state->big_counter += TouchCalibrate15_calibrateSide(b_state);
        if(BUTTON_IS_PRESSED__CONSUME)
        {
           b_state->counter_1 = 0;
           b_state->counter_2 = 0;
           b_state->big_counter = 3;
        }
    }
    else if (b_state->big_counter == 3)
    {
        if(!++b_state->counter_1)
        {
            if(!++b_state->counter_2)
                b_state->big_counter = 4;


            LCDClear();
            unsigned char str[side_size*5 + 1];//*str = "DONE!";
            unsigned char j, k;
            for(j = 0; j < side_size; j++)
            {
                k = 5*j;
                str[k] = '(';
                str[k+1] = 48 +  (unsigned char)side_left_pad[j];
                str[k+2] = ',';
                str[k+3] = 48 +  (unsigned char)side_right_pad[j];
                str[k+4] = ')';
            }
            str[k+5 ] = 0;
            LCDString(str);
        }
        if(BUTTON_IS_PRESSED__CONSUME )
        {
            b_state->big_counter = 4;
        }
    }
    else
    {
        //switch_state(b_state, &sprite_maker_state);
        switch_state(b_state, &etch_a_sketch_state);
    }

    return 0;
}

unsigned char TouchCalibrate15_calibrateBottom(struct BadgeState *b_state)
{
    //only sample occasionally
    if(!b_state->counter_1++)
    {
        // check if buffered filled...should also have time out
        if(btm_size == MAX_SIZE)
        {
            b_state->counter_1 = 0;
            b_state->counter_2 = 0;
            return 1;
        }
        else
        {
            //first, search values and make sure it's not duplicate
            //  start search from beginning, stop where left off
            unsigned char i, j, k, found = 0;
            for(i = 0; i < btm_size && !found; i++)
            {
                //check if at least one doesn't match
                if (bottom_left_pad[i] == G_lower_slider_left
                   && bottom_right_pad[i] == G_lower_slider_right)
                {
                    found = 1;
                }
            }

            //this sample is original and not 00
            if(!found && G_lower_slider_left && G_lower_slider_right)
            {
                bottom_left_pad[btm_size] = G_lower_slider_left;
                bottom_right_pad[btm_size] = G_lower_slider_right;
                btm_size++;
            }
            LCDClear();
            char *out = "  Calibrate\n   Bottom:\n";
            LCDString(out);
            printTouchVals(1, 0);
        }

    }
    return 0;
}

unsigned char TouchCalibrate15_calibrateSide(struct BadgeState *b_state)
{
    //only sample occasionally
    if(!b_state->counter_1++)
    {
        // check if buffered filled...should also have time out
        if(side_size == MAX_SIZE )
        {
            b_state->counter_1 = 0;
            b_state->counter_2 = 0;
            return 1;
        }
        else
        {
            //first, search values and make sure it's not duplicate
            unsigned char i, j, k, found = 0;
            for(i = 0; i < side_size && !found; i++)
            {
                //check if at least one doesn't match
                if (side_left_pad[i] == G_side_slider_left
                   && side_right_pad[i] == G_side_slider_right)
                {
                    found = 1;
                }
            }

            //this sample is original
            if(!found && G_side_slider_left && G_side_slider_right)
            {
                side_left_pad[side_size] = G_side_slider_left;
                side_right_pad[side_size] = G_side_slider_right;
                side_size++;
            }

            LCDClear();
            char *out = "  Calibrate\n    Side:\n";
            LCDString(out);
            printTouchVals(0,1);
        }
    }
    return 0;
}


ON_ENTER_PROTO(TouchCalibrate)
{
    LCDClear();
    return 0;
}

ON_EXIT_PROTO(TouchCalibrate)
{
    return 0;
}

