/*/*!
 * @file DAC.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "DAC.h"

DAC *g_dacExternal = nullptr;

DAC::DAC(channelDAC_t channel) : Gpio(Gpio::PORT0, channel == DAC_0 ? 17 : 29, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH),
m_channel{channel},
m_buffer{0},
m_max_range{(DAC_CR_VALUE_MASK >> DAC_CR_VALUE_SHIFT)} {
	this->initialize();
}

void DAC::initialize(void) {
	this->bindChannel();
    this->enableIOCON();
}

void DAC::analogWrite(uint16_t value) {
	uint16_t auxiliar;
	this->m_buffer = value;
	if (this->m_buffer > this->m_max_range) this->m_buffer = this->m_max_range;
	auxiliar = (this->m_buffer * (DAC_CR_VALUE_MASK >> DAC_CR_VALUE_SHIFT)) / this->m_max_range;
	if (this->m_channel == DAC_0) DAC0->CR = (auxiliar << DAC_CR_VALUE_SHIFT);
	else if (this->m_channel == DAC_1) DAC1->CR = (auxiliar << DAC_CR_VALUE_SHIFT);
}

uint16_t DAC::readBuffer(void) const {
	return this->m_buffer;
}

void DAC::bindChannel(void) {
    this->enablePower();
    this->enableClock();
    this->enableSWM();
}

void DAC::unbindChannel(void) {
    this->disablePower();
    this->disableClock();
    this->disableSWM();
}

void DAC::setMaxRange(uint16_t max_range) {
	if (max_range < (DAC_CR_VALUE_MASK >> DAC_CR_VALUE_SHIFT)) this->m_max_range = max_range;
	if (this->m_buffer > this->m_max_range) this->m_buffer = this->m_max_range;
}

uint16_t DAC::getMaxRange(void) const {
	return this->m_max_range;
}

void DAC::enablePower(void) {
	SYSCON->PDRUNCFG &= ~(1 << (13 + this->m_channel));
}

void DAC::enableClock(void) {
	if (this->m_channel == DAC_0) SYSCON->SYSAHBCLKCTRL0 |= (1 << FST_SYS_OUT);
	else if (this->m_channel == DAC_1) SYSCON->SYSAHBCLKCTRL1 |= (1 << SND_SYS_OUT);
}

void DAC::enableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_channel == DAC_0) SWM->PINENABLE0 &= ~(1 << FST_SWM_OUT);
	else if (this->m_channel == DAC_1) SWM->PINENABLE0 &= ~(1 << SND_SWM_OUT);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void DAC::enableIOCON(void) {
	IOCON->PIO[IOCON_INDEX_PIO0[this->m_bit]] |= (1 << IOCON_DAC);
}

void DAC::disablePower(void) {
	SYSCON->PDRUNCFG |= (1 << (13 + this->m_channel));
}

void DAC::disableClock(void) {
	if (this->m_channel == DAC_0) SYSCON->SYSAHBCLKCTRL0 &= ~(1 << FST_SYS_OUT);
	else if (this->m_channel == DAC_1) SYSCON->SYSAHBCLKCTRL1 &= ~(1 << SND_SYS_OUT);
}

void DAC::disableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_channel == DAC_0) SWM->PINENABLE0 |= (1 << FST_SWM_OUT);
	else if (this->m_channel == DAC_1) SWM->PINENABLE0 |= (1 << SND_SWM_OUT);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void DAC::disableIOCON(void) {
	IOCON->PIO[IOCON_INDEX_PIO0[this->m_bit]] &= ~(1 << IOCON_DAC);
}

DAC::~DAC() {
	this->unbindChannel();
}

//////////////////////////
/// DAC initialization ///
//////////////////////////

void initDAC(void) {
	#if defined(CN7_PINS) && defined(DAC_SND_CHANNEL_ENABLED)

	static DAC dac(DAC::DAC_1);

	g_dacExternal = &dac;

	#endif // defined(CN7_PINS) && defined(DAC_SND_CHANNEL_ENABLED)
}
