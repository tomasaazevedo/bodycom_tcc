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
 * Function for Manchester Data Link Layer
 */

//============== MODULE INCLUDES ===============
#include "MDLL_DataLinkLayer.h"

//============== COMMUNICATION INCLUDES ===============
// BodyCom Transmit
#include "TX_Manchester.h"
// BodyCom Receive
#include "RX_Manchester.h"

//**********************************************
//============== APPLICATION DEFINES ==========
//**********************************************
//******************************
//============== BASE ==========
//******************************
#ifdef BASE
// Device Address
#define MDLL_DEVICE_ADDRESS         {0, 0, 0, 0}
// Mask to be used
#define MDLL_DEVICE_ADDRESS_MASK    {0xFF, 0xFF, 0xFF, 0xFF}

#define  MDLL_POSTTX_RESET      1   // 1 or 0 only
#define MANT_PERIOD_1MS          128   // 1.00ms  ????
#define MANT_PERIOD_100US        13    // 101.56us ????
#define  MDLL_PREDELAY              20  // In ms
#define  MDLL_GAP_DELAY             4   // Gap in hundreds of u seconds
#define  MDLL_FILTERHIGH_DELAY      1
#define  MDLL_FILTERLOW_DELAY       2
#define  MDLL_PREAMBLE_REPEATS      0   // additional preambles
#define  MDLL_POSTAMBLE_DELAY       2
#define  MDLL_REPEAT_DELAY          34  // delay between attempts, long enough for inactivity timeout
#define  MDLL_TRANSMISSION_REPEATS  0   // Additional transmissions

// I/O
#define BC_MODOUT_N_TRIS    TRISA2
#define BC_ENABLERX_OUT     LATC7
#define BC_MODOUT_OUT       LATC3
#define BC_MODOUT_N_OUT     LATA2
#define BC_SERIAL_TRIS      TRISC6
#define BC_MODOUT_TRIS      TRISC3

// Manchester Driver
#define MANT_DRIVER_STATE   MDBIT

//******************************
//============== MOBILE ==========
//******************************

#elif MOBILE
// Device Address
#define MDLL_DEVICE_ADDRESS         {1, 2, 3, 5}
// Mask to be used
#define MDLL_DEVICE_ADDRESS_MASK    {0, 0, 0, 0}

#define  MDLL_POSTTX_RESET      1       // 1 or 0 only
#define MANT_PERIOD_1MS             128   // 1.00ms
#define MANT_PERIOD_100US           13    // 101.56us
#define  MDLL_PREDELAY              20  // In ms
#define  MDLL_GAP_DELAY             4   // Gap in hundreds of u seconds
#define  MDLL_FILTERHIGH_DELAY      2
#define  MDLL_FILTERLOW_DELAY       1
#define  MDLL_PREAMBLE_REPEATS      1   // additional preambles
#define  MDLL_POSTAMBLE_DELAY       2
#define  MDLL_REPEAT_DELAY          17  // delay between attempts, long enough for inactivity timeout
#define  MDLL_TRANSMISSION_REPEATS  1   // Additional transmissions

// Manchester Driver
#define MANT_DRIVER_STATE       MDBIT

// I/O
// Driver TX/RX Controller
#define MANT_RX_EN_TRIS  TRISB2
#define MANT_RX_nEN_LAT  LATB2
#define MANT_RX_EN_ANSEL ANSB2

#endif

//**********************************************
//============== COMMON INCLUDES ==========
//**********************************************
/** Size of BodyCom Addresses */
#define ADDRESS_SIZE    4
//============== TIMER DEFINES ===============
/** Manchester Receive Timer Register */
#define MDLL_TIMER T4CON
/** Manchester Receive Timer Configuration */
// Timer is off, Postscaler 1:4, Prescaler 4:1
#define MDLL_TIMER_CONFIG 0b00011101
/** Manchester Receive Timer Period register*/
#define MDLL_PERIOD PR4
/** Manchester Receive Timer Interrupt bit*/
#define MDLL_INTERUPT TMR4IE
/** Manchester Receive Timer flag bit*/
#define MDLL_FLAG TMR4IF
/** Manchester Receive Timer Enable bit */
#define MDLL_TIMER_ON    TMR4ON

//============== GLOBAL VARIABLES ===============
MDLL_PacketData_t MDLL_PacketDataBuffer;
MDLL_status_t MDLL_status;
MDLL_mode_t MDLL_mode;
MDLL_txState_t MDLL_Txstate = MDLL_TXSTATE_INIT;
uint8_t MDLL_DeviceAddress[ADDRESS_SIZE] = MDLL_DEVICE_ADDRESS;
uint8_t MDLL_DeviceAddressMask[ADDRESS_SIZE] = MDLL_DEVICE_ADDRESS_MASK;

//============== FILE VARIABLES ===============
static uint8_t MDLL_preambleRepeats;
static uint8_t MDLL_transmissionRepeats;

//============== FUNCTIONS ===============
void MDLL_Isr(void) {
    // Check for Interrupt
    if (MDLL_INTERUPT && MDLL_FLAG) {
        // Clear flag so we can escape
        MDLL_FLAG = 0;
        // In half duplex mode RX has higher priority than TX. If we are servicing RX we do not service TX at all
        if (MDLL_mode == MDLL_MODE_RX) {
            if ((RX_manchesterState == MANR_DATA_READY) || 
                    (RX_manchesterState == MANR_TIMEOUT)) {
                // Finished with RX
                MDLL_setMode(MDLL_MODE_RX_DATAPENDING);
            } else {
                // Continue with RX
                MANR_RxHandler();
            }
        } else if (MDLL_mode == MDLL_MODE_TX) {
            if (MDLL_Txstate == MDLL_TXSTATE_TXDONE) 
            {
                // Done with TX
                MDLL_Txstate = MDLL_TXSTATE_IDLE;
                MDLL_setMode(MDLL_MODE_RX);
            } else {
                // Continue with TX
                MDLL_TxHandler();
            }
        }
    }
    // IOC Receiever check
    MANR_ISR();
}

void MDLL_TxHandler(void) {
    // Private variable
    static uint8_t timeoutCounter;
    // Handle Tx operation
    switch (MDLL_Txstate) {
        case MDLL_TXSTATE_INIT:
            // Prepare
            MDLL_status.PostTxAFEReset = MDLL_POSTTX_RESET;
            MDLL_Txstate = MDLL_TXSTATE_IDLE;
            break;

        case MDLL_TXSTATE_IDLE:
            // Now that everything is set up we can start the timer
            // Timer is off, Postscaler 1:4, Prescaler 4:1
            MDLL_TIMER = MDLL_TIMER_CONFIG; 
            MDLL_PERIOD = MANT_PERIOD_1MS;
            MDLL_Txstate = MDLL_TXSTATE_PREDELAY;
            timeoutCounter = 0;
            break;

        case MDLL_TXSTATE_PREDELAY:
            // Wait for pre delay to end
            if (++timeoutCounter >= MDLL_PREDELAY) {
                // Prepare for preamble
                MDLL_Txstate = MDLL_TXSTATE_PREAMBLE;
                MANT_HardwareEnableDriver;
                MDBIT = 1;
                timeoutCounter = 0;
                MDLL_transmissionRepeats = 0;
                MDLL_preambleRepeats = 0;
                // Number of preambles counter
                MDLL_PERIOD = MANT_PERIOD_1MS;
            }
            break;
            
        case MDLL_TXSTATE_PREAMBLE:
            // Send the Preamble
            if (++timeoutCounter >= ManchesterPreambleMs) {
                // Prepare for gap
                MDLL_Txstate = MDLL_TXSTATE_GAP;
                MDBIT = 0;
                TMR4 = 0;
                // Since this is a reduction in MDLL_PERIOD we need to take care
                MDLL_PERIOD = MANT_PERIOD_100US * MDLL_GAP_DELAY;
            }
            break;

        case MDLL_TXSTATE_GAP:
            // Prepare for high filter time
            MDLL_Txstate = MDLL_TXSTATE_FILTERHIGH;
            MDBIT = 1;
            // Set to approx 1.1ms (we need that x 2 = 2.2 for the High
            MDLL_PERIOD = (MANT_PERIOD_1MS + 2 * MANT_PERIOD_100US); 
            timeoutCounter = 0;
            break;

        case MDLL_TXSTATE_FILTERHIGH:
            // Wait till end of high time
            if (++timeoutCounter >= MDLL_FILTERHIGH_DELAY) {
                // Prepare for low filter time
                MDLL_Txstate = MDLL_TXSTATE_FILTERLOW;
                MDBIT = 0;
                // Set to approx 1.1ms (we need that x 2 = 1.1 for the High
                MDLL_PERIOD = (MANT_PERIOD_1MS + MANT_PERIOD_100US); 
                timeoutCounter = 0;
            }
            break;

        case MDLL_TXSTATE_FILTERLOW:
            // Wait for end of low time
            if (++timeoutCounter >= MDLL_FILTERLOW_DELAY)
            {
                // Check if preamble count is reached
                if (++MDLL_preambleRepeats > MDLL_PREAMBLE_REPEATS) {
                    // Reached; Prepare to TX data
                    MDLL_Txstate = MDLL_TXSTATE_DATA;
                    MDBIT = 1;
                    MDLL_PERIOD = MANT_PERIOD_100US;
                    // Init the transmit states and start the timer
                    MANT_StartTransmission();
                } else {
                    // Not Reached; Prepare another preamble
                    MDLL_Txstate = MDLL_TXSTATE_PREAMBLE;
                    MDBIT = 1;
                    timeoutCounter = 0;
                    MDLL_PERIOD = (MANT_PERIOD_1MS + MANT_PERIOD_100US);
                }
            }
            break;

            // Activate sub-statemachine for handling the individual data bits
        case MDLL_TXSTATE_DATA:
            // Sub state machine returns 0 when done
            if (MANT_TxBitBanger() == 5) {
                MDBIT = 1;
                MDLL_Txstate = MDLL_TXSTATE_POSTAMBLE;
                // Set to  1ms
                MDLL_PERIOD = MANT_PERIOD_1MS; 
                timeoutCounter = 0;
            }
            break;

        case MDLL_TXSTATE_POSTAMBLE:
            // Wait for postamble to end
            if (++timeoutCounter >= MDLL_POSTAMBLE_DELAY) 
            {
                // Prepare to repeat Tx
                MANT_HardwareDisableDriver;
                MDBIT = 0;
                MDLL_Txstate = MDLL_TXSTATE_REPEATDELAY;
                 // Set to approx 1ms
                MDLL_PERIOD = MANT_PERIOD_1MS;
                timeoutCounter = 0;
            }
            break;

        case MDLL_TXSTATE_REPEATDELAY:
             // Delay between retransmissions
            if (++timeoutCounter >= MDLL_REPEAT_DELAY)
            {
                // Check if we repeat
                if (++MDLL_transmissionRepeats > MDLL_TRANSMISSION_REPEATS) {
                    // Finished
                    // Reset AFE here if it is required;
                    // This must occur after this function is complete
                    MDLL_Txstate = MDLL_TXSTATE_TXDONE;
                } else {
                    // Prepare to repeat
                    MDLL_Txstate = MDLL_TXSTATE_PREAMBLE;
                    MANT_HardwareEnableDriver;
                    MDBIT = 1;
                    timeoutCounter = 0;
                    MDLL_PERIOD = MANT_PERIOD_1MS;
                }
            }

        case MDLL_TXSTATE_TXDONE:
            // Wait here till new TX or to go to Rx
            break;
    }
}

uint8_t MDLL_setMode(MDLL_mode_t mode) {
    switch (mode) {            
        case MDLL_MODE_DISABLED:
            // Disable all receive and transmit modes, used for e.g. RSSI mode
            MANT_HardwareDisableDriver;
            MDBIT = 0;
            MANR_HardwareDisableRXIoc();
            MDLL_mode = MDLL_MODE_DISABLED;
            break;
            
        case MDLL_MODE_RX:
            // Go to RX mode
            // Disable the TX
            MANT_HardwareDisableDriver;
            MDBIT = 0;
            // Disable the timer
            TMR4ON = 0;
            MDLL_mode = MDLL_MODE_RX;
            // Enable IOC
            MANR_HardwareEnableRXIoc();
            break;

        case MDLL_MODE_RX_DATAPENDING:
            // Disable the TX
            MANT_HardwareDisableDriver;
            MDBIT = 0;
            MDLL_mode = MDLL_MODE_RX_DATAPENDING;
            break;
            
        case MDLL_MODE_TX:
            // Enable the TX
            // Disable the RX
            MANR_HardwareDisableRXIoc();
            // Now that everything is set up we can start the timer
            // Timer is on, Postscaler 1:4, Prescaler 4:1
            T4CON = 0b00011101; 
            MDLL_PERIOD = MANT_PERIOD_1MS;
            MDLL_mode = MDLL_MODE_TX;
            break;
    }
    // The mode has changed, return the new mode to the caller
    return MDLL_mode;
}

uint8_t MDLL_sendPacket(MDLL_PacketData_t* packetData) {
    // Detect transmit overrun if we are still busy with last transmission
    if (MDLL_mode == MDLL_MODE_TX) {
        return 0;
    }
//===================================================
/* ***********************************************
    // Over here we will insert encryption if/when needed
    uint8_t encryptedData[32];
    // Little NULL encryption implementation, just copy the data over as-is
    memcpy(&encryptedData[0], (void*)packetData, (*packetData).DataLength);
*********************************************** */
    // Change the mode to Transmit
    if (MDLL_setMode(MDLL_MODE_TX) == MDLL_MODE_TX) {
        // Signal transmit framework that a transmission is requested/queued
         // Start with the header length
        uint8_t length = 5;
        // Data to be sent
        if (packetData->DataLength > 0) {
            length += packetData->DataLength + 1;
        }
        // Success
        return MANT_Send((uint8_t*) packetData, length);
    } else {
        // Failed
        return 0;
    }
}

uint8_t MDLL_receiveDataPacket(void) {
    // Tempory variables
    uint8_t checkSum = 0;
    uint8_t dataLength = 0;
    uint8_t receiveLength = 0;
    // If we have no data available we notify our caller of failure immediately
    if (MDLL_mode != MDLL_MODE_RX_DATAPENDING)
    {
        // Return length of receieve packet
        return receiveLength;
    }
    // Check if this is a broadcast
    if ((DecodePacketBuffer[1] == 0xFF) &&
        (DecodePacketBuffer[2] == 0xFF) &&
        (DecodePacketBuffer[3] == 0xFF) &&
        (DecodePacketBuffer[4] == 0xFF))
    {
       // Packet broadcast valid
       receiveLength = 5;
    } else  {
        // Check the received address
        if (((DecodePacketBuffer[1] | MDLL_DeviceAddressMask[0]) == (MDLL_DeviceAddress[0] | MDLL_DeviceAddressMask[0])) &&
                ((DecodePacketBuffer[2] | MDLL_DeviceAddressMask[1]) == (MDLL_DeviceAddress[1] | MDLL_DeviceAddressMask[1])) &&
                ((DecodePacketBuffer[3] | MDLL_DeviceAddressMask[2]) == (MDLL_DeviceAddress[2] | MDLL_DeviceAddressMask[2])) &&
                ((DecodePacketBuffer[4] | MDLL_DeviceAddressMask[3]) == (MDLL_DeviceAddress[3] | MDLL_DeviceAddressMask[3]))) {
            // Valid Address found
            receiveLength = 5;
        }
    }
    // Calculate the packet length from packet if it is a data bearing packet if matched only
    if (receiveLength && (DecodePacketBuffer[0] != 0)){
        // Check MSB to see if data bearing
        if (DecodePacketBuffer[0] & 0x80) 
        {
            // This is a data bearing packet, read the length from the packet itself
            dataLength = DecodePacketBuffer[5];
            // Make sure data does not overflow
            if (dataLength > (MDLL_MAX_DATA_LENGTH))
                dataLength = (MDLL_MAX_DATA_LENGTH);
            // Add the length byte to the calculation
            dataLength++; 
        } 
        // Clear checksum for check
        checkSum = 0;
        // Add data if any to length size
        receiveLength = dataLength + 5;
        // Calculate the checksum, the checksum is not included in the length so we add 1  (<= as opposed to <)
        for (int i = 0; i <= receiveLength ; i++) {
            // Calculate checksum
            checkSum ^= DecodePacketBuffer[i];
        }
        // Valid data produces 0 checksum
        if (checkSum) {
            receiveLength = 0;
        } else {
            // Valid packet, make a copy here
            memcpy((void *) &MDLL_PacketDataBuffer, &DecodePacketBuffer[0], receiveLength);
        }
    } else {
        // If DecodePacketBuffer[0] == 0 we received nothing, set length to 0
        receiveLength = 0;
    }
    // Clear processed DecodePacketBuffer values for next decode
    memset(DecodePacketBuffer, 0, sizeof (DecodePacketBuffer));
    // Flush the buffer if we received nothing.
    if (receiveLength == 0)
    {
        // Flush
        memset((void *) &MDLL_PacketDataBuffer, 0, sizeof (MDLL_PacketDataBuffer));
    }
    // Ready for next bye, indicate to lower layer that this one is processed
    MDLL_setMode(MDLL_MODE_RX);
    return receiveLength;
}




