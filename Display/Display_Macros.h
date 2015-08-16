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
 * Display macros used in application
 */

#ifndef __DISPLAY_MACROS_H
#define __DISPLAY_MACROS_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <string.h>

//============== DISPLAY MACROS ===============
/** Application Splash Screen */
#define Display_Splash                                                  \
memcpy(&DisplayBuffer[0], "  BodyCom Eval  ""  Board: V 0.0  ", 32);    \
DisplayBuffer[0x1B] = SW_VERSION1 + 0x30;                               \
DisplayBuffer[0x1D] = SW_VERSION2 + 0x30;
/** Touch based Application example */
#define Display_EvaluationTouch                                 \
memcpy(DisplayBuffer, " BodyCom Touch   Wait for Touch ", 32);
/** Int CVD symbol */
#define Display_CvdInitilizing   DisplayBuffer[15] = '#';
/** Pressed CVD symbol */
#define Display_CvdPressed  DisplayBuffer[15] = '^';
/** Released CVD symbol */
#define Display_CvdReleased DisplayBuffer[15] = '_';
/** Clear CVD symbol */
#define Display_CvdSpace    DisplayBuffer[15] = ' ';
/** Button based Application example */
#define Display_EvaluationProximity                                 \
memcpy(DisplayBuffer, " BodyCom - Prox S4 to Begin Scan", 32);
/** Debug menu */
#define Display_DebugMenu                                \
memcpy(DisplayBuffer, "   Debug Menu   S4 to Enter/Exit", 32);
/** RSSI values */
#define Display_RSSI                               \
memcpy(DisplayBuffer, "Receive Strength", 16);  \
    DBUF_Clear(2);  \
    DBUF_GoTo(16);  \
    DBUF_PrintfDecrement((uint16_t) (ADRESL + (ADRESH << 8)));  \
/** Receieve packet info */
#define Display_RxPacket                                \
memcpy(DisplayBuffer, " Receive Packet ", 16);  \
    DBUF_Clear(2);  \
    DBUF_GoTo(16);  
/** Continuous Wave splash */
#define Display_ContinuousWave                               \
memcpy(DisplayBuffer, "    Transmit     Continuous Wave", 32);
/** Loopback splash */
#define Display_LoopBack                            \
memcpy(DisplayBuffer, " Trans Loopback S4 to Start/Stop", 32);
/** Touch values */
#define Display_Touch                            \
memcpy(DisplayBuffer, " Touch Operation", 16);  \
        DBUF_GoTo(16);  \
        DBUF_Printf((uint8_t*) "R:00000 A:00000 "); \
        DBUF_GoTo(18);  \
        DBUF_PrintfDecrement(CvdSensorData);    \
        DBUF_GoTo(26);  \
        DBUF_PrintfDecrement((uint16_t) (CvdAverage >> CVD_AVG_WEIGHT));
/** Paired list splash */
#define Display_PairedListMenu                            \
memcpy(DisplayBuffer, "Valid Mobile IDsS4 to Enter/Exit", 32);
/** Control from GUI splash */
#define Display_GuiControl                           \
memcpy(DisplayBuffer, "Control from GUIAddress Mode:GUI", 32);
/** Device ID index display */
#define Display_PairedList(index)  \
    memcpy(DisplayBuffer, "Select TAG:00/00", 16);  \
    DisplayBuffer[0x0C] = (uint8_t) (index + 0x31); \
    DisplayBuffer[0x0F] = (uint8_t) (PairedCount + 0x30);   \
    DBUF_Clear(2);  \
    DBUF_GoTo(16);  \
    DBUF_Printf((uint8_t*) "ID:");  \
    DBUF_GoTo(20);

#endif      //__DISPLAY_MACROS_H

