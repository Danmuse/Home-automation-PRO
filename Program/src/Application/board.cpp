/*/*!
 * @file board.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "board.h"

void initDevice(void) {
	#if FREQ_CLOCK_MCU != 12000000UL
	initPhaseLockedLoop();
	#endif // FREQ_CLOCK_MCU != 12000000UL
	// Setting the systick to less than 200 milliseconds will produce serious instabilities in the program.
	// The most appropriate values are from 500 milliseconds to 1000 milliseconds.
	if ((FREQ_SYSTICK < 200 && FREQ_CLOCK_MCU == 12000000UL) || (FREQ_SYSTICK < 100 && FREQ_CLOCK_MCU == 24000000UL)) while (1);
	initSysTick(FREQ_SYSTICK);
}
