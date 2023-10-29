/*/*!
 * @file DAC.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "DAC.h"

DAC::DAC(channel_t channel, uint16_t max_range) : Gpio(Gpio::PORT0, channel == FST_CHANNEL ? 17 : 29, Gpio::REPEATER, Gpio::INPUT, Gpio::LOW),
m_channel{channel},
m_buffer{0},
m_max_range{max_range},
m_error{OK} {
	if (this->m_channel > SND_CHANNEL) this->m_error = ERROR;
}

DAC::error_t DAC::initialize(void) {
	if (this->m_error == OK) {
		this->EnablePower();
		this->EnableClock();
		this->EnableSWM();
		this->EnableIOCON();
	}
	return this->m_error;
}

void DAC::set(uint16_t value) {
	uint16_t auxiliar;
	this->m_buffer = value;
	if (this->m_buffer > this->m_max_range) this->m_buffer = this->m_max_range;
	auxiliar = (this->m_buffer * MAX_DAC_DEFAULT_VALUE) / this->m_max_range; // Paso del rango del usuario al rango real del registro DAC (10 bits de resolucion)
	if (this->m_channel == FST_CHANNEL) DAC0->CR = (auxiliar << DAC_CR_VALUE_SHIFT);
	else if (this->m_channel == SND_CHANNEL) DAC1->CR = (auxiliar << DAC_CR_VALUE_SHIFT);
}

uint16_t DAC::get(void) const {
	return this->m_buffer;
}

void DAC::setMaxRange(uint16_t max_range) {
	this->m_max_range = max_range;
	if (this->m_buffer > this->m_max_range) this->m_buffer = this->m_max_range;
}

uint16_t DAC::getMaxRange(void) const {
	return this->m_max_range;
}

DAC& DAC::operator=(uint16_t value) {
	this->set(value);
	return *this;
}

bool DAC::operator==(uint16_t value) const {
	return (this->m_buffer == value) ? true : false;
}

bool DAC::operator<(uint16_t value) const {
	return (this->m_buffer < value) ? true : false;
}

bool DAC::operator<=(uint16_t value) const {
	return (this->m_buffer <= value) ? true : false;
}

bool DAC::operator>(uint16_t value) const {
	return (this->m_buffer > value) ? true : false;
}

bool DAC::operator>=(uint16_t value) const {
	return (this->m_buffer >= value) ? true : false;
}

bool DAC::operator!=(uint16_t value) const {
	return (this->m_buffer != value) ? true : false;
}

DAC::~DAC() {
	if (this->m_error == OK) {
		this->DisableSWM();
		this->DisableClock();
		this->DisablePower();
	}
}

void DAC::EnablePower(void) {
	SYSCON->PDRUNCFG &= ~(1 << (13 + this->m_channel));
}

void DAC::EnableClock(void) {
	if (this->m_channel == FST_CHANNEL) SYSCON->SYSAHBCLKCTRL0 |= (1 << FST_SYS_OUT);
	else if (this->m_channel == SND_CHANNEL) SYSCON->SYSAHBCLKCTRL1 |= (1 << SND_SYS_OUT);
}

void DAC::EnableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_channel == FST_CHANNEL) SWM->PINENABLE0 |= (1 << FST_SWM_OUT);
	else if (this->m_channel == SND_CHANNEL) SWM->PINENABLE0 |= (1 << SND_SWM_OUT);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void DAC::EnableIOCON(void) {
	IOCON->PIO[IOCON_INDEX_PIO0[this->m_bit]] |= (1 << IOCON_DAC);
}

void DAC::DisablePower(void) {
	SYSCON->PDRUNCFG |= (1 << (13 + this->m_channel));
}

void DAC::DisableClock(void) {
	if (this->m_channel == FST_CHANNEL) SYSCON->SYSAHBCLKCTRL0 &= ~(1 << FST_SYS_OUT);
	else if (this->m_channel == SND_CHANNEL) SYSCON->SYSAHBCLKCTRL1 &= ~(1 << SND_SYS_OUT);
}

void DAC::DisableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_channel == FST_CHANNEL) SWM->PINENABLE0 &= ~(1 << FST_SWM_OUT);
	else if (this->m_channel == SND_CHANNEL) SWM->PINENABLE0 &= ~(1 << SND_SWM_OUT);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void DAC::DisableIOCON(void) {
	IOCON->PIO[IOCON_INDEX_PIO0[this->m_bit]] &= ~(1 << IOCON_DAC);
}
