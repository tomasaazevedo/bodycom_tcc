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
 * Function for Manchester Transmit
 */

//============== MODULE INCLUDES ===============
#include "TX_Manchester.h"

//***********************************************
//============== SETUP DEFINES ==================
//***********************************************
//============== TIME VALUES  ===============
//**********************************************
//============== APPLICATION DEFINES ==========
//**********************************************
//******************************
//============== BASE ==========
//******************************
#ifdef BASE
// Transmit timing/baud rate configuration
#define MANT_PERIOD_SHORT        53
#define MANT_PERIOD_LONG         65
#define MANT_PERIOD_STANDARD     59

//******************************
//============== MOBILE ==========
//******************************
#elif MOBILE
// Transmit timing/baud rate configuration
#define MANT_PERIOD_SHORT        55     // 429.69 us
#define MANT_PERIOD_LONG         67     // 523.44 us
#define MANT_PERIOD_STANDARD     61     // 476.56 us
#endif
//**********************************************
//============== COMMON INCLUDES ==========
//**********************************************
//============== TIMER DEFINES ===============
/** Manchester Transmit Timer Register */
#define MANT_TX_TIMER T4CON
/** Manchester Transmit Timer Configuration */
// Timer is off, Postscaler 1:4, Prescaler 4:1
#define MANT_TIMER_CONFIG 0b00011001
/** Manchester Transmit Timer Period register*/
#define MANT_TX_PERIOD PR4
/** Manchester Transmit Timer Interrupt bit*/
#define MANT_TX_INTERUPT TMR4IE
/** Manchester Transmit Timer Enable bit */
#define MANT_TX_TIMER_ON    TMR4ON

//============== DATA  ===============
/** Max Data value for TX */
#define DATA_MAX_SIZE   16
/** Buffer Max Size */
#define BUFFER_MAX_SIZE   32
/** Default */
/** Default value */
#define MANT_PREAMBLE_DELAY 6

//============== DEBUG HELPER  ===============
// Debug LAT must be defined for use.....
//#define MAN_RX_DEBUGPIN

//==============  ENUMERATORS ===============
/** Manchester Transmit Byte Handler State */
typedef enum {
    MANT_ENCODER_IDLE = 0,
    MANT_ENCODER_SENDING = 1,
    MANT_ENCODER_CHECKSUM = 2,
    MANT_ENCODER_FINAL_BYTE = 3,
} ManchesterSendState_t;
/** Manchester BitBanger Tx Handler State */
typedef enum{
    MANT_BB_IDLE = 0,
    MANT_BB_START_BIT_HIGH = 1,
    MANT_BB_START_BIT_LOW = 2,
    MANT_BB_DATA_OUT = 3,
    MANT_BB_STOP_BIT_HIGH = 4,
    MANT_BB_COMPLETE = 5,
}ManchesterBitBangerState_t;

//==============  GLOBAL VARIABLES ===============
uint8_t ManchesterPreambleMs = MANT_PREAMBLE_DELAY;

//==============  FILE VARIABLES ===============
/** Current State within Byte Encoder  */
static ManchesterSendState_t ManchesterTxEncoderStates = MANT_ENCODER_IDLE;
/** State variable to track transmission of every bit */
static ManchesterBitBangerState_t ManchesterSendState = MANT_BB_IDLE;
/** Length of packet to transmit */
static uint8_t ManchesterTxLength = 0;
/** Location Index within buffer */
static uint8_t ManchesterTxIndex = 0;
/** Transmit Buffer */
static uint8_t ManchesterTxBuffer[BUFFER_MAX_SIZE] = 0;
/** Data for buffer */
static uint8_t ManchesterData = 0;

//==============  FUNCTIONS ===============
uint8_t MANT_Send(uint8_t* data, uint8_t length) {
    // Ensure we have enough space to handle the request else truncate it
    if (length > BUFFER_MAX_SIZE)
        length = BUFFER_MAX_SIZE;
    // Copy passed variables into packet bufffer
    ManchesterTxLength = length;
    memcpy(ManchesterTxBuffer, data, ManchesterTxLength);
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       //
    MAN_RX_DEBUGPIN = 1;     //
#endif                       //
//=============================
    // Kick off the Transmit
    MANT_StartTransmission();
    return length;
}
void MANT_StartTransmission(void) {
    // Make sure buffer index is at head of packet
    ManchesterTxIndex = 0;
    // Make sure Manchester Tx state machines are at start position
    ManchesterTxEncoderStates = MANT_ENCODER_SENDING;
    ManchesterSendState = MANT_BB_START_BIT_HIGH;
    // Now that everything is set up we can start the timer
    MANT_TX_TIMER = MANT_TIMER_CONFIG;
    // Load period length into timer
    MANT_TX_PERIOD = MANT_PERIOD_STANDARD;
    // Turn on timer and interrupt
    MANT_TX_INTERUPT = 1;
    MANT_TX_TIMER_ON = 1;
    // Start transmit of first byte
    MANT_TxByteEncoder();
}

uint8_t MANT_TxByteEncoder(void) {
    // CheckSum Variable for checking packet integrity
    static uint8_t ManchesterCheckSum = 0;
    // Tx Byte Encoder Handler
    switch (ManchesterTxEncoderStates) {
        // Encoder is Idle
        case MANT_ENCODER_IDLE: 
            break;

        // Encoder is sending byte information
        case MANT_ENCODER_SENDING:
            // If new packet reset Checksum for new integrity check
            if (ManchesterTxIndex == 0){
                ManchesterCheckSum = 0;
            }
            // Get new data for transmit from buffer
            ManchesterData = ManchesterTxBuffer[ManchesterTxIndex++];
            // Do calculation for CheckSum to send at end of packet
            ManchesterCheckSum ^= ManchesterData;
            // Packet has completed transmit. Prepare to send CheckSum
            if (ManchesterTxIndex >= ManchesterTxLength){
                ManchesterTxEncoderStates = MANT_ENCODER_CHECKSUM; // Goto Checksum mode
            }
            break;

            // All data in packet has been transmitted. Send computed CheckSum value
        case MANT_ENCODER_CHECKSUM: 
            ManchesterData = ManchesterCheckSum;
            ManchesterTxEncoderStates = MANT_ENCODER_FINAL_BYTE;
            break;

            // Encoder is sending final byte
        case MANT_ENCODER_FINAL_BYTE:
            // Final byte has been sent; return to IDLE
            ManchesterTxEncoderStates = MANT_ENCODER_IDLE;
            break;

            // Erorr
        default:    
            ManchesterTxEncoderStates = MANT_ENCODER_IDLE;
            break;
    }
    // return current state of encoder
    return ManchesterTxEncoderStates;
}

uint8_t MANT_TxBitBanger(void) {
    // Local Function Variables
    static uint8_t BitCount = 0;
    static uint8_t PreviousBit = 0;
    // Bit Bang state machine
    switch (ManchesterSendState) {
            // Prepare to transmit
        case MANT_BB_IDLE: // Idle
            ManchesterSendState = MANT_BB_START_BIT_HIGH;
            break;

            // Send 1st half start bit (high)
        case MANT_BB_START_BIT_HIGH:
            MANT_TX_PERIOD = MANT_PERIOD_STANDARD; // Update timer period
            // Prepare for new bit
            PreviousBit = 0;
            BitCount = 0;
            // Signal Driver [High]
            MDBIT = 1;
            // Prepare for 2nd half start bit
            ManchesterSendState = MANT_BB_START_BIT_LOW;
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       //
    MAN_RX_DEBUGPIN = 1;     //
#endif                       //
//=============================
            break;

            // Send 2nd half start bit (low)
        case MANT_BB_START_BIT_LOW:
            // Signal Driver [Low]
            MDBIT = 0;
            // Prepare to send data
            ManchesterSendState = MANT_BB_DATA_OUT;
            break;

            // Data, one bit at a time, 8 symbols, 16 bits (8 bits of data encoded 2 bits/symbol)
        case MANT_BB_DATA_OUT:
            // Stay in this state until data packet sized reached
            if (BitCount >= DATA_MAX_SIZE) {
                // Signal Driver [Low]
                MDBIT = 0;
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       //
    MAN_RX_DEBUGPIN = 1;     //
#endif                       //
//=============================
                // Done, Stop bit part 1
                ManchesterSendState = MANT_BB_STOP_BIT_HIGH;
                break;
            }
            // Data is a 1
            if (ManchesterData & 0x01) {
                MDBIT = 0 ^ (BitCount & 0x01);
                // In second half of this bit
                if (BitCount & 0x01) {
                    MANT_TX_PERIOD = MANT_PERIOD_LONG;
                } else {
                    // Previous bit was also one. Short timer
                    if (PreviousBit) {
                        MANT_TX_PERIOD = MANT_PERIOD_SHORT; 
                    } else {     // Previous bit was 0
                        MANT_TX_PERIOD = MANT_PERIOD_STANDARD;
                    }
                }
            } else { // Data is a 0
                MDBIT = 1 ^ (BitCount & 0x01);
                // In second half of this bit
                if (BitCount & 0x01) {
                    MANT_TX_PERIOD = MANT_PERIOD_SHORT;
                } else {
                    if (PreviousBit) {
                        MANT_TX_PERIOD = MANT_PERIOD_STANDARD;
                    } else {
                        MANT_TX_PERIOD = MANT_PERIOD_LONG;
                    }
                }
            }
            // In first half of this bit
            if (BitCount & 0x01) {
                PreviousBit = (ManchesterData & 0x01);
                ManchesterData >>= 1;
            } else {
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       //
    MAN_RX_DEBUGPIN = 1;     //
#endif                       //
//=============================
            }
            // Increment Bit count
            BitCount++;
            break;

            // Stop bit part 2 (high)
        case MANT_BB_STOP_BIT_HIGH:
            // Signal Driver [Low]
            MDBIT = 1;
            // Reset bit count
            BitCount = 0;
            // Check to see if there are more bytes to send
            if (MANT_TxByteEncoder() == MANT_ENCODER_IDLE) {
                // Transmit is complete
                ManchesterSendState = MANT_BB_COMPLETE;
            } else {
                // Additional Bytes to send. Start new byte
                ManchesterSendState = MANT_BB_START_BIT_HIGH;
            }
            break;

            // Transmission Complete
        case MANT_BB_COMPLETE:
            break;

            // Error
        default:
            break;
    }
    // Return current state
    return ManchesterSendState;
}


