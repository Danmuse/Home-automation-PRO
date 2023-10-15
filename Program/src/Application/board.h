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
#include "Keyboard.h"
#include "SevenSegmentDisplay.h"
#include "LCD1602.h"
#include "Semaphore.h"

//#include "UART.h" // Review the ProgramConfig
#include "I2C.h" // Review the ProgramConfig
//#include "SPI.h" // Review the ProgramConfig

extern SevenSegmentDisplay *g_display;
extern Keyboard *g_keyboard;
extern LCD1602 *g_lcd1602;
extern I2C *g_IIC;

void initDevice(void);

void initDisplay(void);

void initKeyboard(void);

void initLCD1602(void);

void initIIC(void);

#endif /* BOARD_H_ */
