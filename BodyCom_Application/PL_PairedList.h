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
 * Function for mantaining paired list for BodyCom Application
 */

#ifndef __PL_PAIREDLIST_H
#define __PL_PAIREDLIST_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>

//============== EXTERNAL ===============
/** Current Device ID array used for communicaiton */
extern uint8_t DeviceInfo[];
/** Number of paired devices */
extern uint8_t PairedCount;

extern uint16_t HandlerRssiValue;   //TODO

//============== DEFINES ===============
/** Number of Mobile Unit entry in the pairing list */
#define TAG_MAX_ENTRY   8
/** Device ID size; 4 bytes + 4 [0xFF] spaces in EEPROM */
#define TAG_ENTRY_SIZE  8
/** Start position in EEPROM of paired ID's */
#define TAG_ENTRY_START 16

//============== PROTOTYPE ===============
/** This function updates the paired list by reading EEPROM */
void PL_UpdateList(void);
/** Check to see if there is room for new ID
 *  return PairedCount - Number of paired devices */
uint8_t PL_PairedCount(void);
/** Deletes Paired ID
 *  param entry - index position of ID to delete */
void PL_DeletePairedDevice(uint8_t entry);
/** Add ID to EEPROM
 *  param data - ID to add to list
 *  return status - Max limit reached or device added */
uint8_t PL_AddPairedDevice(uint8_t* data);
/** Read ID to EEPROM
 *  param entry - Reads item from index position
 *  param destination - pointer for read ID */
uint8_t PL_ReadPaired(uint8_t entry, uint8_t *destination);
/** Read ID to EEPROM
 *  param address - look for ID in list; ADD if not in list */
uint8_t PL_CheckAlreadyPaired(uint8_t* address);

#endif  // __PL_PAIREDLIST_H