/*/*!
 * @file Semaphore.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/08/2023 01:02:47
 */

#include "Semaphore.h"

Timers g_timers_list;

void delay(uint32_t milliseconds) {
	static Timer delayTIMER;
	static bool flagPush = false;
	if (!flagPush) {
		flagPush = !flagPush;
		g_timers_list << delayTIMER;
	}
    delayTIMER.timerStart(milliseconds, nullptr, Timer::MILLI);
	while (delayTIMER.getTicks());
}
