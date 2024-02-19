/*!
 * @file MG90S.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/01/2024 21:52:13
 * @brief MG90S Servo Class API (Application Programming Interface).
 */

#ifndef MG90S_H_
#define MG90S_H_

#include "PWM.h"
#include "ProgramConfig.h"

#define MG90S_MAX_ANGLE 180
#define MG90S_MIN_DUTY 4
#define MG90S_PULSE_MAX_INCREMENT 14

class MG90S : protected PWM {
private:
	uint8_t m_currentPosition;
public:
	MG90S() = delete;
	MG90S(const Gpio& controlPin);
	void setAngle(uint8_t angle);
	uint8_t getAngle(void) const;
	virtual ~MG90S() = default;
};

extern MG90S *g_servo;

void initServo(void);

#endif // MG90S_H_
