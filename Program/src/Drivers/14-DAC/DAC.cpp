/*/*!
 * @file DAC.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "DAC.h"

DAC::DAC(channel_t channel, uint16_t max_range) : Gpio(Gpio::PORT0, channel == DAC_CHANNEL_0 ? 17 : 29, Gpio::REPEATER, Gpio::INPUT, Gpio::LOW),
m_channel{channel},
m_buffer{0},
m_max_range{max_range},
m_error{OK} {
	if (this->m_channel > DAC_CHANNEL_1) this->m_error = ERROR;
}

DAC::error_t DAC::Inicializar(void) {
	if (this->m_error == OK) {
		this->PowerDAC();
		this->EnableClock();
		this->EnableSWM();
		this->EnableIOCONDAC();
	}
	return this->m_error;
}

void DAC::Set(uint16_t value) {
	uint16_t auxiliar;
	this->m_buffer = value;
	if (this->m_buffer > this->m_max_range) this->m_buffer = this->m_max_range;
	auxiliar = (this->m_buffer * MAX_DAC_DEFAULT_VALUE) / this->m_max_range; // Paso del rango del usuario al rango real del registro DAC (10 bits de resolucion)
	if (this->m_channel == DAC_CHANNEL_0) DAC0->CR = (auxiliar << DAC_CR_VALUE_SHIFT);
	else if (this->m_channel == DAC_CHANNEL_1) DAC1->CR = (auxiliar << DAC_CR_VALUE_SHIFT);
}

uint16_t DAC::Get(void) const {
	return this->m_buffer;
}

void DAC::SetMaxRange(uint16_t max_range) {
	this->m_max_range = max_range;
	if (this->m_buffer > this->m_max_range) this->m_buffer = this->m_max_range;
}

uint16_t DAC::GetMaxRange(void) const {
	return this->m_max_range;
}

DAC& DAC::operator=(uint16_t value) {
	this->Set(value);
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
		this->UnPowerDAC();
	}
}

void DAC::PowerDAC(void) {
	SYSCON->PDRUNCFG &= ~(1 << (13 + this->m_channel));
}

void DAC::EnableClock(void) {
	if (this->m_channel == DAC_CHANNEL_0) SYSCON->SYSAHBCLKCTRL0 |= (1 << SYS_DACOUT_0);
	else if (this->m_channel == DAC_CHANNEL_1) SYSCON->SYSAHBCLKCTRL1 |= (1 << SYS_DACOUT_1);
}

void DAC::EnableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_channel == DAC_CHANNEL_0) SWM->PINENABLE0 |= (1 << SWM_DACOUT_0);
	else if (this->m_channel == DAC_CHANNEL_1) SWM->PINENABLE0 |= (1 << SWM_DACOUT_1);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void DAC::EnableIOCONDAC(void) {
	IOCON->PIO[IOCON_INDEX_PIO0[this->m_bit]] |= (1 << IOCON_DAC);
}

void DAC::UnPowerDAC(void) {
	SYSCON->PDRUNCFG |= (1 << (13 + this->m_channel));
}

void DAC::DisableClock(void) {
	if (this->m_channel == DAC_CHANNEL_0) SYSCON->SYSAHBCLKCTRL0 &= ~(1 << SYS_DACOUT_0);
	else if (this->m_channel == DAC_CHANNEL_1) SYSCON->SYSAHBCLKCTRL1 &= ~(1 << SYS_DACOUT_1);
}

void DAC::DisableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_channel == DAC_CHANNEL_0) SWM->PINENABLE0 &= ~(1 << SWM_DACOUT_0);
	else if (this->m_channel == DAC_CHANNEL_1) SWM->PINENABLE0 &= ~(1 << SWM_DACOUT_1);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void DAC::DisableIOCONDAC(void) {
	IOCON->PIO[IOCON_INDEX_PIO0[this->m_bit]] &= ~(1 << IOCON_DAC);
}

