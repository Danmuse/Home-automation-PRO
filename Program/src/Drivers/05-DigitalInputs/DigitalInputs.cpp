/*/*!
 * @file DigitalInputs.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 */

#include "DigitalInputs.h"

DigitalInput::DigitalInput(const Gpio& input, uint8_t maxBounces) : Gpio(input), Callback(),
m_maxBounces{maxBounces},
m_bounceCounter{0},
m_lastPressed{false} {
	if (this->m_direction != Gpio::INPUT) this->toggleDir();
	this->m_currentPressed = this->getPin();
	g_callback_list.push_back(this);
}

bool DigitalInput::getHW(void) {
	return this->getPin() ? true : false;
}

void DigitalInput::getSW(bool statusPressed) {
	if (statusPressed == false) {
		this->m_bounceCounter = 0;
		this->m_lastPressed = false;
	} else if (this->m_bounceCounter == 0) {
		this->m_lastPressed = statusPressed;
		this->m_bounceCounter++;
	} else if (this->m_lastPressed == statusPressed) {
		if (this->m_bounceCounter < this->m_maxBounces) this->m_bounceCounter++;
		else if (this->m_bounceCounter == this->m_maxBounces) {
			this->m_currentPressed = this->m_lastPressed;
			this->m_bounceCounter++;
		}
	} else {
		this->m_bounceCounter = 0;
		this->m_lastPressed = false;
	}
}

void DigitalInput::callbackMethod(void) {
	bool keyPressed = this->getHW();
	this->getSW(keyPressed);
}

bool DigitalInput::get(void) {
	bool statusPressed = this->m_currentPressed;
	this->m_currentPressed = false;
	return statusPressed;
}

DigitalInput::operator bool(void) {
	return this->get();
}

bool DigitalInput::operator!(void) {
	return !this->get();
}

DigitalInput::~DigitalInput() { }
