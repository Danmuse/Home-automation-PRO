/*/*!
 * @file DigitalOutputs.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 */

#include "DigitalOutputs.h"

DigitalOutput::DigitalOutput(const Gpio& output, Gpio::activity_t state) : Gpio(output), Callback() {
	if (this->m_direction != Gpio::OUTPUT) this->toggleDir();
	if (state == Gpio::HIGH) this->setPin();
	else this->clearPin();
	g_callback_list.push_back(this);
}

void DigitalOutput::set(void) {
    this->setPin();
}

void DigitalOutput::clear(void) {
    this->clearPin();
}

DigitalOutput& DigitalOutput::operator=(Gpio::activity_t state) {
	if (state == Gpio::HIGH) this->setPin();
	else this->clearPin();
	return *this;
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void DigitalOutput::callbackMethod(void) {
	if (this->getPin()) this->setPin();
	else this->clearPin();
}

#pragma GCC pop_options