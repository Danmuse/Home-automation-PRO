/*/*!
 * @file SCTimer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 */

#include "SCTimer.h"

SCTimer::SCTimer(uint8_t period) {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SCT_MASK;
	SCT0->CONFIG |= (1 << 0) | (1 << 17);
	SCT0->MATCH[0] = FREQ_CLOCK_MCU / period;
	SCT0->MATCHREL[0] = FREQ_CLOCK_MCU / period;

	SCT0->EV[0].STATE = 1; // Other than '0'
	SCT0->EV[0].CTRL = 0 | (1 << 12);
	SCT0->OUTPUT = 0xC;
	SCT0->RES = 0b01010101;
	SCT0->EVEN = 1;

	SCT0->CTRL &= ~(1 << 2);
}

void SCTimer::setTime(uint32_t time, uint32_t channel) {
	SCT0->MATCH[channel] = time;
	SCT0->MATCHREL[channel] = time;

	SCT0->EV[channel].STATE = 0xFFFFFFFF;
	SCT0->EV[channel].CTRL = (channel << 0) | (1 << 12);
}

void SCTimer::startTimer(void) {
	SCT0->CTRL &= ~(1 << 2);
}

void SCTimer::stopTimer(void) {
	SCT0->CTRL |= (1 << 2);
}

void SCTimer::setUnify(bool status) {
	if (status) SCT0->CONFIG |= (1 << 0);
	else SCT0->CONFIG &= ~(1 << 0);
}

void SCTimer::setAutoLimit(bool status) {
	if (status) SCT0->CONFIG |= (1 << 17);
	else SCT0->CONFIG &= ~(1 << 17);
}

void SCTimer::bindSCTOUT(uint8_t bit, uint8_t port, uint8_t out_number) {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
	uint8_t aux = ~(bit + port * 0x20); // Default: 0xFF

	switch (out_number) {
	case 0:
		SWM->PINASSIGN_DATA[7] &= ~(aux << 24);
		break;
	case 1:
		SWM->PINASSIGN_DATA[8] &= ~(aux << 0);
		break;
	case 2:
		SWM->PINASSIGN_DATA[8] &= ~(aux << 8);
		break;
	case 3:
		SWM->PINASSIGN_DATA[8] &= ~(aux << 16);
		break;
	case 4:
		SWM->PINASSIGN_DATA[8] &= ~(aux << 24);
		break;
	case 5:
		SWM->PINASSIGN_DATA[9] &= ~(aux << 0);
		break;
	case 6:
		SWM->PINASSIGN_DATA[9] &= ~(aux << 8);
		break;
	}

	SYSCON->PRESETCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SCT_MASK;
	SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
}
