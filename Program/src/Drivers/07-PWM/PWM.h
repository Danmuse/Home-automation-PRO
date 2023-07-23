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
	const uint8_t m_activity;		/**< Activo alto/bajo del PWM */
	uint32_t m_timeON;				/**< Tiempo de encendido del PWM en micro segundos	*/
	uint32_t m_timeOFF;				/**< Tiempo de apagado del PWM en micro segundos	*/
	const uint8_t m_pwm_channel;	/**< Canal del objeto PWM */
public:
	enum pwm_time_unit_t 	{ SEG, MILI_SEG, MICRO_SEG };
	enum pwm_channel_t 		{ CHANNEL_1 = 1, CHANNEL_2, CHANNEL_3, CHANNEL_4, CHANNEL_5, CHANNEL_6 };
	enum activity_t			{ LOW, HIGH };

	PWM(port_t port, uint8_t bit, uint8_t activity, pwm_channel_t number);
	void Inicializar(uint32_t timeON, uint32_t timeOFF, pwm_time_unit_t unit = MICRO_SEG);
	void SetTon(uint32_t time, pwm_time_unit_t unit = MICRO_SEG);
	void SetPeriod(uint32_t time, pwm_time_unit_t unit = MICRO_SEG);
	void turnON();
	void turnOFF();
	virtual ~PWM();
};

#endif /* PWM_H_ */
