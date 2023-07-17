/*!
 * @file board.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "board.h"

Gpio LED_RED(Gpio::PORT1, 2, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio LED_GREEN(Gpio::PORT1, 0, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio LED_BLUE(Gpio::PORT1, 1, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);

Gpio UserSwitch(Gpio::PORT0, 4, Gpio::REPEATER, Gpio::INPUT, Gpio::LOW);

Timers g_timers_list;

void initDevice(void) {
	// initPhaseLockedLoop();
	initSysTick(FREQ_SYSTICK);

	LED_RED.ClearPin();
	LED_GREEN.ClearPin();
	LED_BLUE.ClearPin();
}
