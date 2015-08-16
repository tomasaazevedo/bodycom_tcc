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

//============== COMPILER INCLUDES ===============
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//============== MODULE INCLUDES ===============
#include "RX_Manchester.h"

//============== DEBUG HELPER  ===============
// Debug LAT must be defined for use.....
//#define MAN_RX_DEBUGPIN

//**********************************************
//============== APPLICATION DEFINES ==========
//**********************************************
//============== I/O  ===============

//******************************
//============== BASE ==========
//******************************
#ifdef BASE
/** RX Port pin */
#define ManchesterPinRead        RB5
/** RX Port pin (-) IOC*/
#define BC_RX_IOC_N       IOCBNbits.IOCBN5
/** RX Port pin (+) IOC*/
#define BC_RX_IOC_P       IOCBPbits.IOCBP5
/** RX Port pin IOC Flag*/
#define BC_RX_IOC_IF      IOCBFbits.IOCBF5
/** For this we have Prescalar = Tosc x 16 x PR4 = 1.953 us * PR4 */
#define MANR_SAMPLING_PERIOD     26

#elif MOBILE
//******************************
//============== MOBILE ==========
//******************************
/** RX Port pin */
#define ManchesterPinRead        RB4
/** RX Port pin (-) IOC*/
#define BC_RX_IOC_N       IOCBNbits.IOCBN4
/** RX Port pin (+) IOC*/
#define BC_RX_IOC_P       IOCBPbits.IOCBP4
/** RX Port pin IOC Flag*/
#define BC_RX_IOC_IF      IOCBFbits.IOCBF4
/** For this we have Prescalar = Tosc x 16 x PR4 = 1.953 us * PR4 */
#define MANR_SAMPLING_PERIOD     25
#endif

//**********************************************
//============== COMMON INCLUDES ==========
//**********************************************
//============== DATA  ===============
/** Byte Size */
#define RX_BYTE_SIZE    8
/** Byte Size */
#define RX_MAX_PACKET_SIZE 23

//============== TIMER DEFINES ===============
/** Manchester Receive Timer Register */
#define MANR_RX_TIMER T4CON
/** Manchester Receive Timer Configuration */
// Prescaler 1:1,  Postscaler 1:4
#define MANR_TIMER_CONFIG 0b00011000
/** Manchester Receive Timer Period register*/
#define MANR_RX_PERIOD PR4
/** Manchester Receive Timer Interrupt bit*/
#define MANR_RX_INTERUPT TMR4IE
/** Manchester Receive Timer Enable bit */
#define MANR_RX_TIMER_ON    TMR4ON

//============== TIMER THRESHOLDS===============
/** Edge */
#define TH1  4   
/** sample buffer */
#define TH2  4    
/** sample full; Baud Rate, must be half number of samples */
#define TH3  10   
/**  Edge detect timeout */
#define TH4  9   

//============== GLOBAL VARIABLES ===============
Manchester_State_t RX_manchesterState = MANR_IDLE;
uint8_t DecodePacketBuffer[32] = 0;

//============== FILE VARIABLES ===============
/** Manchester Transmit Byte Handler State */
static uint8_t RX_byteCount = 0;

//============== FUNCTIONS ===============
void MANR_RxHandler(void) {
    // Private Variables
    static uint8_t SampleCount = 0;
    static int8_t BitValue = 0;
    static uint8_t HighSample = 0;
    static uint8_t LowSample = 0;
    static uint8_t TempData = 0;
    static uint8_t BitCount = 0;

    switch (RX_manchesterState) {
            // Idle
        case MANR_IDLE:
            RX_manchesterState = MANR_RX_INIT;
            break;

            // Prepare for new bit (wait for start bit starts here)
        case MANR_RX_INIT:
            SampleCount = 0;
            BitValue = 0;
            HighSample = 0;
            LowSample = 0;
            TempData = 0;
            BitCount = 0;
            RX_manchesterState = MANR_SYNCH_WAIT_HIGH;
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       
            MAN_RX_DEBUGPIN = 1;
#endif                       
//=============================
            break;

        case MANR_SYNCH_WAIT_HIGH:
            SampleCount++;
            // Signal was high; change state to wait for falling
            if (ManchesterPinRead) {
                RX_manchesterState = MANR_SYNCH_FALLING_EDGE;
                SampleCount = 0;
            }
            // This returns to idle after the reception is ended due to no edge detected for 5 bits (10 half bits)
            if (SampleCount > TH3 * 10) {
                // Timeout on falling edge, go back and wait for next rising edge
                RX_manchesterState = MANR_TIMEOUT;
            }
            break;

            // Sync to start bit falling edge
        case MANR_SYNCH_FALLING_EDGE:
            SampleCount++;
            if (!ManchesterPinRead) {
                LowSample++;
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       
                MAN_RX_DEBUGPIN = 1;
#endif                       
//=============================
            }
            // Look for low signal
            if (LowSample >= TH1) {
                SampleCount = LowSample;
                LowSample = 0;
                RX_manchesterState = MANR_WAIT_ONLY;
            }
            // This returns to idle after the reception is ended due to no edge detected for 5 bits (10 half bits)
            if (SampleCount > TH3 * 10) {
                // Timeout on falling edge, go back and wait for next rising edge
                RX_manchesterState = MANR_TIMEOUT;
            }
            break;

            // Sample only
        case MANR_SAMPLE_ONLY: 
            SampleCount++;
            // See if bit value is +(high) or -(low)
            if (ManchesterPinRead) {
                BitValue++;
            } else {
                BitValue--;
            }
            // End sample limit
            if (SampleCount >= TH2) {
                // Prepare for new samples
                RX_manchesterState = MANR_SAMPLE_EDGE_CHECK;
                LowSample = 0;
                HighSample = 0;
            }
            break;

            // Sample + edge detector
        case MANR_SAMPLE_EDGE_CHECK: 
            SampleCount++;
            // See if bit value is +(high) or -(low)
            if (ManchesterPinRead) {
                HighSample++;
                BitValue++;
            } else {
                LowSample++;
                BitValue--;
            }
            // Required samples reached; decide if bit is high or low
            if (SampleCount >= TH3) {
                SampleCount = 1;
                TempData |= (BitValue > 0 ? 0 : (1 << BitCount));
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       
                MAN_RX_DEBUGPIN = 1;
#endif                      
//=============================
                // See if Bit limit for Byte is ready
                if (++BitCount >= RX_BYTE_SIZE) {
                    // Store byte,
                    DecodePacketBuffer[RX_byteCount++] = TempData; 
                    // Wait for Stop bit
                    HighSample = 0;
                    RX_manchesterState = MANR_STOP_BIT; 
                    break;
                }
                // if zero
                if (BitValue > 0) 
                {
                    // High waiting for low edge
                    RX_manchesterState = MANR_WAIT_HIGH_FALLING_EDGE; 
                    HighSample = 0;
                } else { // it is a one
                    // Low waiting for raising edge
                    RX_manchesterState = MANR_WAIT_LOW_RAISING_EDGE; 
                    LowSample = 0;
                }
            }
            break;

             // Wait for falling edge
        case MANR_WAIT_HIGH_FALLING_EDGE:
            // Check if pin is low
            if (!ManchesterPinRead) {
                LowSample++;
            }
            // Edge timeout
            if (SampleCount++ >= TH4) 
            {
                RX_manchesterState = MANR_TIMEOUT;
            }
            // Edge detected sample count reached
            if (LowSample >= TH1) {
                SampleCount = LowSample;
                LowSample = 0;
                RX_manchesterState = MANR_WAIT_ONLY;
            }
            break;

            //Wait for raising edge
        case MANR_WAIT_LOW_RAISING_EDGE: 
            // Check if pin is high
            if (ManchesterPinRead) {
                HighSample++;
            }
            // Edge timeout
            if (SampleCount++ >= TH4) 
            {
                RX_manchesterState = MANR_TIMEOUT;
            }
            // Edge detected sample count reached
            if ((HighSample) >= TH1) {
                SampleCount = HighSample;
                HighSample = 0;
                RX_manchesterState = MANR_WAIT_ONLY;
            }
            break;

            // Wait for sample only
        case MANR_WAIT_ONLY: 
            if (++SampleCount >= TH3) {
                BitValue = 0;
                SampleCount = 1;
                RX_manchesterState = MANR_SAMPLE_ONLY;
            }
            break;

            // Bit tail end plus half stop bit
        case MANR_STOP_BIT:
            // End of stop bit
            if (SampleCount++ >= (TH3 + TH3 / 2)) 
            {
                SampleCount = 0;
                // Wait for next start bit
                RX_manchesterState = MANR_FILTER_EDGE; 
//========== DEBUG HELPER =====
#ifdef MAN_RX_DEBUGPIN       
                MAN_RX_DEBUGPIN = 1;
#endif                      
//=============================
            }
            break;

            // Wait for filter edge
        case MANR_FILTER_EDGE: 
            if (ManchesterPinRead) {
                HighSample++;
            }
            // Edge timeout
            if (SampleCount++ >= TH3 + 1) 
            {
                HighSample = TH1;
            }
            // All data is received
            if (HighSample >= TH1) {                
                if (RX_byteCount >= RX_MAX_PACKET_SIZE) {
                    RX_manchesterState = MANR_DATA_READY;
                    // Slow timer while we are idle ...
                    MANR_RX_PERIOD = 255;
                } else {
                    // Go back to init, that receives the next byte
                    RX_manchesterState = MANR_RX_INIT;
                }
            }
            break;

            // Complete
        case MANR_TIMEOUT:
        case MANR_DATA_READY:
            RX_byteCount = 0;
            break;

        default: // Error
            break;
    }
}

uint8_t RX_ManchesterRxReceiverStart(void) {
    // Get ready to receieve
    RX_manchesterState = MANR_RX_INIT;
    MANR_RX_TIMER = MANR_TIMER_CONFIG;
    MANR_RX_PERIOD = MANR_SAMPLING_PERIOD;
    RX_byteCount = 0;
    MANR_RX_INTERUPT = 1;
    MANR_RX_TIMER_ON = 1;
    // Success
    return 1;
}

void MANR_HardwareEnableRXIoc(void) {
    //Prepare to trigger on IOC
    BC_RX_IOC_N = 0;
    BC_RX_IOC_P = 1;
    BC_RX_IOC_IF = 0; 
    RX_manchesterState = MANR_IDLE;
}

void MANR_HardwareDisableRXIoc(void) {
    // Disable IOC
    BC_RX_IOC_N = 0;
    BC_RX_IOC_P = 0;
    BC_RX_IOC_IF = 0; 
}

void MANR_ISR(void) {
    // Manchester Receive I/O for start bit from AFE on LFDATA pin
    if (BC_RX_IOC_IF && IOCIE) {
        // Clear Flag
        BC_RX_IOC_IF = 0;
        // IOC is only used for waking up, when the receiver timer is started it can be disabled.
        MANR_HardwareDisableRXIoc();
        // Start RX sampling Timer on fast timer mode
        RX_ManchesterRxReceiverStart();
    }
}
