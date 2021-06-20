/*********************************************************************
 *
 *                  PIC32 Boot Loader
 *
 *********************************************************************
 * FileName:        Bootloader.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Compiler:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ï¿½Companyï¿½) for its PIC32 Microcontroller is intended
 * and supplied to you, the Companyï¿½s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ï¿½AS ISï¿½ CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * $Id:  $
 * $Name: $
 *
 **********************************************************************/
#include "Include\GenericTypeDefs.h"
#include "Include\HardwareProfile\HardwareProfile.h"
#include "Include\Bootloader.h"
#include "Include\FrameWork\Framework.h"
#include <stdlib.h>
#include <plib.h>

// <editor-fold defaultstate="collapsed" desc="Configuration Bits">
/** CONFIGURATION **************************************************/
// Configuration Bit settings
// SYSCLK = 48 MHz (12MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 48 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
#pragma config FWDTEN = OFF         /* WDT Disabled (SWDTEN Bit Controls) */
#pragma config FPLLMUL = MUL_24     /* PLL multiplier = 24x */
#pragma config FPLLIDIV = DIV_3     /* PLL divider = 1/3 */
#pragma config FPLLODIV = DIV_2     /* PLL postscaler = 1/2 */
#pragma config POSCMOD = HS         /* Primary HS oscillator */
#pragma config FNOSC = PRIPLL       /* Primary oscillator with PLL */
#pragma config FPBDIV = DIV_1       /* Peripheral bus clock = SYSCLK/1 */
#pragma config OSCIOFNC = OFF       /* CLKO output disabled */
#pragma config FCKSM = CSDCMD       /* Fail-safe clock monitor disable */
#pragma config FSOSCEN = OFF        /* Secondary Oscillator Disabled */
//#pragma config DEBUG = OFF        /* ICE debugger disabled */
#pragma config DEBUG = 3            /* ICE debugger disabled */
#pragma config JTAGEN = OFF         /* Disable JTAG port */
#pragma config ICESEL = ICS_PGx1    /* ICE/ICD Communicate on PGEC1/PGED1 */

#pragma config UPLLEN   = ON        /* USB PLL Enabled */
#pragma config UPLLIDIV = DIV_3     /* USB PLL divider = (1/3) (12MHz/(3)*24/2) */
#pragma config FUSBIDIO = OFF        /* USB USID Controlled by the USB Module */
#pragma config FVBUSONIO = OFF       /* USB VBUS ON Controlled by USB Module */
#pragma config USERID = 0xffff      /* User-defined ID */
// </editor-fold>


#define SWITCH_PRESSED 0



#ifdef __cplusplus
extern "C" {
#endif
BOOL CheckTrigger(void);
void JumpToApp(void);
BOOL ValidAppPresent(void);
#ifdef __cplusplus
}
#endif

/*SDA operations*/
#define SDA_PIN     BIT_9
#define SDA_SET     TRISBSET = SDA_PIN
#define SDA_CLR     TRISBCLR = SDA_PIN
#define IS_SDA      ((PORTB & SDA_PIN) != 0)

/*SCL operations*/
#define SCL_PIN     BIT_8
#define SCL_SET     TRISBSET = SCL_PIN
#define SCL_CLR     TRISBCLR = SCL_PIN
#define IS_SCL      ((PORTB & SCL_PIN) != 0)

/*PGC operations*/
#define PGD_PIN     BIT_0
#define PGD_SET     TRISBSET = PGD_PIN
#define PGD_CLR     TRISBCLR = PGD_PIN
#define IS_PGD      ((PORTB & PGD_PIN) != 0)

/*PGC operations*/
#define PGC_PIN     BIT_1
#define PGC_SET     TRISBSET = PGC_PIN
#define PGC_CLR     TRISBCLR = PGC_PIN
#define IS_PGC      ((PORTB & PGC_PIN) != 0)


static void delay(void)
{
 Nop();
 Nop();
 Nop();
 Nop();
 ClearWDT();
}
//---------------------------------------------------------------------------


/********************************************************************
* Function: 	main()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		None.
*
* Side Effects:	None.
*
* Overview: 	Main entry function. If there is a trigger or 
*				if there is no valid application, the device 
*				stays in firmware upgrade mode.
*
*			
* Note:		 	None.
********************************************************************/
INT main(void)
{
	UINT pbClk;

        DDPCONbits.JTAGEN = 0;
        DDPCONbits.TROEN = 0;

        ANSELA = 0;
        TRISA = 0xffffffff;
        LATA = 0;

        ANSELB = 0;
        TRISB = 0xffffffff;
        CNPUBSET = BIT_7+BIT_8+BIT_9; // RB7 (INT) pin pullup enable
        LATB = 0;

        ANSELA = 0x0000;	//All pin digital
	ANSELB = 0x0000;

        CNPUBSET = SDA_PIN;     //sda pullup set
        CNPUBSET = SCL_PIN;     //scl pullup set
        TRISBSET = SDA_PIN;     //sda pin TRIS set (input, pin high)
        TRISBSET = SCL_PIN;     //scl pin TRIS set (input)
        LATBCLR  = SDA_PIN;     //sda LAT clear (prepare to clear pin if TRIS = 0)
        LATBCLR  = SCL_PIN;     //scl LAT clear

        CNPUBSET = PGD_PIN;     //pgd pullup set
        CNPUBSET = PGC_PIN;     //pgc pullup set
        TRISBSET = PGD_PIN;     //pgd pin TRIS set (input, pin high)
        TRISBSET = PGC_PIN;     //pgc pin TRIS set (input)
        LATBCLR  = PGD_PIN;     //pgd LAT clear (prepare to clear pin if TRIS = 0)
        LATBCLR  = PGC_PIN;     //pgc LAT clear


	// Setup configuration
	pbClk = SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	
	InitLED();
	
	// Enter firmware upgrade mode if there is a trigger or if the application is not valid
	if(CheckTrigger() || !ValidAppPresent()|| mGetSWRFlag())
	{
		// Initialize the transport layer - UART/USB/Ethernet
		TRANS_LAYER_Init(pbClk);
		
		while(!FRAMEWORK_ExitFirmwareUpgradeMode()) // Be in loop till framework recieves "run application" command from PC
		{
			// Enter firmware upgrade mode.
			// Be in loop, looking for commands from PC
			TRANS_LAYER_Task(); // Run Transport layer tasks
			FRAMEWORK_FrameWorkTask(); // Run frame work related tasks (Handling Rx frame, process frame and so on)
			// Blink LED (Indicates the user that bootloader is running).
			BlinkLED();
		}
		// Close trasnport layer.
		TRANS_LAYER_Close();
	}

	
	// No trigger + valid application = run application.
	JumpToApp();
	
	return 0;
}			


/********************************************************************
* Function: 	CheckTrigger()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		TRUE: If triggered
				FALSE: No trigger
*
* Side Effects:	None.
*
* Overview: 	Checks if there is a trigger to enter 
				firmware upgrade mode.
*
*			
* Note:		 	None.
********************************************************************/
BOOL  CheckTrigger(void)
{
  //BOOL shorted;

  SDA_SET;
  SCL_SET;
  PGC_SET;
  PGD_SET;
  delay();
  SCL_CLR;
  PGC_CLR;
  delay();
  //shorted = IS_SDA == 0;
  return (!IS_SDA)||(!IS_PGD);
}	


/********************************************************************
* Function: 	JumpToApp()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		
*
* Side Effects:	No return from here.
*
* Overview: 	Jumps to application.
*
*			
* Note:		 	None.
********************************************************************/
void JumpToApp(void)
{	
	void (*fptr)(void);
	fptr = (void (*)(void))USER_APP_RESET_ADDRESS;
	fptr();
}	


/********************************************************************
* Function: 	ValidAppPresent()
*
* Precondition: 
*
* Input: 		None.
*
* Output:		TRUE: If application is valid.
*
* Side Effects:	None.
*
* Overview: 	Logic: Check application vector has 
				some value other than "0xFFFFFFFF"
*
*			
* Note:		 	None.
********************************************************************/
BOOL ValidAppPresent(void)
{
	DWORD *AppPtr;
	AppPtr = (DWORD *)USER_APP_RESET_ADDRESS;
	if(*AppPtr == 0xFFFFFFFF)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}			


/*********************End of File************************************/


