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

#ifndef __AFE_H
#define __AFE_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>

//============== ENUMERATORS ===============
/** Select AFE_Out mode */
typedef enum{
    LFDATA = 0,
    RSSI = 1,
}AfeOutput_type;

//============== AFE Define Commands ===============
/** Sleep and wait for wake event */
#define AFE_SLEEP           0x02
/** Lock AGC */
#define AFE_LOCK            0x03
/** UnLock AGC */
#define AFE_UNLOCK          0x04
/** Reset AFE for next communication */
#define AFE_SOFT_RESET      0x05
/** AFE Read Command */
#define AFE_READ_COMMAND    6
/** AFE Write Command */
#define AFE_WRITE_COMMAND   7

//============== PROTOTYPES ===============
/** Send a byte to AFE via SPI interface
 * @param byte - Value of data to be sent to AFE */
void AFE_WriteByte(uint8_t byte);
/** Send a byte to AFE via SPI interface
 * @param byte - Value of data to be sent to AFE */
void AFE_WriteCommand(uint8_t command);
/** Write Data to AFE register via SPI interface
 * @param address - AFE register address
 * @return data - Value of data stored in specified AFE register */
uint8_t AFE_ReadRegister(uint8_t address);
/** Read Status Register of AFE
 *  @return AfeStatus */
uint16_t AFE_ReadStatus(void);
/** Set AFE into LFDATA or RSSI mode
 * @param mode - Value of data to be sent to AFE *LFDATA or RSSI (AfeOutput_type) */
uint8_t AFE_Out(AfeOutput_type mode);
/** Write Register information via SPI to AFE
 * @param address - Address to be sent over SPI
 * @param data - Data to be sent over SPI */
void AFE_WriteRegister(uint8_t address, uint8_t data);
/** Setup AFE into LFDATA mode by custom setup
 * @param byte(0-6) - AFE Register 0 - 6 setup */
uint8_t AFE_LfData_Setup(uint8_t Byte0, uint8_t Byte1, uint8_t Byte2, uint8_t Byte3, uint8_t Byte4, uint8_t Byte5, uint8_t Byte6);

#endif  // __AFE_H
