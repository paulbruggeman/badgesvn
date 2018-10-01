#include "lander.h"

void lander_bound(unsigned char *x_pos, unsigned char *y_pos,
                  unsigned char *x_vel, unsigned char *y_vel)
{
    // Overflow or at edge
    if(*x_pos < 5 || *x_pos > 200){
        *x_vel = 0;
        *x_pos = 5;
    }
    else if(*x_pos > (132 - LANDER_WIDTH - 3)){
        *x_vel = 0;
        *x_pos = 132 - LANDER_WIDTH - 3;
    }

    // Overflow or at edge
    if(*y_pos < 1 || *y_pos > 200){
        *y_vel = 0;
        *y_pos = 1;
    }
    else if(*y_pos > (132 - LANDER_HEIGHT)){
        *y_vel = 0;
        *y_pos = 132 - LANDER_HEIGHT;
    }
}

// given points and offset, see if two ranges are overlapping
unsigned char lander_is_overlap(unsigned char p1, unsigned char o1,
                                unsigned char p2, unsigned char o2){
    return ((p1+o1) > p2) && (p1 < (p2 + o2));
}


unsigned char lander_is_colliding(unsigned char x1,unsigned char y1,unsigned char w1,unsigned char h1,
                         unsigned char x2,unsigned char y2,unsigned char w2,unsigned char h2)
{
    unsigned char x_check = lander_is_overlap(x1, w1, x2, w2);
    unsigned char y_check = lander_is_overlap(y1, h1, y2, h2);
    return (x_check && y_check);
}

unsigned char lander_obj_is_colliding(object_state o1,
                                      object_state o2)
{
    //unsigned char x_check = lander_is_overlap(x1, w1, x2, w2);
    //unsigned char y_check = lander_is_overlap(y1, h1, y2, h2);
    unsigned char x_check = lander_is_overlap(o1.pos_x, o1.width, o2.pos_x, o2.width);
    unsigned char y_check = lander_is_overlap(o1.pos_y, o1.height, o2.pos_y, o2.height);
    return (x_check && y_check);
}
