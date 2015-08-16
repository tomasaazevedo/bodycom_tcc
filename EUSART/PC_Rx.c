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
 * Functions for handling Receive operation to PC
 */

//============== MODULE INCLUDES ===============
#include "PC_Rx.h"

//============== PRIVATE DEFINES ===============
/** Start of Frame Command */
#define PC_SOF   	0x0A
/** End of Frame Command */
#define PC_EOF   	0x0D
/** Data Link Escape */
#define PC_DLE    	0x04

//============== ENUMERATOR ===============
/** PC RX States */
enum {
    PC_RX_START,
    PC_RX_IN_PROGRESS,
    PC_RX_DLE
} Pc_Rx_State;

//============== GLOBAL VARIABLES ===============
uint8_t PcRxBuffer[PC_RX_BUFFER_SIZE] = 0;
uint8_t PcRxIndex = 0;
uint8_t PcRxWriteCount = 0;

//============== FUNCITONS ===============
uint8_t PC_RxHandler(void) {
    // Temp variable
    uint8_t data;
    // Store information from EUART receive register
    data = RCREG;
    // Determine actions to take based on current Rx state
    switch (Pc_Rx_State) {
        // Start receieve after Start-of-Frame [SOF] character is receieved
        case PC_RX_START:
            if (data == PC_SOF) {
                PcRxIndex = 0; // clear the receiver buffer index
                PcRxWriteCount = 0; // clear the receive counter
                // Update state
                Pc_Rx_State = PC_RX_IN_PROGRESS;
            }
            break;

        // Compile receieved packet
        case PC_RX_IN_PROGRESS:
            // Start-of-Frame [SOF] receieved; restart packet construction; [Guard check]
            if (data == PC_SOF) {
                PcRxIndex = 0; // clear the receiver buffer index
                PcRxWriteCount = 0; // clear the receive counter
            }
            // Data-Link-Escape [DLE] character received
            else if (data == PC_DLE) {
                Pc_Rx_State = PC_RX_DLE;
            }
            // End-of-Frame [EOF] character receieved
            else if (data == PC_EOF) {
                // Packet is done; Handle command
                Pc_Rx_State = PC_RX_START;
                return 1;
            }
            // Packet is still being constructed
            else {
                PcRxBuffer[PcRxWriteCount++] = data;
                PcRxWriteCount &= (PC_RX_BUFFER_SIZE);
            }
            break;

            // DLE was infront of data; load data into buffer; return to receiving packet remaining
        case PC_RX_DLE:
            PcRxBuffer[PcRxWriteCount++] = data;
            PcRxWriteCount &= (PC_RX_BUFFER_SIZE);

            Pc_Rx_State = PC_RX_IN_PROGRESS;
            break;

        default: //Error
            break;
    }
    // Packet is not ready to be handled
    return 0;
}
