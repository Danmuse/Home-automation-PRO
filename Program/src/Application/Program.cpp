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

static void red_green(PWM &red, PWM &green);
static void green_blue(PWM &green, PWM &blue);
static void blue_red(PWM &blue, PWM &red);

int main(void) {
	initDevice();
	initLCD1602();

	bool statusFlag = false;
	uint8_t dutyCycle = 100, counterCycles = 0;
	PWM externPWM_FST(LED_RED, dutyCycle), externPWM_SND(LED_GREEN, dutyCycle), externPWM_TRD(LED_BLUE, dutyCycle);

	while (1) {
		while (counterCycles < 4) {
			g_lcd1602->write("Cycle counter: ");
			g_lcd1602->write(counterCycles, 0, 15);
			if (!statusFlag) {
				if (counterCycles % 2) {
					externPWM_TRD.setDuty(0);
					externPWM_FST.setDuty(dutyCycle--);
					externPWM_SND.setDuty(100 - dutyCycle);
				} else {
					externPWM_SND.setDuty(0);
					externPWM_FST.setDuty(dutyCycle--);
					externPWM_TRD.setDuty(100 - dutyCycle);
				}
				if (dutyCycle < 1) statusFlag = !statusFlag;
				delay(10);
			} else {
				if (counterCycles % 2) {
					externPWM_TRD.setDuty(0);
					externPWM_FST.setDuty(dutyCycle++);
					externPWM_SND.setDuty(100 - dutyCycle);
				} else {
					externPWM_SND.setDuty(0);
					externPWM_FST.setDuty(dutyCycle++);
					externPWM_TRD.setDuty(100 - dutyCycle);
				}
				if (dutyCycle > 100 - 1) {
					statusFlag = !statusFlag;
					counterCycles++;
				}
				delay(10);
			}
		}
		externPWM_FST.setDuty(0);
		externPWM_SND.setDuty(0);
		externPWM_TRD.setDuty(0);
		red_green(externPWM_FST, externPWM_SND);
		externPWM_FST.setDuty(0);
		externPWM_SND.setDuty(0);
		green_blue(externPWM_SND, externPWM_TRD);
		externPWM_SND.setDuty(0);
		externPWM_TRD.setDuty(0);
		blue_red(externPWM_TRD, externPWM_FST);
		externPWM_TRD.setDuty(0);
		externPWM_FST.setDuty(0);
		g_lcd1602->clear();
		counterCycles = 0;
		g_timers_list.TimerEvents();
	}
}

static void red_green(PWM &red, PWM &green) {
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("RED", 0, 7);
	red.setDuty(100);
	delay(2500);
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("YELLOW", 0, 7);
	red.setDuty(46);
	green.setDuty(50);
	delay(2500);
}

static void green_blue(PWM &green, PWM &blue) {
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("GREEN", 0, 7);
	green.setDuty(100);
	delay(2500);
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("SKY BLUE", 0, 7);
	green.setDuty(36);
	blue.setDuty(50);
	delay(2500);
}

static void blue_red(PWM &blue, PWM &red) {
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("BLUE", 0, 7);
	blue.setDuty(100);
	delay(2500);
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("VIOLET", 0, 7);
	blue.setDuty(50);
	red.setDuty(45);
	delay(2500);
}

#else // DEBUG_MODE

int main(void) {
	initDevice();
	initDisplay();
	initDS3231();
	initPreset();
	initUSB0();

	int32_t valueADC = 0;

	while (1) {
		valueADC = g_preset->get();
		g_display->set(valueADC);
		g_timers_list.TimerEvents();
		delay(1000);
	}

	/*
	while (1) {
		g_usb->Transmit("INFO 2\n", 7);
		g_timers_list.TimerEvents();
		delay(1000);
	}
	*/

	/*
	LED_RED.ClearPin();
	LED_GREEN.ClearPin();
	LED_BLUE.ClearPin();

	if (g_ds3231->set(30, 59, 23, 1, 1, 2000)) {
		LED_GREEN.SetPin();
		LED_RED.SetPin();
	} else LED_RED.ClearPin();

	RTC_st rtc;

	while (1) {
		rtc = g_ds3231->get();
		if (g_ds3231->getStatus()) {
			LED_GREEN.SetPin();
			LED_BLUE.SetPin();
		} else LED_BLUE.ClearPin();
		g_display->set(rtc.TIME.HOUR, 0);
		g_display->set(rtc.TIME.MIN, 1);
		g_timers_list.TimerEvents();
		delay(1000);
	}
	*/
}

#endif // DEBUG_MODE macro definition in precompile stage
