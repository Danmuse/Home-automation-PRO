/*!
 * @file systick.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "systick.h"

extern std::list<Callback*> g_callbacklist;

uint8_t flagInterruptTicks = 0;
volatile void (*SysTick_Callback)(void) = nullptr; //!< UNUSED Variable

void initSysTick(uint32_t period_us) {
	// SysTick_Handler = nullptr;
	uint32_t ticks = (FREQ_CLOCK_MCU / 1000000) * period_us;
	SysTick->RELOAD = ticks - 1;
	SysTick->CTRL = 0B0111;
	SysTick->CURR = 0;
}

void installCallbackSysTick(void (*myCallback)(void)) {
	if (myCallback != nullptr) SysTick_Callback = (volatile void (*)(void)) myCallback; //!< Pointer to Scheduler
}

void SysTick_Handler(void) {
	// COMIENZA LA EJECUCION DE UNO DE LOS CALLBACKS
	// TODO: The following code fragment should be another function that will be grouped into the list of pointers to function 'myCallbak'.
	//	static uint32_t counterTicks = 0;
	//	counterTicks++;
	//	counterTicks %= 75;
	//	if (!counterTicks) {
	//		// flagInterruptTicks = 1;
	//		LED_GREEN.SetTogglePin();
	//	}
	// FINALIZA LA EJECUCION DE UNO DE LOS CALLBACKS

	// COMIENZA LA EJECUCION DE UNO DE LOS CALLBACKS
	//  if (SysTick_Callback) SysTick_Callback();
	// FINALIZA LA EJECUCION DE UNO DE LOS CALLBACKS

	// COMIENZA LA EJECUCION DE LA LISTA DE CALLBACKS
	for (Callback* q : g_callbacklist) q->myCallback();
	// FINALIZA LA EJECUCION DE LA LISTA DE CALLBACKS
}
