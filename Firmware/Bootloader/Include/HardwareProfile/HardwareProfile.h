/********************************************************************
 FileName:      HardwareProfile.h
 Dependencies:  See INCLUDES section
 Processor:     PIC32 USB Microcontrollers
 Hardware:      PIC32MX250F128B
 Compiler:      Microchip XC32 (for PIC32)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the Company) for its PIC® Microcontroller is intended and
 supplied to you, the Companys customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN AS IS CONDITION. NO WARRANTIES,
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
#define __PIC32MX1XX_2XX__
#define DEMO_BOARD PIC32MX220F032B_MY_BOARD
#define PIC32MX220F032B_MY_BOARD
#define SYS_FREQ 48000000

/** LED ************************************************************/
#define InitLED()           LATBbits.LATB15 = 0; TRISBbits.TRISB15 = 0;\
                            LATBbits.LATB14 = 0; TRISBbits.TRISB14 = 0;

//#define BlinkLED()          mLED_1 = !mLED_1; mLED_2 = !mLED_2;
#define BlinkLED()          (mLED_1 = ((ReadCoreTimer() & 0x0800000) != 0))
#define mInitAllLEDs()      LATBbits.LATB15 = 0; TRISBbits.TRISB15 = 0;\
                            LATBbits.LATB14 = 0; TRISBbits.TRISB14 = 0;

#define mLED_1              LATBbits.LATB15
#define mLED_2              LATBbits.LATB14
#define mLED_3
#define mLED_4

#define mGetLED_1()         mLED_1
#define mGetLED_2()         mLED_2
#define mGetLED_3()         1
#define mGetLED_4()         1

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()
#define mLED_4_On()

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()
#define mLED_4_Off()

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()
#define mLED_4_Toggle()

/** I/O pin definitions ********************************************/
#define INPUT_PIN 1
#define OUTPUT_PIN 0
