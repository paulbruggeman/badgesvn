#include "BadgyBird.h"

CONSTRUCT_PROTO(BadgyBird)
{
    BASIC_CONSTRUCT(BadgyBird)
    return 0;
}

HANDLER_PROTO(BadgyBird)
{
    if(BUTTON_IS_PRESSED__CONSUME)
    {
        //b_state->counter_2++;
        y_acc_mag = -8;
        y_acc_length = 7;
        b_state->counter_1 = 0;
        b_state->state_handler = HANDLER_REF(BadgyBirdPlay);
    }
}

HANDLER_PROTO(BadgyBirdPlay)
{
    unsigned char i = 0, j = 0;
    // game playing
    if (BUTTON_IS_PRESSED__CONSUME)
    {
        y_acc_mag = -8;
        y_acc_length = 7;
        b_state->counter_1 = 0;

        //gen random from time between btn presses
        b_rand ^= ReadCoreTimer() & 0xff;
    }

     // update occasionally
     if(b_state->big_counter++ > BIRD_RATE)
     {
         //clean things up
         clear_screen_buff();
         fill_buff(&main_buff, 0x00);

         //hit floor or pipe, enter start screen after this draw
         if(collision)
         {
            switch_state(b_state, &badgy_bird_state);
            return 0;
         }

        //flapping bird, apply flap accel
        if( y_acc_length && y_acc_mag)
        {
            y_acc_length--;
            y_acc_mag++;
            bird_y_vel = (G_ACC + y_acc_mag + b_state->counter_1);
        }
        //falling bird
        else
        {
            bird_y_vel += (G_ACC + b_state->counter_1);
        }

        //apply velocity
        bird_y += bird_y_vel;

        if(bird_y_vel > 0)
            bird_idle_buff.pixels = bird_idle;
        else
            bird_idle_buff.pixels = bird_flap;

        if(bird_y >= 35)
        {
            collision |= 1;
            bird_y_vel = 0;
        }
        else
            b_state->counter_1++;

        b_state->big_counter = 0;

        for(i = 0; i < MAX_PIPES; i++)
        {
            // reset pipe
            if(pipe_locs[i].x == 0)
            {
                pipe_locs[i].x = 0xff;
                pipe_locs[i].y = 0xff;
            }
            // pipe not in play, check if it can be and
            // randomly put it into play
            else if(pipe_locs[i].x == 0xff)
            {
                // j is  index of pipe in front of i pipe
                //j = (i == 0) ?  MAX_PIPES - 1: i -1;
                if(!i)
                    j = MAX_PIPES - 1;
                else
                    j = i - 1;

                if(pipe_locs[j].x != 0xff || pipe_locs[j].x < PIPE_X_B4_ADD)
                {
                    //random is div 3
                    if(pipe_locs[j].x < PIPE_X_B4_ADD && !(b_rand % 3))
                    {
                        pipe_locs[i].x = 83 - PIPE_W - 1;
                        pipe_locs[i].y = PIPE_OPENING_HIEGHT + (b_rand % 15);
                    }
                }
                else if( !i )
                {
                    pipe_locs[i].x = 83 - PIPE_W - 1;
                    pipe_locs[i].y = PIPE_OPENING_HIEGHT + (b_rand % 15);
                }
            }
            //pipe needs drawing
            if(pipe_locs[i].x != 0 && pipe_locs[i].x != 0xff)// && !collision)
            {
                unsigned char h = pipe_locs[i].y;
                pipe_locs[i].y = 0;

                //draw top part of pipe
                fill_buff_area(pipe_locs[i], PIPE_W, h - PIPE_H,
                                   0xff, &main_buff);

                pipe_locs[i].y = h;
                //bottom part of pipe
                fill_buff_area(pipe_locs[i], PIPE_W, 48 - pipe_locs[i].y - 1,
                                    0xff, &main_buff);

                pipe_locs[i].x -= 2;
                if(pipe_locs[i].x < (BIRD_X - PIPE_W - 1) && (i != last_cleared))
                {
                    last_cleared = i;
                    pipes_cleared++;
                    if(pipes_cleared > high_score)
                        high_score = pipes_cleared;
                }
            }
        }
        draw_square(&main_buff, loc, 83, 48);
        collision |= blitBuff_toBuff_collision(&bird_idle_buff, &main_buff,
                                            BIRD_X, (unsigned char) bird_y, ALPHA );

        set_score(pipes_cleared);

        buffString(64, 2,
                    score_str,
                    &main_buff);

        blitBuff_opt(&main_buff, 0, 0);
    }
    return 0;
}




ON_ENTER_PROTO(BadgyBird)
{
    unsigned char i;
    char badgy_txt[] = "Badgy", bird_txt[] = "Bird";

    b_state->counter_2      = 1;
    b_state->counter_1      = 0;
    b_state->big_counter    = BIRD_RATE;
    b_state->big_counter_1  = 0;
    b_state->counter_1      = 0;

    // state globals (declared in state header)
    draw_pipe               = 1;
    collision               = 0;
    pipes_cleared           = 0;
    last_cleared            = 99;
    bird_y                  = BIRD_ST_HIEGHT;
    
    main_buff.pixels = pix;
    main_buff.width  = 84;
    main_buff.height = 48;

    loc.x = 0;
    loc.y = 0;

    //init all pipes
    for(i = 0; i < MAX_PIPES; i++)
    {
        pipe_locs[i].x = 0xff;
        pipe_locs[i].y = 0xff;
    }

    // prep LCD and screen buffer
    LCDClear();
    clear_screen_buff();
    fill_buff(&main_buff, 0x00);

    draw_square(&main_buff, loc, 83, 48);

    buffString(31, 2,
                badgy_txt,
                &main_buff);

    buffString(41, 10,
                bird_txt,
                &main_buff);

    // show high score on start screen
    if(high_score)
    {
        set_score(high_score);
        buffString(45, 23,
                    score_str,
                    &main_buff);
        set_score(0);
    }

    bird_idle_buff.pixels = bird_flap;

    //setup paused screen
    blitBuff_toBuff(&bird_idle_buff, &main_buff,
                    20, (unsigned char) bird_y,
                    ALPHA );

    blitBuff_opt(&main_buff, 0, 0);

    // This could be set a few different places, but it better be set by here
    b_state->state_handler = HANDLER_REF(BadgyBird);

    return 0;
}

ON_EXIT_PROTO(BadgyBird)
{
    return 0;
}

void set_score(unsigned char set_score)
{
    score_str[0] = 48 + (set_score) / 100;
    score_str[1] = 48 + (set_score) % 100 / 10;
    score_str[2] = 48 + (set_score) % 100 % 10;
}