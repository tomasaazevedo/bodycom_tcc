/***********************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and
 * any derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip's products. Microchip and its licensors
 * retain all ownership and intellectual property rights in the
 * accompanying software and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products. You
 * agree that you are solely responsible for testing the software and
 * determining its suitability. Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE,
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *************************************************************************/
/****************************************************************************
 * Revision History
 * Date     Rev  Author                     Comments
 * 08.07.12 0.99 BodyComm dev team          Public Release
 * 09.28.12 1.0  BodyComm dev team          Official Release
 * 12.18.12 1.1  BodyComm dev team          Updated for V XC 1.11+ compiler changes.
 * 04.02.13 1.2  BodyComm dev team          Major Framework upgrade
 ****************************************************************************/
/**
 * This file demonstrates the use of the BodyCom system in an application
 *
 * NOTE SETTINGS FOR PROJECTS:
 * Right Click project -> Properties
 * XC8 global option -> XC8 Compiler    [Project include files and BASE/MOBILE ready defines]
 *                  ->Macro Defines: BASE
 *                  ->Identifiler Length: 63
 *                  ->Include Directories:
 *                      ./
 *                      ../
 *                      ../AFE_Driver
 *                      ../BodyCom_Application
 *                      ../Manchester
 *                      ../LCD
 *                      ../Button
 *                      ../Touch
 *                      ../EUSART
 *                      ../Display
 *              -> Operation:
 *                  -> Operation Mode: PRO
 * XC8 global option -> XC8 Linker  [For Bootloader]
 *                  -> Memory Model:
 *                      ->ROM ranges: default,-1E20-1FFF
 */

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//============== MODULE INCLUDES ===============
// BodyCom Development Board Application
#include "BC_Application.h"
// BodyCom Application Paired List
#include "PL_PairedList.h"
// PC Interaction
#include "PC_Handler.h"
// BodyCom Communication
#include "MDLL_DataLinkLayer.h"
// AFE
#include "AFE.h"
// LCD communicaiton protocol
#include "I2CM.h"
// LCD Display control
#include "LCDM_St7032.h"
// Displays
#include "Display_Macros.h"
// Display Printer
#include "DBUF_DisplayBuffer.h"

//============== CONFIGURATION WORD ===============
// Code Protect
#pragma config CPD = OFF
// Brown-out Reset
#pragma config BOREN = OFF
// Internal/External Switchover
#pragma config IESO = OFF
// Oscillator Selection
#pragma config FOSC = ECH
// Fail-Safe Clock Monitor
#pragma config FCMEN = OFF
// Master Clear Pin
#pragma config MCLRE = OFF
// Watchdog Timer
#pragma config WDTE = OFF
// Code Protect
#pragma config CP = OFF
// Power-up Timer
#pragma config PWRTE = ON
// Clock Out
#pragma config CLKOUTEN = OFF
// Phase Loop Lock
#pragma config PLLEN = OFF
// Flash Memory Self-Write Protection
#pragma config WRT = OFF
// Stack Overflow/Underflow Reset
#pragma config STVREN = ON
// Brown-out Reset Voltage
#pragma config BORV = LO
// Low-Voltage Programming
#pragma config LVP = OFF

//============== FUNCTIONS ===============
/** System interrupt service routine */
void interrupt isr(void) {
    // Bodycom ISR handling timers and IOC for BC operation
    MDLL_Isr();
    // Reset AFE Here after a TX operation completes
    if ( (MDLL_Txstate == MDLL_TXSTATE_TXDONE) && (MDLL_status.PostTxAFEReset == 1) ) {
        BC_HardwareResponseReceiverReset();
    }
    // Timer overflows every 10ms
    if (TMR2IE && TMR2IF) {
        // Function variable
        static uint8_t lcdPopupTimer = 0;
        // Clear timer flag
        TMR2IF = 0;
        // Increment command timer
        if (BCOM_cmdTimer <= BCOM_cmdTimeout) {
            BCOM_cmdTimer++;
        } else {    // Command Timer ran out; trigger Timeout
            if ((BCOM_cmdState != BCOM_CMD_SUCCESS) && (BCOM_cmdState != BCOM_CMD_ERROR)) {
                BCOM_cmdState = BCOM_CMD_TIMEOUT;
            }
        }
        // If popup display is true
        if ((DisplayPopupState > NORMAL_DISPLAY) && (lcdPopupTimer == LCD_POPUP_TIME)) {
            lcdPopupTimer = 0;  // Clear timer when popup timer is finished
        } else {
            if (lcdPopupTimer < LCD_POPUP_TIME) {   // Display popup
                if (++lcdPopupTimer == LCD_POPUP_TIME) {
                    // If popup timer runs out change to normal display
                    DisplayPopupState = NORMAL_DISPLAY;
                    memset(&DisplayPopupBuffer[0], 0, sizeof(DisplayPopupBuffer));
                }
            }
        }
    }
    // Pc ISR
    PC_Isr();
    // ISR for handling reset button
    if (IOCAF3 && IOCIE) {
        IOCAF3 = 0;
        I2CM_Stop();
        SSP1CON1bits.SSPEN = 0;
        RESET();
    }
}
/** This main function demonstrates the initialization and operation of the Bodycom Library */
void main(void) {
    // Init Board for Application Demo
    BC_BoardHardwareInit();
    // Init BodyCom System
    BC_PrepareBodyComSystem();
    // Small loop to do splash startup screen immediately
    DBUF_RefreshNow();
    // Show Splash screen for 1/2 second
    __delay_ms(500);
    // Debug Settings menu check
    // BC_ConfigureDebugSettingsCheck();
    // Main loop
    while (1) {
        // Handle PC state changes
        PC_StateChangeHandler();
        // Handle board operation
        BC_DemoMenuHandler();
        // Update/refresh LCD handler
        DBUF_Handler();
        // Handle BodyCom Communication
        BC_CommandHandler();
    }
}
