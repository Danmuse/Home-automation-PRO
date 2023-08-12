/*
 * PwmManager.cpp
 *
 *  Created on: Aug 12, 2023
 *      Author: Agusin Ordoñez
 */

#include <PwmManager.h>

uint8_t PwmManager::m_pwmNumber=0;
uint32_t PwmManager::m_frequency=1000;


PwmManager::PwmManager(){
	initPwm();
}

PwmManager::PwmManager(uint32_t frequency){
	initPwm();
	m_frequency=frequency;
}

PWM PwmManager::createPwm(const Gpio &outputPort, float duty){
	return PWM(outputPort, m_pwmNumber++, duty, outputPort.getActivity(),m_frequency);
}

void PwmManager::initPwm() const{
	SYSCON->SYSAHBCLKCTRL0 |= (1<<7) | (1<<8);
	SCT->CONFIG |= (1<<0)|(1<<17);
	SCT->MATCH[0]=FREQ_CLOCK_MCU/(1000);
	SCT->MATCHREL[0]=FREQ_CLOCK_MCU/(1000);

	SCT->EV[0].STATE=1;// tiene que ser distinto de 0
	SCT->EV[0].CTRL= 0 | (1<<12);
	SCT->OUTPUT=0xC;
	SCT->RES=0b01010101;
	SCT->EVEN=1;

	SCT->CTRL &= ~(1 << 2);
}
