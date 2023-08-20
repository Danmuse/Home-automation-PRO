/*!
 * @file Program.cpp
 * @par Authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 17/08/2023 00:56:24
 */

#include <cr_section_macros.h>
#include "board.h"

#if !DEBUG_MODE

int main(void) {
	initDevice();

	bool increaseFlag = false;
	uint8_t duty = 255;
	PwmManager pwmManager;
	PWM pwm = pwmManager.createPwm(LED_BLUE, duty);

	while (1) {
		if (!increaseFlag) {
			pwm.setDuty(duty--);
			if (duty < 1) {
				pwm.setDuty(0);
				increaseFlag = !increaseFlag;
				delay(1000);
			}
		} else {
			pwm.setDuty(duty++);
			if (duty > 254) increaseFlag = !increaseFlag;
		}

		for (uint32_t index = 0; index < 2500; index++);
		g_timers_list.TimerEvents();
	}
}

#else

#endif
