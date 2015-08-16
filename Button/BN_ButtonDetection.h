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

#ifndef __BUTTONS_H
#define __BUTTONS_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//============== MODULE INCLUDES ===============

//============== DEFINES ===============
/** Number of times to check valid press states for debouncing */
#define BUTTON_DEBOUNCE_TIME    10
/** S2 Button (Up) press is detected */
#define SCROLL_UP       button_press == S2
/** S3 Button (Down) press is detected */
#define SCROLL_DOWN     button_press == S3
/** S4 Button (Select) press is detected */
#define SCROLL_SELECT   button_press  == S4

//============== ENUMBERATORS ===============
/** Stores current button event */
typedef enum {
    No_Event = 0,
    S2_Event = 1,
    S3_Event = 2,
    S4_Event = 3
} Buttons_Event;
/** Possible Button events */
typedef enum {
    NONE,
    S2,
    S3,
    S4
} Buttons_t;
/** Button "Naming" based on Buttons_Event */
typedef enum {
    None = 0,
    Up = 1,
    Down = 2,
    Select = 3
} Button_Action;

//============== PROTOTYPES ===============
/** This function is used to Initialize the Push Buttons */
void BTN_Init(void);
/** This function is used to update current button press */
void BTN_Update(void);
/** This function is used to return which button was pressed
 *  return - Which button was pressed */
Buttons_Event BTN_Get(void);
/** This function is used to debounce pressed button
 *  return - Status of buttons */
Buttons_t BTN_Check(void);

#endif  // __BUTTONS_H