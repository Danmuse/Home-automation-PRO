/*!
 * @file board.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "board.h"

void initDevice(void) {
	#if FREQ_CLOCK_MCU == 48000000UL
	initPhaseLockedLoop();
	#endif // FREQ_CLOCK_MCU == 48000000UL
	initSysTick(FREQ_SYSTICK);
}
