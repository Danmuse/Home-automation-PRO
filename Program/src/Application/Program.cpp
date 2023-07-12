/*!
 * @file CallbackProgram.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:16:24
 */

#include <cr_section_macros.h>
#include "board.h"

void toggleRed(void) {
	static uint32_t counterTicks = 0;
	counterTicks++;
	counterTicks %= 125;
	if (!counterTicks) {
		// flagInterruptTicks = 1;
		LED_RED.SetTogglePin();
	}
}

void toggleGreen(void) {
	static uint32_t counterTicks = 0;
	counterTicks++;
	counterTicks %= 250;
	if (!counterTicks) {
		// flagInterruptTicks = 1;
		LED_GREEN.SetTogglePin();
	}
}

void toggleBlue(void) {
	static uint32_t counterTicks = 0;
	counterTicks++;
	counterTicks %= 500;
	if (!counterTicks) {
		// flagInterruptTicks = 1;
		LED_BLUE.SetTogglePin();
	}
}

int main(void) {
	initDevice();

	Callback Red(toggleRed);
	Callback Green(toggleGreen);
	Callback Blue(toggleBlue);

	while(1);
}
