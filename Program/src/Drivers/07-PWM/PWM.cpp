/*/*!
 * @file PWM.cpp
 * @par Author & Doxygen Editor
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@frba.utn.edu.ar</a>
 * @date 12/08/2023 00:11:27
 */

#include "PWM.h"

PWM::PWM(const Gpio &outputPort, const uint8_t pwmOutput, float duty, uint32_t frequency) :
        m_outputPort{outputPort}, m_pwmOutput{pwmOutput},m_frequency{frequency} {
    //Copying object by reference because it is a global object which won't be destroyed

    this->enable();

    setDuty(duty);

}


void PWM::bindOutput(const Gpio::port_t port, const uint8_t bit) const {
    uint8_t offset;
    if (m_pwmOutput > 0 && m_pwmOutput < 7) {
        offset = (8 * (m_pwmOutput - 1));
        SWM0->PINASSIGN.PINASSIGN8 &= (((32 * port + bit) << offset) | ~(255 << offset)); //Hve to set since bit 24
    }
    else {
        //Ignoring >7....
        offset = 24;
        SWM0->PINASSIGN.PINASSIGN7 &= (((32 * port + bit) << offset) | ~(255 << offset)); //Hve to set since bit 24
    }
}

void PWM::unbindOutput(const Gpio::port_t port, const uint8_t bit) const {
    uint8_t offset;
    if (m_pwmOutput > 0 && m_pwmOutput < 7) {
        offset = (8 * (m_pwmOutput - 1));
        SWM0->PINASSIGN.PINASSIGN8 &= (~(255 << offset)); //Have to set since bit 24
    }
    else {
        //Ignoring >7....
        offset = 24;
        SWM0->PINASSIGN.PINASSIGN7 &= (~(255 << offset)); //Have to set since bit 24
    }
}

float PWM::getDuty() const {
    return m_duty;
}

void PWM::setDuty(float duty) {
    if (duty > 100) {
        duty = 100;
    }
    else if (duty < 0) {
        duty = 0;
    }
    m_duty = duty;


    if (m_outputPort.getActivity() == Gpio::HIGH) {
        duty /= duty;
    }
    else {
        duty = (100 - duty) / 100;
    }

    SCT->MATCHREL[m_pwmOutput + 1] = FREQ_CLOCK_MCU / (m_frequency) * duty;

}

void PWM::enable() {

    uint8_t eventNumber = m_pwmOutput + 1;

    SCT->EV[eventNumber].STATE = 1;// different thaan 0
    SCT->EV[eventNumber].CTRL = eventNumber | (1 << 12);  //Only uses specified match event
    SCT->OUT[m_pwmOutput].SET = 1; //Enciende el PWM el eveento 0
    SCT->OUT[m_pwmOutput].CLR = (1 << 1);//Apagaa el PWM el evento 1

    bindOutput(m_outputPort.m_port, m_outputPort.m_bit); //TODO: HORRIBLE ESTE AVVESO
}

void PWM::disable() {
    //EVENTOS, OUT y SWM pero no toco las variables
    uint8_t eventNumber = m_pwmOutput + 1;

    SCT->EV[eventNumber].STATE = 0;
    SCT->EV[eventNumber].CTRL = 0;
    SCT->OUT[m_pwmOutput].SET = 0;
    SCT->OUT[m_pwmOutput].CLR = 0;

    unbindOutput(m_outputPort.m_port, m_outputPort.m_bit); //TODO: HORRIBLE ESTE AVVESO

}


