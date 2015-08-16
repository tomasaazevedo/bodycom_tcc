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

//============== MODULE INCLUDES ===============
#include "PL_PairedList.h"

//============== PRE-PAIRED IDs ===============
/** First bit is the default AFE settings, TAG's start after that */
__EEPROM_DATA(0x00, 0x01, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
__EEPROM_DATA(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

//============== PRE-PAIRED IDs ===============
/** Here are preloaded Valid Mobile IDs (2 of 8 possible) */
__EEPROM_DATA(0x01, 0x02, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x01, 0x02, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00);

//============== DEFINES ===============
#define ID_BYTE 4

//============== GLOBAL VARIABLES ===============
uint8_t DeviceInfo[ID_BYTE] = 0;
uint8_t PairedCount = 0;

//============== STATIC VARIABLES ===============
uint16_t HandlerRssiValue = 0;  // TODO: make local?

//============== FUNCTIONS ===============
void PL_UpdateList(void) {
    // Temp variable
    uint8_t count = 0;
    // Clear for update
    PairedCount = 0x00;
    // Read up to paired list count
    for (count = 0; count < TAG_MAX_ENTRY; count++) {
        PL_ReadPaired(count, DeviceInfo);
        // Stop reading if 0xFF or 0x00 found
        if (((DeviceInfo[0] != 0) || (DeviceInfo[1] != 0) || (DeviceInfo[2] != 0) || (DeviceInfo[3] != 0)) &&
                ((DeviceInfo[0] != 0xFF) || (DeviceInfo[1] != 0xFF) || (DeviceInfo[2] != 0xFF) || (DeviceInfo[3] != 0xFF))) {
            PairedCount++;
        }
    }
}

uint8_t PL_PairedCount(void) {
    // Too many devices
    if (PairedCount >= TAG_MAX_ENTRY)
        return 0xFF;
        // Return current paired list count
    else
        return PairedCount;
}

void PL_DeletePairedDevice(uint8_t entry) {
    // Tempory variable
    uint8_t position = 0,
            data = 0,
            read = 0;
    // Too many devices
    if (entry >= TAG_MAX_ENTRY) {
        return;
    }
    // Find postion
    position = entry * TAG_ENTRY_SIZE + TAG_ENTRY_START;
    // Read List
    while (entry < (TAG_MAX_ENTRY - 1)) {
        for (data = 0; data < TAG_ENTRY_SIZE; data++) {
            read = eeprom_read(position + TAG_ENTRY_SIZE);
            eeprom_write(position++, read);
        }
        entry++;
    }
    // Figure out write position
    position = entry * TAG_ENTRY_SIZE + TAG_ENTRY_START;
    // Update list
    for (data = 0; data < TAG_ENTRY_SIZE; data++) {
        eeprom_write(position++, 0xFF);
    }
    // Decrement if not 0
    if (PairedCount > 1) {
        PairedCount--;
    }
}
uint8_t PL_AddPairedDevice(uint8_t* data) {
    // Tempory variable
    uint8_t position = 0,
            count = 0;
    // Function variable
    static uint8_t PairingInProgress = 0;
    // Pairing is already in progress;
    if (PairingInProgress)
        return 0;
    // Device list is full; Cannot add anymore ID
    if (PairedCount >= TAG_MAX_ENTRY) {
        return 0xFF;
    }
    // Pairing Process is starting
    PairingInProgress = 1;
    // Check to see if device ID is already paired; If not yet paired (0) add to list
    if (PL_CheckAlreadyPaired(data) == 0) {
        // Compute EEPROM location to write
        position = PairedCount * TAG_ENTRY_SIZE + TAG_ENTRY_START;
        // Add new ID to EEPROM
        for (count = 0; count < TAG_ENTRY_SIZE; count++) {
            eeprom_write(position++, *data++);
        }
        // Increment Device Count (number of ID's in List)
        PairedCount++;
    }
    // Successful Add
    PairingInProgress = 0;
    return 1;
}
uint8_t PL_ReadPaired(uint8_t entry, uint8_t *destination) {
    // Tempory variable
    uint8_t position = 0;
    // Failed
    if (entry >= TAG_MAX_ENTRY)
        return 0;
    // Look up device at index position
    position = entry * TAG_ENTRY_SIZE + TAG_ENTRY_START;
    destination[0] = eeprom_read(position++);
    destination[1] = eeprom_read(position++);
    destination[2] = eeprom_read(position++);
    destination[3] = eeprom_read(position++);
    // Return successful
    return 1;
}
uint8_t PL_CheckAlreadyPaired(uint8_t* address) {
    // Tempory variable
    uint8_t position = 0;
    // Check EEPROM for ID repeat
    for (uint8_t i = 0; i < PairedCount; i++) {
        // Compute position to read
        position = TAG_ENTRY_START + (i * TAG_ENTRY_SIZE);
        // Check ID
        if ( (address[0] == eeprom_read(position++) ) &&
                (address[1] == eeprom_read(position++) ) &&
                (address[2] == eeprom_read(position++) ) &&
                (address[3] == eeprom_read(position) ) ) {
            // ID is already paired
            return 1;
        }
    }
    // ID is new
    return 0;
}

