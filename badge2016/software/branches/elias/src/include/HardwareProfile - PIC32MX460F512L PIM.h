/********************************************************************
 FileName:      HardwareProfile - PIC32MX460F512L PIM.h
 Dependencies:  See INCLUDES section
 Processor:     PIC32 USB Microcontrollers
 Hardware:      PIC32MX460F512L PIM
 Compiler:      Microchip C32 (for PIC32)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the Company) for its PIC Microcontroller is intended and
 supplied to you, the Companys customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
  2.3   09/15/2008   Broke out each hardware platform into its own
                     "HardwareProfile - xxx.h" file
********************************************************************/

#ifndef HARDWARE_PROFILE_PIC32MX460F512L_PIM_H
#define HARDWARE_PROFILE_PIC32MX460F512L_PIM_H

    /*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    //#define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISAbits.TRISA2    // Input
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
    #define USB_BUS_SENSE       1 

    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/
    /******** Application specific definitions *************************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/

    /** Board definition ***********************************************/
    //These defintions will tell the main() function which board is
    //  currently selected.  This will allow the application to add
    //  the correct configuration bits as wells use the correct
    //  initialization functions for the board.  These defitions are only
    //  required in the stack provided demos.  They are not required in
    //  final application design.
    #define DEMO_BOARD PIC32MX460F512L_PIM
    #define EXPLORER_16

    //#define RUN_AT_48MHZ
    //#define RUN_AT_24MHZ
// PEB    #define RUN_AT_60MHZ
    #define RUN_AT_40MHZ
    
    // Various clock values
    #if defined(RUN_AT_48MHZ)
        #define GetSystemClock()            48000000UL
        #define GetPeripheralClock()        48000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_24MHZ)
        #define GetSystemClock()            24000000UL
        #define GetPeripheralClock()        24000000UL
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_60MHZ)    
        #define GetSystemClock()            60000000UL
        #define GetPeripheralClock()        60000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2) ???
    #elif defined(RUN_AT_40MHZ)    
        #define GetSystemClock()            40000000UL
        #define GetPeripheralClock()        40000000UL  // Will be divided down
        #define GetInstructionClock()       (GetSystemClock() / 2)
    #else
        #error Choose a speed
    #endif

    /** UART ***********************************************************/
//    #define BAUDRATE2       57600UL
//    #define BRG_DIV2        4 
//    #define BRGH2           1 

    /** I/O pin definitions ********************************************/
    #define INPUT_PIN 1
    #define OUTPUT_PIN 0

    //These definitions are only relevant if the respective functions are enabled
    //in the usb_config.h file.
    //Make sure these definitions match the GPIO pins being used for your hardware
    //setup.
//    #define UART_DTS PORTDbits.RD6
//    #define UART_DTR LATAbits.LATA1
//    #define UART_RTS LATFbits.LATF13
//    #define UART_CTS PORTFbits.RF12
    
//    #define mInitRTSPin() {TRISFbits.TRISF13 = 0;}   //Configure RTS as a digital output.  
//    #define mInitCTSPin() {TRISFbits.TRISF12 = 1;}   //Configure CTS as a digital input.  (Make sure pin is digital if ANxx functions is present on the pin)
//    #define mInitDTSPin() {TRISDbits.TRISD6 = 1;}   //Configure DTS as a digital input.  (Make sure pin is digital if ANxx functions is present on the pin)
//    #define mInitDTRPin() {TRISAbits.TRISA1 = 0;}   //Configure DTR as a digital output.
#endif  //HARDWARE_PROFILE_PIC32MX460F512L_PIM_H
