 
/* 
 * File:   draw.h
 * Author: jonathan46000.com
 *
 * Created on May 12, 2016, 12:08 PM
 */

#ifndef DRAW_H
#define	DRAW_H

void draw_cb();
void draw_set_color();
void draw_send();
unsigned char draw_send_message();
void draw_receive();
void draw_continue();
void draw_init();
void set_draw_exit();
unsigned char draw_image_received;

#endif	/* DRAW_H */