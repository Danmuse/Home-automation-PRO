#include "SG90S.h"

SG90S::SG90S(Gpio& controlPin) : m_pwm(controlPin, MIN_DUTY, 50) {
}

void SG90S::setAngle(int angle) {
    if (angle > MAX_ANGLE) {
        angle = MAX_ANGLE;
    } else if (angle < 0) {
        angle = 0;
    }
    m_pwm.setDuty(MIN_DUTY + angle * (PULSE_MAX_INCREMENT / MAX_ANGLE));
}
