/*!
 * @file board.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 * @brief Initialize Board API (Application Programming Interface).
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "GPIO.h"
#include "Callback.h"

extern Gpio LED_RED;
extern Gpio LED_GREEN;
extern Gpio LED_BLUE;

extern Gpio UserSwitch;

void initDevice(void);

//! UNUSED Function
void myCallback(void); //!< Scheduler - It is executed as many times as the time set in the SysTick which is the function called in SysTick_Handler.

#endif /* BOARD_H_ */
