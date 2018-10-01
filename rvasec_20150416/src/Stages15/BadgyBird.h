/* 
 * File:   BadgyBird.h
 * Author: morgan
 *
 * Created on January 24, 2015, 5:29 PM
 */

#ifndef BADGYBIRD_H
#define	BADGYBIRD_H
#include "../badge15.h"
#include "nokia.h"

#define PIPE_OPENING_HIEGHT 27 //min height for a pipe
#define BIRD_RATE 3000
#define BIRD_ST_HIEGHT 20
#define BIRD_X 20
#define PIPE_RATE 50
#define SHORTEST_PIPE_D 30
#define PIPE_X_B4_ADD 84 - SHORTEST_PIPE_D
#define G_ACC 1
#define PIPE_W 5    //width of the pipes
#define PIPE_H 25   //height of openings
#define MAX_PIPES 2
struct coord pipe_locs[MAX_PIPES], loc;
unsigned char score_str[] = "000";
unsigned char bird_y    = BIRD_ST_HIEGHT,
              collision = 0, 
              b_rand    = 0, 
              pipes_cleared = 0, high_score = 0,
              last_cleared = 99;

char          bird_y_vel   = 0,
              y_acc_length = 0,
              y_acc_mag    = -1,
              draw_pipe    = 0;


DECLARE_BASE_STATE(BadgyBird)

//====================
//Extra state handlers
HANDLER_PROTO(BadgyBirdPlay);

//====================
//Helper functions
void set_score(unsigned char set_score);
#endif	/* BADGYBIRD_H */

