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

//============== MODULE INCLUDES ===============
#include "PC_Handler.h"
// Handler Transmit
#include "PC_Tx.h"
// Handler Receieve
#include "PC_Rx.h"

//============== APPLICATION INCLUDES ===============
// Display information
#include "DBUF_DisplayBuffer.h"
// CVD value and state
#include "TD_CvdHandler.h"
// BodyCom Application Paired List
#include "PL_PairedList.h"
// BodyCom Development Board Application
#include "BC_Application.h"

//***********************************************
//============== SETUP DEFINES ==================
//***********************************************
//============== I/O DEFINES ===============
/** Receieve Pin */
#define PC_RX_PIN_TRIS  TRISC5
/** Transmit Pin */
#define PC_TX_PIN_TRIS  TRISC3

//============== REGISTER DEFINES ===============
/** Transmit Status and Control Register Config */
#define PC_TXSTA    0b00100000
/** Receive Status and Control Register Config */
#define PC_RCSTA    0b10010000
/** Baudrate Control Register Config */
#define PC_BAUDCON  0b00001000
/** Alternative Pin Configuration Register */
#define PC_APFCON0  0b10000100
/** Baudrate Generator Low */
#define PC_SPBRGL   52
/** Baudrate Generator Low */
#define PC_SPBRGH   0

//============== DATA ===============
/** Max pc buffer size */
#define PC_BUFFER_MAX   20

//============== ENUMERATORS ===============
/** PC Handler Commands */
typedef enum {
    Pc_App_Version = 'V',
    Pc_App_State = 'S',
    Pc_App_RxCmd = 'W',
    Pc_App_BoardStatus = 'U'
}PcCommandRequest_t;

/** Board State Interactive */
typedef enum {
    Pc_App_Rx_TouchPckt = '0',
    Pc_App_Rx_RssiPckt = '1',
    Pc_App_Rx_MobileData = '2',
    Pc_App_Rx_Pckt = '3',
    Pc_App_Rx_ReadList = '4',
    Pc_App_Rx_ListDelete = '5',
    Pc_App_Rx_ListSelect = '6',
    Pc_App_Rx_ListAdd = '7',
    Pc_App_Rx_Status = 'X',
    Pc_App_Rx_LcdRead = 'L',
    Pc_App_Rx_NewCmd = 'N'
}PcStatusRequest_t;

//============== GLOBAL VARIABLES ===============
MDLL_PacketData_t  pcRxPacket;

//============== FILE VARIABLES ===============
/** PC Handler Command Buffer */
uint8_t PcCommandBuffer[PC_BUFFER_MAX] = 0;

//============== VARIABLES ===============
/** Have the PC request State if available */
bit PcStateRequestAvailable = 0;
/** Current state of PC handler */
uint8_t PcCurrentState = 0;
/** Current PC Command value */
uint8_t PcCommand = 0;
/** Current PC Command Length */
uint8_t PcCommandLength = 0;

//============== FUNCITONS ===============
void PC_Isr(void) {
    // Handle a PC transmit
    if (TXIF && TXIE) {
        LATA0 = 1;
        PC_TxHandler();
        LATA0 = 0;
    }
    // Handle a PC recieve and process command
    if (RCIF && RCIE) {
        if ( PC_RxHandler() ) {
            // Packet RX successfull; Handle command
            PC_CommandHandler();
        }
        // Handle Overrun
        if (OERR == 1) {
            // Re-enable on overrun
            CREN = 0;
            CREN = 1;
        }
        // Receive was serviced
        RCIF = 0;
    }
}
void PC_ComInit(void) {
    // Pin Selection Initialization
    PC_RX_PIN_TRIS = 1;
    PC_TX_PIN_TRIS = 0;
    // Register Initialization
    SPBRGL = PC_SPBRGL;
    SPBRGH = PC_SPBRGH;
    TXSTA = PC_TXSTA;
    RCSTA = PC_RCSTA;
    BAUDCON = PC_BAUDCON;
    APFCON0 = PC_APFCON0;
    // Interrupt Initialization
    RCIF = 0;
    RCIE = 1;
    SPEN = 1;
    CREN = 1;
    TXEN = 1;
}
void PC_StateChangeHandler(void) {
    // Allows GUI to changes states
    if (PcStateRequestAvailable != 0) {
        PcStateRequestAvailable = 0;
        BodyComState = PcCurrentState;
    }
}
void PC_BodyComRequestHandler(void) {
    // PC has requested an action
    if (PcCommand != 0) {
        // Process Command
        switch (PcCommand) {
            case PING_PAIRED:
                // Send a broadcast ping and wait for a response.
                // Display response ID.
                if (BC_SendCommand(PcCommand, PcCommandBuffer, PING_TIMEOUT*2) > 0)
                {
                    // Board handles as a BCOM_CMD_PING_SENT command
                    BCOM_cmdState = BCOM_CMD_PING_SENT;
                }
                break;

            case ECHO_REQUEST:
                // Send PC command, on this level lengh does not include the header.
                // Display Echo Data.
                if (BC_SendDataCommand(PcCommand, PcCommandBuffer,  &PcCommandBuffer[4], PcCommandLength-5, PING_TIMEOUT*2))
                {
                   // Board handles as BCOM_CMD_ECHOREQ_SENT
                   BCOM_cmdState = BCOM_CMD_ECHOREQ_SENT;
                }
                break;

            case PAIR_DEVICE:
                    // PC requsted a Pair Operation. Broadcast ping; wait for response.
                    // Add response ID to paired list; if already on list display only.
                    if (BC_SendCommand(PING_PAIRED,PcCommandBuffer, PING_TIMEOUT*2) > 0)
                    {
                       // Board handles as BCOM_CMD_PAIRPING_SENT
                       BCOM_cmdState = BCOM_CMD_PAIRPING_SENT;
                    }                
                break;

            default:    // Error
                break;
        }
        // Action has been serviced. Clear request.
        PcCommand = 0;
    }
}

void PC_CommandHandler(void) {
    // Tempory Variables
    PcCommandRequest_t pcApplicationCommand;
    PcStatusRequest_t pcStateStatus;    
    uint8_t length = 0;
    // Store PC-Board command
    pcApplicationCommand = PcRxBuffer[PcRxIndex++];
    // Store status command request
    pcStateStatus = PcRxBuffer[PcRxIndex++];
    // Handle PC-Board Command
    switch (pcApplicationCommand) {
            // PC requested board version; 'V'
        case Pc_App_Version:
            if (PcRxIndex == PcRxWriteCount) {
                PC_SendVersion();   // Send Application Version
            }
            break;

            // PC requested board State; 'S'
        case Pc_App_State:
            if (PcRxIndex == PcRxWriteCount) {
                PcStateRequestAvailable = 1;    // Allow GUI/Board to update state via PC_StateChangeHandler()
                PcCurrentState = pcStateStatus; // Read desired state change
                PC_BoardState(pcStateStatus);   // Execute state change for demo
            }
            break;

            // PC has BodyCom action command request 'W'
        case Pc_App_RxCmd:
            PcCommand = pcStateStatus;                  // Get command
            PcCommandLength = PcRxBuffer[PcRxIndex++];  // Get packet length
            // Loop for packet length
            for (length = 0; length < PcCommandLength; length++) {
                PcCommandBuffer[length] = PcRxBuffer[PcRxIndex++];  // Construct packet
                PcRxIndex &= (PC_RX_BUFFER_SIZE);   // Make sure packet size cannot exceed max
            }
            // Send Updated state and device count
            PC_SendStatus();    
            break;

            // PC has requested status update based on boards current state 'U'
        case Pc_App_BoardStatus:
        {
            // Handle Board Status command request
            switch (pcStateStatus) {
                    // Send touch value information '0'
                case Pc_App_Rx_TouchPckt:
                    PC_SendTouchPacket();
                    break;

                    // Send RSSI value information  '1'
                case Pc_App_Rx_RssiPckt:
                    PC_SendRssiPacket();
                    break;

                    // Send information from last communicated mobile   '2'
                case Pc_App_Rx_MobileData:
                    PC_SendMobileData();
                    break;

                    // Send a Custom packet   '3'
                    // Dummy packet state for custom development
                case Pc_App_Rx_Pckt:
                    PC_SendPacket();
                    break;

                    // Send paired list information '4'
                case Pc_App_Rx_ReadList:
                    // Read list until no more device remain
                    if (PcRxIndex == PcRxWriteCount) {
                        break;
                    }
                    length = PcRxBuffer[PcRxIndex++];       // Number of paired devices
                    PcRxIndex &= (PC_RX_BUFFER_SIZE);       // Make sure Buffer size is ok
                    PC_ReadPairedList(length);              // Read IDs till finished
                    break;

                    // Send updated list after delete request is handled    '5'
                case Pc_App_Rx_ListDelete:
                    // When operation is complete; break
                    if (PcRxIndex == PcRxWriteCount) {
                        break;
                    }
                    length = PcRxBuffer[PcRxIndex++];       // Device index to delete
                    PcRxIndex &= (PC_RX_BUFFER_SIZE);       // Make sure Buffer size is ok
                    PL_DeletePairedDevice(length);  // Remove device at location
                    PL_ReadPaired(0, DeviceInfo); // Update Paired list after delete
                    PC_SendStatus();                            // Send state and new Device Count
                    break;

                    // Update selected device on list   '6'
                case Pc_App_Rx_ListSelect:
                     // When operation is complete; break
                    if (PcRxIndex == PcRxWriteCount) {
                        break;
                    }
                    length = PcRxBuffer[PcRxIndex++];       // Device index to select
                    PcRxIndex &= (PC_RX_BUFFER_SIZE);       // Make sure Buffer size is ok
                    PL_ReadPaired(length, DeviceInfo);  // Read device info at location
                    PC_SendStatus();                            // Send state and new Device Count
                    break;

                    // Send updated list after add request is handled   '7'
                case Pc_App_Rx_ListAdd:
                    PL_AddPairedDevice(&PcRxBuffer[PcRxIndex]); // Add device to paired list
                    PC_SendStatus();                                    // Send state and new Device Count
                    break;

                    // Send current state and number of paired devices  'X'
                case Pc_App_Rx_Status:
                    PC_SendStatus();            // Send state and new Device Count
                    break;

                    // Send LCD display information 'L'
                case Pc_App_Rx_LcdRead:
                    // When operation is complete; break
                    if (PcRxIndex == PcRxWriteCount) {
                        break;
                    }
                    length = PcRxBuffer[PcRxIndex++];   // Line to be read
                    PcRxIndex &= (PC_RX_BUFFER_SIZE);    // Make sure Buffer size is ok
                    PC_ReadLcd(length);                  // Read LCD line as requested
                    break;

                    // Place holder for custom/new commands 'N'
                case Pc_App_Rx_NewCmd:                  // Not used; Write your own.....
                    break;

                default: //error
                    break;
            }
        }
            break;

        default: //error
            break;
    }

    // Not needed??????
    while (PcRxIndex != PcRxWriteCount) {
        pcApplicationCommand = PcRxBuffer[PcRxIndex++];
        PcRxIndex &= (PC_RX_BUFFER_SIZE);
    }
}

//============== COMMAND FUNCITONS ===============
void PC_SendVersion(void) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    PcTxBuffer[count++] = 'V';                      // Version Command
    PcTxBuffer[count++] = (0x30 + SW_VERSION2);     // Minor Version
    PcTxBuffer[count++] = (0x30 + SW_VERSION1);     // Major Version
    PcTxBuffer[count++] = 0xFF;                     
    PcTxBuffer[count++] = BodyComState;       // Current Board State
    PcTxBuffer[count++] = 'V';                      //
    PcTxWriteCount = count;         // Number of characters to write
    TXIE = 1;                       // Trigger Transmit
}
void PC_BoardState(uint8_t state) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    PcTxBuffer[count++] = 'S';  
    PcTxBuffer[count++] = state;   
    PcTxWriteCount = count;     // Number of characters to write
    TXIE = 1;       // Trigger Transmit
}
void PC_SendMobileData(void) {
    // Temporty variable
    uint8_t count = 0;
    // Only send if we have mobile data, else the command is ignored
    if (pcRxPacket.Command == 0) {
        PcTxBuffer[count++] = 'D';
    } else {
        // Build Buffer
        PcTxBuffer[count++] = 'D';
        PcTxBuffer[count++] = pcRxPacket.Address[0];
        PcTxBuffer[count++] = pcRxPacket.Address[1];
        PcTxBuffer[count++] = pcRxPacket.Address[2];
        PcTxBuffer[count++] = pcRxPacket.Address[3];
        PcTxBuffer[count++] = pcRxPacket.Command;
        // Handle Packets Data
        PcTxBuffer[count++] = (pcRxPacket.DataLength == 0) ? 1 : 2;
        PcTxBuffer[count++] = pcRxPacket.DataLength;
        for (uint8_t i = 0; i <= pcRxPacket.DataLength; i++) {
            PcTxBuffer[count++] = pcRxPacket.DataBuffer[i];
        }
    }
    PcTxWriteCount = count; // Number of characters to write
    TXIE = 1;   // Trigger Transmit
    // Data sent, clear the buffer
    memset(&pcRxPacket, 0, sizeof(pcRxPacket));
}
void PC_SendTouchPacket(void) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    // Send Touch packet
    // Load 10-bit (2 byte); Current CVD value
    // Load 10-bit (2 byte); Average CVD value
    PcTxBuffer[count++] = 'T';
    PcTxBuffer[count++] = (CvdSensorData >> 8);
    PcTxBuffer[count++] = (CvdSensorData & 0xFF);
    PcTxBuffer[count++] = (CvdAverage >> 8);
    PcTxBuffer[count++] = (CvdAverage & 0xFF);
    PcTxWriteCount = count; // Number of characters to write
    TXIE = 1;   // Trigger Transmit
}
void PC_SendPacket(void) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    // Custom Packet
    PcTxBuffer[count++] = 'R';
    PcTxBuffer[count++] = 0x30;
    PcTxBuffer[count++] = 0x30;
    PcTxWriteCount = count; // Number of characters to write
    TXIE = 1;   // Trigger Transmit
}
void PC_SendStatus(void) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    // Board Status command
    PcTxBuffer[count++] = 'X';      
    PcTxBuffer[count++] = 0xFF;
    PcTxBuffer[count++] = BodyComState;
    PcTxBuffer[count++] = PairedCount;
    PcTxBuffer[count++] = 1;
    PcTxWriteCount = count; // Number of characters to write
    TXIE = 1;   // Trigger Transmit
}
void PC_SendRssiPacket(void) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    // Rssi Packet command
    // Load 10-bit (2 byte) value
    PcTxBuffer[count++] = 'I';                  
    PcTxBuffer[count++] = HandlerRssiValue >> 8;    
    PcTxBuffer[count++] = HandlerRssiValue & 0xFF;
    PcTxWriteCount = count; // Number of characters to write
    TXIE = 1;   // Trigger Transmit
}
void PC_ReadLcd(uint8_t line) {
    // Temporty variable
    uint8_t index = 0,
            count = 0;
    // Build Buffer
    // Read LCD line command
    PcTxBuffer[count++] = 'L';  
    PcTxBuffer[count++] = line;
    // Top line; 0
    if (line == 0) {
        index = 0;
    } else {    // Bottom line; 1
        index = 16;
    }
    // Copy LcdBuffer[line] to PC buffer for Tx
    memcpy(PcTxBuffer + 2, &DisplayBuffer[index], 16);
    PcTxWriteCount = count + 16; // Add the 16 from the LCD line
    TXIE = 1;   // Trigger Transmit
}
void PC_ReadPairedList(uint8_t device) {
    // Temporty variable
    uint8_t count = 0;
    // Build Buffer
    // Pairing list command
    // Device position
    PcTxBuffer[count++] = 'P';      
    PcTxBuffer[count++] = device + 0x30;    
    // Read from list
    PL_ReadPaired(device, &PcTxBuffer[count]);
    count += 4;
    // Send
    PcTxWriteCount = count; // Number of characters to write
    TXIE = 1;   // Trigger Transmit
}
