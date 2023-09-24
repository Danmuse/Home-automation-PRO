/*/*!
 * @file SCTimer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 */

#include "SCTimer.h"

SCTimer::SCTimer() {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << CLK_SCT);
}

void SCTimer::SetTime(uint32_t time, uint32_t channel) {
	SCT->MATCH[channel] = time;
	SCT->MATCHREL[channel] = time;

	SCT->EV[channel].STATE = 0xFFFFFFFF;
	SCT->EV[channel].CTRL = (channel << 0) | (1 << 12);
}

void SCTimer::StartTimer(void) {
	SCT->CTRL &= ~(1 << 2);
}

void SCTimer::StopTimer(void) {
	SCT->CTRL |= (1 << 2);
}

void SCTimer::SetUnify(bool status) {
	if (status) SCT->CONFIG |= (1 << 0);
	else SCT->CONFIG &= ~(1 << 0);
}

void SCTimer::SetAutoLimit(bool status) {
	if (status) SCT->CONFIG |= (1 << 17);
	else SCT->CONFIG &= ~(1 << 17);
}

void SCTimer::SetSwitchMatrixSCTOUT(uint8_t bit, uint8_t port, uint8_t out_number) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << CLK_SWM);
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

	SYSCON->PRESETCTRL0 |= (1 << CLK_SCT);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << CLK_SWM);
}

SCTimer::~SCTimer() { }
