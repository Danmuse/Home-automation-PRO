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
#include "UART.h"
#include "Keyboard.h"
#include "Semaphore.h"
#include "SevenSegmentDisplay.h"

extern SevenSegmentDisplay *g_display;
extern Keyboard *g_keyboard;

void initDevice(void);

void initDisplay(void);

void initKeyboard(void);

#endif /* BOARD_H_ */
