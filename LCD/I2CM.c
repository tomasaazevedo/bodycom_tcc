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
 * Function for I2C operation
 */

//============== MODULE INCLUDES ===============
#include "I2CM.h"

//***********************************************
//============== SETUP DEFINES ==================
//***********************************************
//============== I/O DEFINES ===============
/** SDA analog/digital pin */
#define ANSEL_I2C_SDA           ANSB4
/** Clock Analog/Digital pin */
#define ANSEL_I2C_CLK  	        ANSB6
/** SDA direction control */
#define DIR_SSP_I2C_SDA         TRISB4
/** Direction of clock pin */
#define DIR_SSP_I2C_CLK  	TRISB6

//============== REGISTER DEFINES ===============
/** MSSP status register */
#define MSSPSTAT		SSP1STAT
/** MSSP ADD register */
#define MSSPADD			SSP1ADD
/** MSSP Config register */
#define MSSPCON1		SSPCON1
/** MSSP Config register */
#define MSSPCON2		SSP1CON2
/** MSSP Config register bits */
#define MSSPCON2bits            SSP1CON2bits
/** MSSP Buffer */
#define MSSPBUF			SSP1BUF
/** MSSP status register bits */
#define MSSPSTATbits            SSP1STATbits

//============== GLOBAL VARIABLES ===============
// I2C state machine state
I2cmHandlerState_t I2CM_state = I2CM_IDLE;
uint8_t I2CM_dataBuffer[I2CM_BUFFER_LEN];

//============== LOCAL VARIABLES ===============
static uint8_t I2CM_dataPos = 0;
static uint8_t I2CM_dataLength = 0;

//============== FUNCTIONS ===============
void I2CM_Init(void) {
    // Prepare I2C pins for operations
    ANSEL_I2C_SDA = 0;
    ANSEL_I2C_CLK = 0;

    DIR_SSP_I2C_SDA = 1; // configure I2C data pin for LCD
    DIR_SSP_I2C_CLK = 1; // configure I2C clock pin for LCD

    // Configure SSP for I2C use
    MSSPSTAT = 0x00;
    MSSPADD = 0x0A; // 400kHz @ 16MHz clock - limited  by LCD specs

    MSSPCON1 = 0x28; //	SEN = 1; I2C MASTER
    MSSPCON2 = 0x00;
}
void I2CM_Handler(void) {
    switch (I2CM_state) {
        case I2CM_IDLE: // Idle
            break;

        case I2CM_START: // Wait Start Condition for Send
            if (MSSPCON2bits.SEN == 0) {
                MSSPBUF = I2CM_dataBuffer[I2CM_dataPos++];
                I2CM_state = I2CM_SENDING;
            }
            break;

        case I2CM_SENDING: // Sending bytes
            if (MSSPSTATbits.R_nW == 0) {
                if (MSSPCON2bits.ACKSTAT == 0) {
                    if (I2CM_dataPos < I2CM_dataLength) {
                        MSSPBUF = I2CM_dataBuffer[I2CM_dataPos++];
                    } else {
                        MSSPCON2bits.PEN = 0x01;
                        I2CM_state = I2CM_WAIT4_STOP;
                    }
                } else {
                    I2CM_state = I2CM_ERROR_NAK;
                }
            }
            break;

        case I2CM_WAIT4_STOP: // Wait Stop Condition
            if (!MSSPCON2bits.PEN) {
                I2CM_state = I2CM_IDLE;
            }
            break;

        case I2CM_WAIT4_START_READ: // Wait Start Condition for Read
            if (!MSSPCON2bits.SEN)
                I2CM_state = I2CM_SENDING;
            break;

        case I2CM_READ_NEXT_BYTE: // Read next byte
            I2CM_state = I2CM_WAIT4_READ_BYE;
            break;

        case I2CM_WAIT4_READ_BYE: // Wait for read byte
            if (!MSSPCON2bits.RCEN)
                I2CM_state = I2CM_READ_NEXT_BYTE;

            // If no more bytes goto Stop
            break;

        case I2CM_ERROR_NAK:
            break;

        default:
            I2CM_state = I2CM_ERROR_NAK;
            break;
    }
}
uint8_t I2CM_GetByte(void) {
    MSSPCON2bits.RCEN = 1;

    while (MSSPCON2bits.RCEN) {
        CLRWDT();
    }
    NOP(); //for Breakpoint

    return MSSPBUF;
}
void I2CM_Stop(void) {
    MSSPCON2bits.PEN = 0x01;

    while (MSSPCON2bits.PEN) {
        CLRWDT();
    }
    NOP(); //for Breakpoint
}
void I2CM_SendStart (uint8_t len)
{
    MSSPCON2bits.SEN = 0x01;
    I2CM_dataPos = 0;
    I2CM_dataLength = len;
    I2CM_state = I2CM_START;
}