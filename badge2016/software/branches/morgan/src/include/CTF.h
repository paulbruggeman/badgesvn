/* 
 * File:   gamectf.h
 * Author: Jason P.
 *
 * Created on May 6, 2015, 9:34 PM
 */

#ifndef GAMECTF_H
#define	GAMECTF_H

//Serial server states
enum{
    CTF_LOGIN,
    CTF_HACKIT,
    CTF_INJECTION1,
    CTF_INJECTION2,
    CTF_INJECTION3,
    CTF_INJECTION4,
    CTF_INJECTION5,
    CTF_INJECTION6,
    CTF_INJECTION7,
    CTF_INJECTION8,
    CTF_INJECTION9,
    CTF_INJECTION10,
    CTF_INJECTION11,
    CTF_INJECTION12,
    CTF_INJECTION13,
    CTF_INJECTION14,
    CTF_HELLO,
    CTF_HELLO2,
    CTF_HELLO3,
    CTF_HELLO4,
    CTF_HELLO5,
    CTF_HELLO6,
    CTF_HELLO7,
    CTF_DONE,
    CTF_RESET_TRIES,
    CTF_IO_LOGIN,
    CTF_IO_LOGIN2,
    CTF_IO_GOOD,
    CTF_IR_LOGIN,
    CTF_IR_LOGIN2,
    CTF_IR_GOOD,
    CTF_TEMPEST_LOGIN,
    CTF_TEMPEST_LOGIN2,
    CTF_TEMPEST_GOOD,
    CTF_RESET_SYSTEM
};

volatile char *ctf_hw[];
volatile char *ctf_num[];

//CTF Passcodes
//#define CTF_SOUND       "HWF8372"
//#define CTF_LED         "HWF6253"
//#define CTF_IO          "HWF6525"
//#define CTF_IR          "HWF1221"
//#define CTF_SERIAL      "HWF9823"
//#define CTF_TEMPEST     "HWF7028"



void ctf_app();

extern unsigned int ctf_get_server_state(void);
void ctf_set_server_state(unsigned int value);

unsigned int ctf_get_serial_state(void);
void ctf_set_serial_state(unsigned int value);


//void ctf_print_ser(char *s);

#endif	/* GAMECTF_H */

