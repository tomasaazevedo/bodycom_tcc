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
 * Functions for touch/proximity detection support routines
 */

//============== MODULE INCLUDES ===============
#include "TD_CvdHandler.h"

//***********************************************
//============== SETUP DEFINES ==================
//***********************************************
//============== I/O DEFINES ===============
/** LAT register of sensor */
#define CVD_SENSOR_PORT                 LATC
/** TRIS register of sensor */
#define CVD_SENSOR_TRIS                 TRISC
/**  Sensor's port pin */
#define CVD_SENSOR_PIN                  2

//============== REGISTER DEFINES ===============
/** ADCON1 value to initialize the ADC for mTouch */
#define CVD_ADCON1_VALUE                0b10010000

//============== DEFINES ===============
/** ADCON0 value to point to sensor */
#define CVD_CURRENT_CHANNEL             0x19
/** ADCON0 value to point to an unimplemented channel and NOT set the Go/Done bit */
#define CVD_SELECT_ISO_NOGO             0x41
/** Number of times to oversample the sensor */
#define CVD_SAMPLES_PER_SCAN            10
/** Number of initialization scans before sensor becomes active */
#define CVD_POWER_UP_SAMPLES            50
/** Number of consecutive 'pressed' scans before forcing a released state change */
#define CVD_PRESS_TIMEOUT               150 //50
/** Number of counts below the average the reading must fall to trigger a press */
#define CVD_PRESS_THRESHOLD             15 //30
/** Multiplied by CVD_PRESS_THRESHOLD to determine the release threshold (100 * 0.5 = 50) */
#define CVD_RELEASE_FACTOR              0.5
/** Average will update only every Nth sample */
#define CVD_AVG_RATE                    4
/** Number of times to debounce the state transition from RELEASE to PRESS */
#define CVD_DEBOUNCE_PRESS              2
/** Number of times to debounce the state transition from PRESS to RELEASE */
#define CVD_DEBOUNCE_RELEASE            5
/**  Number used for calculations */
#define CVD_DEFAULT_DECIMATION_VALUE    0x758
/**  Number used for calculations */
#define CVD_DEFAULT_JITTER_VALUE        0x55
/** Trivial abstraction macro */
#define RAW                             CvdSensorData
/** Trivial abstraction macro */
#define AVERAGE                         CvdAverage

//============== LOCAL CALCULATION MARCOS ===============
/** Used for CVD Dynamic calculation/setup */
#define     MASKBANK(var,bank)              ((var)^((bank)*80h))
/**  Used for CVD Dynamic calculation/setup */
#define     __paste1(a,b)                   a##b
/**  Used for CVD Dynamic calculation/setup */
#define     __paste(a,b)                    __paste1(a,b)
/**  Calculates the value (threshold) used to trigger a cvd press/release */
#define     CVD_RELEASE_THRESHOLD           (uint16_t)((float)CVD_PRESS_THRESHOLD * CVD_RELEASE_FACTOR)

//============== GLOBAL VARIABLES ===============
CvdState CvdSensorState = CVD_INITIALIZING;
bit CvdEvent = 0;
uint16_t CvdSensorData = 0;
uint32_t CvdAverage = 0;

//============== FILE VARIABLES ===============
/** CVD accumulator value used for senseing */
static uint32_t CvdAccumulator = 0;
/** Stores ADC results for differential calculation */
static uint16_t CvdResult = 0;
/** Decimate-filtered differential ADC result register */
static uint16_t CvdDecimatedResult = CVD_DEFAULT_DECIMATION_VALUE;
/**Stores the current random seed value for jittering */
static const uint8_t CvdJitter = CVD_DEFAULT_JITTER_VALUE;
/** Number of average values counted */
static uint8_t CvdAverageCount = 0;
/** Defined Threshold trigger value */
static const uint8_t CvdPressThreshold = CVD_PRESS_THRESHOLD;
/** Defined Threshold release value */
static const uint8_t CvdReleaseThreshold = (uint8_t) (CVD_RELEASE_THRESHOLD & 0xFF);
/**  How long a Press was detected */
static uint8_t CvdPressTimer = 0;
/** Debounce value to prevent false trigger */
static uint8_t CvdDebounce = 0;

//============== FUNCTIONS ===============
void TD_CvdInit(void) {
    // Weak pull-up on I/O
    WPUC2 = 0;
    // Cvd Setup
    CvdAccumulator = 0;
    CvdResult = 0;
    CvdSensorData = 0;
    CvdSensorState = CVD_INITIALIZING;
    CvdAverage = 0;
    CvdAverageCount = 0;
    CvdPressTimer = CVD_POWER_UP_SAMPLES;
    CvdDebounce = 0;
    CvdEvent = 0;
}

void TD_CvdScan(void) {
    // Tempory variable
    int16_t delta;
    // Reset for new scan
    CvdResult = 0;
    CvdAccumulator = 0;
    // Prepare ADC
    ADCON1 = CVD_ADCON1_VALUE; // right justified, uses FVR as reference, Tad = Fosc/64
    // Disable interrupts for scan
    GIE = 0;
    //  Jitter the Sampling Rate
    for (uint8_t i = 0; i < CVD_SAMPLES_PER_SCAN; i++) {
        //
        //  Jitter the Sampling Rate
        //      then update CVD_jitter with the next pseudorandom value
        //
        for (uint8_t jitter = CvdJitter; jitter <= 0; jitter--) {
            NOP();
        }
#define NOBANK(var)  (var & 0x7F)

        asm("BANKSEL    _CvdJitter"); /* Make sure we're starting in the correct bank     */
        asm("bcf    " ___mkstr(STATUS) ",0"); /* Clear the carry bit                              */
        asm("rrf    " ___mkstr(NOBANK(_CvdJitter)) ",W"); /* Right shift the current jitter seed value        */
        asm("btfsc  " ___mkstr(STATUS) ",0"); /* Check the carry bit - if set, perform XOR        */
        asm("xorlw      0xB4"); /* (cond) XOR the jitter seed with 0xB4             */
        asm("movwf  " ___mkstr(NOBANK(_CvdJitter))); /* Store the result as the new jitter seed value    */
        //
        //  CVD Scan A
        //
        asm("movlb    1"); /* Make sure we start off in the correct bank                       */
        asm("movlw " ___mkstr(CVD_CURRENT_CHANNEL)); /* Load the sensor's ADCON0 value and set the GO/DONE bit           */
        asm("movwf " ___mkstr(MASKBANK(_ADCON0, 1))); /* Write the value to the ADCON0 register                           */
        asm("bsf   " ___mkstr(MASKBANK(_ADCON0, 1)) ", 1"); /* Set the GO/DONE bit                                              */
        asm("movlw " ___mkstr(CVD_SELECT_ISO_NOGO)); /* Load an ADCON0 value that will point to an unimplemented channel */
        asm("movwf " ___mkstr(MASKBANK(_ADCON0, 1))); /* Write the value to the ADCON0 register                           */
        asm("movlb    2"); /*                                                                  */
        asm("bsf   " ___mkstr(MASKBANK(CVD_SENSOR_PORT, 2)) "," ___mkstr(CVD_SENSOR_PIN)); /* SETTING THE SENSOR PIN */
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("movlb    1"); /*                                                                  */
        asm("movlw " ___mkstr(CVD_CURRENT_CHANNEL)); /* Load the sensor's ADCON0 value (do not set GO/DONE)              */
        asm("bsf   " ___mkstr(MASKBANK(CVD_SENSOR_TRIS, 1)) "," ___mkstr(CVD_SENSOR_PIN));
        asm("movwf " ___mkstr(MASKBANK(_ADCON0, 1))); /* Write the sensor's ADCON0 value to ADCON0                        */
        asm("nop"); /*                                                                  */
        asm("nop"); /* Delay while capacitance voltages settle                          */
        asm("nop"); /*                                                                  */
        asm("bsf   " ___mkstr(MASKBANK(_ADCON0, 1)) ", 1"); /* Begin the conversion by setting the GO/DONE bit                  */
        asm("bcf   " ___mkstr(MASKBANK(CVD_SENSOR_TRIS, 1)) "," ___mkstr(CVD_SENSOR_PIN));
        // Do conversion
        while (GO_nDONE);
        // Capture result
        CvdResult = (uint16_t) (((ADRESH | 0x04) << 8) + ADRESL);
        //
        //  CVD Scan B
        //
        asm("movlb    1"); /* Make sure we start off in the correct bank                       */
        asm("movlw " ___mkstr(CVD_CURRENT_CHANNEL)); /* Load the sensor's ADCON0 value and set the GO/DONE bit           */
        asm("movwf " ___mkstr(MASKBANK(_ADCON0, 1))); /* Write the value to the ADCON0 register                           */
        asm("bsf   " ___mkstr(MASKBANK(_ADCON0, 1)) ", 1"); /* Set the GO/DONE bit                                              */
        asm("movlw " ___mkstr(CVD_SELECT_ISO_NOGO)); /* Load an ADCON0 value that will point to an unimplemented channel */
        asm("movwf " ___mkstr(MASKBANK(_ADCON0, 1))); /* Write the value to the ADCON0 register                           */
        asm("movlb    2"); /*                                                                  */
        asm("bcf   " ___mkstr(MASKBANK(CVD_SENSOR_PORT, 2)) "," ___mkstr(CVD_SENSOR_PIN)); /* CLEARING THE SENSOR PIN */
        asm("movlb    1"); /*                                                                  */
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("movlw " ___mkstr(CVD_CURRENT_CHANNEL)); /* Load the sensor's ADCON0 value (do not set GO/DONE)              */
        asm("bsf   " ___mkstr(MASKBANK(CVD_SENSOR_TRIS, 1)) "," ___mkstr(CVD_SENSOR_PIN));
        asm("movwf " ___mkstr(MASKBANK(_ADCON0, 1))); /* Write the sensor's ADCON0 value to ADCON0                        */
        asm("nop"); /*                                                                  */
        asm("nop"); /* Delay while capacitance voltages settle                          */
        asm("nop"); /*                                                                  */
        asm("bsf   " ___mkstr(MASKBANK(_ADCON0, 1)) ", 1"); /* Begin the conversion by setting the GO/DONE bit                  */
        asm("bcf   " ___mkstr(MASKBANK(CVD_SENSOR_TRIS, 1)) "," ___mkstr(CVD_SENSOR_PIN));
        // Do conversion
        while (GO_nDONE) CLRWDT();
        // Compare result
        CvdResult -= (uint16_t) ((ADRESH << 8) + ADRESL);
        //
        //  Run the CVD Result through the Decimation Filter
        //
        if (CvdResult > CvdDecimatedResult) {
            CvdDecimatedResult++;
        } else if (CvdResult < CvdDecimatedResult) {
            CvdDecimatedResult--;
        }
        //
        //  Accumulate the CVD Results
        //
        CvdAccumulator += CvdDecimatedResult;
    }
    // Get RAW value
    RAW = (uint16_t) (CvdAccumulator >> 4);
    //
    //  CVD Detection Algorithm
    //
    delta = RAW - (AVERAGE >> CVD_AVG_WEIGHT);
    if (delta < 0) // If a negative shift is detected, clear delta value.
    {
        delta = 0; // Shifts should increase.
        // Setting to 0 helps to prevent triggering
        // on negative spikes.
    }
    // Begin state machine
    switch (CvdSensorState) 
    {
        // Sensor is warming up and still performing initialization scans
        case CVD_INITIALIZING: 
        {
            if (--CvdPressTimer == 0) // If initialization delay has elapsed
            {
                // State Transition -> Released
                CvdSensorState = CVD_RELEASED; // Sensor is now released and stable
                CvdDebounce = 0; // Initialize sensor's debounce counter
            }
            AVERAGE = RAW << CVD_AVG_WEIGHT;
            break;
        }

        // Sensor is initialized and released
        case CVD_RELEASED: 
        {
            if (delta > CvdPressThreshold) // If reading has crossed the press threshold
            {
                if (++CvdDebounce > CVD_DEBOUNCE_PRESS) // Increment the debounce counter
                {                                       // and check if it has crossed its limit
                    // State Transition -> Pressed
                    CvdSensorState = CVD_PRESSED; // Sensor is now pressed
                    CvdDebounce = CVD_DEBOUNCE_RELEASE; // Initialize the pressed state's debounce variable
                    CvdPressTimer = CVD_PRESS_TIMEOUT; // Initialize the press timer counter
                }
            } else // If the reading has not crossed the press threshold
            {
                // Reset the debounce variable
                CvdDebounce = 0; 
                if (delta == 0) {
                    AVERAGE = RAW << CVD_AVG_WEIGHT;
                } else {
                    if (++CvdAverageCount >= CVD_AVG_RATE) // If the average counter has been exceeded
                    {
                        CvdAverageCount = 0; // Reset the average counter
                        AVERAGE -= AVERAGE >> CVD_AVG_WEIGHT; // Update the average
                        AVERAGE += (uint16_t) (RAW);
                    }
                }
            }
            break;
        }
        // Sensor is initialized and pressed
        case CVD_PRESSED: 
        {
            if (delta < CvdReleaseThreshold) // If the reading has crossed the release threshold
            {
                if (--CvdDebounce == 0) // Decrement the debounce counter
                { // and check if it has reached 0.
                    CvdSensorState = CVD_RELEASED; // Sensor is now released
                }
            } else // If the reading has not crossed the release threshold
            {

                if (--CvdPressTimer == 0) // If pressed delay has elapsed
                {
                    // State Transition -> Released
                    // Re-initialize Sensor
                    asm("NOP");
                    asm("NOP");
                    CvdSensorState = CVD_INITIALIZING;
                    CvdPressTimer = CVD_POWER_UP_SAMPLES;

                }

                CvdDebounce = CVD_DEBOUNCE_RELEASE; // Reset the debounce counter
            }
            break;
        }

        default: break; // Error
    }
    // Enable Interrupt
    GIE = 1;
    // Update State
    if (CvdSensorState == CVD_PRESSED) {
        CvdEvent = 1;
    } else {
        CvdEvent =0;
    }
}





