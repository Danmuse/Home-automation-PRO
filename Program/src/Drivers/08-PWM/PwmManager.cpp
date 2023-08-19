/*/*!
 * @file PwmManager.cpp
 * @par Author
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 12/08/2023 21:32:21
 */

#include "PwmManager.h"

uint8_t PwmManager::m_pwmNumber = 0;
uint32_t PwmManager::m_frequency = 1000;

PwmManager::PwmManager() {
	initPwm();
}

PwmManager::PwmManager(uint32_t frequency) {
	initPwm();
	m_frequency=frequency;
}

PWM PwmManager::createPwm(const Gpio &outputPort, float duty) {
	return PWM(outputPort, m_pwmNumber++, duty,m_frequency);
}

void PwmManager::initPwm(void) const {
	SYSCON->SYSAHBCLKCTRL0 |= (1<<7) | (1<<8);
	SCT->CONFIG |= (1<<0)|(1<<17);
	SCT->MATCH[0]=FREQ_CLOCK_MCU/(1000);
	SCT->MATCHREL[0]=FREQ_CLOCK_MCU/(1000);

	SCT->EV[0].STATE = 1; // Other than '0'
	SCT->EV[0].CTRL = 0 | (1<<12);
	SCT->OUTPUT = 0xC;
	SCT->RES = 0b01010101;
	SCT->EVEN = 1;

	SCT->CTRL &= ~(1 << 2);
}

PwmManager::~PwmManager() { }
