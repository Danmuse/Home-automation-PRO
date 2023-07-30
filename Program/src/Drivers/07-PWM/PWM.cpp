/*/*!
 * @file PWM.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 */

#include "PWM.h"

PWM::PWM(port_t port, uint8_t bit, activity_t activity, pwm_channel_t channel) : Gpio(port, bit, PUSHPULL, OUTPUT, activity),
m_pwm_channel{channel},
m_timeON{0},
m_timeOFF{0} { }

void PWM::SetTon(uint32_t time, pwm_time_unit_t unit) {
	this->m_timeON = time;
	if (unit == MILLI_SEC) this->m_timeON *= 1000;
	if (unit == SEC) this->m_timeON *= 1000000;

	this->SetTime(this->m_timeON * (FREQ_CLOCK_MCU / 1000000), this->m_pwm_channel);
}

void PWM::SetPeriod(uint32_t time, pwm_time_unit_t unit) {
	this->m_timeOFF = time;
	if (unit == MILLI_SEC) this->m_timeOFF *= 1000;
	if (unit == SEC) this->m_timeOFF *= 1000000;
	this->m_timeOFF -= this->m_timeON;

	this->SetTime((this->m_timeON + this->m_timeOFF) * (FREQ_CLOCK_MCU / 1000000), 0);
}

void PWM::InitPeriod(uint32_t timeON, uint32_t timeOFF, pwm_time_unit_t unit) {
	this->SetSwitchMatrizSCTOUT(this->m_bit, this->m_port, this->m_pwm_channel - 1);

	this->SetUnify(true);
	this->SetAutoLimit(true);

	this->m_timeON = timeON;
	this->m_timeOFF = timeOFF;
	if (unit == SEC) {
		this->m_timeON *= 1000000;
		this->m_timeOFF *= 1000000;
	}
	if (unit == MILLI_SEC) {
		this->m_timeON *= 1000;
		this->m_timeOFF *= 1000;
	}

	this->SetTime((this->m_timeON + this->m_timeOFF) * (FREQ_CLOCK_MCU / 1000000), 0);	/*	CHANNEL 0 ALWAYS IS THE PERIOD	*/
	this->SetTime(this->m_timeON * (FREQ_CLOCK_MCU / 1000000), this->m_pwm_channel);	/*	Setteo del tiempo de encendido	*/

	if (this->m_activity == HIGH) {
		SCT->OUT[this->m_pwm_channel - 1].SET = (1 << 0); // Event 0 sets OUT0
		SCT->OUT[this->m_pwm_channel - 1].CLR = (1 << this->m_pwm_channel); // event 1 clear OUT0

		SCT->OUTPUT &= ~(1 << (this->m_pwm_channel - 1)); // Default clear OUT[m_pwm_channel]
		SCT->RES &= ~(0b11 << ((this->m_pwm_channel - 1) * 2)); // Clean the buffer if necessary
		SCT->RES |= (0b10 << ((this->m_pwm_channel - 1) * 2)); 	// Conflict: Inactive state takes precedence
	} else {
		SCT->OUT[this->m_pwm_channel - 1].CLR = (1 << 0);
		SCT->OUT[this->m_pwm_channel - 1].SET = (1 << this->m_pwm_channel);

		SCT->OUTPUT |= (1 << (this->m_pwm_channel - 1));
		SCT->RES &= ~(0b11 << ((this->m_pwm_channel - 1) * 2)); // Clean the buffer if necessary
		SCT->RES |= (0b01 << ((this->m_pwm_channel - 1) * 2)); 	// Conflict: Active state takes precedence
	}
}

void PWM::turnON(void) {
	this->StartTimer();
}

void PWM::turnOFF(void) {
	this->StopTimer();
}

PWM::~PWM() { }
