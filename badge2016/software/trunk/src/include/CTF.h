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
    CTF_EYECOLOR_SER,
    CTF_EYECOLOR_SER2,
    CTF_SERIALKILLER_SER,
    CTF_SERIALKILLER_SER2,
    CTF_EYECOLOR_PASS,
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
    CTF_RESET_SYSTEM,
    CTF_BITBANGING_SER,
    CTF_BITBANGING2_SER,
    CTF_BITBANGING_CHAL_SER,
    CTF_BITBANGING_CHAL_SER2,
    CTF_BITBANGING_PASS
};

volatile char *ctf_hw[];
volatile char *ctf_num[];

//Free Pins

//TRISCbits.TRISC1 = 0;    /* output */
//    LATCbits.LATC1 = 0;      /* blue init low */
//    CNPDCbits.CNPDC1 = 0;    /* pulldown == off */

//#define CTF_FP_1_DIR    TRISBbits.TRISB14
#define CTF_FP_2_DIR    TRISCbits.TRISC2
#define CTF_FP_3_DIR    TRISAbits.TRISA8
#define CTF_FP_4_DIR    TRISCbits.TRISC4
#define CTF_FP_5_DIR    TRISCbits.TRISC5
//#define CTF_FP_6_DIR    TRISBbits.TRISB5
#define CTF_FP_7_DIR    TRISBbits.TRISB7

//#define CTF_FP_1_P      PORTBbits.RB14
#define CTF_FP_2_P      PORTCbits.RC2
#define CTF_FP_3_P      PORTAbits.RA8
#define CTF_FP_4_P      PORTCbits.RC4
#define CTF_FP_5_P      PORTCbits.RC5
//#define CTF_FP_6_P      PORTBbits.RB5
#define CTF_FP_7_P      PORTBbits.RB7

//#define CTF_FP_1_L      LATBbits.LATB14
#define CTF_FP_2_L      LATCbits.LATC2
#define CTF_FP_3_L      LATAbits.LATA8
#define CTF_FP_4_L      LATCbits.LATC4
#define CTF_FP_5_L      LATCbits.LATC5
//#define CTF_FP_6_L      LATBbits.LATB5
#define CTF_FP_7_L      LATBbits.LATB7



void ctf_app();

extern unsigned int ctf_get_server_state(void);
void ctf_set_server_state(unsigned int value);

unsigned int ctf_get_serial_state(void);
void ctf_set_serial_state(unsigned int value);


//void ctf_print_ser(char *s);

#endif	/* GAMECTF_H */

