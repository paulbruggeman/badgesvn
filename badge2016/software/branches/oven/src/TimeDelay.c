/******************************************************************************

File Name:       TimeDelay.c
Dependencies:    None
Processor:       PIC18/PIC24/dsPIC30/dsPIC33/PIC32
Compiler:        C30 v3.12
Company:         Microchip Technology, Inc.

Copyright (C) 2010 Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Author          Date    Comments
--------------------------------------------------------------------------------
AKN 2009.10.14  FILE CREATED
AKN 2009.10.15  CHANGED C18 DELAY ROUTINE TO DECREMENT ENTIRE NUMBER OF CYCLES
AKN 2009.10.19  CHANGED C30 DELAY ROUTINE TO MATCH C18 IMPLEMENTATION
AKN 2009.10.26  ADDED C32 DELAY ROUTINE TO MATCH C18 IMPLEMENTATION
AKN 2009.10.27  CONSOLIDATED C30 AND C32 IMPLEMENTATIONS, ADDED PADDING TO
                MAKE C30 DELAYS MORE ACCURATE
PAT 2010.01.26  CONVERTED LOCALS TO VOLATILE
PAT 2010.03.07  ADDED include "Compiler.h"
MWM 2011.03     Added Delay1us
*******************************************************************************/
#include <plib.h>
#include "TimeDelay.h"
#include "HardwareProfile.h"

/****************************************************************************
  Function:
    void Delay10us( unsigned long int MicroSecondCounter )

  Description:
    This routine performs a software delay in intervals of 10 microseconds.

  Precondition:
    None

  Parameters:
    UINT32 MicroSecondCounter - number of microsecond delays
    to perform at once.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void Delay1us( unsigned long int MicroSecondCounter )
{
    volatile signed long int cyclesRequiredForEntireDelay;

    if(GetInstructionClock() <= 500000) //for all FCY speeds under 500KHz (FOSC <= 1MHz)
    {
        //10 cycles burned through this path (includes return to caller).
        //For FOSC == 1MHZ, it takes 5us.
        //For FOSC == 4MHZ, it takes 0.5us
        //For FOSC == 8MHZ, it takes 0.25us.
        //For FOSC == 10MHZ, it takes 0.2us.
    }
    else
    {
        //7 cycles burned to this point.

        //We want to pre-calculate number of cycles required to delay 10us * tenMicroSecondCounter using a 1 cycle granule.
        cyclesRequiredForEntireDelay = (INT32)(GetInstructionClock()/1000000)*MicroSecondCounter;

        //We subtract all the cycles used up until we reach the while loop below, where each loop cycle count is subtracted.
        //Also we subtract the 5 cycle function return.
        cyclesRequiredForEntireDelay -= 24; //(19 + 5)

        if(cyclesRequiredForEntireDelay <= 0)
        {
            // If we have exceeded the cycle count already, bail!
        }
        else
        {
            while(cyclesRequiredForEntireDelay>0) //19 cycles used to this point.
            {
                cyclesRequiredForEntireDelay -= 8; //Subtract cycles burned while doing each delay stage, 8 in this case.
            }
        }
     }
}

/****************************************************************************
  Function:
    void Delay10us( unsigned long int tenMicroSecondCounter )

  Description:
    This routine performs a software delay in intervals of 10 microseconds.

  Precondition:
    None

  Parameters:
    UINT32 tenMicroSecondCounter - number of ten microsecond delays
    to perform at once.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void Delay10us( unsigned long int tenMicroSecondCounter )
{
    volatile signed long int cyclesRequiredForEntireDelay;

    if(GetInstructionClock() <= 500000) //for all FCY speeds under 500KHz (FOSC <= 1MHz)
    {
        //10 cycles burned through this path (includes return to caller).
        //For FOSC == 1MHZ, it takes 5us.
        //For FOSC == 4MHZ, it takes 0.5us
        //For FOSC == 8MHZ, it takes 0.25us.
        //For FOSC == 10MHZ, it takes 0.2us.
    }
    else
    {
        //7 cycles burned to this point.

        //We want to pre-calculate number of cycles required to delay 10us * tenMicroSecondCounter using a 1 cycle granule.
        cyclesRequiredForEntireDelay = (INT32)(GetInstructionClock()/100000)*tenMicroSecondCounter;

        //We subtract all the cycles used up until we reach the while loop below, where each loop cycle count is subtracted.
        //Also we subtract the 5 cycle function return.
        cyclesRequiredForEntireDelay -= 24; //(19 + 5)

        if(cyclesRequiredForEntireDelay <= 0)
        {
            // If we have exceeded the cycle count already, bail!
        }
        else
        {
            while(cyclesRequiredForEntireDelay>0) //19 cycles used to this point.
            {
                cyclesRequiredForEntireDelay -= 8; //Subtract cycles burned while doing each delay stage, 8 in this case.
            }
        }
    }
}

/****************************************************************************
  Function:
    void DelayMs( unsigned short int ms )

  Description:
    This routine performs a software delay in intervals of 1 millisecond.

  Precondition:
    None

  Parameters:
    UINT16 ms - number of one millisecond delays to perform at once.

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void DelayMs( unsigned short int ms )
{
    volatile unsigned char i;

    while (ms--)
    {
        i = 4;
        while (i--)
        {
            Delay10us( 25 );
        }
    }
}

