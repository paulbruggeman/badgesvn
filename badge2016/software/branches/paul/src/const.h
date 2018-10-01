// #include <plib.h>

// include/peripheral/rtcc.h
// union/structure for read/write of time into the RTCC device
typedef union
{   
    struct
    {   
        unsigned char   rsvd;       // reserved for future use. should be 0
        unsigned char   sec;        // BCD codification for seconds, 00-59
        unsigned char   min;        // BCD codification for minutes, 00-59
        unsigned char   hour;       // BCD codification for hours, 00-24
    };                              // field access
    unsigned char       b[4];       // byte access
    unsigned short      w[2];       // 16 bits access
    unsigned long       l;          // 32 bits access
}rtccTime;

// union/structure for read/write of date into the RTCC device
typedef union
{   
    struct
    {   
        unsigned char   wday;       // BCD codification for day of the week, 00-06
        unsigned char   mday;       // BCD codification for day of the month, 01-31
        unsigned char   mon;        // BCD codification for month, 01-12
        unsigned char   year;       // BCD codification for years, 00-99
    };                              // field access
    unsigned char       b[4];       // byte access
    unsigned short      w[2];       // 16 bits access
    unsigned long       l;          // 32 bits access
}rtccDate;


/*
 /opt/microchip/xc32/v1.34/pic32mx/include/lega-c/proc/p32mx250f128d.h 
*/
struct OSCBITS {
    unsigned OSWEN:1;
    unsigned SOSCEN:1;
    unsigned UFRCEN:1;
    unsigned CF:1;
    unsigned SLPEN:1;
    unsigned SLOCK:1;
    unsigned ULOCK:1;
    unsigned CLKLOCK:1;
    unsigned NOSC:3;
    unsigned :1;
    unsigned COSC:3;
    unsigned :1;
    unsigned PLLMULT:3;
    unsigned PBDIV:2;
    unsigned PBDIVRDY:1;
    unsigned SOSCRDY:1;
    unsigned :1;
    unsigned FRCDIV:3;
    unsigned PLLODIV:3;
} *OSCBITS;

struct INTBITS {
    unsigned CTIE:1;
    unsigned CS0IE:1;
    unsigned CS1IE:1;
    unsigned INT0IE:1;
    unsigned T1IE:1;
    unsigned IC1EIE:1;
    unsigned IC1IE:1;
    unsigned OC1IE:1;
    unsigned INT1IE:1;
    unsigned T2IE:1;
    unsigned IC2EIE:1;
    unsigned IC2IE:1;
    unsigned OC2IE:1;
    unsigned INT2IE:1;
    unsigned T3IE:1;
    unsigned IC3EIE:1;
    unsigned IC3IE:1;
    unsigned OC3IE:1;
    unsigned INT3IE:1;
    unsigned T4IE:1;
    unsigned IC4EIE:1;
    unsigned IC4IE:1;
    unsigned OC4IE:1;
    unsigned INT4IE:1;
    unsigned T5IE:1;
    unsigned IC5EIE:1;
    unsigned IC5IE:1;
    unsigned OC5IE:1;
    unsigned AD1IE:1;
    unsigned FSCMIE:1;
    unsigned RTCCIE:1;
    unsigned FCEIE:1;
} *INTBITS;

struct CFGBITS{
  unsigned TDOEN:1;
  unsigned :2;
  unsigned JTAGEN:1;
  unsigned :8;
  unsigned PMDLOCK:1;
  unsigned IOLOCK:1;
} *CFGBITS;

struct INT1REMAPBITS {
  unsigned INT1R:4;
} INT1REMAPBITS ;

struct INT_EDGEP {
    unsigned INT0EP:1;
    unsigned INT1EP:1;
    unsigned INT2EP:1;
    unsigned INT3EP:1;
    unsigned INT4EP:1;
    unsigned :3;
    unsigned TPC:3;
    unsigned :1;
    unsigned MVEC:1;
    unsigned :3;
    unsigned SS0:1;
} *INT_EDGEP;

struct INT_PRI{
    unsigned T1IS:2;
    unsigned T1IP:3;
    unsigned :3;
    unsigned IC1IS:2;
    unsigned IC1IP:3;
    unsigned :3;
    unsigned OC1IS:2;
    unsigned OC1IP:3;
    unsigned :3;
    unsigned INT1IS:2;
    unsigned INT1IP:3;
} *INT_PRI;

struct IFS0bits {
    unsigned CTIF:1;
    unsigned CS0IF:1;
    unsigned CS1IF:1;
    unsigned INT0IF:1;
    unsigned T1IF:1;
    unsigned IC1EIF:1;
    unsigned IC1IF:1;
    unsigned OC1IF:1;
    unsigned INT1IF:1;
    unsigned T2IF:1;
    unsigned IC2EIF:1;
    unsigned IC2IF:1;
    unsigned OC2IF:1;
    unsigned INT2IF:1;
    unsigned T3IF:1;
    unsigned IC3EIF:1;
    unsigned IC3IF:1;
    unsigned OC3IF:1;
    unsigned INT3IF:1;
    unsigned T4IF:1;
    unsigned IC4EIF:1;
    unsigned IC4IF:1;
    unsigned OC4IF:1;
    unsigned INT4IF:1;
    unsigned T5IF:1;
    unsigned IC5EIF:1;
    unsigned IC5IF:1;
    unsigned OC5IF:1;
    unsigned AD1IF:1;
    unsigned FSCMIF:1;
    unsigned RTCCIF:1;
    unsigned FCEIF:1;
} *IFS0bits;


typedef enum
{
    RTCC_CLK_ON,            // success, clock is running
    RTCC_SOSC_NRDY,         // SOSC not running
    RTCC_CLK_NRDY,          // RTCC clock not running
    RTCC_WR_DSBL,           // WR is disabled
}rtccRes;


struct R {
     unsigned b0:1;
     unsigned b1:1;
     unsigned b2:1;
     unsigned b3:1;
     unsigned b4:1;
     unsigned b5:1;
     unsigned b6:1;
     unsigned b7:1;

     unsigned b8:1;
     unsigned b9:1;
     unsigned b10:1;
     unsigned b11:1;
     unsigned b12:1;
     unsigned b13:1;
     unsigned b14:1;
     unsigned b15:1;
} *R;

#define NULL 0

#define TIME rtccTime
#define DATE rtccDate

#define A(X) *((unsigned long *)X)

/* A */
#define TRISA    A(0xBF886010)
#define TRISACLR A(0xBF886014)
#define TRISASET A(0xBF886018)
#define TRISAINV A(0xBF88601C)

#define PORTA    A(0xBF886020)
#define PORTACLR A(0xBF886024)
#define PORTASET A(0xBF886028)
#define PORTAINV A(0xBF88602C)

#define LATA    A(0xBF886030)
#define LATACLR A(0xBF886034)
#define LATASET A(0xBF886038)
#define LATAINV A(0xBF88603C)

#define ANSELA A(0xBF886000)
#define ANSELACLR A(0xBF886004)
#define ANSELASET A(0xBF886008)
#define ANSELAINV A(0xBF88600C)

#define CNPUA    A(0xBF886050)
#define CNPUACLR A(0xBF886054)
#define CNPUASET A(0xBF886058)
#define CNPUAINV A(0xBF88605C)

#define CNPDA    A(0xBF886060)
#define CNPDACLR A(0xBF886064)
#define CNPDASET A(0xBF886068)
#define CNPDAINV A(0xBF88606C)


/* B */
#define TRISB    A(0xBF886110)
#define TRISBCLR A(0xBF886114)
#define TRISBSET A(0xBF886118)
#define TRISBINV A(0xBF88611C)

#define PORTB    A(0xBF886120)
#define PORTBCLR A(0xBF886124)
#define PORTBSET A(0xBF886128)
#define PORTBINV A(0xBF88612C)

#define LATB    A(0xBF886130)
#define LATBCLR A(0xBF886134)
#define LATBSET A(0xBF886138)
#define LATBINV A(0xBF88613C)

#define ANSELB  A(0xBF886100)
#define ANSELBCLR A(0xBF886104)
#define ANSELBSET A(0xBF886108)
#define ANSELBINV A(0xBF88610C)

#define CNPDB     A(0xBF886160)
#define CNPDBCLR  A(0xBF886164)
#define CNPDBSET  A(0xBF886168)
#define CNPDBINV  A(0xBF88616C)

#define CNPUB    A(0xBF886150)
#define CNPUBCLR A(0xBF886154)
#define CNPUBSET A(0xBF886158)
#define CNPUBINV A(0xBF88615C)

/* C */
#define TRISC    A(0xBF886210)
#define TRISCCLR A(0xBF886214)
#define TRISCSET A(0xBF886218)
#define TRISCINV A(0xBF88621C)

#define PORTC    A(0xBF886220)
#define PORTCCLR A(0xBF886224)
#define PORTCSET A(0xBF886228)
#define PORTCINV A(0xBF88622C)

#define LATC    A(0xBF886230)
#define LATCCLR A(0xBF886234)
#define LATCSET A(0xBF886238)
#define LATCINV A(0xBF88623C)

#define ANSELC    A(0xBF886200)
#define ANSELCCLR A(0xBF886204)
#define ANSELCSET A(0xBF886208)
#define ANSELCINV A(0xBF88620C)

#define CNPDC    A(0xBF886260)
#define CNPDCCLR A(0xBF886264)
#define CNPDCSET A(0xBF886268)
#define CNPDCINV A(0xBF88626C)

#define CNPUC    A(0xBF886250)
#define CNPUCCLR A(0xBF886254)
#define CNPUCSET A(0xBF886258)
#define CNPUCINV A(0xBF88625C)


#define SYSKEY    A(0xBF80F230)
#define SYSKEYCLR A(0xBF80F234)
#define SYSKEYSET A(0xBF80F238)
#define SYSKEYINV A(0xBF80F23C)

#define OSCCON    A(0xBF80F000)
#define OSCCONCLR A(0xBF80F004)
#define OSCCONSET A(0xBF80F008)
#define OSCCONINV A(0xBF80F00C)

#define T2CON    A(0xBF800800)
#define T2CONCLR A(0xBF800804)
#define T2CONSET A(0xBF800808)
#define T2CONINV A(0xBF80080C)

#define TMR2    A(0xBF800810)
#define TMR2CLR A(0xBF800814)
#define TMR2SET A(0xBF800818)
#define TMR2INV A(0xBF80081C)

#define PR2    A(0xBF800820)
#define PR2CLR A(0xBF800824)
#define PR2SET A(0xBF800828)
#define PR2INV A(0xBF80082C)

#define T3CON    A(0xBF800A00)
#define T3CONCLR A(0xBF800A04)
#define T3CONSET A(0xBF800A08)
#define T3CONINV A(0xBF800A0C)

#define TMR3 A(0xBF800A10)
#define TMR3CLR A(0xBF800A14)
#define TMR3SET A(0xBF800A18)
#define TMR3INV A(0xBF800A1C)

#define PR3 A(0xBF800A20)
#define PR3CLR A(0xBF800A24)
#define PR3SET A(0xBF800A28)
#define PR3INV A(0xBF800A2C)


#define T4CON A(0xBF800C00)
#define T4CONCLR A(0xBF800C04)
#define T4CONSET A(0xBF800C08)
#define T4CONINV A(0xBF800C0C)

#define TMR4 A(0xBF800C10)
#define TMR4CLR A(0xBF800C14)
#define TMR4SET A(0xBF800C18)
#define TMR4INV A(0xBF800C1C)

#define PR4 A(0xBF800C20)
#define PR4CLR A(0xBF800C24)
#define PR4SET A(0xBF800C28)
#define PR4INV A(0xBF800C2C)

#define INT1RE A(0xBF80FA04)
#define INT1REMAP ((struct INT1REMAPBITS*)INT1RE)->INT1R

#define CFGCON A(0xBF80F200)
#define JTAGEN ((struct CFGBITS*)CFGCON)->JTAGEN
#define CFGIOLOCK ((struct CFGBITS*)CFGCON)->IOLOCK

#define OSCCON A(0xBF80F000)
#define OSC_ENABLE ((struct OSCBITS*)OSCCON)->SOSCEN

#define IEC0    A(0xBF881060)
#define IEC0CLR A(0xBF881064)
#define IEC0SET A(0xBF881068)
#define IEC0INV A(0xBF88106C)
#define INT1ENABLE ((struct INTBITS*)IEC0)->INT1IE
#define TIMER2ENABLE ((struct INTBITS*)IEC0)->T2IE

#define INTCON  A(0xBF881000)
#define INT1EP ((struct INT_EDGEP*)INTCON)->INT1EP

#define IPC1 A(0xBF8810A0)
#define INTPRI ((struct INT_PRI*)IPC1)->INT1IP
#define INTSUB ((struct INT_PRI*)IPC1)->INT1IS

#define IFS0 A(0xBF881030)
#define INT1IF ((struct IFS0bits*)IFS0)->INT1IF

#define __ISR(X, Y) 


/*
#define PIEZO_BIT LATAbits.LATA9
*/
#define PIEZO_BIT ((struct R*)LATASET)->b9 

/*
#define BACKLIGHT_TRIS TRISCbits.TRISC9
#define BACKLIGHT_PIN LATCbits.LATC9
#define BACKLIGHT_PULLDOWN CNPDCbits.CNPDC9
*/
#define BACKLIGHT_TRIS ((struct R*)TRISCSET)->b9 
#define BACKLIGHT_PIN ((struct R*)LATCSET)->b9 
#define BACKLIGHT_PULLDOWN ((struct R*)CNPDCSET)->b9 


/*
#define nRESET_TRIS TRISCbits.TRISC8
#define nRESET_PIN LATCbits.LATC8
#define nRESET_PULLDOWN CNPDCbits.CNPDC8
*/

#define nRESET_TRIS ((struct R*)TRISCSET)->b8
#define nRESET_PIN ((struct R*)LATCSET)->b8
#define nRESET_PULLDOWN ((struct R*)CNPDCSET)->b8

/*
#define A0_TRIS TRISCbits.TRISC7
#define A0_PIN LATCbits.LATC7
#define A0_PULLDOWN CNPDCbits.CNPDC7
*/

#define A0_TRIS ((struct R*)TRISASET)->b7
#define A0_PIN ((struct R*)LATASET)->b7
#define A0_PULLDOWN ((struct R*)CNPDASET)->b7

/*
#define SDA_TRIS TRISCbits.TRISC6
#define SDA_PIN LATCbits.LATC6
#define SDA_PULLDOWN CNPDCbits.CNPDC6
*/

#define SDA_TRIS ((struct R*)TRISCSET)->b6
#define SDA_PIN ((struct R*)LATCSET)->b6
#define SDA_PULLDOWN ((struct R*)CNPDCSET)->b6


/*
#define SCLK_TRIS TRISBbits.TRISB9
#define SCLK_PIN LATBbits.LATB9
#define SCLK_PULLDOWN CNPDBbits.CNPDB9
*/

#define SCLK_TRIS ((struct R*)TRISBSET)->b9
#define SCLK_PIN ((struct R*)LATBSET)->b9
#define SCLK_PULLDOWN ((struct R*)CNPDBSET)->b9

/*
#define nCS_TRIS TRISBbits.TRISB8
#define nCS_PIN LATBbits.LATB8
#define nCS_PULLDOWN CNPDBbits.CNPDB8
*/

#define nCS_TRIS ((struct R*)TRISBSET)->b8
#define nCS_PIN ((struct R*)LATBSET)->b8
#define nCS_PULLDOWN ((struct R*)CNPDBSET)->b8

/*
#define RED_TRIS TRISCbits.TRISC0
#define RED_PIN LATCbits.LATC0
#define RED_PULLDOWN CNPDCbits.CNPDC0
*/
#define RED_TRIS ((struct R*)TRISCSET)->b0
#define RED_PIN ((struct R*)LATCSET)->b0
#define RED_PULLDOWN ((struct R*)CNPDC)->b0

/*
#define BLUE_TRIS TRISCbits.TRISC1
#define BLUE_PIN LATCbits.LATC1
#define BLUE_PULLDOWN CNPDCbits.CNPDC1
*/

#define BLUE_TRIS ((struct R*)TRISCSET)->b1
#define BLUE_PIN ((struct R*)LATCSET)->b1
#define BLUE_PULLDOWN ((struct R*)CNPDC)->b1

/*
#define GREEN_TRIS TRISBbits.TRISB3
#define GREEN_PIN LATBbits.LATB3
#define GREEN_PULLDOWN CNPDBbits.CNPDB3
*/

#define GREEN_TRIS ((struct R*)TRISCSET)->b3
#define GREEN_PIN ((struct R*)LATCSET)->b3
#define GREEN_PULLDOWN ((struct R*)CNPDC)->b3

#define PORTA_ANALOG ANSELA 
#define PORTB_ANALOG ANSELB 
#define PORTC_ANALOG ANSELC 

#define PORTA_TRIS TRISA 
#define PORTA_PINS LATA 

#define PORTB_TRIS TRISB 
#define PORTB_PINS LATB 

#define PORTC_TRIS TRISC 
#define PORTC_PINS LATC 

/*
#define BUTTON_TRIS TRISCbits.TRISC3
#define BUTTON_PIN LATCbits.LATC3
#define BUTTON_PULLUP CNPUCbits.CNPUC3
*/
#define BUTTON_TRIS ((struct R*)TRISCSET)->b3
#define BUTTON_PIN ((struct R*)LATCSET)->b3
#define BUTTON_PULLUP ((struct R*)CNPUC)->b3

/*
#define SPEAKER_TRIS TRISAbits.TRISA9
#define SPEAKER_PIN LATAbits.LATA9
#define SPEAKER_PULLUP CNPUAbits.CNPUA9
#define SPEAKER_PULLDOWN CNPDAbits.CNPDA9
*/
#define SPEAKER_TRIS ((struct R*)TRISASET)->b9
#define SPEAKER_PIN ((struct R*)LATASET)->b9
#define SPEAKER_PULLUP ((struct R*)CNPUA)->b9
#define SPEAKER_PULLDOWN ((struct R*)CNPDA)->b9


#define TOUCH_A0 TRISAbits.TRISA0
#define TOUCH_A1 TRISAbits.TRISA1

#define TOUCH_B1 TRISBbits.TRISB1
#define TOUCH_B2 TRISBbits.TRISB2

#define IR_REC_TRIS ((struct R*)TRISBSET)->b0
#define IR_REC_PIN ((struct R*)LATBSET)->b0
#define IR_REC_PULLUP ((struct R*)CNPUB)->b0
#define IR_REC_PULLDOWN ((struct R*)CNPDB)->b0

#define IRLED_TRIS ((struct R*)TRISBSET)->b13
#define IRLED_PIN ((struct R*)LATBSET)->b13
#define IRLED_PULLUP ((struct R*)CNPUB)->b13
#define IRLED_PULLDOWN ((struct R*)CNPDB)->b13


#define _T2CON_ON_POSITION                       0x0000000F
#define _T2CON_ON_MASK                           0x00008000
#define _T2CON_ON_LENGTH                         0x00000001

#define _T3CON_ON_POSITION                       0x0000000F
#define _T3CON_ON_MASK                           0x00008000
#define _T3CON_ON_LENGTH                         0x00000001

#define _T4CON_ON_POSITION                       0x0000000F
#define _T4CON_ON_MASK                           0x00008000
#define _T4CON_ON_LENGTH                         0x00000001


/*
include/peripheral/timer.h
*/


#define OpenTimer2(config, period)  (T2CON = ((config)&~(T2_ON)), TMR2 = 0, PR2 = (period), T2CONSET=((config)&(T2_ON)) )

    /******************************************************************************
     * Available options for config parameter
     *****************************************************************************/
        // On/off control - values are mutually exclusive
        #define T2_ON                           (1 << _T2CON_ON_POSITION)       /* Timer2 ON */
        #define T2_OFF                          (0)

        // Stop-in-idle control - values are mutually exclusive
        #define T2_IDLE_STOP                    (1 << _T2CON_SIDL_POSITION)     /* stop during idle */
        #define T2_IDLE_CON                     (0)                             /* operate during idle */

        // Timer gate control - values are mutually exclusive
        #define T2_GATE_ON                      (1 << _T2CON_TGATE_POSITION)    /* Timer Gate accumulation mode ON */
        #define T2_GATE_OFF                     (0)

        // Prescale values - values are mutually exclusive
        #define T2_PS_1_256                     (7 << _T2CON_TCKPS_POSITION)    /* Prescaler 1:256 */
        #define T2_PS_1_64                      (6 << _T2CON_TCKPS_POSITION)    /*           1:64 */
        #define T2_PS_1_32                      (5 << _T2CON_TCKPS_POSITION)    /*           1:32 */
        #define T2_PS_1_16                      (4 << _T2CON_TCKPS_POSITION)    /*           1:16 */
        #define T2_PS_1_8                       (3 << _T2CON_TCKPS_POSITION)    /*           1:8 */
        #define T2_PS_1_4                       (2 << _T2CON_TCKPS_POSITION)    /*           1:4 */
        #define T2_PS_1_2                       (1 << _T2CON_TCKPS_POSITION)    /*           1:2 */
        #define T2_PS_1_1                       (0)                             /*           1:1 */

        // 32-bit or 16-bit - values are mutually exclusive
        #define T2_32BIT_MODE_ON                (1 << _T2CON_T32_POSITION)      /* Enable 32-bit mode */
        #define T2_32BIT_MODE_OFF               (0)

        // Sync external clock option - values are mutually exclusive
        #define T2_SOURCE_EXT                   (1 << _T2CON_TCS_POSITION)      /* External clock source */
        #define T2_SOURCE_INT                   (0)                             /* Internal clock source */


#define ConfigIntTimer2(config) (mT2ClearIntFlag(), mT2SetIntPriority(((config) & 7)),\
        mT2SetIntSubPriority(((config) >> 4) & 3), mT2IntEnable((config) >> 15))

    /******************************************************************************
     * Available options for config parameter
     *****************************************************************************/
        // Interrupt on/off - values are mutually exclusive
        #define T2_INT_ON                       (1 << 15)       /* T2 Interrupt Enable */
        #define T2_INT_OFF                      (0)

        // Interrupt priority - values are mutually exclusive
        #define T2_INT_PRIOR_7                  (7)
        #define T2_INT_PRIOR_6                  (6)
        #define T2_INT_PRIOR_5                  (5)
        #define T2_INT_PRIOR_4                  (4)
        #define T2_INT_PRIOR_3                  (3)
        #define T2_INT_PRIOR_2                  (2)
        #define T2_INT_PRIOR_1                  (1)
        #define T2_INT_PRIOR_0                  (0)

        // Interrupt sub-priority - values are mutually exclusive
        #define T2_INT_SUB_PRIOR_3              (3 << 4)
        #define T2_INT_SUB_PRIOR_2              (2 << 4)
        #define T2_INT_SUB_PRIOR_1              (1 << 4)
        #define T2_INT_SUB_PRIOR_0              (0 << 4)



#define OpenTimer3(config, period)  (T3CON = ((config)&~(T3_ON)), TMR3 = 0, PR3 = (period), T3CONSET=((config)&(T3_ON)) )

    /******************************************************************************
     * Available options for config parameter
     *****************************************************************************/
        // On/off control - values are mutually exclusive
        #define T3_ON                           (1 << _T3CON_ON_POSITION)       /* Timer3 ON */
        #define T3_OFF                          (0)

        // Stop-in-idle control - values are mutually exclusive
        #define T3_IDLE_STOP                    (1 << _T3CON_SIDL_POSITION)     /* stop during idle */
        #define T3_IDLE_CON                     (0)                             /* operate during idle */

        // Timer gate control - values are mutually exclusive
        #define T3_GATE_ON                      (1 << _T3CON_TGATE_POSITION)    /* Timer Gate accumulation mode ON */
        #define T3_GATE_OFF                     (0)

        // Timer prescaler control - values are mutually exclusive
        #define T3_PS_1_256                     (7 << _T3CON_TCKPS_POSITION)    /* Prescaler 1:256 */
        #define T3_PS_1_64                      (6 << _T3CON_TCKPS_POSITION)    /*           1:64 */
        #define T3_PS_1_32                      (5 << _T3CON_TCKPS_POSITION)    /*           1:32 */
        #define T3_PS_1_16                      (4 << _T3CON_TCKPS_POSITION)    /*           1:16 */
        #define T3_PS_1_8                       (3 << _T3CON_TCKPS_POSITION)    /*           1:8 */
        #define T3_PS_1_4                       (2 << _T3CON_TCKPS_POSITION)    /*           1:4 */
        #define T3_PS_1_2                       (1 << _T3CON_TCKPS_POSITION)    /*           1:2 */
        #define T3_PS_1_1                       (0)                             /*           1:1 */

        // Source selection - values are mutually exclusive
        #define T3_SOURCE_EXT                   (1 << _T3CON_TCS_POSITION)      /* External clock source */
        #define T3_SOURCE_INT                   (0)                             /* Internal clock source */


#define ConfigIntTimer3(config) (mT3ClearIntFlag(), mT3SetIntPriority(((config) & 7)),\
        mT3SetIntSubPriority(((config) >> 4) & 3), mT3IntEnable((config) >> 15))

    /******************************************************************************
     * Available options for config parameter
     *****************************************************************************/
        // Interrupt on/off - values are mutually exclusive
        #define T3_INT_ON                       (1<<15)     /* Interrupt Enable */
        #define T3_INT_OFF                      (0)

        // Interrupt priority - values are mutually exclusive
        #define T3_INT_PRIOR_7                  (7)
        #define T3_INT_PRIOR_6                  (6)
        #define T3_INT_PRIOR_5                  (5)
        #define T3_INT_PRIOR_4                  (4)
        #define T3_INT_PRIOR_3                  (3)
        #define T3_INT_PRIOR_2                  (2)
        #define T3_INT_PRIOR_1                  (1)
        #define T3_INT_PRIOR_0                  (0)

        // Interrupt sub-priority - values are mutually exclusive
        #define T3_INT_SUB_PRIOR_3              (3 << 4)
        #define T3_INT_SUB_PRIOR_2              (2 << 4)
        #define T3_INT_SUB_PRIOR_1              (1 << 4)
        #define T3_INT_SUB_PRIOR_0              (0 << 4)


#define OpenTimer4(config, period)  (T4CON = ((config)&~(T4_ON)), TMR4 = 0, PR4 = (period), T4CONSET=((config)&(T4_ON)) )

    /******************************************************************************
     * Available options for config parameter
     *****************************************************************************/
        // On/off control - values are mutually exclusive
        #define T4_ON                           (1 << _T4CON_ON_POSITION)       /* Timer4 ON */
        #define T4_OFF                          (0)

        // Stop-in-idle control - values are mutually exclusive
        #define T4_IDLE_STOP                    (1 << _T4CON_SIDL_POSITION)     /* stop during idle */
        #define T4_IDLE_CON                     (0)                             /* operate during idle */

        // Timer gate control - values are mutually exclusive
        #define T4_GATE_ON                      (1 << _T4CON_TGATE_POSITION)    /* Timer Gate accumulation mode ON */
        #define T4_GATE_OFF                     (0)

        // Timer prescaler control - values are mutually exclusive
        #define T4_PS_1_256                     (7 << _T4CON_TCKPS_POSITION)    /* Prescaler 1:256 */
        #define T4_PS_1_64                      (6 << _T4CON_TCKPS_POSITION)    /*           1:64 */
        #define T4_PS_1_32                      (5 << _T4CON_TCKPS_POSITION)    /*           1:32 */
        #define T4_PS_1_16                      (4 << _T4CON_TCKPS_POSITION)    /*           1:16 */
        #define T4_PS_1_8                       (3 << _T4CON_TCKPS_POSITION)    /*           1:8 */
        #define T4_PS_1_4                       (2 << _T4CON_TCKPS_POSITION)    /*           1:4 */
        #define T4_PS_1_2                       (1 << _T4CON_TCKPS_POSITION)    /*           1:2 */
        #define T4_PS_1_1                       (0)                             /*           1:1 */

        // 32-bit or 16-bit option - values are mutually exclusive
        #define T4_32BIT_MODE_ON                (1 << _T4CON_T32_POSITION)      /* Enable 32-bit mode */
        #define T4_32BIT_MODE_OFF               (0)

        // Source selection - values are mutually exclusive
        #define T4_SOURCE_EXT                   (1 << _T4CON_TCS_POSITION)      /* External clock source */
        #define T4_SOURCE_INT                   (0)                             /* Internal clock source */

#define ConfigIntTimer4(config) (mT4ClearIntFlag(), mT4SetIntPriority((config) & 7), \
        mT4SetIntSubPriority(((config) >> 4) & 3),  mT4IntEnable((config) >> 15))


    /******************************************************************************
     * Available options for config parameter
     *****************************************************************************/
        // Interrupt on/off - values are mutually exclusive
        #define T4_INT_ON                       (1<<15)     /* Interrupt Enable */
        #define T4_INT_OFF                      (0)

        // Interrupt priority - values are mutually exclusive
        #define T4_INT_PRIOR_7                  (7)
        #define T4_INT_PRIOR_6                  (6)
        #define T4_INT_PRIOR_5                  (5)
        #define T4_INT_PRIOR_4                  (4)
        #define T4_INT_PRIOR_3                  (3)
        #define T4_INT_PRIOR_2                  (2)
        #define T4_INT_PRIOR_1                  (1)
        #define T4_INT_PRIOR_0                  (0)

        // Interrupt sub-priority - values are mutually exclusive
        #define T4_INT_SUB_PRIOR_3              (3 << 4)
        #define T4_INT_SUB_PRIOR_2              (2 << 4)
        #define T4_INT_SUB_PRIOR_1              (1 << 4)
        #define T4_INT_SUB_PRIOR_0              (0 << 4)

