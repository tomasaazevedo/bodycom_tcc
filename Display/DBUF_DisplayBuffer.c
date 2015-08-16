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
 * Function for the Display Buffer
 */

//============== MODULE INCLUDES ===============
#include "DBUF_DisplayBuffer.h"

//============== DISPLAY INCLUDES ===============
// Display Communication Protocol
#include "I2CM.h"
// Display controller
#include "LCDM_St7032.h"

//============== DEFINES ===============
/** Display Buffer size */
#define DISPLAY_BUFFER_MAX_SIZE 32
/** Popup Buffer size */
#define POPUP_DISPLAY_BUFFER_MAX_SIZE 32

//============== FILE ENUMERATORS ===============
//typedef enum {
//    LCD_GOTO_LINE1_ORIGIN = 0,
//    LCD_UPDATE_LINE_1 = 1,
//    LCD_GOTO_LINE2_ORIGIN = 2,
//    LCD_UPDATE_LINE_2 = 3,
//    LCD_BUSY = 4,
//    LCD_DONE = 5
//} LCDM_State_t;
//
///** Current Display Buffer State */
//LCDM_State_t LCDM_State;

//============== GLOBAL VARIABLES ===============
DisplayType_t DisplayPopupState = NORMAL_DISPLAY;
uint8_t DisplayBuffer[DISPLAY_BUFFER_MAX_SIZE];
uint8_t DisplayPopupBuffer[POPUP_DISPLAY_BUFFER_MAX_SIZE];
uint8_t DisplayBufferIndex = 0;

//============== FUNCTIONS ===============
void DBUF_Printf(uint8_t* data) {
    // Tempory variable
    uint8_t rd = *data++;
    // Print data to buffer
    while (rd != 0) {
        DisplayBuffer[DisplayBufferIndex++] = rd;
        DisplayBufferIndex &= 0x1F;
        rd = *data++;
    }
}

void DBUF_PrintfDecrement(uint16_t value) {
    // Tempory Variable
    uint8_t count = 0;
    // Get count
    while (value >= 10000) {
        value -= 10000;
        count++;
    }
    // Display
    DisplayBuffer[DisplayBufferIndex++] = (uint8_t) (count + 0x30);
    // Clear for next check
    count = 0;
    // Get new count
    while (value >= 1000) {
        value -= 1000;
        count++;
    }
    // Display
    DisplayBuffer[DisplayBufferIndex++] = (uint8_t) (count + 0x30);
    // Clear for next check
    count = 0;
    // Get new count
    while (value >= 100) {
        value -= 100;
        count++;
    }
    // Display
    DisplayBuffer[DisplayBufferIndex++] = (uint8_t) (count + 0x30);
    // Clear for next check
    count = 0;
    // Get new count
    while (value >= 10) {
        value -= 10;
        count++;
    }
    // Display
    DisplayBuffer[DisplayBufferIndex++] = (uint8_t) (count + 0x30);
    // Clear for next check
    count = 0;
    // Last digit is already ones, just use it
    DisplayBuffer[DisplayBufferIndex++] = (uint8_t) (value + 0x30);
    // Clear for next check
    count = 0;
    // Update Index
    DisplayBufferIndex &= 0x1F;
}

void DBUF_GoTo(uint8_t position) {
    // Goto position
    DisplayBufferIndex = position;
}

void DBUF_Clear(ClearBuffer_t clear) {
    // Set the buffer to all blanks, that clears the LCD properly !!
    if (clear & 0x01)
        memset(&DisplayBuffer[0], ' ', 16);
    if (clear & 0x02)
        memset(&DisplayBuffer[16], ' ', 16);
}

void DBUF_RefreshNow(void) {
    // Ensure we update both lines and do not simply complete the current update !!
    LCDM_State = 0; 
    while (LCDM_State < LCD_DONE) {
        DBUF_Handler();
    }
}

void DBUF_Handler(void) {
    // Handle display communication updates
    I2CM_Handler();
    // Only continue if the display isn't idle or in error
    if (!(I2CM_state == I2CM_IDLE) && !(I2CM_state == I2CM_ERROR_NAK)) {
        return;
    }
    // Handle buffer to display
    switch (LCDM_State) {
        case LCD_GOTO_LINE1_ORIGIN:
            lcd_goto(0, 0);
            LCDM_State++; // Go to LCD_UPDATE_LINE_1;
            break;

        case LCD_UPDATE_LINE_1:
            // Popup or normal display on line 1
            if (DisplayPopupState & 0x01) { // Popup
                lcd_putstr(&DisplayPopupBuffer[0], 16);
            } else { //Normal
                lcd_putstr(&DisplayBuffer[0], 16);
            }
            LCDM_State++; // Go to LCD_GOTO_LINE2_ORIGIN;
            break;

        case LCD_GOTO_LINE2_ORIGIN:
            lcd_goto(0, 1);
            LCDM_State++; // Go to LCD_UPDATE_LINE_2;
            break;

        case LCD_UPDATE_LINE_2:
            // Same as above for line 2
            if (DisplayPopupState & 0x02) {
                lcd_putstr(&DisplayPopupBuffer[16], 16);
            } else {
                lcd_putstr(&DisplayBuffer[16], 16);
            }
            LCDM_State++; // Go to LCD_BUSY;
            break;

        case LCD_BUSY: // Wait for the update of line 2 to complete
            LCDM_State++; // Go to LCD_DONE;
            break;

        case LCD_DONE: // Idle/Done  - Use this state to wait for LCD to complete operation
            LCDM_State = LCD_GOTO_LINE1_ORIGIN;
            break;
    }
}

