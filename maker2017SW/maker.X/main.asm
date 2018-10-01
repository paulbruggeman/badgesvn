;******************************************************************************
;******************************************************************************




; PIC12LF1571 Configuration Bit Settings

; Assembly source line config statements

; #include "p12lf1571.inc"
#include "p12f1572.inc"

; CONFIG1
; __config 0x39A4
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF
; CONFIG2
; __config 0x1CFF
 __CONFIG _CONFIG2, _WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _BORV_LO & _LPBOREN_OFF & _LVP_OFF



;	extern SendRC5              ; so linker will see it
;------------------------------------------------------------------------------
; VARIABLE DEFINITIONS
;
; Available Data Memory divided into Bank 0-15.  Each Bank may contain 
; Special Function Registers, General Purpose Registers, and Access RAM 
;
;------------------------------------------------------------------------------

; cblock = constants
;cblock 0x20
;  LOOP0
;  LOOP1
;  LOOPCNT
;endc

buttonCnt      EQU    0x70        ; so no need to BANKSEL
redpwm         EQU    0x71        ; so no need to BANKSEL
greenpwm       EQU    0x72        ; so no need to BANKSEL
bluepwm        EQU    0x73        ; so no need to BANKSEL
speakerpwm     EQU    0x74        ; so no need to BANKSEL
red            EQU    0x75        ; so no need to BANKSEL
green          EQU    0x76        ; so no need to BANKSEL
blue           EQU    0x77        ; so no need to BANKSEL
speaker        EQU    0x78        ; so no need to BANKSEL
pwm            EQU    0x79        ; so no need to BANKSEL

;LOOP0         EQU    0x73        ; so no need to BANKSEL
;LOOP1         EQU    0x74        ; so no need to BANKSEL
;LOOPCNT         EQU    0x75        ; so no need to BANKSEL


; uninited data = bss
	udata
audioDur	res 1
audioDurCnt	res 1
audioFreq	res 1
audioFreqCnt	res 1

LOOP0	res 1
LOOP1	res 1
LOOPCNT	res 1

;------------------------------------------------------------------------------
; flash init
;------------------------------------------------------------------------------

; HEF    ORG  0x380 ; piclf1271 high endurance flash 128 bytes
; HEF    ORG  0x780 ; piclf1272 high endurance flash 128 bytes
;     DE    "derp"  ; Place 'derp'

;------------------------------------------------------------------------------
; RESET VECTOR
;------------------------------------------------------------------------------

    ORG     0x0000            ; processor reset vector
    GOTO    START             ; When using debug header, first inst.
                              ; may be passed over by ICD2.  

;------------------------------------------------------------------------------
; interrupt service routine
;---------------------------------------------------------------------------
			      
    ORG      0x0004

;   check if button is pressed
    banksel LATA
    bcf LATA, LATA1 ; set off to drain
    nop ; wait to drain

    banksel TRISA
    bsf TRISA, TRISA1 ; set IR led/button as an input
    nop ; wait to drain

    banksel WPUA ; pullup on
    bsf WPUA, WPUA1
    nop ; wait to drain

    banksel INLVLA ; set input level=TTL off= ~0.15v 
    bcf INLVLA, INLVLA1
    nop ; wait to charge

    banksel PORTA
    movlw 0xFF
    movwf buttonCnt
    btfss PORTA, RA1 ; = 1 = not pressed
    bra doNote
    movlw 0x1 ; off
    movwf buttonCnt
    bra skipNote

doNote
    banksel audioDur
    movlw 0x0 ; decimal
    movwf audioFreqCnt
    movlw 0x0 ; decimal
    movwf audioDurCnt

    movlw 0xFF ; decimal
    movwf audioFreq
    movwf blue
    movlw 0xFF ; decimal
    movwf audioDur
skipNote

;    BANKSEL PORTA
;    movlw 0xFF
;    movwf buttonCnt
;    btfss PORTA, RA1 ; = 1 = not pressed
;    BRA S1
;    movlw 0x1 ; off
;    movwf buttonCnt
;S1

    banksel WPUA 
    bcf WPUA, WPUA1 ; pullup off

    banksel TRISA
    bcf TRISA, TRISA1 ; set IR led/button as an output
;   
;   done with button
;   

    call doAudio

;
; RGB LED pwm
;
;    BCF LATA, LATA4 ; off
;    decfsz speakerpwm, F
;    bra l_speakeron
;    movlw 0x1  ; so next loop see 1-1=0
;    movwf speakerpwm
;    bra l_blue
;l_speakeron
;    BSF LATA, LATA4 ; on

    banksel LATA
l_blue
    bcf LATA, LATA5 ; off
    decfsz bluepwm, F
    bra l_blueon
    movlw 0x1  ; so next loop see 1-1=0
    movwf bluepwm
    bra l_green
l_blueon
    bsf LATA, LATA5 ; on
    
;
l_green
    bcf LATA, LATA0 ; set off
    decfsz greenpwm, F
    bra l_greenon
    movlw 0x1  ; so next loop see 1-1=0
    movwf greenpwm
    bra l_red
l_greenon
    bsf LATA, LATA0 ; set on

;
l_red
    bcf LATA, LATA2 ; off
    decfsz redpwm, F
    bra l_redon
    movlw 0x1  ; so next loop see 1-1=0
    movwf redpwm
    bra l_pwm
l_redon
    bsf LATA, LATA2 ; set on

; reset pwm period and values?
l_pwm
    decfsz pwm, F
    bra done

    movlw 0xFF  ; restart pwm period
    movwf pwm  ; 

    movf red, W ; reset pwm vals
    movwf redpwm
    incf red, F ; increment for fun

    movf green, W ; reset pwm vals
    movwf greenpwm
    decf green, F ; for fun

    movf blue, W ; reset pwm vals
    movwf bluepwm

    movf speaker, W ; reset pwm vals
    movwf speakerpwm
    incf speaker, F ; increment for fun

done
    banksel INTCON
    bcf INTCON, TMR0IF 
    bsf INTCON, TMR0IE 
    ; MOVLW B'11111011' ; timer0 off 
    ; ANDWF INTCON, F

    ; MOVLW B'00100000' ; bit 5=timer0  int enable
    ; IORWF INTCON, F
    retfie                    ; return from interrupt
    
delay
    banksel LOOPCNT
    movwf   LOOPCNT ; save w reg to cnt
    movlw   0xFF
    movwf   LOOP0
    movlw   0x1
    movwf   LOOP1
L0  decfsz  LOOP0, F
    bra	    L0
    decfsz  LOOPCNT, F
    bra	    L0
    decfsz  LOOP1, F
    bra	    L0
    return

doAudio
   banksel audioDur

   ; if (G_duration == 0) return;
   movf audioDur, F ; move to self sets the Z flag
   btfsc STATUS, Z ; zero?
   return

   incf audioFreqCnt, F
   incf audioDurCnt, F

   ; if (G_duration_cnt != G_duration) {
   movf audioDur, W
   subwf audioDurCnt, W
   btfsc STATUS, Z ; zero?
   bra audioDone

   ;   if (G_freq_cnt == G_freq)  {
   movf audioFreq, W
   subwf audioFreqCnt, W
   btfsc STATUS, Z ; zero?
   bra audioOff
   movlw 0x0
   movwf audioFreqCnt

   banksel LATA
   bsf LATA, LATA4 ; on
   return
   ;      G_freq_cnt = 0;
   ;      LATAbits.LATA9 = 1; // on
   ;   }
   ;   else
   ;      LATAbits.LATA9 = 0; // off
   ;   }
audioDone
   movlw 0x0
   movwf audioDur
   ; else {
   ;     G_duration = 0;

audioOff
   banksel LATA
   bcf LATA, LATA4 ; off
   ;    LATAbits.LATA9 = 0; // off
   ;    if (G_audioAssetId != 255) assetList[G_audioAssetId].datacb(G_audioAssetId, G_audioFrame) ; /* callback routine */

audioRet
   return
;
;------------------------------------------------------------------------------
; MAIN PROGRAM
;------------------------------------------------------------------------------

START
    ; INTIALIZING PORTA
    banksel PORTA
    clrf PORTA	; init port A
    
    banksel LATA
    clrf LATA	; init data latch
    
    banksel ANSELA
    clrf ANSELA

    
    ; clock speed
    banksel OSCCON
    ; MOVLW B'01111010' ; pll=0, 1111=ircf, x=0, 1x=SCS   oscilator 16mhz
    movlw B'11110000' ; pll=1, 1110=ircf, x=0, 00=SCS   oscilator 32mhz
    movwf OSCCON
    nop
    nop
    nop ; clock stable ?
    
    ; set port I/O direction
    banksel TRISA
    ; MOVLW B'00001000' ; RA0=out/red, RA1=out/IR LED, in=button,  RA2=OUT/red LED,  RA3=in/IR recv, RA4 = out speaker, RA5: out/blue
    movlw B'00001010' ; RA0=out/red, RA1=out/IR LED, in=button,  RA2=OUT/red LED,  RA3=in/IR recv, RA4 = out speaker, RA5: out/blue
    movwf TRISA

    ; enable weak pull-ups
    banksel OPTION_REG
    bcf OPTION_REG, NOT_WPUEN ; pull up enable bit is active low
    bcf OPTION_REG, PSA ; enable prescaler
    bcf OPTION_REG, PS2_OPTION_REG ; prescaler0,1,2 = 0x001 = 4
    bcf OPTION_REG, PS1_OPTION_REG ; prescaler0,1,2 = 0x001 = 4
    bcf OPTION_REG, PS0_OPTION_REG ; prescaler0,1,2 = 0x001 = 4

    ; pull-up on button
    ; BANKSEL WPUA
    ; BSF WPUA, RA1

    ; 
    ; init vars before any chance of interupt
    ; 
    movlw 0xFF
    movwf buttonCnt

    movlw 0xFF  ; pwm period
    movwf pwm  ; 

    movlw 0xF0
    movwf red  ; 

    movlw 0x20
    movwf green  ; 

    movlw 0xA0
    movwf blue  ; 

    movlw 0x0
    movwf speaker  ; 

; init audio
    banksel audioDur

    movlw 0x0 ; decimal
    movwf audioFreqCnt
    movlw 0x0 ; decimal
    movwf audioDurCnt

    movlw 0x0 ; decimal
    movwf audioFreq
    movlw 0x0 ; decimal
    movwf audioDur

    ; 
    ; timer0 clock source
    ; 
    banksel OPTION_REG
    bcf OPTION_REG, TMR0CS ; int clock-> Fosc/4

    ; timer0 int enable
    banksel INTCON
    bsf INTCON, TMR0IE

    ; global interupts enable 
    bsf  INTCON, GIE 

;    BANKSEL LATA
;    MOVLW B'00110100' ; ON  '00110100' LED 2,4,5 ON, IR 1 LED OFF
;    MOVWF LATA

TOP

    movf  buttonCnt, W
    movwf blue
    ;movwf green
    ;rrf   red, F
    ;rrf   green, F
    ;rrf   blue, F

;    movf  buttonCnt, W
;    movwf speaker

    BRA TOP
;    call SendRC5 ; force linker to include IR code for space check
    END
