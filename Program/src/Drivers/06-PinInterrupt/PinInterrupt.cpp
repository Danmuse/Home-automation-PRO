/*/*!
 * @file PinInterrupt.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 */

#include "PinInterrupt.h"

uint8_t PinInterrupt::m_quantity = 0;

PinInterrupt *g_gpiohandler[MAX_PIN_INTERRUPTS];

PinInterrupt::PinInterrupt(const Gpio& interrupt, Gpio::interrupt_mode_t interrupt_mode) : Gpio(interrupt),
m_interrupt_mode{interrupt_mode},
m_interrupt_number{this->m_quantity} {
	if (this->m_direction != Gpio::INPUT) this->ToggleDir();
	// SYSTEM STOPPED, MAXIMUM PIN INTERRUPTIONS REACHED
	if (this->m_quantity >= MAX_PIN_INTERRUPTS) while (1);
	g_gpiohandler[this->m_interrupt_number] = this;
	this->m_quantity++;
}

void PinInterrupt::EnableClockInterrupt(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 28);
}

void PinInterrupt::enableInterrupt(void) {
	SYSCON->PINTSEL[this->m_interrupt_number] = this->m_bit + this->m_port * 32;
}

void PinInterrupt::disableInterrupt(void) {
	SYSCON->PINTSEL[this->m_interrupt_number] = 0;
}

void PinInterrupt::initInterrupt(void) {
	if (this->m_quantity == 1) this->EnableClockInterrupt();

	SYSCON->PINTSEL[this->m_interrupt_number] = this->m_bit + this->m_port * 32;
	NVIC->ISER[0] |= (1 << (24 + this->m_interrupt_number));

	if (this->m_interrupt_mode != Gpio::LOW_LEVEL && this->m_interrupt_mode != Gpio::HIGH_LEVEL)
		PIN_INTERRUPT->ISEL &= ~(1 << this->m_interrupt_number);
	else PIN_INTERRUPT->ISEL |= (1 << this->m_interrupt_number);

	if (this->m_interrupt_mode != Gpio::FALLING)
		PIN_INTERRUPT->SIENR |= (1 << this->m_interrupt_number);
	if (this->m_interrupt_mode == Gpio::FALLING || this->m_interrupt_mode == Gpio::CHANGE || this->m_interrupt_mode == Gpio::HIGH_LEVEL)
		PIN_INTERRUPT->SIENF |= (1 << this->m_interrupt_number);
	if (this->m_interrupt_mode == Gpio::LOW_LEVEL)
		PIN_INTERRUPT->CIENF |= (1 << this->m_interrupt_number);

	PIN_INTERRUPT->IST |= (1 << this->m_interrupt_number);
}

PinInterrupt::~PinInterrupt() {
    this->disableInterrupt();
	this->m_quantity--;
}

void PININT0_IRQHandler(void) {
    g_gpiohandler[0]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 0);
}

void PININT1_IRQHandler(void) {
    g_gpiohandler[1]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 1);
}

void PININT2_IRQHandler(void) {
    g_gpiohandler[2]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 2);
}

void PININT3_IRQHandler(void) {
    g_gpiohandler[3]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 3);
}

void PININT4_IRQHandler(void) {
    g_gpiohandler[4]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 4);
}

void PININT5_IRQHandler(void) {
    g_gpiohandler[5]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 5);
}

void PININT6_IRQHandler(void) {
    g_gpiohandler[6]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 6);
}

void PININT7_IRQHandler(void) {
    g_gpiohandler[7]->gpioHandler();
	PIN_INTERRUPT->IST |= (1 << 7);
}
