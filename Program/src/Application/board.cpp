/*!
 * @file board.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "board.h"

std::list<Callback*> g_callbacklist;

Gpio LED_RED(Gpio::PORT1, 2, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio LED_GREEN(Gpio::PORT1, 0, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio LED_BLUE(Gpio::PORT1, 1, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);

Gpio UserSwitch(Gpio::PORT0, 4, Gpio::REPEATER, Gpio::INPUT, Gpio::LOW);

void initDevice(void) {
	// TODO: Initialize the board (PLL)
	// installCallbackSysTick(myCallback);
	initSysTick(1000);

	LED_RED.ClearPin();
	LED_GREEN.ClearPin();
	LED_BLUE.ClearPin();
}

void myCallback(void) {
//	static uint32_t counterTicks = 0;
//	counterTicks++;
//	counterTicks %= 50;
//	if (!counterTicks) {
//		// flagInterruptTicks = 1;
//		LED_RED.SetTogglePin();
//	}
	for (Callback* q : g_callbacklist) q->myCallback();
}
