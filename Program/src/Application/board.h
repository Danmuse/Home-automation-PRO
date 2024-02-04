/*!
 * @file board.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 * @brief Initialize Board API (Application Programming Interface).
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "PLL.h"
#include "PWM.h"
#include "DigitalOutputs.h"
#include "DigitalInputs.h"
#include "PinInterrupt.h"
#include "SevenSegmentDisplay.h"
#include "Keyboard.h"
#include "LCD1602.h"
#include "DS3231.h"
#include "M24C16.h"
#include "MFRC522.h"
#include "S5050DJ.h"
#include "MG90S.h"
#include "DFPlayerMini.h"
#include "UART.h"
#include "ADC.h"
#include "DAC.h"

#include "utilities.h"

#include "IoTManager.h"
#include "QTConnection.h"

/////////////////////////////////
/// HARDWARE GLOBAL VARIABLES ///
/////////////////////////////////

/// Global variables defined in the program to resolve the initialization of hardware modules.
/// To use them you must initialize the peripherals by entering the corresponding initialization function within the main scope of the program.
/// --- "SevenSegmentDisplay *g_display" located in "Hardware/01-SevenSegmentDisplay/Display/SevenSegmentDisplay.h"
/// --- "Keyboard *g_keyboard" located in "Hardware/02-MatrixKeyboard/Keyboard.h"
/// --- "LCD1602 *g_lcd1602" located in "Hardware/03-LCD1602/LCD1602.h"
/// --- "DS3231 *g_ds3231" located in "Hardware/04-DS3231\ RTC/DS3231.h"
/// --- "FM24C16U *g_eeprom" located in "Hardware/05-M24C16\ EEPROM/M24C16.h"
/// --- "MFRC522 *g_rfid" located in "Hardware/06-MFRC522\ RFID/MFRC522.h"
/// --- "S5050DJ *g_leds" located in "Hardware/07-S5050DJ\ IR\ Transmitter/S5050DJ.h"
/// --- "SG90S *g_servo" located in "Hardware/08-MG90S\ Servo/MG90S.h"
/// --- "DFPlayer *g_dfplayer" located in "Hardware/09-DFPlayerMini/DFPlayerMini.h"
/// --- "UART *g_usb" located in "Drivers/10-UART/UART.h"
/// --- "ADC *g_preset" located in "Drivers/13-ADC/ADC.h"
/// --- "ADC *g_adcExternal" located in "Drivers/13-ADC/ADC.h"
/// --- "DAC *g_dacExternal" located in "Drivers/14-DAC/DAC.h"

/////////////////////////////////
/// SOFTWARE GLOBAL VARIABLES ///
/////////////////////////////////

/// Global variables defined in the program for macro configurations.
/// To use them you must initialize the peripherals by entering "initDevice" function within the main scope of the program.
/// --- "uint32_t g_systick_freq" located in "Firmware/systick.h"

/// Global variables defined in the program to simplify the use of classes.
/// --- "Timers g_timers_list" located in "Application/Semaphore.h"
/// --- "list<Callback*> g_callback_list" located in "Drivers/03-Callback/Callback.h"

/// Internal global variables defined in the program to avoid overlapping communication channels or override interruption handlers.
/// --- "AsyncComm *g_usart[MAX_USART_CHANNELS]" located in "Drivers/10-UART/UART.cpp"
/// --- "SyncCommTWI *g_TWI[MAX_I2C_CHANNELS]" located in "Drivers/11-I2C/I2C.cpp"
/// --- "SyncCommSPI *g_SPI[MAX_SPI_CHANNELS]" located in "Drivers/12-SPI/SPI.cpp"
/// --- "PinInterrupt *g_gpiohandler[MAX_PIN_INTERRUPTS]" located in "Drivers/06-PinInterrupt/PinInterrupt.cpp"
/// --- "ADC *g_adc[MAX_ADC_CHANNELS]" located in "Drivers/13-ADC/ADC.cpp"

//////////////////////////////////
/// CRITICAL MACRO DEFINITIONS ///
//////////////////////////////////

/// Sensitive macro definitions in the program.
/// --- "FREQ_CLOCK_MCU" located in "LPC845.h"
/// --- "MAX_ADC_CHANNELS" located in "Drivers/13-ADC/ADC.h"
/// --- "MAX_DAC_CHANNELS" located in "Drivers/14-DAC/DAC.h"
/// --- "MAX_USART_CHANNELS" located in "Drivers/10-UART/UART.h"
/// --- "MAX_I2C_CHANNELS" located in "Drivers/11-I2C/I2C.h"
/// --- "MAX_SPI_CHANNELS" located in "Drivers/12-SPI/SPI.h"
/// --- "MAX_PWM_CHANNELS" located in "Drivers/09-PWM/PWM.h"
/// --- "MAX_PIN_INTERRUPTS" located in "Drivers/06-PinInterrupt/PinInterrupt.h"
/// --- "MAX_CTIMER0_MATCH_CHANNELS" located in "Firmware/CTimer.h"
/// --- "MAX_CTIMER0_CAPTURE_CHANNELS" located in "Firmware/CTimer.h"


#define RFID_USER_UID_SIZE 4
#define USERS_INIT_POSITION 2

void initDevice(void);

bool isUserRegistered(const MFRC522::UID_st& uid);
bool registerNewUser(const MFRC522::UID_st& uid);

#endif /* BOARD_H_ */
