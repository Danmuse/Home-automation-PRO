/*/*!
 * @file PWM.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 12/08/2023 00:11:27
 */

#include "PWM.h"

uint8_t PWM::m_quantity = 0;
uint32_t PWM::m_period = 1000;

PWM::PWM(const Gpio &output, float duty, uint32_t period) : Gpio(output),
m_channel{m_quantity++} {
	if (this->m_direction != Gpio::OUTPUT) this->ToggleDir();
	this->m_period = period;
	// The period that the PWM-controlled outputs will have in common begins.
	// The following line of code must be executed only once,
	// otherwise the SCT->MATCH register would overflow
	if (this->m_quantity == 1) this->initPWM();
    this->enable();
    this->setDuty(duty);
}

void PWM::initPWM(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7) | (1 << 8);
	SCT->CONFIG |= (1 << 0) | (1 << 17);
	SCT->MATCH[0] = FREQ_CLOCK_MCU / this->m_period;
	SCT->MATCHREL[0] = FREQ_CLOCK_MCU / this->m_period;

	SCT->EV[0].STATE = 1; // Other than '0'
	SCT->EV[0].CTRL = 0 | (1 << 12);
	SCT->OUTPUT = 0xC;
	SCT->RES = 0b01010101;
	SCT->EVEN = 1;

	SCT->CTRL &= ~(1 << 2);
}

void PWM::bind(void) const {
    uint8_t offset;
    if (this->m_channel > 0 && this->m_channel < MAX_PWM_CHANNELS) {
        offset = (8 * (this->m_channel - 1));
        SWM0->PINASSIGN.PINASSIGN8 &= (((32 * this->m_port + this->m_bit) << offset) | ~(255 << offset)); // Have to set since bit 24
    } else {
        offset = 24;
        SWM0->PINASSIGN.PINASSIGN7 &= (((32 * this->m_port + this->m_bit) << offset) | ~(255 << offset)); // Have to set since bit 24
    }
}

void PWM::unbind(void) const {
    uint8_t offset;
    if (this->m_channel > 0 && this->m_channel < MAX_PWM_CHANNELS) {
        offset = (8 * (this->m_channel - 1));
        SWM0->PINASSIGN.PINASSIGN8 &= (~(255 << offset)); // Have to set since bit 24
    } else {
        offset = 24;
        SWM0->PINASSIGN.PINASSIGN7 &= (~(255 << offset)); // Have to set since bit 24
    }
}

float PWM::getDuty(void) const {
    return this->m_duty;
}

void PWM::setDuty(float duty) {
    if (duty > 100) duty = 100;
    else if (duty < 0) duty = 0;
    this->m_duty = duty;

    if (this->m_activity == Gpio::HIGH) duty /= 100;
    else duty = (100 - duty) / 100;

    SCT->MATCHREL[this->m_channel + 1] = FREQ_CLOCK_MCU / (this->m_period) * duty;
}

void PWM::enable(void) {
    uint8_t eventNumber = this->m_channel + 1;

    SCT->EV[eventNumber].STATE = 1; // Other than '0'
    SCT->EV[eventNumber].CTRL = eventNumber | (1 << 12); // Only uses specified match event
    SCT->OUT[this->m_channel].SET = 1; // Turn on the first channel
    SCT->OUT[this->m_channel].CLR = (1 << eventNumber); // Turn off the following channel

    this->bind();
}

void PWM::disable(void) {
    uint8_t eventNumber = this->m_channel + 1;

    SCT->EV[eventNumber].STATE = 0;
    SCT->EV[eventNumber].CTRL = 0;
    SCT->OUT[this->m_channel].SET = 0;
    SCT->OUT[this->m_channel].CLR = 0;

    this->unbind();
}

PWM::~PWM() { }

