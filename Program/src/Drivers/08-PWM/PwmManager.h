/*!
 * @file PwmManager.h
 * @par Author
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 12/08/2023 21:32:21
 * @brief Pulse Width Modulation Manager Class API (Application Programming Interface).
 */

#ifndef PWM_MANAGER_H_
#define PWM_MANAGER_H_

#include "LPC845.h"
#include <vector>
#include <array>
#include "PWM.h"

class PwmManager {
protected:
	const uint8_t MAX_PWM = 7;
	static uint8_t m_pwmNumber;
	static uint32_t m_frequency;
public:
	PwmManager();
	PwmManager(uint32_t frequency);
	PWM createPwm(const Gpio &outputPort, float duty);
	void initPwm(void) const;
	virtual ~PwmManager();
};

#endif /* PWM_MANAGER_H_ */
