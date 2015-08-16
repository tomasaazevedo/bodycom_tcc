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
 * Function for LCD with ST7032 controller
 */

//============== MODULE INCLUDE ===============
#include "LCDM_St7032.h"

//============== COMMUNICATION INCLUDE ===============
#include "I2CM.h"

//============== ST7032 INSTRUCTIONS ===============
//Sitronix ST7032 Dot Matrix LCD Controller/Driver Datasheet V1.2 (10/17/05)
// Page (20-27) In order of use
//===================================================
// === Page 22
/** Clear command to ST7032 */
#define ST7032_CLEAR                               0x01
/** Home command to ST7032 */
#define ST7032_HOME                                0x02

//============== LCD FUNCTION CONTROL ===============
// === Page 24
/** ST7032 Funciton control */
#define ST7032_FUNCTION 0x20
/** Control function for lcd */
#define FUNCTION_4BITS 0x00
/** Control function for lcd */
#define FUNCTION_8BITS 0x10
/** Control of ST7032 line */
#define FUNCTION_1_LINE 0x00
/** Control of ST7032 line */
#define FUNCTION_2_LINE 0x08
/** Control of ST7032 functions */
#define FUNCTION_1_HIGH 0x00
/** Control of ST7032 functions */
#define FUNCTION_2_HIGH 0x04

//============== Instruction Page (IS) ===============
// === Page 24
/** Control of ST7032 functions (1st)*/
#define FUNCTION_nIS 0x00
/** Control of ST7032 functions (2nd)*/
#define FUNCTION_IS 0x01

//============== LCD ADDRESSES ===============
// === Page 25
/** ST7032 CGRAM address control */
#define ST7032_CGRAM_ADDRESS(adr) (0x40 | (adr & 0x3F))
/** ST7032 DDRAM address control */
#define ST7032_DDRAM_ADDRESS(adr) (0x80 | (adr & 0x7F))
/** LCD ICON address */
#define ST7032_ICON_ADDRESS(adr) (0x40 | (adr & 0x0F))
/** LCD Address mask */
#define ST7032_ADDRESS_MASK   0x7F

//============== READ BUSY ===============
// === Page 25
/** LCD Busy Flag Mask */
#define ST7032_BUSY_FLAG_MASK 0x80

//============== BIAS ===============
// === Page 27
/** ST7032 Bias control */
#define BIAS_1_5 0x00
/** ST7032 Bias control */
#define BIAS_1_4 0x08

//============== FREQUENCY ===============
// === Page 27
/** ST7032 Frequency Control */
#define FREQ_CNTRL(f) (f&0x07)
/** Operating Frequency */
#define ST7032_OSC_FREQ 0x10

//============== POWER/ICON/CONTRAST ===============
// === Page 27
/** LCD contrast control */
#define ST7032_CONTRAST(c) (0x70 | (c & 0x0F))
/** LCD power controller */
#define ST7032_PWR_CONTROL 0x50
/** Invert ICON command */
#define nICON 0x00
/** ICON command */
#define ICON  0x08
/** Invert LCD booster command */
#define nBOOST 0x00
/** Control LCD booster */
#define BOOST 0x04
/** Control LCD contrast */
#define CONTRAST(c) (c&0x03)

//============== FOLLOWER ===============
// === Page 28
/** LCD follower gain value */
#define FOLLOWER_GAIN(g) (g&0x07)
/** Disable follower */
#define ST7032_FOLLOWER_OFF            0x60
/** Enable Follower */
#define ST7032_FOLLOWER_ON             0x68

//============== ENTRY ===============
// === Page 22
/** ST7032 Entry mode */
#define ST7032_ENTRY_MODE 0x04
//============== SHIFT ===============
/** Inverted shift cursor command */
#define CURSOR_nSHIFT  0x00
/** Shift cursor command */
#define CURSOR_SHIFT   0x01

//============== DATA ===============
// === Page 25
/** Decrement LCD Data */
#define DATA_DECREMENT 0x00
/** Increment LCD Data */
#define DATA_INCREMENT 0x02

//============== DISPLAY ===============
// === Page 23
/** Disable LCD */
#define ST7032_DISPLAY_OFF 0x08
/** Enables Display */
#define ST7032_DISPLAY_ON  0x0C

//============== CURSOR ===============
// === Page 23
/** Disable cursor */
#define CURSOR_OFF  0x00
/** Enables cursor */
#define CURSOR_ON   0x02
/** Disable Cursor Blink */
#define BLINK_OFF   0x00
/** Enable Cursor Blink */
#define BLINK_ON    0x01

//============== COMMANDS ===============
/** Line 1 of lcd */
#define ST7032_ADDRESS_LINE_1 0x00
/** Line 2 of lcd */
#define ST7032_ADDRESS_LINE_2 0x40
/** LCD Slave command */
#define ST7032_SLAVE          0x7C
/** LCD communicaiton send command */
#define ST7032_COMSEND        0x00
/** LCD Data to be sent */
#define ST7032_DATASEND       0x40

//============== FILE VARIABLES ===============
static const int8_t line_address[] = {ST7032_ADDRESS_LINE_1, ST7032_ADDRESS_LINE_2};

//============== PRIVATE FUNCTIONS ===============
/** Places a character at the current cursor on the LCD
 * param c -   character to write */
void lcd_putchar(const int8_t c);
/** This function sends a command to the LCD
 * param  Command - Command to go to the LCD */
void lcd_putcmd(uint8_t Command);

//============== FUNCTIONS ===============
void LCDM_Init(void) {
    // Configure peripheral registers
    I2CM_Init();

    // Send init commands
    I2CM_dataBuffer[0] = ST7032_SLAVE;
    I2CM_dataBuffer[1] = ST7032_COMSEND;
    I2CM_dataBuffer[2] = ST7032_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_1_LINE | FUNCTION_nIS;
    I2CM_dataBuffer[3] = ST7032_FUNCTION | FUNCTION_8BITS | FUNCTION_1_HIGH | FUNCTION_2_LINE | FUNCTION_IS;
    I2CM_dataBuffer[4] = ST7032_OSC_FREQ | BIAS_1_5 | FREQ_CNTRL(4);
    I2CM_dataBuffer[5] = ST7032_CONTRAST(10);
    I2CM_dataBuffer[6] = ST7032_PWR_CONTROL | nICON | BOOST | CONTRAST(2);
    I2CM_dataBuffer[7] = ST7032_FOLLOWER_ON | FOLLOWER_GAIN(4);
    I2CM_dataBuffer[8] = ST7032_ENTRY_MODE | CURSOR_nSHIFT | DATA_INCREMENT;
    I2CM_dataBuffer[9] = ST7032_DISPLAY_ON | CURSOR_OFF | BLINK_OFF;

    // Init the LCD with data from buffer
    I2CM_SendStart(10);
}
void lcd_goto(const uint8_t x, const uint8_t y) {
    lcd_putcmd(ST7032_DDRAM_ADDRESS(line_address[y] + x));
}
void lcd_putchar(const int8_t c) {
    uint8_t bufferPosition = 0;

    I2CM_dataBuffer[bufferPosition++] = ST7032_SLAVE;
    I2CM_dataBuffer[bufferPosition++] = ST7032_DATASEND;
    I2CM_dataBuffer[bufferPosition++] = c;

    I2CM_SendStart(bufferPosition);
}
void lcd_putstr(char *s, uint8_t length) {
    uint8_t bufferPosition = 0;

    I2CM_dataBuffer[bufferPosition++] = ST7032_SLAVE;
    I2CM_dataBuffer[bufferPosition++] = ST7032_DATASEND;

    // If length exceeds posible buffer length; do nothing
    if (length > I2CM_BUFFER_LEN - bufferPosition)
        return;

    // Build string in buffer till no more characters are left
    while (length > 0) // Write data to LCD up to null
    {
        I2CM_dataBuffer[bufferPosition++] = *s++;
        length--;
    }

    I2CM_SendStart(bufferPosition);
}
void lcd_putcmd(uint8_t Command) {
    uint8_t bufferPosition = 0;

    I2CM_dataBuffer[bufferPosition++] = ST7032_SLAVE;
    I2CM_dataBuffer[bufferPosition++] = ST7032_COMSEND;
    I2CM_dataBuffer[bufferPosition++] = Command;

    I2CM_SendStart(bufferPosition++);
}

