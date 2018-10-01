/* 
 * File:   Snake.h
 * Author: morgan
 *
 * Created on January 24, 2015, 3:51 PM
 */

#ifndef SNAKE_H
#define	SNAKE_H
#include "../badge15.h"
#include "nokia.h"

#define STATE_NAME snake
#define SNAKE_RATE 6000
#define SNAKE_START_SIZE 5

struct CoOrd
{
    unsigned char x, y;
};

//movement vector
struct Velocity
{
    unsigned char xV, yV;
};

//snake linked list
struct SnakeSeg
{
    struct CoOrd location;
    struct Velocity direction;
    struct SnakeSeg *next;

};
struct SnakeSeg snake_ll[200];
DECLARE_BASE_STATE(STATE_NAME)

#endif	/* SNAKE_H */

