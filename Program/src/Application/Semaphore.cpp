/*/*!
 * @file Semaphore.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/08/2023 01:02:47
 */

#include "Semaphore.h"

Timers g_timers_list;

static void delayFUNC(void);

static void delayFUNC(void) { }

void delay(uint32_t milliseconds) {
	static Timer delayTIMER, debounce;
	g_timers_list << delayTIMER;
	delayTIMER.TimerStart(milliseconds, delayFUNC, Timer::MILLI);
	while (delayTIMER.GetTimer());
}
