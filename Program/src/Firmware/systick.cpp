/*/*!
 * @file systick.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "systick.h"

uint32_t g_systick_freq = 0;

void initSysTick(uint32_t period_us) {
	uint32_t ticks = (FREQ_CLOCK_MCU / 1000000) * period_us;
	g_systick_freq = period_us;
	SysTick->RELOAD = ticks - 1;
	SysTick->CTRL = 0B0111;
	SysTick->CURR = 0;
}

void SysTick_Handler(void) {
	if (!g_callback_list.empty())
		for (Callback* q : g_callback_list) q->CallbackMethod();
}
