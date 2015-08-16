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
 * Function for development board BodyCom application
 */

//============== MODULE INCLUDES ===============
// BodyCom Development Board Application
#include "BC_Application.h"
// BodyCom Application Paired List
#include "PL_PairedList.h"
// AFE
#include "AFE.h"
// LCD Display control
#include "LCDM_St7032.h"
// Display Printer
#include "DBUF_DisplayBuffer.h"
// Displays
#include "Display_Macros.h"
// Cvd Interaction
#include "TD_CvdHandler.h"
// Buttons Interaction
#include "BN_ButtonDetection.h"
// PC Interaction
#include "PC_Handler.h"

//============== DEFINES ===============
/** Control pin for the BodyCom receiver enable */
#define BC_ENABLERX_TRIS    TRISC7
/** ON, Internal, 50% Duty, divide by 64 */
#define BC_CLKRCON_SET	0b10010110
/** Enable/Disable bit */
#define BC_CLKREN	CLKREN
/** FVR enable din High Range; use 2.048 V as Reference */
#define BC_FVRCON_SET   0x94
/** Positive input from FVR, Negative From C12IN3- [RC3 - MODOUT pin] */
#define BC_CM1CON1_SET 	0x23
/** Comparitor enabled in High power mode with hysteresis */
#define BC_CM1CON0_SET 	0x86
/** Modulation occurs on Reference Clock [High Carrier] */
#define BC_MDCON_SET 	0xC1
/** CARH [High Carrier] set to CLKR */
#define BC_MDCARH_SET   0x83
/** CARL (Low Carrier) set to Vss */
#define BC_MDCARL_SET 	0x80
#define BC_ENABLERX_OUT     LATC7

// *****************************************************************************
//          Manchester Receive and Transmit Config                            **
// *****************************************************************************
// IOC I/O
#define BC_RX_IOC_N       IOCBNbits.IOCBN5
#define BC_RX_IOC_P       IOCBPbits.IOCBP5
#define BC_RX_IOC_IF      IOCBFbits.IOCBF5
// I/O Setup
#define BC_SERIAL_TRIS      TRISC6
#define BC_MODOUT_TRIS      TRISC3
#define BC_MODOUT_N_TRIS    TRISA2
#define BC_ENABLERX_OUT     LATC7
#define BC_MODOUT_OUT       LATC3
#define BC_MODOUT_N_OUT     LATA2

/** Latch bit for pin that acts as mux control for Serial TX */
#define BC_SERIAL_OUT       LATC6
// Max packet data length
#define MAX_LENGTH  16

// *****************************************************************************
//          GUI Config Defines                                                **
// *****************************************************************************
/** Serial Communication Baudrate configuration */
#define PC_BAUDCON          0b00001000
/** Configure EUSART pins for alternative use */
#define PC_APFCON0          0b10000100

//============== GLOBAL VARIABLES ===============
uint16_t BCOM_cmdTimeout;
uint16_t BCOM_cmdTimer;
DevelopmentBoardState_t BodyComState = EVAL_TOUCH;
BCOM_cmdHandlerStates_t BCOM_cmdState = BCOM_CMD_IDLE;

//============== FILE VARIABLES ===============
/** Current state in debug setting menu */
// static uint8_t debugMenuCount = 0;
// /** Mod Depth config */
// static uint8_t afeModDepth = 0;
// /** Wake Up filter setting */
// static uint8_t afeWakeUp = 0;
// /** Preamble length */
// static uint8_t afePreamble = 4;
/** Prox mode scan */
static uint8_t ReadyToScan = 0;

#define ATTEMPTS_TRY 5

//============== FILE ENUMERATORS ===============
/** States for Debug setting menu */
// typedef enum {
//     SPLASH_SCREEN = 0,
//     MOD_DEPTH = 1,
//     WAKE_UP_TIMER = 2,
//     PREAMBLE_SETTING = 3,
//     RETRY_ID_COUNT = 4,
//     DEMO_RETURN = 5,
//     SETUP_FINISHED = 6
// }AfeDebugMenu_t;
// /** Current State inside debug menu */
// AfeDebugMenu_t debugMenuState = SPLASH_SCREEN;

//============== MENU MATRIX ===============
/** 11 states with Up down and select each. */
    // { UP, DOWN, SELECT }
const uint8_t MenuMatrix[2][3] = {
    {MODE_NONE, MODE_NONE, MODE_NONE}, // EVAL_TOUCH
    // {EVAL_TOUCH, DEBUG_MENU, MODE_NONE}, // EVAL_PROX
    // {EVAL_TOUCH, PAIR_MENU, RX_RSSI}, // DEBUG_MENU
    // {TOUCH, RX_PCKT, DEBUG_MENU}, // RX_RSSI
    // {RX_RSSI, TX_CW, DEBUG_MENU}, // RX_PCKT
    // {RX_PCKT, TX_LPBK, DEBUG_MENU}, // TX_CW
    // {TX_CW, TOUCH, MODE_NONE}, // TX_LPBK
    // {TX_LPBK, RX_RSSI, DEBUG_MENU}, // TOUCH
    // {EVAL_TOUCH, EVAL_TOUCH, VIEW_ID}, // PAIR_MENU
    // {MODE_NONE, MODE_NONE, PAIR_MENU}, // VIEW_ID
    {MODE_NONE, MODE_NONE, MODE_NONE} // ADD_MODE_GUI
};

//============== FUNCTIONS ===============

//============== Configure Functions ===============
void BC_BoardHardwareInit(void) {
    // Configure the system clock
    OSCCON = 0x78;
    // Set button pin directions
    BTN_Init();
    // Configure MCLR as INPUT, Reset in ISR with controlled i2c stop condition
    TRISA3 = 1;
    WPUA3 = 1;
    IOCAN3 = 1;
    IOCAF3 = 0;
    IOCIE = 1;
    // LCD Inititalization
    LCDM_Init();
    // PC com initialization
    PC_ComInit();
    // User input initialization (Touch)
    TD_CvdInit();
    // Start the board in Bodycom Touch Eval menu
    BodyComState = EVAL_TOUCH;
    // Do Splash Screen
    Display_Splash;
    // Debug LED direction bits
    TRISA0 = 0;
    ANSA0 = 0;
    // Configures Registers for use with GUI
    BAUDCON = PC_BAUDCON;
    APFCON0 = PC_APFCON0;
}

void BC_BodyComHardwareInit(void) ///< Initialised the BodyCom specific hardware
{
    //MDLL
    // Hardware I/O Port settings
    BC_ENABLERX_OUT = 1; // 1 enables Rx and 0 disables Rx
    BC_ENABLERX_TRIS = 0;
    // Driver
    // Reference Clock setup
    CLKRCON = BC_CLKRCON_SET;
    BC_CLKREN = 1;
    // Enable voltage reference for comparator
    FVRCON = BC_FVRCON_SET;
    // Comparitor Setup for the output of th9e DSM module
    CM1CON1 = BC_CM1CON1_SET; // C1 used to invert 125kHz modulated signal
    CM1CON0 = BC_CM1CON0_SET; //
    C1OE = 0; //  Disable comparator output to give pin control to LAT
    // DSM Module configuration
    MDCON = BC_MDCON_SET; //Reference clock enabled
    MDCARH = BC_MDCARH_SET; //modulation high signal reference clock module
    MDCARL = BC_MDCARL_SET; //modulation low signal VSS9r
    MDSRC = 0x00; // modulation source MDBIT
    CLKRCON = 0x96;
    // MANR
    // IOC pins
    ANSB5 = 0;
    BC_RX_IOC_N = 0; // configure data input for Int on Change
    BC_RX_IOC_P = 0; //
    BC_RX_IOC_IF = 0;
    IOCIF = 0; // clear interrupt flag
    IOCIE = 1; // enable interrupt
    // MDLL
    BC_MODOUT_TRIS = 0;
    BC_MODOUT_N_TRIS = 0;
    BC_MODOUT_OUT = 0; // Drive output to MOD_OUT from PIN
    BC_MODOUT_N_OUT = 0;
    BC_SERIAL_TRIS = 1;
    BC_SERIAL_OUT = 0;
    // LOCAL
    // Command timer
    T2CON = 0xFF;
    PR2 = 20;
    TMR2IE = 1;
    TMR2ON = 1;
}

void BC_HardwarePrepareRssi(void) {
    ADCON0 = 0b00101101; // select AN11 for RSSI
    ADCON1 = 0b11110000; // right justified, clock from dedicated RC oscillator
    GO_nDONE = 1; // start next conversion
    while (GO_nDONE); // if conversion is not complete, wait for it
}

//============== System Setup Functions ===============
// void BC_ConfigureDebugSettingsCheck(void) {
//     // Tempory Variables
//     uint8_t debugWaitTime = 0;
//     uint8_t button = 0;
//     // Give time to Push S4 Button
//     while (debugWaitTime < 250) {
//         // Scan buttons
//         BTN_Update();
//         button = (uint8_t) BTN_Get();
//         // Press detected move on
//         if (button == Select) {
//             debugWaitTime = 255;
//         } else  // Wait for time out
//             debugWaitTime++;
//     }
//     // Load afe EEPROM defaults
//     afeWakeUp = eeprom_read(0);
//     afeModDepth = eeprom_read(1);
//     afePreamble = eeprom_read(2);
//     // Ensure known state/stage
//     //debugMenuState = SPLASH_SCREEN;
//     // Enter debug settings if button detected
//     if (button == Select) {
//         do {
//             BC_DebugSettingsConfigure(); // Setup Afe
//         } while (debugMenuState < 6);
//     }
//     // Apply new afe settings or defaults
//     BC_ConfigureAfe();
//     // Save afe settings to EEPROM
//     eeprom_write(0, afeWakeUp);
//     eeprom_write(1, afeModDepth);
//     eeprom_write(2, afePreamble);
// }

// void BC_ConfigureAfe(void) {
//     // Configure AFE with Wakeup filter ENABLED (default operation)
//     if (!afeWakeUp) {
//         if (afeModDepth == 0) // @ 8% modulation depth
//         {
//             AFE_LfData_Setup(0x86, 0x00, 0x00, 0x00, 0x00, 0x30, 0x49);
//         } else if (afeModDepth == 1) // @ 14% modulation depth
//         {
//             AFE_LfData_Setup(0x86, 0x00, 0x00, 0x00, 0x00, 0x20, 0x59);
//         } else if (afeModDepth == 2) // @ 33% modulation depth
//         {
//             AFE_LfData_Setup(0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79);
//         } else if (afeModDepth == 3) // @ 60% modulation depth
//         {
//             AFE_LfData_Setup(0x86, 0x00, 0x00, 0x00, 0x00, 0x10, 0x69);
//         }
//     } else {  // Configure AFE with Wakeup filter DISABLED (debugging operation)
//         if (afeModDepth == 0) // @ 8% modulation depth
//         {
//             AFE_LfData_Setup(0x06, 0x00, 0x00, 0x00, 0x00, 0x30, 0xC9);
//         } else if (afeModDepth == 1) // @ 14% modulation depth
//         {
//             AFE_LfData_Setup(0x06, 0x00, 0x00, 0x00, 0x00, 0x20, 0xD9);
//         } else if (afeModDepth == 2) // @ 33% modulation depth
//         {
//             AFE_LfData_Setup(0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF9);
//         } else if (afeModDepth == 3) // @ 60% modulation depth
//         {
//             AFE_LfData_Setup(0x06, 0x00, 0x00, 0x00, 0x00, 0x10, 0xE9);
//         }
//     }
// }

void BC_PrepareBodyComSystem(void) {
    // Initialize body comm system
    BC_BodyComHardwareInit(); // configure variables and peripherals for BodyComm
    // Configure AFE IC. We get stuck here until his works
    while (AFE_Out(0) == 0) {
        LATA0 = 1;
    }
    // Load Mobile List for Challenge and Response Protocol
    PL_UpdateList();
    // Reset BC AFE
    AFE_WriteCommand(AFE_SOFT_RESET);
    // Make sure DLL is in known state
    MDLL_setMode(MDLL_MODE_DISABLED);
    // Enable interrupts (PEIE and GIE)
    PEIE = 1;
    GIE = 1;
}

void BC_HardwareResponseReceiverReset(void) {
    // Issue lock and reset for improved signal demodulation
    AFE_WriteCommand(AFE_LOCK);
    AFE_WriteCommand(AFE_SOFT_RESET);
}

//============== Menu Functions ===============
void BC_DemoMenuNavigator(uint8_t Action) {
    // Place holder for menu change
    uint8_t newState;
    //  Change state based on matrix action
    newState = MenuMatrix[BodyComState][Action - 1];
    // Do nothing if MODE_NONE is action
    if (newState != MODE_NONE) {
        BodyComState = newState;
    } 
    if ( (BodyComState == EVAL_TOUCH) || (BodyComState == DEBUG_MENU) ) {
        ReadyToScan = 0;
        // Make sure we are not TX/RX
        MDLL_setMode(MDLL_MODE_DISABLED);
    }
}

void BC_DemoMenuHandler(void) {
    // Tempory variables
    uint8_t change = 0,
            button = None;
    // Function variable
    static uint8_t checkCVD = 0;
    static uint8_t PairedListIndex = 0;
    static uint8_t checkRssi = 0;
    //PC commands sent directly to Mobile Unit
    PC_BodyComRequestHandler();
    // Button sampling handler
    BTN_Update();
    button = (uint8_t) BTN_Get();
    // Button press was detected prepare for menu action
    if (button > None) {
        change = 1;
    }
    // Demo menu actions
    switch (BodyComState) {
        case EVAL_TOUCH:
            // If the tranceiver is idle
            if (checkCVD) {
                // Check for CVD event
                TD_CvdScan();
                TD_CvdScan();
                BC_DisplayCvdIndicator();
                TD_CvdScan();
                TD_CvdScan();
                BC_DisplayCvdIndicator();
                // Touch detected; begin BodyCom communication
                if (CvdEvent) {
                    // Show searching first up
                    BC_CommunicationDisplay(PROT_PREPARE_CHALLENGE);
                    // Change cmd state for Handler
                    BC_PingPairedList();
                    // Stop CVD scanning
                    checkCVD = 0;
                }
            }
            // Cannot do CVD here, we are receiving here and it will interfere
            if ((BCOM_cmdState == BCOM_CMD_SUCCESS) ||
                    (BCOM_cmdState == BCOM_CMD_TIMEOUT) ||
                    (BCOM_cmdState == BCOM_CMD_ERROR)) {
                // Update display for next touch event
                TD_CvdScan();
                BC_DisplayCvdIndicator();
                Display_EvaluationTouch;
                // Star CVD scanning
                checkCVD = 1;
            }
            break;

        // case EVAL_PROX:
        //     // Butten scan event detected
        //     if ( (button == Select) && (ReadyToScan == 0) ){
        //         ReadyToScan = 1;
        //     } else if ( (button == Select) && (ReadyToScan == 1) ) {
        //         ReadyToScan = 0;
        //     }
            
        //     // Wait till BodyCom scan is done
        //     if ((BCOM_cmdState == BCOM_CMD_SUCCESS) ||
        //             (BCOM_cmdState == BCOM_CMD_TIMEOUT) ||
        //             (BCOM_cmdState == BCOM_CMD_ERROR)) {
        //         if (ReadyToScan){
        //             // Show searching first up
        //             BC_CommunicationDisplay(PROT_PREPARE_CHALLENGE);
        //             // Talk to anyone
        //             // Broadcast address loaded
        //             uint8_t tmpAddress[] = {0xFF, 0xFF, 0xFF, 0xFF};
        //             if (BC_SendCommand(PING_PAIRED, tmpAddress, PING_TIMEOUT))
        //             {
        //                 // Update state for handler
        //                 BCOM_cmdState = BCOM_CMD_PING_SENT;
        //             }
        //         } else {
        //             Display_EvaluationProximity
        //         }

        //     }
        //     break;

        // case DEBUG_MENU:
        //     // Print Debug menu display
        //     Display_DebugMenu
        //     break;

        // case RX_RSSI:
        //     if (checkRssi == 0) {
        //         // Only do once
        //         checkRssi = 1;
        //         // Make sure we are not TX/RX
        //         MDLL_setMode(MDLL_MODE_DISABLED);
        //         // Configure AFE IC. We get stuck here until his works
        //         while (AFE_Out(1) == 0) {
        //             LATA0 = 1;
        //         }
        //     }
        //     // On any button press we change state and go back to normal mode
        //     if (button != None) {
        //         BC_ConfigureAfe();
        //         // Make sure we can do the setup operation
        //         checkRssi = 0;
        //     } else { // Check RSSI value
        //         GIE = 0;
        //         BC_HardwarePrepareRssi();
        //         Display_RSSI
        //         GIE = 1;
        //     }
        //     break;

        // case RX_PCKT:  
        //     // Initial Setup
        //     if (MDLL_mode != MDLL_MODE_RX) {
        //         MDLL_setMode(MDLL_MODE_RX);
        //         BCOM_cmdState = BCOM_CMD_RXPACKET;
        //     }
        //     // Do nothing but wait to receive and display.
        //     // Command Handler does the rest.
        //     break;

        // case TX_CW:
        //         // Print CW display
        //         Display_ContinuousWave;
        //         // Drive LC tank
        //         MANT_HardwareEnableDriver;
        //         MDBIT = 1;

        //         if (button != None) {
        //         MANT_HardwareDisableDriver
        //         }

        //     break;

        // case TX_LPBK:
        //     // On any button press we change state and go back to normal mode
        //     if (button == Select){
        //         // If handler is available
        //         if (BCOM_cmdState >= BCOM_CMD_SUCCESS)
        //         {
        //             // Broadcast address loaded
        //             uint8_t tmpAddress[] = {0xFF, 0xFF, 0xFF, 0xFF};
        //             // String Message to Echo; Max Size 16 characters
        //             uint8_t  echoMessageString[] = "ECHO MESSAGE";
        //             // Send Data; change command state only if successful
        //             if (BC_SendDataCommand(ECHO_REQUEST, tmpAddress, echoMessageString, ((uint8_t)sizeof(echoMessageString)-1), PING_TIMEOUT*2)) {
        //                 BCOM_cmdState = BCOM_CMD_ECHOREQ_SENT;
        //             }
        //         }
        //     }
        //     // Wait till BodyCom scan is done
        //     if ((BCOM_cmdState == BCOM_CMD_SUCCESS) ||
        //             (BCOM_cmdState == BCOM_CMD_TIMEOUT) ||
        //             (BCOM_cmdState == BCOM_CMD_ERROR)) {
        //         // Update display
        //         Display_LoopBack
        //     }
        //     break;

        // case TOUCH:
        //     // Display Touch info
        //     Display_Touch
        //     // Touch CVD scan
        //     TD_CvdScan();
        //     break;

        // case PAIR_MENU:
        //     // Display List text
        //     Display_PairedListMenu;
        //     // Reset Index position for viewing
        //     PairedListIndex = 0;
        //     break;

        // case VIEW_ID:
        //     // Increment list indext
        //     if (button == Up)   {
        //         if (PairedListIndex < PairedCount) {
        //             PairedListIndex++;  // Increment index position
        //         } else{
        //             PairedListIndex = PairedCount - 1; // Top of list
        //         } // Decrement list index
        //     } else if (button == Down) {
        //         if (PairedListIndex > 0) {
        //             PairedListIndex--;  // Decrement indext position
        //         } else {
        //             PairedListIndex = 0;    // Bottom of list
        //         }
        //     }
        //     // Display ID at location
        //     BC_DisplayPairedList(PairedListIndex);
        //     break;

        case ADD_MODE_GUI:
            // Display text for GUI interaction state
            Display_GuiControl
            break;

        default:            // Error
            break;
    }
    // Handle Menu transistions
    if (change == 1) {
        BC_DemoMenuNavigator(button);
    }
}

// void BC_DebugSettingsConfigure(void) {
//     // Function only variables
//     uint8_t confirmSetup = 0;
//     uint8_t button = 0;
//     // Scan & Acquire button press
//     BTN_Update();
//     button = (uint8_t) BTN_Get();
//     // Menu Scrolling action
//     if (button == Up) {
//         debugMenuCount++;
//     } else if (button == Down) {
//         if (debugMenuCount > 0) {
//             debugMenuCount--;
//         }
//     }
//     //Debug Menu Settings States
//     switch (debugMenuState) {
//         case SPLASH_SCREEN:
//             // Display splash
//             DBUF_Clear(3);
//             memcpy(&DisplayBuffer[1], "Debug Settings", 15);
//             memcpy(&DisplayBuffer[18], "S4 to Begin", 11);
//             // Save selection and move on
//             if (button == Select) {
//                 debugMenuCount = afeModDepth;
//                 debugMenuState = MOD_DEPTH;
//             }
//             break;

//         case MOD_DEPTH:
//             // Display
//             DBUF_Clear(3);
//             memcpy(&DisplayBuffer[0], "Mod Depth", 9);
//             // Select mod depth and update display
//             if (!debugMenuCount) {
//                 memcpy(&DisplayBuffer[18], "8%", 2);
//                 afeModDepth = 0;
//             } else if (debugMenuCount == 1) {
//                 memcpy(&DisplayBuffer[18], "14%", 3);
//                 afeModDepth = 1;
//             } else if (debugMenuCount == 2) {
//                 memcpy(&DisplayBuffer[18], "33%", 3);
//                 afeModDepth = 2;
//             } else if (debugMenuCount >= 3) {
//                 debugMenuCount = 3;
//                 memcpy(&DisplayBuffer[18], "60%", 3);
//                 afeModDepth = 3;
//             }
//             // Save selection and move on
//             if (button == Select) {
//                 if (afeWakeUp) {
//                     debugMenuCount = 1;
//                 } else {
//                     debugMenuCount = 0;
//                 }
//                 debugMenuState = WAKE_UP_TIMER;
//             }
//             break;

//         case WAKE_UP_TIMER:
//             // Display
//             DBUF_Clear(3);
//             memcpy(&DisplayBuffer[2], "Wakeup Timer", 12);
//             // Select selection
//             if (!debugMenuCount) {
//                 memcpy(&DisplayBuffer[20], "Enabled", 7);
//                 afeWakeUp = 0;
//             } else if (debugMenuCount) {
//                 debugMenuCount = 1;
//                 memcpy(&DisplayBuffer[20], "Disabled", 8);
//                 afeWakeUp = 1;
//             }
//             // Save selection and move on
//             if (button == Select) {
//                 debugMenuCount = afePreamble;
//                 debugMenuState = PREAMBLE_SETTING;
//             }
//             break;

//         case PREAMBLE_SETTING:
//             // Display
//             DBUF_Clear(2);
//             memcpy(&DisplayBuffer[0], "Preamble Timing ", 16);
//             memcpy(&DisplayBuffer[21], "  mSec", 6);
//             // Change count value
//             if (debugMenuCount >= 6) {
//                 debugMenuCount = 6;
//             }
//             // Load new value
//             afePreamble = debugMenuCount;
//             // Display value
//             DisplayBuffer[21] = (uint8_t) (afePreamble + 0x31);
//             // Save selection and move on
//             if (button == Select) {
//                 debugMenuCount = 0;
//                 debugMenuState = DEMO_RETURN;
//             }
//             break;

//         case DEMO_RETURN:
//             // Display
//             DBUF_Clear(3);
//             memcpy(&DisplayBuffer[1], "Debug Settings", 14);
//             // Display selections
//             if (!debugMenuCount) {
//                 memcpy(&DisplayBuffer[18], "Confirm Setup", 13);
//                 confirmSetup = 0;
//             } else if (debugMenuCount) {
//                 debugMenuCount = 1;
//                 confirmSetup = 1;
//                 memcpy(&DisplayBuffer[19], "Retry Setup", 11);
//             }
//             // Save selection and move on
//             if (button == Select) {
//                 if (!confirmSetup) {
//                     debugMenuState = SETUP_FINISHED;
//                 } else {
//                     debugMenuState = SPLASH_SCREEN;
//                 }
//             }
//             break;

//         case SETUP_FINISHED: // Wait to escape loop
//             break;

//         default: // State Machine Error
//             break;
//     }
//     // Update/refresh LCD handler
//     DBUF_Handler();
// }

//============== DISPLAYS ===============
void BC_CommunicationDisplay(BodyComProtocol_t handle_state) {
    switch (handle_state) {
            // Demo detection type display
        case PROT_DETECT:
            DBUF_GoTo(16);
            if (BodyComState == EVAL_TOUCH) {
                Display_EvaluationTouch;
            }
            // else if (BodyComState == EVAL_PROX) {
            //     DBUF_Printf((uint8_t*) "S4 to Begin Scan");
            // }
            break;

            //Valid ID found; print ID
        case PROT_VALID_RESPONSE:
            DisplayPopupBuffer[16] = 'I';
            DisplayPopupBuffer[17] = 'D';
            DisplayPopupBuffer[18] = ':';
            memset(&DisplayPopupBuffer[19], ' ', 13);
            DBUF_GoTo(20);
            BC_DisplayId(1, MDLL_PacketDataBuffer.Address);
            break;


        case PROT_PAIRED_INTERROGATE:
        case PROT_PREPARE_CHALLENGE:
        case PROT_NO_RESPONSE:
            if (BodyComState == RX_PCKT) {
                DBUF_Clear(2);
                DBUF_GoTo(16);
                DBUF_Printf((uint8_t*) "Listening...");
            } else {
                DBUF_Clear(2);
                DBUF_GoTo(16);
                DBUF_Printf((uint8_t*) "Searching...");
            }
            break;

        default:    // Error
            break;
    }
    // Do an immediate blocking LCD refresh. Since this will never be called from interrupt this should
    //     not affect the timing.
    DBUF_RefreshNow();
}
void BC_DisplayId(uint8_t destination, uint8_t* id) {
    // Tempory variables
    uint8_t count = 0,
            read = 0;
    // Popup or Normal display
    if (destination > 0) {
        DisplayPopupState = POPUP_LINE_2;
    }
    // Read bytes from pointer data
    for (count = 0; count < 8; count++) {
        if ((count & 0x01) == 0x01) {
            read = (uint8_t) (id[(uint8_t) (count >> 1)] & 0x0F);
        } else {
            read = (uint8_t) (id[(uint8_t) (count >> 1)] >> 4);
        };
        // Truncate at end of LCD
        if (DisplayBufferIndex + count > 31)
            return;
            // Display to index
        if (read < 0xA) {
            if (destination == 0) {
                DisplayBuffer[(uint8_t) (DisplayBufferIndex + count)] = (uint8_t) (read + (uint8_t) 0x30);
            } else {
                DisplayPopupBuffer[(uint8_t) (DisplayBufferIndex + count)] = (uint8_t) (read + (uint8_t) 0x30);
            }
        } else {
            if (destination == 0) {
                DisplayBuffer[(uint8_t) (DisplayBufferIndex + count)] = (uint8_t) (read + ((uint8_t) 'A' - 0x0A));
            } else {
                DisplayPopupBuffer[(uint8_t) (DisplayBufferIndex + count)] = (uint8_t) (read + ((uint8_t) 'A' - 0x0A));
            }
        }
    }
}

void BC_DisplayCvdIndicator(void) {
    // Display Current Cap Touch Sensor State
    switch (CvdSensorState) {
            // Data is being
        case CVD_INITIALIZING:
           Display_CvdInitilizing
            break;
            // Touch Detected
        case CVD_PRESSED:
            Display_CvdPressed
            break;
            // Recent touch detection
        case CVD_RELEASED:
            Display_CvdReleased
            break;
            // Clear display
        default:
            Display_CvdSpace
            break;
    }
}

// void BC_DisplayRxPacket(void){
//     // Valid packet received
//     if (MDLL_receiveDataPacket() > 0) {
//         // Show ID ...
//         BC_CommunicationDisplay(PROT_VALID_RESPONSE);
//         // Wait so it shows visibly
//         __delay_ms(500);
//     } else {
//         // Show Searching ...
//         BC_CommunicationDisplay(PROT_NO_RESPONSE);
//     }
// }

void BC_DisplayPairedList(uint8_t index) {
    // Index location to read
    if ((index + 1) > PairedCount) {
        index = PairedCount - 1;
    }
    // Display
    memcpy(DisplayBuffer, "Select TAG:00/00", 16);
    // Display Id index value
    DisplayBuffer[0x0C] = (uint8_t) (index + 0x31);
    // Display value of paired devices
    DisplayBuffer[0x0F] = (uint8_t) (PairedCount + 0x30);
    // Prepare display
    DBUF_Clear(2);
    DBUF_GoTo(16);
    DBUF_Printf((uint8_t*) "ID:");
    DBUF_GoTo(20);
    // Read Id at index location
    PL_ReadPaired(index, &DeviceInfo[0]);
    // Display ID
    BC_DisplayId(0, &DeviceInfo[0]);
}

//============== BodyCom Handlers ===============
void BC_CommandHandler(void) {
    // Handle BodyCom Applications
    switch (BCOM_cmdState) {
        case BCOM_CMD_IDLE:
            break;

        case BCOM_CMD_PING_SENT:
            // When we get the response
            if (MDLL_receiveDataPacket() > 0) {
                //BCOM_cmdState =
                ;
                BC_CommunicationDisplay(PROT_VALID_RESPONSE);
                // Make a copy for the PC app to peruse at its leasure ...
                memcpy(&pcRxPacket, &MDLL_PacketDataBuffer, sizeof(MDLL_PacketDataBuffer));

                // Let it time out, or we will tx while retransmit is still busy
                //BCOM_cmdState = BCOM_CMD_SUCCESS;
            }
            break;

        case BCOM_CMD_ECHOREQ_SENT:
            // When we get the response
            if (MDLL_receiveDataPacket() > 0) {
                memset(&DisplayPopupBuffer[16], ' ',  16);
                memcpy(&DisplayPopupBuffer[16], MDLL_PacketDataBuffer.DataBuffer, MDLL_PacketDataBuffer.DataLength);
                DisplayPopupState = POPUP_LINE_2; // Set line 2 to popup state
                // Make a copy for the PC app to peruse at its leasure ...      // TODO
                memcpy(&pcRxPacket, &MDLL_PacketDataBuffer, sizeof(MDLL_PacketDataBuffer));
                BCOM_cmdState = BCOM_CMD_SUCCESS;
            }
            break;

        case BCOM_CMD_PAIRPING_SENT:
            // We have just done a broadcast ping. If we get a response in time add the unit (if it is new)
            if (MDLL_receiveDataPacket() > 0) {
                // Check if the unit address received is already in our paired list; if not add.
                PL_AddPairedDevice(MDLL_PacketDataBuffer.Address);
                // Update LCD display
                BC_CommunicationDisplay(PROT_VALID_RESPONSE);
                // Make a copy for the PC app to peruse at its leasure ...
                memcpy(&pcRxPacket, &MDLL_PacketDataBuffer, sizeof (MDLL_PacketDataBuffer));
                // If it was not found add it now.
                // BodyCom action complete
                BCOM_cmdState = BCOM_CMD_SUCCESS;
            }
            break;

        // case BCOM_CMD_RXPACKET:
        //     // Display Receive information
        //     Display_RxPacket
        //     BC_DisplayRxPacket();
        //     break;

            // All these are status only, no specific implementation, exit through reset
        case BCOM_CMD_SUCCESS:
        case BCOM_CMD_TIMEOUT:
        case BCOM_CMD_ERROR:
        default:
            // If not waiting for data in particular, just ignore it
            MDLL_receiveDataPacket();
            break;
    }
}
uint8_t BC_PingPairedListHandler(void) {
    static uint8_t PairedIdCount = 0;
    // Update list
    PL_UpdateList();
    // Cout is old start over
    if (PairedIdCount >= PairedCount) {
        PairedIdCount = 0;
    }
    // Read next ID from EEPROM
    PL_ReadPaired(PairedIdCount, DeviceInfo);
    // Search for ID
    if (BC_SendCommand(PING_PAIRED, DeviceInfo, PING_TIMEOUT))
    {   // Increment count to ping next in index next time
       PairedIdCount++;
       return 1;
    }
    // Finished
    return 0;
}
//============== BodyCom Commands ===============
uint8_t BC_PingPairedList(void) {
    // Ping list
    if (BC_PingPairedListHandler())
    {
        BCOM_cmdState = BCOM_CMD_PING_SENT;
        return 1;
    }
    // Finished
    return 0;
}

uint8_t BC_SendDataCommand(BodyCom_Commands_t cmd, uint8_t* Address, uint8_t *data, uint8_t  length, uint8_t timeout)
{
    // Create tempory packet
    MDLL_PacketData_t tempPacket;
    // Make sure we don't overflow
    if (length > MAX_LENGTH) {
        length = MAX_LENGTH;
    }
    // Reset timer
    BCOM_cmdTimer = 0;
    // Load timemout value
    BCOM_cmdTimeout = timeout;
    // Build packet
    tempPacket.Command = cmd;
    tempPacket.Address[0] = Address[0];
    tempPacket.Address[1] = Address[1];
    tempPacket.Address[2] = Address[2];
    tempPacket.Address[3] = Address[3];
    tempPacket.DataLength = length;
    // Copy data to buffer
    memcpy(tempPacket.DataBuffer, data, length);
    // Send packet
    return MDLL_sendPacket(&tempPacket);
}

uint8_t BC_SendCommand(BodyCom_Commands_t cmd, uint8_t * Address, uint8_t timeout) {
    // Create tempory packet
    MDLL_PacketData_t tempPacket;
    // Reset timer
    BCOM_cmdTimer = 0;
    // Load timemout value
    BCOM_cmdTimeout = timeout; 
    // Build packet
    tempPacket.Command = cmd;
    tempPacket.Address[0] = Address[0];
    tempPacket.Address[1] = Address[1];
    tempPacket.Address[2] = Address[2];
    tempPacket.Address[3] = Address[3];
    tempPacket.DataLength = 0;
    // Send packet
    return MDLL_sendPacket(&tempPacket);
}