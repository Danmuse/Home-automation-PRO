/*!
 * @file systick.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 * @brief System Tick Timer API (Application Programming Interface).
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "LPC845.h"
#include "Callback.h"

extern uint32_t g_systick_freq;

#if defined (__cplusplus)
extern "C" {
	void SysTick_Handler(void);
}
#endif

void initSysTick(uint32_t period_us);

#endif /* SYSTICK_H_ */
