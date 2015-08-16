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
 * Function for Button Operation
 */

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//============== MODULE INCLUDES ===============
#include "BN_ButtonDetection.h"
//============== STATIC VARIABLES ===============
/**
 * @static CurrentButtonEvent
 * @brief  Variable to store current button event
 */
Buttons_Event CurrentButtonEvent = No_Event;
//============== FUNCTIONS ===============
void BTN_Init(void) {
    TRISA4 = 1;
    ANSA4 = 1;
    CurrentButtonEvent = No_Event;
}

void BTN_Update(void) {
    //Function Variables
    static Buttons_t Key_PreviousValue = NONE;
    static uint8_t DebounceTimer = BUTTON_DEBOUNCE_TIME;

    Buttons_t Key_CurrentValue = NONE;

    Key_CurrentValue = BTN_Check();

    //Check if current pressed key matchs previous pressed key.
    // If match, debounce. If different reload debounce.
    if (Key_CurrentValue == NONE) {
        DebounceTimer = BUTTON_DEBOUNCE_TIME;
    } else if (Key_CurrentValue != Key_PreviousValue) {
        DebounceTimer = BUTTON_DEBOUNCE_TIME;
    } else {
        if (DebounceTimer) {
            DebounceTimer--;
            if (DebounceTimer == 0) {
                // Store current debounced press event.
                switch (Key_CurrentValue) {
                    case S2:
                        CurrentButtonEvent = S2_Event;
                        break;
                    case S3:
                        CurrentButtonEvent = S3_Event;
                        break;
                    case S4:
                        CurrentButtonEvent = S4_Event;
                        break;
                    default:
                        CurrentButtonEvent = No_Event;
                        break;
                }
            }
        }
    }
    // Store current press into previous press for future reference.
    Key_PreviousValue = Key_CurrentValue;
}

Buttons_Event BTN_Get(void) {
    // Tempory variable
    Buttons_Event Ret = No_Event;
    // Button press
    Ret = CurrentButtonEvent;
    // Clear
    CurrentButtonEvent = No_Event;
    // return
    return Ret;
}

Buttons_t BTN_Check(void) {
    // Tempory Variable
    uint16_t buttonValue = 0;

    // select AN3 for buttons ************ CONFIG HOOK FOR THIS
    GIE = 0;    // Disable for stability
    ADCON0 = 0b00001101;
    ADCON1 = 0b01110000;
    GO_nDONE = 1;
    while (GO_nDONE);
    // Get value
    buttonValue = ADRESH;
    GIE = 1;
    // Detect button
    if (buttonValue < 50) {
        buttonValue = S2;
    } else if (buttonValue < 150) {
        buttonValue = S3;
    } else if (buttonValue < 200) {
        buttonValue = S4;
    } else buttonValue = NONE;
    // Return button
    return (Buttons_t) buttonValue;
}
