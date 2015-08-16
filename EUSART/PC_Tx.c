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
 * Functions for handling Transmit operation to PC
 */

//============== MODULE INCLUDES ===============
#include "PC_Tx.h"

//============== PRIVATE DEFINES ===============
/** Start of Frame Command */
#define PC_SOF   	0x0A
/** End of Frame Command */
#define PC_EOF   	0x0D
/** Data Link Escape */
#define PC_DLE    	0x04
/** Max Packet Size */
#define PC_MAX_PACKET_SIZE  31

//============== ENUMERATORS ===============
/** PC TX States */
enum {
    PC_TX_START,
    PC_TX,
    PC_TX_IN_PROGRESS,
    PC_TX_DLE,
    PC_TX_END
} PcTxStates_t;

//============== GLOBAL VARIABLES ===============
uint8_t PcTxBuffer[32] = 0;
uint8_t PcTxWriteCount = 0;

//============== FUNCTIONS ===============
void PC_TxHandler(void) {
    // Private function variables
    static uint8_t PcTxIndex = 0;
    static uint8_t PcTxTemp = 0;
    // Handle TX operation to PC
    switch (PcTxStates_t) {
        // Begin of TX packet; insert SOF character into buffer
        case PC_TX_START:
            PcTxTemp = PC_SOF;
            // Prepare to load data
            PcTxStates_t = PC_TX;
            break;

        case PC_TX:
            // Transmit data
            TXREG = PcTxTemp;
            // Prepare for next TX
            PcTxStates_t = PC_TX_IN_PROGRESS;
            break;

        case PC_TX_IN_PROGRESS:
            // Load with next data in buffer
            PcTxTemp = PcTxBuffer[PcTxIndex++];     // increment buffer index
            PcTxIndex &= (PC_MAX_PACKET_SIZE);  // Make sure buffer index is not greater then max size
            // Buffer still has data to send
            if (PcTxIndex <= PcTxWriteCount) {
                // Data to be sent shares value with PC_DLE, PC_SOF or PC_EOF; insert DLE infront of value to make valid.
                if ((PcTxTemp == PC_DLE) || (PcTxTemp == PC_SOF) || (PcTxTemp == PC_EOF)) {
                    PcTxStates_t = PC_TX_DLE;   // Data is a operation character; insert DLE infront and Send.
                } else {
                    PcTxStates_t = PC_TX;   // Data sent is valid; Send
                }
            }
            // All data has been sent from buffer
            if (PcTxIndex > PcTxWriteCount) {
                // Prepare to finish packet
                PcTxStates_t = PC_TX_END;
            }
            break;
            // Data shares value with operation character. Insert DLE infront to make valid.
        case PC_TX_DLE:
            // Prepare
            TXREG = PC_DLE;
            // Send
            PcTxStates_t = PC_TX;
            break;
            // Packet is done sending. Insert EOF
        case PC_TX_END:
            // Send EOF to indicate end of packet
            TXREG = PC_EOF;
            // Clear for next operation
            TXIE = 0;
            PcTxIndex = 0x00; // clear the transmit buffer index
            PcTxWriteCount = 0x00; // clear the transmit counter
            // Wait at Start state till new packet needs to be sent. 
            PcTxStates_t = PC_TX_START;
            break;

        default: // error occured
            break;
    }
}
