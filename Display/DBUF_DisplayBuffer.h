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

#ifndef __DBUF_DISPLAYBUFFER_H
#define __DBUF_DISPLAYBUFFER_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <string.h>

//============== ENUMERATOR ===============
/** Buffer Display Type */
typedef enum {
    NORMAL_DISPLAY = 0,
    POPUP_LINE_1 = 1,
    POPUP_LINE_2 = 2,
    POPUP_BOTH = 3,
} DisplayType_t;
/** Popup or normal display */
extern DisplayType_t DisplayPopupState;
/** Options to clear buffer */
typedef enum {
    CLEAR_NOTHING = 0,
    CLEAR_TOP = 1,
    CLEAR_BUTTOM = 2,
    CLEAR_ALL = 3,
} ClearBuffer_t;

//============== EXTERNAL VARIABLES ===============
/** Display Buffer; DISPLAY_BUFFER_MAX_SIZE bytes */
extern uint8_t DisplayBuffer[];
/** Popup Display Buffer; POPUP_DISPLAY_BUFFER_MAX_SIZE bytes */
extern uint8_t DisplayPopupBuffer[];
/** Buffer Index for LCD display */
extern uint8_t DisplayBufferIndex;

//============== PROTOTYPES ===============
/** Print buffer to display
 *  param  data -  Information to print on display */
void DBUF_Printf(uint8_t* data);
/** Print Variable Value
 *  param  data - Information decremented for display */
void DBUF_PrintfDecrement(uint16_t value);
/** Goto a buffer position
 *  param  position - Position in buffer  */
void DBUF_GoTo(uint8_t position);
/** Clear buffer
 *  param clear - None-0, Top-1, Bottom-2, All-3  */
void DBUF_Clear(ClearBuffer_t clear);
/** This function immediately updates the display in a BLOCKING fashion  */
void DBUF_RefreshNow(void);
/** This function immediately updates the display in a PASSIVE fashion; Update when it can */
void DBUF_Handler(void);

#endif   //__DBUF_DISPLAYBUFFER_H
  

