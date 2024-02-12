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
uint32_t PWM::m_frequency = 1000;

PWM::PWM(const Gpio &output, float duty, uint32_t frequency) : Gpio(output),
m_channel{m_quantity++} {
	if (this->m_direction != Gpio::OUTPUT) this->toggleDir();
	this->m_frequency = frequency;
	// The frequency that the PWM-controlled outputs will have in common begins.
	// The following line of code must be executed only once,
	// otherwise the SCT0->MATCH register would overflow
	if (this->m_quantity == 1) this->initSCTimer();
    this->bindChannel();
    this->setDuty(duty);
}

/*!
 * @brief Initializes the SCTimer peripheral for PWM management.
 */
void PWM::initSCTimer(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 8);
	SCT0->CONFIG |= (1 << 0) | (1 << 17);
	SCT0->MATCH[0] = FREQ_CLOCK_MCU / this->m_frequency;
	SCT0->MATCHREL[0] = FREQ_CLOCK_MCU / this->m_frequency;

	SCT0->EV[0].STATE = 1; // Other than '0'
	SCT0->EV[0].CTRL = 0 | (1 << 12);
	SCT0->OUTPUT = 0xC;
	SCT0->RES = 0b01010101;
	SCT0->EVEN = 1;

	SCT0->CTRL &= ~(1 << 2);
}

/*!
 * @brief Uses the switching matrix to link the PWM to the established pin.
 */
void PWM::enableSWM(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
    if (!this->m_channel) SWM->PINASSIGN.PINASSIGN7 &= (((0x20 * this->m_port + this->m_bit) << 24) | ~(0xFF << 24));
    else if (this->m_channel < MAX_PWM_CHANNELS - 2) SWM->PINASSIGN.PINASSIGN8 &= (((0x20 * this->m_port + this->m_bit) << (8 * (this->m_channel - 1))) | ~(0xFF << (8 * (this->m_channel - 1))));
    else if ((this->m_channel == MAX_PWM_CHANNELS - 2) || (this->m_channel == MAX_PWM_CHANNELS - 1)) SWM->PINASSIGN.PINASSIGN9 &= (((0x20 * this->m_port + this->m_bit) << (8 * (this->m_channel - 5))) | ~(0xFF << (8 * (this->m_channel - 5))));
    SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
}

/*!
 * @brief Uses the switching matrix to unlink the PWM to the established pin.
 */
void PWM::disableSWM(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
    if (!this->m_channel) SWM->PINASSIGN.PINASSIGN7 |= ((~(0x20 * this->m_port + this->m_bit)) << 24);
    else if (this->m_channel < MAX_PWM_CHANNELS - 2) SWM->PINASSIGN.PINASSIGN8 |= ((~(0x20 * this->m_port + this->m_bit)) << (8 * (this->m_channel - 1)));
    else if ((this->m_channel == MAX_PWM_CHANNELS - 2) || (this->m_channel == MAX_PWM_CHANNELS - 1)) SWM->PINASSIGN.PINASSIGN9 |= ((~(0x20 * this->m_port + this->m_bit)) << (8 * (this->m_channel - 5)));
    SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
}

/*!
 * @brief Returns the PWM duty cycle.
 * @return The PWM duty cycle.
 */
float PWM::getDuty(void) const {
    return this->m_duty;
}

/*!
 * @brief Sets the PWM duty cycle.
 * @param duty The PWM duty cycle.
 */
void PWM::setDuty(float duty) {
    if (duty > 100) duty = 100;
    else if (duty < 0) duty = 0;
    this->m_duty = duty;

    if (this->m_activity == Gpio::HIGH) duty /= 100;
    else duty = (100 - duty) / 100;

    SCT0->MATCHREL[this->m_channel + 1] = FREQ_CLOCK_MCU / (this->m_frequency) * duty;
}

/*!
 * @brief Select which of the SCTimer match registers to use
 */
void PWM::bindChannel(void) {
    uint8_t eventNumber = this->m_channel + 1;
    SCT0->EV[eventNumber].STATE = 1; // Other than '0'
    SCT0->EV[eventNumber].CTRL = eventNumber | (1 << 12); // Only uses specified match event
    SCT0->OUT[this->m_channel].SET = 1; // Turn on the first channel
    SCT0->OUT[this->m_channel].CLR = (1 << eventNumber); // Turn off the following channel
    this->enableSWM();
}
/*!
 * @brief Unselects the SCTimer match register being used
 */
void PWM::unbindChannel(void) {
    uint8_t eventNumber = this->m_channel + 1;
    SCT0->EV[eventNumber].STATE = 0;
    SCT0->EV[eventNumber].CTRL = 0;
    SCT0->OUT[this->m_channel].SET = 0;
    SCT0->OUT[this->m_channel].CLR = 0;
    this->disableSWM();
}

PWM::~PWM() {
	this->m_quantity--;
	this->unbindChannel();
}
