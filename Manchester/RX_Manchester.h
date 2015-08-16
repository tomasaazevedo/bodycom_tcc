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
 * Function for Manchester Receive
 */

#ifndef __RX_MANCHESTER_H
#define __RX_MANCHESTER_H

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>

//============== ENUMERATORS ===============
/** Rx Manchester Receieve States */
typedef enum {
    MANR_IDLE = 0,
    MANR_RX_INIT = 1,
    MANR_SYNCH_WAIT_HIGH = 2,
    MANR_SYNCH_FALLING_EDGE = 3,
    MANR_SAMPLE_ONLY = 4,
    MANR_SAMPLE_EDGE_CHECK = 5,
    MANR_WAIT_HIGH_FALLING_EDGE = 6,
    MANR_WAIT_LOW_RAISING_EDGE = 7,
    MANR_WAIT_ONLY = 8,
    MANR_STOP_BIT = 9,
    MANR_FILTER_EDGE = 10,
    MANR_DATA_READY = 11,
    MANR_TIMEOUT = 12,
} Manchester_State_t;
/** Rx Manchester Receieve State */
extern Manchester_State_t RX_manchesterState;

//============== EXTERNAL VARIABLES ===============
/** Storage for incoming data */
extern uint8_t DecodePacketBuffer[32];

//============== PROTOTYPES ===============
/** Manchester bits receive main state machine called from sample timer interrupt */
void MANR_RxHandler(void);
/** Init the receiver and start the sample timer */
uint8_t RX_ManchesterRxReceiverStart(void);
/** Enable Interrupt-on-Change detect for Rx Manchester */
void MANR_HardwareEnableRXIoc(void);
/** Disable Interrupt-on-Change detect for Rx Manchester */
void MANR_HardwareDisableRXIoc(void);
/** Manchester Receieve ISR */
void MANR_ISR(void);

#endif  // __RX_MANCHESTER_H