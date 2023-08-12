/*/*!
 * @file GPIO.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 */

#include "GPIO.h"

Gpio::Gpio(const Gpio &original) :
m_port{original.m_port},
m_bit{original.m_bit},
m_mode{original.m_mode},
m_direction{original.m_direction},
m_activity{original.m_activity},
m_error{original.m_error} { }

Gpio::Gpio(port_t port, uint8_t bit, uint8_t mode, direction_t direction, activity_t activity) :
m_port{port},
m_bit{bit},
m_mode{mode},
m_direction{direction},
m_activity{activity} {
	this->m_error = OK;
	if ((m_port != PORT0) && (m_port != PORT1))	this->m_error = ERROR;
    else if (m_port == PORT0 && m_bit >= B_PORT0) this->m_error = ERROR;
    else if (m_port == PORT1 && m_bit >= B_PORT1) this->m_error = ERROR;
	SYSCON->SYSAHBCLKCTRL0 |= ((1 << CLK_GPIO0) | (1 << CLK_GPIO1) | (1 << CLK_IOCON));
	this->SetDir();
}

void Gpio::SetPin(void) {
	if (this->m_activity == LOW) GPIO->SET[this->m_port] |= (1 << this->m_bit);
	else GPIO->CLR[this->m_port] |= (1 << this->m_bit);
}

void Gpio::ClearPin(void) {
	if (this->m_activity == HIGH) GPIO->CLR[this->m_port] |= (1 << this->m_bit);
	else GPIO->SET[this->m_port] |= (1 << this->m_bit);
}

void Gpio::SetDirOutputs(void) {
	GPIO->DIRSET[this->m_port] |= (1 << this->m_bit);
}

void Gpio::SetDirInputs(void) {
	GPIO->DIRCLR[this->m_port] |= (1 << this->m_bit);
}

void Gpio::TogglePin(void) {
	GPIO->NOT[this->m_port] |= (1 << this->m_bit);
}

uint8_t Gpio::GetPin(void) const {
	return	this->m_activity == HIGH ?
			((GPIO->PIN[this->m_port] >> this->m_bit) & 0x01) :
			!((GPIO->PIN[this->m_port] >> this->m_bit) & 0x01);
}

void Gpio::SetPinMode(void) {
	uint8_t index = 0;
	if (this->m_direction == OUTPUT) {
		if (this->m_port == PORT0) index = IOCON_INDEX_PIO0[this->m_bit];
		else if (this->m_port == PORT1) index = IOCON_INDEX_PIO1[this->m_bit];

		IOCON->PIO[index] &= ~(1 << 10);
		IOCON->PIO[index] |= (this->m_mode << 10);
	}
}

void Gpio::SetPinResistor(void) {
	uint8_t index = 0;
	if (this->m_direction == INPUT) {
		if (this->m_port == PORT0) index = IOCON_INDEX_PIO0[this->m_bit];
		else if (this->m_port == PORT1) index = IOCON_INDEX_PIO1[this->m_bit];

		IOCON->PIO[index] &= ~0x180;
		IOCON->PIO[index] |= (this->m_mode << 3);
		// IOCON_PIO_OD(this->m_mode);
	}
}

uint8_t Gpio::SetDir(void) {
	if (this->m_error == OK) {
		if (this->m_direction == INPUT) {
			this->SetDirInputs();
			this->SetPinResistor();
		} else if (this->m_direction == OUTPUT) {
			this->SetDirOutputs();
			this->SetPinMode();
			this->m_activity == LOW ? this->ClearPin() : this->SetPin();
		}
	}
	return this->m_error;
}

uint8_t Gpio::ToggleDir(void) {
	GPIO->DIRNOT[this->m_port] |= (1 << this->m_bit);
	if (this->m_direction == OUTPUT) this->m_direction = INPUT;
	else if (this->m_direction == INPUT) this->m_direction = OUTPUT;
	return this->SetDir();
}

Gpio::activity_t Gpio::getActivity(void)const{
	return m_activity;
}

Gpio::~Gpio() { }
