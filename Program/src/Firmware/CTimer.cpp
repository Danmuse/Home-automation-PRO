/*/*!
 * @file CTimer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 00:01:36
 */

#include "CTimer.h"

CTimer::CTimer() {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_CTIMER_MASK;
	SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_CTIMER_RST_N_MASK;
	SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_CTIMER_RST_N_MASK;
//	CTIMER0->CONFIG |= (1 << 0) | (1 << 17);
//	CTIMER0->MATCH[0] = FREQ_CLOCK_MCU / period;
//	CTIMER0->MATCHREL[0] = FREQ_CLOCK_MCU / period;
//
//	CTIMER0->EV[0].STATE = 1; // Other than '0'
//	CTIMER0->EV[0].CTRL = 0 | (1 << 12);
//	CTIMER0->OUTPUT = 0xC;
//	CTIMER0->RES = 0b01010101;
//	CTIMER0->EVEN = 1;
//
//	CTIMER0->CTRL &= ~(1 << 2);
}

void CTimer::bindCTOUT(void) {

}

CTimer::~CTimer() {

}

