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
 * Functions for handling communication with PC
 */

#ifndef __PC_HANDLER_H
#define __PC_HANDLER_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <string.h>

//============== MODULE INCLUDES ===============
// BodyCom Data Link Layer
#include "MDLL_DataLinkLayer.h"

//============== EXTERNALS ===============
/** Copy of RX packet for PC to display */  // TODO: CLEANUP
extern MDLL_PacketData_t  pcRxPacket;

//============== PROTOTYPES ===============
/** Handles PC interaction with firmware ISR */
void PC_Isr(void);
/** Init board for communication with PC through MCP2200 */
void PC_ComInit(void);
/** Change State of PC based on board state */
void PC_StateChangeHandler(void);
/** Handles BodyCom operations requested by PC */
void PC_BodyComRequestHandler(void);
/** Handle the GUI commands */
void PC_CommandHandler(void);
//============== COMMAND PROTOTYPES ===============
/** Send version of Firmware to PC for display */
void PC_SendVersion(void);
/** Board state sent to PC
 * param state - current board state */
void PC_BoardState(uint8_t state);
/**  Update Mobile Info on PC */
void PC_SendMobileData(void);
/** Send Touch info for PC display */
void PC_SendTouchPacket(void);
/** Send packet to PC */
void PC_SendPacket(void);
/** Status of Demo to PC */
void PC_SendStatus(void);
/** Send PC AFE RSSI value */
void PC_SendRssiPacket(void);
/** Read line of LCD for PC usage
 *  param line - line on LCD to read */
void PC_ReadLcd(uint8_t line);
/** PC updates application paired list
 *  param device - Device ID on list */
void PC_ReadPairedList(uint8_t device);

#endif //__PC_HANDLER_H
