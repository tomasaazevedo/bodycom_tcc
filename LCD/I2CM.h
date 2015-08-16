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

#ifndef __I2CM_H
#define __I2CM_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>

//============== EXTERNAL DEFINE ===============
/** I2C Buffer size */
#define  I2CM_BUFFER_LEN  20

//============== ENUMERATOR ===============
typedef enum {
    I2CM_IDLE = 0,
    I2CM_START = 1,
    I2CM_SENDING = 2,
    I2CM_WAIT4_START_READ = 3,
    I2CM_WAIT4_STOP = 10,
    I2CM_READ_NEXT_BYTE = 20,
    I2CM_WAIT4_READ_BYE = 21,
    I2CM_ERROR_NAK = 99,
}I2cmHandlerState_t;

//============== EXTERNAL FUNCTIONS ===============
// I2C state machine state
extern I2cmHandlerState_t I2CM_state;
extern uint8_t I2CM_dataBuffer[I2CM_BUFFER_LEN];

//============== PROTOTYPE ===============
/** I2C module initialization routine */
void I2CM_Init(void);
/** Haandle I2C operation */
void I2CM_Handler(void);
/** I2C Master get a byte on I2C bus
    return - MSSPBUF - I2C Buffer */
uint8_t I2CM_GetByte(void);
/** I2C Master send stop condition on I2C bus */
void I2CM_Stop(void);
/** Create i2c start condition and change state to wait_start */
void I2CM_SendStart (uint8_t len);

#endif // __I2CM_H