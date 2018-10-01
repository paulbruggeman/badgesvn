#include "Snake.h"

CONSTRUCT_PROTO(snake)
{
    BASIC_CONSTRUCT(snake)
    return 0;
}

HANDLER_PROTO(snake)
{
    unsigned char i = 0, j = 0;
    static struct CoOrd food;
    static struct SnakeSeg head;
    static struct pix_buff buff;

    b_state->slide_handler(&b_state->slide_states);

    //set_leds(b_state->slide_states.front.lower_loc);
    char lr_swipe = b_state->slide_states.front.lr_swipe;
    char bt_swipe = b_state->slide_states.front.bt_swipe;

    if (BUTTON_IS_PRESSED__CONSUME)
    {
        switch_state(b_state, &badgy_bird_state);
        return 0;
    }

    if(bt_swipe < 0 && !snake_ll[0].direction.yV)
    {
        snake_ll[0].direction.yV = 1;
        snake_ll[0].direction.xV = 0;
    }
    if(bt_swipe > 0 && !snake_ll[0].direction.yV )
    {
        snake_ll[0].direction.yV = -1;
        snake_ll[0].direction.xV = 0;
    }
    if(lr_swipe > 0 && !snake_ll[0].direction.xV)
    {
        snake_ll[0].direction.yV = 0;
        snake_ll[0].direction.xV = 1;
    }
    if(lr_swipe < 0 && !snake_ll[0].direction.xV)
    {
        snake_ll[0].direction.yV = 0;
        snake_ll[0].direction.xV = -1;
    }

    if(!b_state->counter_2)
    {
        LCDClear();
        b_state->next_state = b_state;

        main_buff.pixels = pix;
        main_buff.width = 84;
        main_buff.height = 48;

        b_state->counter_2 = 1;

        //need to do random init here instead
        food.x = 40;
        food.y = 20;

        snake_ll[0].location.x = 35;
        snake_ll[0].location.y = 20;
        snake_ll[0].direction.xV = 1;
        snake_ll[0].direction.yV = 0;

        for(i = 1; i < SNAKE_START_SIZE; i ++)
        {
            snake_ll[i].location = snake_ll[i-1].location;
            snake_ll[i].location.x--;
        }
        b_state->counter_1 = i;
    }

    //only update sometimes, game gets faster as snake gets bigger
    if(b_state->big_counter++ > (SNAKE_RATE - (b_state->counter_1 << 7) ))
    {
        struct coord loc;
        LCDClear();

        loc.x = 0;
        loc.y = 0;
        fill_buff(&main_buff, 0x00);

        draw_square(&main_buff, loc, 83, 47);

        loc.x = 62;
        loc.y = 0;
        draw_square(&main_buff, loc, 21, 8);

        char score[] = "999";
        score[0] = 48 + (b_state->counter_1 - SNAKE_START_SIZE) / 100;
        score[1] = 48 + (b_state->counter_1 - SNAKE_START_SIZE) % 100 / 10;
        score[2] = 48 + (b_state->counter_1 - SNAKE_START_SIZE) % 100 % 10;
        buffString(loc.x+2, loc.y + 2,
                    score,
                    &main_buff);

        b_state->big_counter = 0;

        putPixel_toBuff(&main_buff,
                        food.x,
                        food.y,
                        1);

        if(snake_ll[0].location.x == food.x
                 && snake_ll[0].location.y == food.y)
        {
            srand(food.x);

            food.x = (food.x + 5) %62;
            food.y = (food.y + 5) %38 + 10;
//            food.x = rand()%81 + 2;
//            food.y = rand()%45 + 2;
            b_state->counter_1++;
            for(i = 0; i < b_state->counter_1; i++)
            {
                if(snake_ll[i].location.x == food.x
                  && snake_ll[i].location.y == food.y)
                {
                    //re init x and y
                    food.x = (food.x + 5) %62;
                    food.y = (food.y + 5) %38 + 10;
//                    food.x = rand()%81 + 2;
//                    food.y = rand()%45 + 2;
                }
            }
        }

        for(i = b_state->counter_1 - 1; i > 0; i--)
        {
            snake_ll[i].location = snake_ll[i - 1].location;

            putPixel_toBuff(&main_buff,
                            snake_ll[i].location.x,
                            snake_ll[i].location.y,
                            1);
        }

        putPixel_toBuff(&main_buff,
                        snake_ll[0].location.x += snake_ll[0].direction.xV,
                        snake_ll[0].location.y += snake_ll[0].direction.yV,
                            1);

        //check for boundry bust
        if(snake_ll[0].location.x >= 83 || snake_ll[0].location.y >=46
           || snake_ll[0].location.x < 1  || snake_ll[0].location.y < 1)
        {
            switch_state(b_state, &badgy_bird_state);
        }

        for(i = 1; i < b_state->counter_1; i++)
        {
            if(snake_ll[i].location.x == snake_ll[0].location.x
              && snake_ll[i].location.y == snake_ll[0].location.y)
            {
                switch_state(b_state, &badgy_bird_state);
            }
        }
        blitBuff_opt(&main_buff, 0, 0);
    }

    return 0;
}

ON_ENTER_PROTO(snake)
{
    LCDClear();
    return 0;
}

ON_EXIT_PROTO(snake)
{
    b_state->counter_2 = 0;
    b_state->counter_1 = 0;
    b_state->big_counter = 0;
    return 0;
}