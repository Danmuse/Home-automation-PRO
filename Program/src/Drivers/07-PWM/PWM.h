/*!
 * @file PWM.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 * @brief Pulse Width Modulation Class API (Application Programming Interface).
 */

#ifndef PWM_H_
#define PWM_H_

#include "SCTimer.h"
#include "GPIO.h"

class PWM : protected SCTimer, protected Gpio {
protected:
	const uint8_t m_pwm_channel;	//!< PWM object channel
	uint32_t m_timeON;				//!< PWM turn-on time in micro seconds
	uint32_t m_timeOFF;				//!< PWM turn-off time in micro seconds
public:
	enum pwm_time_unit_t 	{ SEC, MILLI_SEC, MICRO_SEC };
	enum pwm_channel_t 		{ CHANNEL_1 = 1, CHANNEL_2, CHANNEL_3, CHANNEL_4, CHANNEL_5, CHANNEL_6 };

	PWM(port_t port, uint8_t bit, activity_t activity, pwm_channel_t channel);
	void InitPeriod(uint32_t timeON, uint32_t timeOFF, pwm_time_unit_t unit = MICRO_SEC);
	void SetTon(uint32_t time, pwm_time_unit_t unit = MICRO_SEC);
	void SetPeriod(uint32_t time, pwm_time_unit_t unit = MICRO_SEC);
	void turnON();
	void turnOFF();
	virtual ~PWM();
};

#endif /* PWM_H_ */
