/* 
 * File:   gameghost.h
 * Author: Jason P.
 *
 * Created on May 6, 2015, 9:34 PM
 */

#ifndef GAMEGHOST_H
#define	GAMEGHOST_H

//Serial server states
enum{
    GHOST_LOGIN,
    GHOST_HACKIT,
    GHOST_INJECTION1,
    GHOST_INJECTION2,
    GHOST_INJECTION3,
    GHOST_INJECTION4,
    GHOST_INJECTION5,
    GHOST_INJECTION6,
    GHOST_INJECTION7,
    GHOST_INJECTION8,
    GHOST_INJECTION9,
    GHOST_INJECTION10,
    GHOST_INJECTION11,
    GHOST_INJECTION12,
    GHOST_INJECTION13,
    GHOST_INJECTION14,
    GHOST_HELLO,
    GHOST_HELLO2,
    GHOST_HELLO3,
    GHOST_HELLO4,
    GHOST_HELLO5,
    GHOST_HELLO6,
    GHOST_HELLO7,
    GHOST_DONE,
    GHOST_RESET_TRIES,
    GHOST_IO_LOGIN,
    GHOST_IO_LOGIN2,
    GHOST_IO_GOOD,
    GHOST_IR_LOGIN,
    GHOST_IR_LOGIN2,
    GHOST_IR_GOOD,
    GHOST_TEMPEST_LOGIN,
    GHOST_TEMPEST_LOGIN2,
    GHOST_TEMPEST_GOOD,
    GHOST_RESET_SYSTEM
};

volatile char *ghost_hw[];
volatile char *ghost_num[];

//CTF Passcodes
//#define CTF_SOUND       "HWF8372"
//#define CTF_LED         "HWF6253"
//#define CTF_IO          "HWF6525"
//#define CTF_IR          "HWF1221"
//#define CTF_SERIAL      "HWF9823"
//#define CTF_TEMPEST     "HWF7028"



void ghost_app();

extern unsigned int ghost_get_server_state(void);
void ghost_set_server_state(unsigned int value);

unsigned int ghost_get_serial_state(void);
void ghost_set_serial_state(unsigned int value);


//void ghost_print_ser(char *s);

#endif	/* GAMEGHOST_H */

