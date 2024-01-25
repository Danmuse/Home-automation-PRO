/*!
 * @file PWM.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 12/08/2023 00:11:27
 * @brief Pulse Width Modulation Class API (Application Programming Interface).
 */

#ifndef PWM_H_
#define PWM_H_

#include "systick.h"
#include "GPIO.h"

#define MAX_PWM_CHANNELS 7

// TODO (Medium priority): Evaluate the use of the "m_channel" member every time any of the pins are detached.
// In the long term, inconsistencies will occur when bind or unbind SCTOUT channels.
// TODO (Minor priority): Protectedly inherit from the SCTImer class and previously,
// correct the implementations in each of the methods according to the operation of this class.
// TODO (Minor priority): Generate an enumeration to differentiate each of the SCTOUT channels.

class PWM : protected Gpio {
protected:
	const uint8_t m_channel;
	static uint8_t m_quantity;
	static uint32_t m_period;
	float m_duty;
private:
	void initSCTimer(void) const;
	void enableSWM(void) const;
	void disableSWM(void) const;
public:
	PWM() = delete;
	PWM(const Gpio &output, float duty, uint32_t period = 1000);
	float getDuty(void) const;
	void setDuty(float duty);
	void bindChannel(void);
	void unbindChannel(void);
	virtual ~PWM();
};

#endif /* PWM_H_ */
