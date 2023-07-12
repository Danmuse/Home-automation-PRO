/*!
 * @file systick.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 * @brief System Tick Timer API (Application Programming Interface).
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <list>
#include "board.h"

extern uint8_t flagInterruptTicks;

#if defined (__cplusplus)
extern "C" {
	void SysTick_Handler(void);
}
#endif

#define FREQ_CLOCK_MCU 12000000

void initSysTick(uint32_t period_us);

//! UNUSED Function
void installCallbackSysTick(void (*myCallback)(void));

#endif /* SYSTICK_H_ */
