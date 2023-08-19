/*/*!
 * @file PWM.cpp
 * @par Author
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 12/08/2023 00:11:27
 */

#include "PWM.h"

// Copying object by reference because it is a global object which won't be destroyed
PWM::PWM(const Gpio &outputPort, const uint8_t pwmOutput, float duty, uint32_t frequency) :
m_outputPort{outputPort},
m_pwmOutput{pwmOutput},
m_frequency{frequency} {
    this->enable();
    setDuty(duty);
}

void PWM::bindOutput(const Gpio::port_t port, const uint8_t bit) const {
    uint8_t offset;
    if (m_pwmOutput > 0 && m_pwmOutput < 7) {
        offset = (8 * (m_pwmOutput - 1));
        SWM0->PINASSIGN.PINASSIGN8 &= (((32 * port + bit) << offset) | ~(255 << offset)); // Have to set since bit 24
    }
    else {
        offset = 24; // Ignoring > 7
        SWM0->PINASSIGN.PINASSIGN7 &= (((32 * port + bit) << offset) | ~(255 << offset)); // Have to set since bit 24
    }
}

void PWM::unbindOutput(const Gpio::port_t port, const uint8_t bit) const {
    uint8_t offset;
    if (m_pwmOutput > 0 && m_pwmOutput < 7) {
        offset = (8 * (m_pwmOutput - 1));
        SWM0->PINASSIGN.PINASSIGN8 &= (~(255 << offset)); // Have to set since bit 24
    }
    else {
        offset = 24; // Ignoring > 7
        SWM0->PINASSIGN.PINASSIGN7 &= (~(255 << offset)); // Have to set since bit 24
    }
}

float PWM::getDuty(void) const {
    return m_duty;
}

void PWM::setDuty(float duty) {
    if (duty > 100) duty = 100;
    else if (duty < 0) duty = 0;
    m_duty = duty;

    if (m_outputPort.getActivity() == Gpio::HIGH) duty /= duty;
    else duty = (100 - duty) / 100;

    SCT->MATCHREL[m_pwmOutput + 1] = FREQ_CLOCK_MCU / (m_frequency) * duty;
}

void PWM::enable(void) {
    uint8_t eventNumber = m_pwmOutput + 1;

    SCT->EV[eventNumber].STATE = 1;// Other than '0'
    SCT->EV[eventNumber].CTRL = eventNumber | (1 << 12); // Only uses specified match event
    SCT->OUT[m_pwmOutput].SET = 1; // Turn on the first channel
    SCT->OUT[m_pwmOutput].CLR = (1 << 1); // Turn off the following channel

    bindOutput(m_outputPort.m_port, m_outputPort.m_bit); // TODO: Refactor this method
}

void PWM::disable(void) {
    uint8_t eventNumber = m_pwmOutput + 1;

    SCT->EV[eventNumber].STATE = 0;
    SCT->EV[eventNumber].CTRL = 0;
    SCT->OUT[m_pwmOutput].SET = 0;
    SCT->OUT[m_pwmOutput].CLR = 0;

    unbindOutput(m_outputPort.m_port, m_outputPort.m_bit); // TODO: Refactor this method
}

PWM::~PWM() { }
