/*!
 * @file board.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 * @brief Initialize Board API (Application Programming Interface).
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <Drivers/05-Timers/Timers.h>
#include "PLL.h"
#include "PWM.h"
#include "systick.h"

extern Gpio LED_RED;
extern Gpio LED_GREEN;
extern Gpio LED_BLUE;

extern Gpio UserSwitch;

extern Timers g_timers_list;

void initDevice(void);

#endif /* BOARD_H_ */
