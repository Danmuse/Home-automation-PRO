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
#include "UART.h"
#include "PwmManager.h"
#include "Semaphore.h"
#include "ProgramConfig.h"
#include "SevenSegmentDisplay.h"

extern SevenSegmentDisplay *g_display;

void initDevice(void);

void initDisplay(void);

#endif /* BOARD_H_ */
