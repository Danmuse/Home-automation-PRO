/*/*!
 * @file MG90S.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/01/2024 21:52:13
 */

#include "MG90S.h"

MG90S *g_servo = nullptr;

MG90S::MG90S(const Gpio& controlPin) : PWM(controlPin, MG90S_MIN_DUTY, 75),
m_currentPosition{0} { }

/*!
 * @brief Set the angle servo's angle.
 * @param angle Angle to set.
 */
void MG90S::setAngle(uint8_t angle) {
    angle = angle > MG90S_MAX_ANGLE ? MG90S_MAX_ANGLE : angle;
    // NOTE: The incorrect conversion of this result to float would produce instabilities in the program.
    this->setDuty(MG90S_MIN_DUTY + (angle * (MG90S_PULSE_MAX_INCREMENT / (float)(MG90S_MAX_ANGLE))));
    this->m_currentPosition = angle;
}

/*!
 * @brief Get the servo's current angle.
 * @return Angle.
 */
uint8_t MG90S::getAngle(void) const {
	return this->m_currentPosition;
}

//////////////////////////////////
/// MG90S Servo initialization ///
//////////////////////////////////

void initServo(void) {
	#if defined(MG90S_SERVO_PIN)

    static MG90S servo(MG90S_SERVO);
    servo.setAngle(0);

    g_servo = &servo;

	#endif // defined(MG90S_SERVO_PIN)
}
