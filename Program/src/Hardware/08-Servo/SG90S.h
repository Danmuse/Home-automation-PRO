#ifndef SERVOSG90S_H
#define SERVOSG90S_H

#include "GPIO.h"
#include "PWM.h"

#define MAX_ANGLE 180
#define MIN_DUTY 5
#define PULSE_MAX_INCREMENT 5

class SG90S {
    private:
        PWM m_pwm;

    public:
        SG90S(Gpio& controlPin);

        void setAngle(int angle);

        ~SG90S() = default;

};


#endif //SERVOSG90S_H