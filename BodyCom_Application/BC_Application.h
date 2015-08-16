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
/*
 * Function for development board BodyCom application
 */

#ifndef __BC_APPLICATION_H
#define __BC_APPLICATION_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//============== MODULE INCLUDES ===============
#include "MDLL_DataLinkLayer.h"

//============== Application Defines ===============
//============== Version ===============
/** AFE This is a user parameter that determines the major software revion */
#define SW_VERSION1         0x01
/** AFE This is a user parameter that determines the minor software revion */
#define SW_VERSION2         0x02
//============== FOSC ===============
/** Application FOSC */
#define _XTAL_FREQ        8192000UL

//============== Data ===============
/** Timeout */
#define  PING_TIMEOUT           24
/** Time in 10ms increments for LCD popup messages */
#define LCD_POPUP_TIME    75  

//============== ENUMBERATORS ===============
/** States of of Demostration Applicatoion */
typedef enum {
    EVAL_TOUCH = 0,
    // EVAL_PROX = 1,
    DEBUG_MENU = 1,
    RX_RSSI = 2,
    RX_PCKT = 3,
    TX_CW = 4,
    TX_LPBK = 5,
    TOUCH = 6,
    PAIR_MENU = 7,
    VIEW_ID = 8,
    ADD_MODE_GUI = 9,
    MODE_NONE = 10, // Add before this item, do not rearrange !
}DevelopmentBoardState_t;
/** Current BodyCom Protocol State */
extern DevelopmentBoardState_t BodyComState;

/** Current BodyCom Command State */
typedef enum {
    BCOM_CMD_IDLE = 0,
    BCOM_CMD_PING_SENT = 1,
    BCOM_CMD_ECHOREQ_SENT = 2,
    BCOM_CMD_PAIRPING_SENT = 3,
    BCOM_CMD_RXPACKET = 4,
    // ******* we use >= BCOM_CMD_SUCCESS to check for idle
    BCOM_CMD_SUCCESS = 5,
    BCOM_CMD_TIMEOUT = 6,
    BCOM_CMD_ERROR = 7,
} BCOM_cmdHandlerStates_t;
/** Current BodyCom Command State */
extern BCOM_cmdHandlerStates_t BCOM_cmdState;

//============== ENUMERATORS ===============
/** Protocol Handler State */
typedef enum {
    PROT_DETECT = 0,
    PROT_PREPARE_CHALLENGE = 1,
    PROT_TRANSMIT_RECEIVE = 2,
    PROT_PAIRED_INTERROGATE = 3,
    PROT_VALID_RESPONSE = 4,
    PROT_CHALLENGE_CONTINUE = 5,
    PROT_NO_RESPONSE = 6,
    PROT_CHALLENGE_COMPLETE = 7,
} BodyComProtocol_t;

//============== EXTERNALS ===============
/** Command Timeout */
extern uint16_t BCOM_cmdTimeout;
/** Command Timer value */
extern uint16_t BCOM_cmdTimer;

//============== PROTOTYPE ===============

//============== HARDWARE ===============
/** Initializes the Board for Demo Applications */
void BC_BoardHardwareInit(void);
/** Configure the Hardware Pins and PIC device for use with BodyCom Technology */
void BC_BodyComHardwareInit(void);
/** Prepare the Hardware to take RSSI reading from AFE */
void BC_HardwarePrepareRssi(void);

//============== CONFIG SYSTEM ===============
/** Check to see if user selected to enter debug settings options */
void BC_ConfigureDebugSettingsCheck(void);
/** Setup AFE for LFDATA */
void BC_ConfigureAfe(void);
/** Initialize the BodyCom System */
void BC_PrepareBodyComSystem(void);
/** Issue a Reset command to the AFE device */
void BC_HardwareResponseReceiverReset(void);

//============== MENUS ===============
/** Handles Menu Navigation
 *  param Action -  Menu Navigation action  */
void BC_DemoMenuNavigator(uint8_t Action);
/** Menu System for Demo  */
void BC_DemoMenuHandler(void);
/** Configure AFE within Debug Settings  */
void BC_DebugSettingsConfigure(void);
/** Display paired device at index
    param index - position to read */
void BC_DisplayPairedList(uint8_t index);

//============== DISPLAYS ===============
/** Handles LCD displays related to BodyCom Protocol state
    param handle state - displays based on command */
void BC_CommunicationDisplay(BodyComProtocol_t handle_state);
/** Print Mobile ID to buffer then display on LCD
 *  param destination -  Destination 0 = main and 1 = popup
 *  param id -  Pointer to data  */
void BC_DisplayId(uint8_t destination, uint8_t* id);
/** Display CVD state indicators  */
void BC_DisplayCvdIndicator(void);
/** Display Received information  */
void BC_DisplayRxPacket(void);

//============== HANDLERS ===============
/** BodyCom Command Handler  */
void BC_CommandHandler(void);
/** Handle list lookup and ping operation */
uint8_t BC_PingPairedListHandler(void);

//============== COMMANDS ===============
/** Ping for devices on paired list
    return - valid ping or finished*/
uint8_t BC_PingPairedList(void);
/** Send a BodyCom command with data
    param cmd - Packet Command
    param Address - Pointer to address
    param data - pointer to data
    param length - length of data
    param timeout - timeout length */
uint8_t BC_SendDataCommand(BodyCom_Commands_t cmd, uint8_t* Address, uint8_t *data, uint8_t  length, uint8_t timeout);
/** Send a BodyCom command without data
    param cmd - Packet Command
    param Address - Pointer to address
    param data - pointer to data
    param length - length of data
    param timeout - timeout length */
uint8_t BC_SendCommand(BodyCom_Commands_t cmd, uint8_t * Address, uint8_t timeout);

#endif // __BC_APPLICATION_H