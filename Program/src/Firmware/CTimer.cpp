/*/*!
 * @file CTimer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 00:01:36
 */

#include "CTimer.h"

CTimer::CTimer() :
m_pulse{0} {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_CTIMER_MASK;
	SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_CTIMER_RST_N_MASK;
	SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_CTIMER_RST_N_MASK;

	CTIMER0->PR = 1;			// TODO: Check!! Prescaler --> clock dividido 2
	NVIC->ISER[0] = (1 << 23); 	// TODO: Check!! Enable interrupt ISE_CT32B0_IRQ

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

/*

void CTimer::configMatch(void) {
	CTIMER0->MR[0] = MATCH0;	// MR0
	CTIMER0->MCR |= (1 << 0);	// Interrupt on MATCH0
	CTIMER0->MCR |= (1 << 1);	// Reset CTimer on interrupt MATCH0
	CTIMER0->TCR = (1 << 1);	// Reset CTimer
	CTIMER0->IR = 1;
	CTIMER0->TCR = 1;
}

uint8_t CTimer::getPulseState(void) {
	return this->m_pulse;
}

void CTimer::setPulse(uint8_t value) {
	this->m_pulse = value;
}

uint32_t CTimer::getPulseTime(void) {
	return this->m_pulses[1] > this->m_pulses[0] ? this->m_pulses[1] - this->m_pulses[0] : ~this->m_pulses[1] + this->m_pulses[0];
}

void CTIMER0_IRQHandler(void) {
	static uint8_t index = 0;
	uint32_t valor;

	// Interrupcion por MR0
	if (CTIMER0->IR & 0x01) {
		CTIMER0->IR |= (1 << 0);					// Reset flag MROint
		CTIMER0->MCR &= ~(1 << 0);					// Disable Interrupt on MATCH0
		CTIMER0->MR[1] = MATCH1;					// MR1
		CTIMER0->MCR |= (1 << 3);					// Interrupt on MATCH1
		// CTIMER0->MCR |= (1 << 4);				// Reset timer on interrupt MATCH1
		GPIO_Set(TRIGGER, 1);
	}

	// Interrupcion por MR1
	if (CTIMER0->IR & 0x02) {
		CTIMER0->IR |= (1 << 1);					// Reset flag MR1int

		CTIMER0->TCR = 0;							// Disable ctimer
		CTIMER0->MCR &= ~(1 << 0);					// Disable Interrupt on MATCH0
		CTIMER0->MCR &= ~(1 << 3);					// Disable Interrupt on MATCH1
		// CTIMER0->MR[0] = MATCH0;					// MR0
		// CTIMER0->MCR |= (1 << 0);				// Interrupt on MATCH0
		// CTIMER0->MCR |= (1 << 1);				// Reset timer on interrupt MATCH0
		GPIO_Set(TRIGGER, 0);

		// CTIMER0->CCR |= (1 << CAP0RE) | (1 << CAP0FE);	//Habilitar captura por falling edge y rising edge
		CTIMER0->CCR |= 1 << CAP0RE;				// Habilitar captura por falling edge
		// CTIMER0->CCR |= 1 << CAP0FE;				// Habilitar captura por falling edge
		valor = (~(0x000000E2) << 8);
		valor |= 0xFF;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);			// Enable 7 = SWM
		SWM0->PINASSIGN_DATA[14] &= valor;			// T0_CAP0 Input Timer Capture channel 0 PINASSIGN14 P029 0x1D es la captura
		SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);		// Disable 7 = SWM
		CTIMER0->CCR |= (1 << CAP0I);				// Interrupt on CAPT0
		CTIMER0->TC = 0;							// Reset la cuenta
		CTIMER0->TCR = 1;							// Enable ctimer
	}

	// Interrupcion por CR0INT captura
	if (CTIMER0->IR & 0x10) {
		CTIMER0->IR |= (1 << 4);					// Reset flag CR0INT
		pulsos[index] = CTIMER0->CR[0];
		index++;

		if (index == 1) {
			CTIMER0->CCR &= ~(1 << CAP0RE);			// Deshabilitar captura por rising edge
			CTIMER0->CCR |= 1 << CAP0FE;			// Habilitar captura por falling edge
		}

		if (index == 2 ) {
			index = 0;
			CTIMER0->TCR = 0;
			CTIMER0->CCR &= ~(1 << CAP0FE);			// Deshabilitar captura por falling edge
			CTIMER0->CCR &= ~(1 << CAP0RE);			// Deshabilitar captura por rising edge

			valor |= 0xFFFFFFFF;
			SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);		// Enable 7 = SWM
			SWM0->PINASSIGN_DATA[14] |= valor;		// Reset T0_CAP0 Input Timer Capture channel 0 PINASSIGN14 P029 0x1D es la captura
			SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);	// Disable 7 = SWM
			CTIMER0->CCR &= ~(1 << CAP0I);			// No Interrupt on CAPT0
			CTIMER0->TCR = 1;						// Enable ctimer
			flagProcesarPulso = 1;
		}
		// CTIMER0->TC = 0; // Reset la cuenta
	}
}
*/

CTimer::~CTimer() { }
