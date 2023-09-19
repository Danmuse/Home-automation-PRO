/*/*!
 * @file DigitalOutputs.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 */

#include "DigitalOutputs.h"

DigitalOutput::DigitalOutput(const Gpio& output, Gpio::activity_t state) : Gpio(output), Callback() {
	if (this->m_direction != Gpio::OUTPUT) this->ToggleDir();
	if (state == Gpio::HIGH) this->SetPin();
	else this->ClearPin();
	g_callback_list.push_back(this);
}

void DigitalOutput::set(void) {
	this->SetPin();
}

void DigitalOutput::clear(void) {
	this->ClearPin();
}

DigitalOutput& DigitalOutput::operator=(Gpio::activity_t state) {
	if (state == Gpio::HIGH) this->SetPin();
	else this->ClearPin();
	return *this;
}

void DigitalOutput::CallbackMethod(void) {
	if (this->GetPin()) this->SetPin();
	else this->ClearPin();
}

DigitalOutput::~DigitalOutput() { }
