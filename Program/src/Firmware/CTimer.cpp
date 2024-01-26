/*/*!
 * @file CTimer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 00:01:36
 */

#include "CTimer.h"

uint8_t CTimer::m_match_quantity = 0;
uint8_t CTimer::m_capture_quantity = 0;

CTimer *g_ctimer = nullptr;

CTimer::CTimer(const Gpio &output, channelFunction_t channelFunction) : Gpio(output),
m_channelFunction{channelFunction},
m_match_channel{(uint8_t)(channelFunction == CTIMER_MATCH ? m_match_quantity++ : 0)},
m_capture_channel{(uint8_t)(channelFunction == CTIMER_CAPTURE ? m_capture_quantity++ : 0)} {
	static bool checkInitialization = false;
	if (this->m_direction != Gpio::OUTPUT && this->m_channelFunction == CTIMER_MATCH) this->toggleDir();
	if (this->m_direction != Gpio::INPUT && this->m_channelFunction == CTIMER_CAPTURE) this->toggleDir();

	if (!checkInitialization) this->initCTimer();
	checkInitialization = true;
	this->enableSWM();
}

void CTimer::initCTimer(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_CTIMER_MASK;
	SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_CTIMER_RST_N_MASK;
	SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_CTIMER_RST_N_MASK;

	NVIC->ISER[0] |= (1 << 23); // Enable CT32B0_IRQ
//	CTIMER0->PR = 1;			// TODO: Check this instruction
}

void CTimer::enableSWM(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
	if (this->m_channelFunction == CTIMER_MATCH && this->m_match_channel < MAX_CTIMER0_MATCH_CHANNELS - 1) SWM->PINASSIGN.PINASSIGN13 &= (((0x20 * this->m_port + this->m_bit) << (8 * this->m_match_channel)) | ~(0xFF << (8 * this->m_match_channel)));
	else if (this->m_channelFunction == CTIMER_MATCH && this->m_match_channel == MAX_CTIMER0_MATCH_CHANNELS - 1) SWM->PINASSIGN.PINASSIGN14 &= (((0x20 * this->m_port + this->m_bit) << 0) | ~(0xFF << 0));
	else if (this->m_channelFunction == CTIMER_CAPTURE && this->m_capture_channel < MAX_CTIMER0_CAPTURE_CHANNELS) SWM->PINASSIGN.PINASSIGN14 &= (((0x20 * this->m_port + this->m_bit) << (8 * (this->m_capture_channel + 1))) | ~(0xFF << (8 * (this->m_capture_channel + 1))));
    SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
}

void CTimer::disableSWM(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
	if (this->m_channelFunction == CTIMER_MATCH && this->m_match_channel < MAX_CTIMER0_MATCH_CHANNELS - 1) SWM->PINASSIGN.PINASSIGN13 |= ((~(0x20 * this->m_port + this->m_bit) << (8 * this->m_match_channel)));
	else if (this->m_channelFunction == CTIMER_MATCH && this->m_match_channel == MAX_CTIMER0_MATCH_CHANNELS - 1) SWM->PINASSIGN.PINASSIGN14 |= ((~(0x20 * this->m_port + this->m_bit) << 0));
	else if (this->m_channelFunction == CTIMER_CAPTURE && this->m_capture_channel < MAX_CTIMER0_CAPTURE_CHANNELS) SWM->PINASSIGN.PINASSIGN14 |= ((~(0x20 * this->m_port + this->m_bit) << (8 * (this->m_capture_channel + 1))));
	SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
}

void CTimer::configMatch(uint32_t timeMillis, matchInterruption_t matchInterruption) {
	CTIMER0->MR[matchInterruption] = timeMillis; // (With 480) 20us --- FREQ_CLOCK_MCU / period ???
	CTIMER0->MCR |= (CTIMER_MCR_MR0I_MASK << (matchInterruption * 3)); // Interrupt on MRn: an interrupt is generated when MRn matches the value in the TC.
	CTIMER0->MCR |= (CTIMER_MCR_MR0R_MASK << (matchInterruption * 3)); // Reset on MRn: the TC will be reset if MRn matches it. 0 = disabled. 1 = enabled.
	CTIMER0->TCR = CTIMER_TCR_CRST_MASK; // Counter reset enabled.
	CTIMER0->IR = (CTIMER_IR_MR0INT_MASK << matchInterruption);	// Interrupt flag for match channel.
	CTIMER0->TCR = CTIMER_TCR_CEN_MASK; // Counter enable enabled.
	g_ctimer = this;
}

//uint8_t CTimer::getPulseState(void) {
//	return this->m_pulse;
//}
//
//void CTimer::setPulse(uint8_t value) {
//	this->m_pulse = value;
//}
//
//uint32_t CTimer::getPulseTime(void) {
//	return this->m_pulses[1] > this->m_pulses[0] ? this->m_pulses[1] - this->m_pulses[0] : ~this->m_pulses[1] + this->m_pulses[0];
//}

//void CTimer::bindChannel(void) {
//    uint8_t eventNumber = this->m_channelFunction == CTIMER_MATCH ? this->m_match_channel + 1 : this->m_capture_channel + 1;
//    // TODO: Completes the implementation if it's necessary
//    this->enableSWM();
//}
//
//void CTimer::unbindChannel(void) {
//    uint8_t eventNumber = this->m_channelFunction == CTIMER_MATCH ? this->m_match_channel + 1 : this->m_capture_channel + 1;
//    // TODO: Completes the implementation if it's necessary
//    this->disableSWM();
//}

void CTimer::CTIMER_IRQHandler(void) {
	uint32_t valor;

	// Interrupt flag for match channel 0.
	if (CTIMER0->IR & CTIMER_IR_MR0INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_MR0INT_MASK;	// Reset MR0INT interrupt flag
		CTIMER0->MCR &= ~CTIMER_MCR_MR0I_MASK;	// Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC.

		CTIMER0->TCR = ~(CTIMER_TCR_CEN_MASK | CTIMER_TCR_CRST_MASK);	// Counter enable and counter reset disabled.
		this->setPin();
//		this->configMatch(480, MAT1INT);
	}

	// Interrupt flag for match channel 1.
	if (CTIMER0->IR & CTIMER_IR_MR1INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_MR1INT_MASK;	// Reset MR1INT interrupt flag
		CTIMER0->MCR &= ~CTIMER_MCR_MR1I_MASK;	// Interrupt on MR1: an interrupt is generated when MR1 matches the value in the TC.

		CTIMER0->TCR = ~(CTIMER_TCR_CEN_MASK | CTIMER_TCR_CRST_MASK);	// Counter enable and counter reset disabled.
		this->clearPin();
//		this->configMatch(480, MAT0INT);

//		// CTIMER0->CCR |= (1 << CAP0RE) | (1 << CAP0FE);	// Habilitar captura por falling edge y rising edge
//		CTIMER0->CCR |= (1 << 0);					// Habilitar captura por falling edge
//		// CTIMER0->CCR |= (1 << 1);				// Habilitar captura por falling edge
//		valor = (~(0x000000E2) << 8);
//		valor |= 0xFF;
//		SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);			// Enable 7 = SWM
//		SWM0->PINASSIGN_DATA[14] &= valor;			// T0_CAP0 Input Timer Capture channel 0 PINASSIGN14 P029 0x1D es la captura
//		SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);		// Disable 7 = SWM
//		CTIMER0->CCR |= (1 << 2);					// Interrupt on CAPT0
//		CTIMER0->TC = 0;							// Reset la cuenta
//		CTIMER0->TCR = 1;							// Enable ctimer
	}

	// Interrupt flag for match channel 2.
	if (CTIMER0->IR & CTIMER_IR_MR2INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_MR2INT_MASK;	// Reset MR2INT interrupt flag
		// TODO: Add the code block as appropriate depending on the implementation of the program.
	}

	// Interrupt flag for match channel 3.
	if (CTIMER0->IR & CTIMER_IR_MR3INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_MR3INT_MASK;	// Reset MR3INT interrupt flag
		// TODO: Add the code block as appropriate depending on the implementation of the program.
	}

	// Interrupt flag for capture channel 0 event.
	if (CTIMER0->IR & CTIMER_IR_CR0INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_CR0INT_MASK;	// Reset CR0INT interrupt flag
		// TODO: Add the code block as appropriate depending on the implementation of the program.
	}
	// Interrupt flag for capture channel 1 event.
	if (CTIMER0->IR & CTIMER_IR_CR1INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_CR1INT_MASK;	// Reset CR1INT interrupt flag
		// TODO: Add the code block as appropriate depending on the implementation of the program.
	}
	// Interrupt flag for capture channel 2 event.
	if (CTIMER0->IR & CTIMER_IR_CR2INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_CR2INT_MASK;	// Reset CR2INT interrupt flag
		// TODO: Add the code block as appropriate depending on the implementation of the program.
	}
	// Interrupt flag for capture channel 3 event.
	if (CTIMER0->IR & CTIMER_IR_CR3INT_MASK) {
		CTIMER0->IR |= CTIMER_IR_CR3INT_MASK;	// Reset CR3INT interrupt flag
		// TODO: Add the code block as appropriate depending on the implementation of the program.
	}
}

void CTIMER0_IRQHandler(void) {
	g_ctimer->CTIMER_IRQHandler();
}

CTimer::~CTimer() {
	this->m_channelFunction == CTIMER_MATCH ? this->m_match_quantity-- : this->m_capture_quantity--;
	NVIC->ICER[0] |= (1 << 23); // Disable CT32B0_IRQ
	this->disableSWM();
}
