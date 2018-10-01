;------------------------------------------------------------------
;	FILE:		ir_tx_RC5.asm
;	AUTHOR:		Tom Perme
;	COMPANY:	Microchip Technology, Inc.
;	DEVICE:		10F206
;	DEVICE:		18F2455 by PEB
;	CREATED:	10/08/2006
;	UPDATED:	mm/dd/yyyy
;
;	DESCRIP:	Application Note example file to illustrate RC5 
;				protocol being transmitted over an infrared LED.
;
;  Software License Agreement:
; 
;  The software supplied herewith by Microchip Technology Incorporated
;  (the "Company") for its PICmicro® Microcontroller is intended and
;  supplied to you, the Company's customer, for use solely and
;  exclusively on Microchip PICmicro Microcontroller products. The
;  software is owned by the Company and/or its supplier, and is
;  protected under applicable copyright laws. All rights are reserved.
;  Any use in violation of the foregoing restrictions may subject the
;  user to criminal sanctions under applicable laws, as well as to
;  civil liability for the breach of the terms and conditions of this
;  license.
; 
;  THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
;  WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
;  TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
;  PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
;  IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
;  CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;------------------------------------------------------------------

#include "p12lf1571.inc"

; RB5 == led output
#define	OUTPUT_LED 		PORTA, RA1	; Define the OUTPUT LED PIN  0/off, 1/on


; CONSTANTS
#define TV1				0x00		; Address for device TV1
#define CD_PLAYER		0x14		; Address value for 'RC5' CD Player

#define PLAY_CMD		0x35		; Command value for RC5 Data "Play" 
#define STOP_CMD		0x36		; Command value for RC5 Data "Stop"
#define	VOLUP_CMD		0x10		; Command values for volume up
#define VOLDOWN_CMD		0x11		;  and volume down.


;    UDATA_ACS

Delay_Count 	EQU 0x7E		; Define registers for
Delay_Count2 	EQU 0x7F		; delay routines (0x10,0x11 for 10F20x)

DataByte	EQU 0x80		; Define a byte to use for RC5 Data
AddrByte	EQU 0x81		; Define a byte to use for RC5 address
ToggByte	EQU 0x82		; Define a byte to determine toggle low or high

	; export vars
;	global DataByte
;	global AddrByte
;	global ToggByte

	; function
	global SendRC5              ; so linker will see it

;
; PEB: dead code from ir_tx_RC5.asm left in case I missed something
;

; IR_Init:
;
; Initialize port functions and directions
;
; 10F20x parts must disable the comparator to use GPIO pins as I/O
; 	bcf 	CMCON0, CMPON		; Disable comparator
;
; Also disable Timer 0, and enable pullups on inputs.
; 	movlw 	b'00001000'			; Enable wake-up on change, pullups, and
; 	option 						; set prescalar to WDT to disable tmr0
;
; 	movlw 	b'00000011'			; Define GP0, GP1 as inputs.
; 	tris 	GPIO				; Load W into TRIS
;
; 	bcf		OUTPUT_LED			; Init output off
;
;
; IR_Main:
; send RC5 transmission with toggle bit = 0.
;	movlw	TV1			
;	movwf	AddrByte			; Load Device Address
;	movlw	VOLDOWN_CMD			
;	movwf	DataByte			; Load Data byte with command
;	movlw	0x00				
;	movwf	ToggByte			; Send Toggle=0 for button down.
;	call	SendRC5
;
; Send a final transmission with toggle bit = 1.
;	movlw	TV1			
;	movwf	AddrByte			; Load Device Address
;	movlw	VOLDOWN_CMD			
;	movwf	DataByte			; Load Data byte with command
;	movlw	0xFF
;	movwf	ToggByte			; Send Toggle=1 for button released
;	call	SendRC5				; Send button released.


    CODE

;******************************************************************
;  SUB-ROUTINES
;******************************************************************

;------------------------------------------------------------------
;  SendRC5
;
;	 	Before calling, preload registers DataByte and AddrByte with 
;	appropriate values to be sent.
; 
;	DataByte = 6 bits of data to send (upper 2 bits ignored)
; 	AddrByte = 5 bits of addr to identify target (upper 3 bits ignored)
; 	ToggByte = 0/1  ;  button -> off/on
;------------------------------------------------------------------

SendRC5
; XXX PEB    bcf INTCON, GIEH        ; disable interupts for the duration

	; Pre-shift Addr Byte
        rlf AddrByte, F		; Must be rotated left 3 bits
        rlf AddrByte, F		;
	rlf AddrByte, F		; MSB now is MSB of 5 bit #
	; Pre-shift Data Byte
	rlf DataByte, F		; Must be rotated left 2 bits
	rlf DataByte, F		; MSB now is MSB of 6 bit #

	; SEND PREAMBLE
	call    SendOne			; S1		Start 1
	call    SendOne			; S2		Start 2

	; SEND TOGGLE
	btfss   ToggByte, 0		; if toggle is one, skip instr
	call    SendZero		; Send a 0, toggle byte is zero
	btfsc   ToggByte, 0		; 
	call    SendOne			; toggle byte is set, need to send a 1

	; SEND DATA IN SPEED EFFICIENT MANNER

	; SEND ADDRESS
	; Begin shifting out address
	; bit 4
	rlf		AddrByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 3
	rlf		AddrByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 2
	rlf		AddrByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 1
	rlf		AddrByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 0
	rlf		AddrByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one

	
	; SEND DATA
	; Shift Out DataByte
	; bit 5
	rlf		DataByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 4
	rlf		DataByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 3
	rlf		DataByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 2
	rlf		DataByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 1
	rlf		DataByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one
	; bit 0
	rlf		DataByte, F		; Shift out MSB.. C = MSB
	btfss	STATUS, C		; if bit is 1, skip next instr.
	call 	SendZero		; bit is 0, send a zero
	btfsc	STATUS, C
	call	SendOne			; bit is 1, send a one	
	
	; 25 ms passed

	; Delay remaining time so that repetitive calls to SendRC5
	; occur at 114ms intervals as per RC5 spec.

	; 114-25 = 89ms
	bcf		OUTPUT_LED			; Set output low for off time


    ; 
    ; PEB XXX FIXME
    ; this delay should be removed
    ; and make the caller do it
    ; 

; PEB REMOVED DELAY
; 
;	movlw	d'89'               
;	movwf	Delay_Count2		; outer loop delay counter
;
;	call	delay_1ms			; Delay 1ms ea. time through loop
;D0 decfsz	Delay_Count2, F		; Go through loop 
;	bra     D0					; if count not 0, keep looping
;	;goto	$-2					; if count not 0, keep looping
;
; XXX PEB    bsf INTCON, GIEH            ; enable interupts

	return						; Return from SendRC5 routine




;------------------------------------------------------------------
;  SendOne
;
; 		A "1" in Manchester goes from Low-High over the bit period
; 	Timing based off 4MHz internal clock with 1MHz Instruction
;	cycle. During the pulsing period, the carrier frequency should
;	be 36kHz, and the duty cycle of the carrier should be about 1/4.
;------------------------------------------------------------------
SendOne:

	; LOW HALF (889us = 889 instr cycles)
	bcf 	OUTPUT_LED			; Turn off LED
		; 1 --> 888us			; (-1) instr cycle from total needed
	;
	movlw 	0xFF				; (-1) Move 0xFF (255) into w
	movwf 	Delay_Count			; (-1) Move w -> Delay_Count
DD0	decfsz 	Delay_Count, F		; (-1) Decrement F, skip if result = 0
	bra 	DD0					; (-2) Go back 1, keep decrementing until 0
;	goto 	$-1					; (-2) Go back 1, keep decrementing until 0
		; Loop Eq. = 3*N-1 cycles
		; 3*254 = 764us completed in loop, + 3 cycles beforehand..
		; 767us completed --> 122us to go.
	;
	movlw	d'39'				; -1 (Load to finish time accurately)
	movwf 	Delay_Count			; -1
DD1	decfsz 	Delay_Count, F		; -1
	bra 	DD1					; -2
;	goto 	$-1					; -2
		; NOTE: there are two cycles following this 
		;		before pulsing will start.. so take 2 cycles off desired
		; 1 + 1 + 3*N-1 = 122 - 2 --> N=39.66
		; Choose N = 39, gives 116 cycles in loop, +2 setup, +2 lagging
		; = 116+2+2 = 120   -->   need 2 nops, or 1 goto $+1
	bra 	DD2					; -2
;	goto 	$+1					; -2

	;
	; HIGH HALF (889us)
	; Toggle 7us on, 21us off for 35.7kHz (~36kHz) for the duration
	; Pulsing 7 on, 21 off yields a 1/4 time duty cycle for carrier.
	; Has 32 pulses of these periods 32*28us = 896us (~889us)
	;
	; These two clock cycles contribute to LOW TIME
DD2	movlw	d'32'				; -1	(2 addit'l low cycles on low time)
	movwf	Delay_Count2		; -1	num pulses counter

CarrierLoopOne:
	bsf		OUTPUT_LED			; -1    (BEGIN ON TIME)
	;goto 	$+1					; -2us
	;goto	$+1					; -2us
	;goto	$+1					; -2us  delayed 7us
    bra DL3
DL3 bra DL4
DL4 bra DL5
DL5 bcf		OUTPUT_LED			; -1	(BEGIN OFF TIME)
	movlw	d'5'				; -1 (Load to finish time accurately)
	movwf 	Delay_Count			; -1
DD3	decfsz 	Delay_Count, F		; -1
	bra 	DD3					; -2
;	goto 	$-1					; -2
		; 1 + 1 + 1 + 3*N-1 = 21-3  --> x = 5.33
		; Choose N=5, 1+1+1 + LOOP=14 =17 --> need 1 nop
	nop
	decfsz	Delay_Count2, F		; -1  3us tacked on each pulse xcept last one
	; goto	CarrierLoopOne		; -2  TAKE OFF OF ABOVE CALC
	bra	CarrierLoopOne		; -2  TAKE OFF OF ABOVE CALC

	; DONE Sending a one
	return						; -2 return from subroutine	
	


;------------------------------------------------------------------
;  SendZero
;
; 		A "0" in Manchester goes from High-Low over the bit period.
; 	The high period is a series of pulses of duty cycle 1/4 at a 
;	frequency of 36kHz.  This implementation yields 35.71kHz.
;------------------------------------------------------------------
SendZero:

	; HIGH HALF (889us)
	; Toggle 7us on, 21us off for 35.7kHz (~36kHz) for the duration
	; Pulsing 7 on, 21 off yields a 1/4 time duty cycle for carrier.
	; Has 32 pulses of these periods 32*28us = 896us (~889us)
	;
	; These two clock cycles contribute to LOW TIME
	movlw	d'32'				; -1	(2 addit'l low cycles on low time)
	movwf	Delay_Count2		; -1	num pulses counter

CarrierLoopZero:
	bsf		OUTPUT_LED			; -1    (BEGIN ON TIME)
;	goto 	$+1					; -2us
;	goto	$+1					; -2us
;	goto	$+1					; -2us  delayed 7us
    bra DZ1
DZ1 bra DZ2
DZ2 bra DZ3
DZ3	bcf		OUTPUT_LED			; -1	(BEGIN OFF TIME)
	movlw	d'5'				; -1 (Load to finish time accurately)
	movwf 	Delay_Count			; -1
DZ4	decfsz 	Delay_Count, F		; -1
    bra DZ4
;	goto 	$-1					; -2
		; 1 + 1 + 1 + 3*N-1 = 21-3  --> x = 5.33
		; Choose N=5, 1+1+1 + LOOP=14 =17 --> need 1 nop
	nop
	decfsz	Delay_Count2, F		; -1  3us tacked on each pulse xcept last one
	bra	CarrierLoopZero		; -2  TAKE OFF OF ABOVE CALC
	; goto	CarrierLoopZero		; -2  TAKE OFF OF ABOVE CALC

	; Last pulse needs its off time 
	; that it misses from goto CarrierLoop
	; goto	$+1					; -2
	bra	DZ5					; -2

	; LOW HALF (889us = 889 instr cycles)
DZ5	bcf 	OUTPUT_LED			; Turn off LED
		; 1 --> 888us			; (-1) instr cycle from total needed
	;
	movlw 	0xFF				; (-1) Move 0xFF (255) into w
	movwf 	Delay_Count			; (-1) Move w -> Delay_Count
DZ6	decfsz 	Delay_Count, F		; (-1) Decrement F, skip if result = 0
	bra 	DZ6					; (-2) Go back 1, keep decrementing until 0
;	goto 	$-1					; (-2) Go back 1, keep decrementing until 0
		; Loop Eq. = 3*N-1 cycles
		; 3*254 = 764us completed in loop, + 3 cycles beforehand..
		; 767us completed --> 122us to go.
	;
	movlw	d'39'				; -1 (Load to finish time accurately)
	movwf 	Delay_Count			; -1
DZ7	decfsz 	Delay_Count, F		; -1
;	goto 	$-1					; -2
	bra 	DZ7					; -2
		; NOTE: there are two cycles following this  (return)
		;		before next bit may be sent.
		; 1 + 1 + 3*N-1 = 122 - 2 --> N=39.66
		; Choose N = 39, gives 116 cycles in loop, +2 setup, +2 lagging
		; = 116+2+2 = 120   -->   return finishes the last 2 instr cycl.

	return						; -2


	

;------------------------------------------------------------------
;  delay_1ms
;
;		Precise delay.  Actually delays 999us, but this allows for
;	any loops calling it to incur 1us of overhead.
;------------------------------------------------------------------
delay_1ms:
	movlw	0xFF				; +1 us
	movwf	Delay_Count			; +1 
D1  decfsz 	Delay_Count, F		; Decrement F, skip if result = 0
	bra 	D1					; Go back 1, keep decrementing until 0
;	goto 	$-1					; Go back 1, keep decrementing until 0
								; Loop = 3*N-1 us = 3*255-1
								; +764
	; Need to delay 236us more.  2 of which will be used in return
	movlw	d'76'				; +1
	movwf	Delay_Count			; +1 234 remain, but only acct for 232
D2	decfsz 	Delay_Count, F		; 3*N-1 = 232 --> N = 77.66
	bra 	D2					; Choose N=77
;	goto 	$-1					; Choose N=77


	return						; +2 Return program flow



;------------------------------------------------------------------
;  DebounceDelay
;	
;		Small and simple delay to provide time for bouncing from
;	a button to settle.
;------------------------------------------------------------------
DebounceDelay:
	movlw 	d'3'				; Move 0xFF into w (count, N)
	movwf 	Delay_Count			; Move w -> Delay_Count
D3	decfsz  Delay_Count, F		; Decrement F, skip if result = 0
	bra 	D3					; Go back 1, keep decrementing until 0
;	goto 	$-1					; Go back 1, keep decrementing until 0
								; Loop delay = 3*N-1
	return						; Return program flow
								; TOTAL DELAY ~12us



	

;``````````````````````````````````````````````````````````````````
	end

