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
 * Function for AFE operation
 */

//============== MODULE INCLUDES ===============
#include "AFE.h"

//**********************************************
//============== APPLICATION DEFINES ==========
//**********************************************
#ifdef BASE
//******************************
//============== BASE ==========
//******************************
// *****************************************************************************
//          AFE Config Defines                                                **
// *****************************************************************************
// LFDATA Setup - References (examples)
//#define AFE_LFDATA_DEFAULT          {0x86,0x00,0x00,0x00,0x00,0x30,0x49};     // Mod depth  8%
//#define AFE_LFDATA_DEFAULT          {0x86,0x00,0x00,0x00,0x00,0x20,0x59};     // Mod depth  14%
//#define AFE_LFDATA_DEFAULT          {0x86,0x00,0x00,0x00,0x00,0x00,0x79};     // Mod depth 33%
//#define AFE_LFDATA_DEFAULT          {0x86,0x00,0x00,0x00,0x00,0x10,0x69};     // Mod depth 60%

#define AFE_LFDATA_DEFAULT          {0x86,0x00,0x00,0x00,0x00,0x20,0x59}   // 33
#define AFE_RSSI_CONFIG_DEFAULT     {0x06,0xC0,0x00,0x00,0x00,0x00,0x39};

//============== I/O  ===============
// Pin Defines
#define AFE_DATA_LAT    LATB5
#define AFE_DATA_TRIS   TRISB5
#define AFE_DATA_PORT_READ   RB5

#define AFE_CLOCK_LAT   LATB7
#define AFE_CLOCK_TRIS  TRISB7

#define AFE_CS_LAT      LATC0
#define AFE_CS_TRIS     TRISC0

#elif MOBILE
//******************************
//============== MOBILE ==========
//******************************
// *****************************************************************************
//          AFE Config Defines                                                **
// *****************************************************************************
// LFDATA Setup - References (examples)
//#define AFE_LFDATA_DEFAULT          {0x66,0x00,0x00,0x00,0x00,0x30,0xA9};     // Mod depth  8%
//#define AFE_LFDATA_DEFAULT          {0x66,0x00,0x00,0x00,0x00,0x20,0xB9};     // Mod depth  14%
//#define AFE_LFDATA_DEFAULT          {0x66,0x00,0x00,0x00,0x00,0x00,0x99};     // Mod depth 33%
//#define AFE_LFDATA_DEFAULT          {0x66,0x00,0x00,0x00,0x00,0x10,0x89};     // Mod depth 60%

// Default Setup - 14%, Wakeup Enabled
#define AFE_LFDATA_DEFAULT          {0x66,0x00,0x00,0x00,0x00,0x20,0xB9}   // Setup AFE for LFDATA
#define AFE_RSSI_CONFIG_DEFAULT     {0x06,0xC0,0x00,0x00,0x00,0x00,0x39};  // Setup AFE for RSSI

//============== I/O  ===============
// Pin Defines
#define AFE_DATA_LAT        LATB4
#define AFE_DATA_TRIS       TRISB4
#define AFE_DATA_PORT_READ  RB4
#define AFE_DATA_ANSEL      ANSB4

#define AFE_CLOCK_LAT   LATB0
#define AFE_CLOCK_TRIS  TRISB0

#define AFE_CS_LAT      LATB5
#define AFE_CS_TRIS     TRISB5

#endif

//============== FILE VARIABLES ===============
/** Register for AFE Status */
static uint16_t AfeStatus = 0;
/** Default Setup - 14%, Wakeup Enabled; LFDATA AFE configuration */
static uint8_t AfeLfData[7] = AFE_LFDATA_DEFAULT;
/** RSSI Setup */
static const uint8_t AfeRSSIConfig[] = AFE_RSSI_CONFIG_DEFAULT; 

//============== Functions ===============
void AFE_WriteByte(uint8_t byte) {
    // Temp varaiables
    uint8_t Position = 0;
    uint8_t ByteValue = byte;
    // Write byte to AFE
    for (Position = 0; Position < 8; Position++) {
        AFE_DATA_LAT = ((ByteValue >> (uint8_t) (7 - Position)) & 0x01);
        asm("nop");
        AFE_CLOCK_LAT = 1;
        asm("nop");
        AFE_CLOCK_LAT = 0;
    }
}
void AFE_WriteCommand(uint8_t command) {
    // Select AFE chip for SPI communication
    AFE_CLOCK_TRIS = 0;
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 0;
    AFE_DATA_TRIS = 0;
    // Send Command Byte
    AFE_WriteByte((uint8_t) ((command & 0x07) << 5));
    //Write dummy byte required by specs
    AFE_WriteByte(0x00);
    // Deselect AFE chip
    AFE_CLOCK_TRIS = 1;
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 1;
    AFE_DATA_TRIS = 1;
}
uint8_t AFE_ReadRegister(uint8_t address) {
    // Temp variables
    uint8_t position = 0,
            data = 0,
            temp = 0;
    // Select AFE chip for SPI communication
    AFE_CLOCK_TRIS = 0;
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 0;
    AFE_DATA_TRIS = 0;
    // Send Read command to AFE
    AFE_WriteByte((uint8_t) (((AFE_READ_COMMAND << 5) | (address << 1))));
    AFE_WriteByte(0x00);
    // Deselect AFE chip
    //Start second read cycle
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 1;
    AFE_DATA_TRIS = 1;
    AFE_CS_LAT = 0;
    //first 7 bits - Dummy Data
    for (position = 0; position < 7; position++) {
        asm("nop");
        AFE_CLOCK_LAT = 1;
        asm("nop");
        AFE_CLOCK_LAT = 0;
    }
    // Read real data
    for (position = 0; position < 8; position++) {
        asm("nop");
        AFE_CLOCK_LAT = 1;
        temp = (uint8_t) AFE_DATA_PORT_READ;
        data = (uint8_t) ((data << 1) | temp);
        AFE_CLOCK_LAT = 0;
    }
    // Stop
    asm("nop");
    AFE_CLOCK_LAT = 1;
    asm("nop");
    AFE_CLOCK_LAT = 0;
    asm("nop");
    AFE_CS_LAT = 1;
    AFE_CLOCK_TRIS = 1;
    // Return byte
    return data;
}
uint16_t AFE_ReadStatus(void) {
    // Temp varaibles
    uint8_t position = 0,
            temp = 0;
    // Clear for new read
    AfeStatus = 0;
    // Select AFE chip for SPI communication
    AFE_CLOCK_TRIS = 0;
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 0;
    AFE_DATA_TRIS = 0;
    // Send Read command
    AFE_WriteByte((AFE_READ_COMMAND << 5) | (7 << 1));
    AFE_WriteByte(0x00);
    // Deselect AFE chip
    //Start second read cycle
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 1;
    AFE_DATA_TRIS = 1;
    AFE_CS_LAT = 0;
    //first 7 bits - Dummy Data
    for (position = 0; position < 7; position++) {
        asm("nop");
        AFE_CLOCK_LAT = 1;
        asm("nop");
        AFE_CLOCK_LAT = 0;
    }
    // Read real data
    for (position = 0; position < 9; position++) {
        asm("nop");
        AFE_CLOCK_LAT = 1;
        temp = (uint8_t) AFE_DATA_PORT_READ;
        AfeStatus = (AfeStatus << 1) | temp;
        AFE_CLOCK_LAT = 0;
    }
    // Deselect
    asm("nop");
    AFE_CS_LAT = 1;
    AFE_CLOCK_TRIS = 1;
    // Return Status
    return AfeStatus;
}
uint8_t AFE_Out(AfeOutput_type mode) {
    // Temp Variables
    uint8_t position = 0;
    uint8_t* data = 0;
    // Select chip
    AFE_CS_TRIS = 0;
    // Write address and data
    AFE_WriteCommand(AFE_SOFT_RESET);
    // Setup for LFDATA or RSSI
    if (mode == 0) {
        data = AfeLfData;
    } else {
        data = (uint8_t*) (&AfeRSSIConfig[0]);
    }
    // Update configuration
    for (position = 0; position < 7; position++) {
        AFE_WriteRegister(position, data[position]);
    }
    // Write address and data
    // Confirm Write to AFE Config Registers worked correctly; Hold and Indicate otherwise
    for (uint8_t i = 0; i < 7; i++) {
        if (data[i] != AFE_ReadRegister(i)) {
            return 0;   // Bad Write
        }
    }
    // Check Parity Bit in Status
    AfeStatus = AFE_ReadStatus();
    if (AfeStatus & 0x01) {
        return 0;   // Bad Write
    }
    // Good write
    return 1;
}
void AFE_WriteRegister(uint8_t address, uint8_t data) {
    // Temp variables
    uint8_t position = 0,
            parity = 0,
            temp = 0,
            afeData1 = 0,
            afeData0 = 0;
    // prepare parity bit
    parity = 1;
    // Calculate the parity bit
    for (position = 0; position < 8; position++) {
        temp = (uint8_t) (data >> position);
        parity ^= temp;
    }
    // prepare data
    afeData1 = (uint8_t) ((AFE_WRITE_COMMAND << 5) | (address << 1) | temp);
    afeData0 = (uint8_t) (data << 1 | (parity & 0x01));
    // Select AFE chip for SPI communication
    AFE_CLOCK_TRIS = 0;
    AFE_CLOCK_LAT = 0;
    AFE_CS_LAT = 0;
    AFE_DATA_TRIS = 0;
    // Write Address and Data
    AFE_WriteByte(afeData1);
    AFE_WriteByte(afeData0);
    // Deselect AFE chip
    AFE_CLOCK_LAT = 0;
    AFE_CLOCK_TRIS = 0;
    AFE_CS_LAT = 1;
    AFE_DATA_TRIS = 1;
    AFE_CLOCK_TRIS = 1;
}
uint8_t AFE_LfData_Setup(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6) {
    // Assign setup
    AfeLfData[0] = byte0;
    AfeLfData[1] = byte1;
    AfeLfData[2] = byte2;
    AfeLfData[3] = byte3;
    AfeLfData[4] = byte4;
    AfeLfData[5] = byte5;
    AfeLfData[6] = byte6;
    // Update LFDATA for custom setup
    return AFE_Out(LFDATA);
}


