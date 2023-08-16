/*!
 * @file PWM.h
 * @par Author & Doxygen Editor
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@frba.utn.edu.ar</a>
 * @date 12/08/2023 00:11:27
 * @brief Pulse Width Modulation Class API (Application Programming Interface).
 */

#ifndef PWM_H_
#define PWM_H_

#include "GPIO.h"


//Todo: maybe create interface and change this class name to simplePWM
class PWM {//PERIOD NEEDS TO BE SET BEFORE, otherwise breaks. PWM should be not centered
    protected:
        Gpio m_outputPort; //Output Gpio
        uint8_t m_pwmOutput; //Port can be btween 0 and 6
        float m_duty; //duty is a % value of the time on
        uint32_t m_frequency;

    public:
        PWM()=delete;
        PWM(const Gpio &outputPort, const uint8_t pwmOutput, float duty, uint32_t frequency);

        void bindOutput(const Gpio::port_t port, const uint8_t bit) const;

        void unbindOutput(const Gpio::port_t port, const uint8_t bit) const;

        float getDuty() const;

        void setDuty(float duty);

        void enable();

        void disable();


};

#endif /* PWM_H_ */
