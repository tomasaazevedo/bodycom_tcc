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

#ifndef __MDLL_DATALINKLAYER_H
#define	__MDLL_DATALINKLAYER_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>

//============== DEFINES ===============
/** Maximum sized packet buffer (1+4+1+16 = 22 bytes*/
#define MDLL_MAX_DATA_LENGTH  16

//============== STRUCTURES ===============
/** Structure to handle packet communication */
typedef struct {
    uint8_t Command;
    uint8_t Address[4];
    uint8_t DataLength;
    uint8_t DataBuffer[MDLL_MAX_DATA_LENGTH];
} MDLL_PacketData_t;
/** Packet data buffer instance for receiving */
extern MDLL_PacketData_t MDLL_PacketDataBuffer;
/** Afe Receiever requires reset */
typedef struct {
    unsigned PostTxAFEReset : 1; 
    // Rest of the byte is unused
} MDLL_status_t;
/** Status bits for Data Link Layer */
extern MDLL_status_t MDLL_status;
//============== ENUMERATORS ===============
/** Enum to define tranceiver status */
typedef enum {
    MDLL_MODE_INIT = 0,
    MDLL_MODE_DISABLED = 1,
    MDLL_MODE_RX = 2,
    MDLL_MODE_RX_DATAPENDING = 3,
    MDLL_MODE_TX = 4
} MDLL_mode_t;
/** Current operation mode */
extern MDLL_mode_t MDLL_mode;
/** Commands */
typedef enum {
    PING_PAIRED = 0x15,
    PAIR_DEVICE = 0x25,
    ECHO_REQUEST = 0x96,
    ECHO_RESPONSE = 0x97,
    DATA = 0x95
} BodyCom_Commands_t;
/** Enum to define tranceiver status */
typedef enum {
    MDLL_TXSTATE_INIT = 0,
    MDLL_TXSTATE_IDLE = 1,
    MDLL_TXSTATE_PREDELAY = 2,
    MDLL_TXSTATE_PREAMBLE = 3,
    MDLL_TXSTATE_GAP = 4,
    MDLL_TXSTATE_FILTERHIGH = 5,
    MDLL_TXSTATE_FILTERLOW = 6,
    MDLL_TXSTATE_DATA = 7,
    MDLL_TXSTATE_POSTAMBLE = 8,
    MDLL_TXSTATE_REPEATDELAY = 9,
    MDLL_TXSTATE_TXDONE = 10,
} MDLL_txState_t;
/** Current state of communication Tx */
extern MDLL_txState_t MDLL_Txstate;

#ifdef BASE
// Macros for Driver control
// Enable
#define MANT_HardwareEnableDriver    \
    BC_SERIAL_TRIS = 1;              \
    BC_MODOUT_TRIS  = 1;             \
    BC_MODOUT_N_TRIS = 0;            \
    BC_ENABLERX_OUT = 0;	     \
    MDOE = 1;                        \
    C1OE = 1;                        \
    BC_MODOUT_OUT = 0;               \
    BC_MODOUT_N_OUT = 0;
// Disable
#define MANT_HardwareDisableDriver \
    BC_SERIAL_TRIS = 1;              \
    BC_MODOUT_OUT  = 0;              \
    BC_MODOUT_N_OUT = 0;             \
    LATC6 = 0;                       \
    BC_MODOUT_TRIS = 0;              \
    BC_MODOUT_N_TRIS = 0;            \
    MDOE = 0;                        \
    C1OE = 0;                        \
    BC_ENABLERX_OUT = 1;

#elif MOBILE
// Macros for Driver control
// Enable
#define MANT_HardwareEnableDriver  \
    MANT_RX_nEN_LAT = 0;             \
    MANT_DRIVER_STATE = 0;           \
// Disable
#define MANT_HardwareDisableDriver \
   MANT_RX_nEN_LAT = 1;              \
   MANT_DRIVER_STATE = 0;            \

#endif
//============== EXTERNAL VARIABLES ===============
/** Current Device Address */
extern uint8_t MDLL_DeviceAddress[];
/** Current Device Address Mask */
extern uint8_t MDLL_DeviceAddressMask[];

//============== PROTOTYPES ===============
/** This function is system ISR function which handles RX and TX in half-duplex mode */
void MDLL_Isr(void);
/** This function is system ISR function which handles RX and TX in half-duplex mode */
void MDLL_TxHandler(void);
/** Change the tranceiver between Disabled, RX, TX and Full Duplex modes */
uint8_t MDLL_setMode(MDLL_mode_t mode);
/** Send a packet using the Manchester Physical layer */
uint8_t MDLL_sendPacket(MDLL_PacketData_t* packetData);
/** Receive a packet if one is available in the receive buffer */
uint8_t MDLL_receiveDataPacket(void);

#endif	/* __MDLL_DATALINKLAYER_H */

