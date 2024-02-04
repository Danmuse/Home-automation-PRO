/*/*!
 * @file GPIO.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 */

#include "GPIO.h"

Gpio::Gpio(const Gpio& original) :
m_port{original.m_port},
m_bit{original.m_bit},
m_activity{original.m_activity},
m_direction{original.m_direction},
m_mode{original.m_mode},
m_error{original.m_error} { }

Gpio::Gpio(port_t port, uint8_t bit, uint8_t mode, direction_t direction, activity_t activity) :
m_port{port},
m_bit{bit},
m_activity{activity},
m_direction{direction},
m_mode{mode} {
	if ((this->m_port != PORT0) && (this->m_port != PORT1))	this->m_error = ERROR;
    else if (this->m_port == PORT0 && this->m_bit >= B_PORT0) this->m_error = ERROR;
    else if (this->m_port == PORT1 && this->m_bit >= B_PORT1) this->m_error = ERROR;
	SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK | SYSCON_SYSAHBCLKCTRL0_GPIO1_MASK | SYSCON_SYSAHBCLKCTRL0_IOCON_MASK);
	this->setDir();
}

void Gpio::setPin(void) {
	if (this->m_activity == HIGH) GPIO->SET[this->m_port] |= (1 << this->m_bit);
	else GPIO->CLR[this->m_port] |= (1 << this->m_bit);
}

void Gpio::clearPin(void) {
	if (this->m_activity == HIGH) GPIO->CLR[this->m_port] |= (1 << this->m_bit);
	else GPIO->SET[this->m_port] |= (1 << this->m_bit);
}

void Gpio::setDirOutputs(void) {
	GPIO->DIRSET[this->m_port] |= (1 << this->m_bit);
}

void Gpio::setDirInputs(void) {
	GPIO->DIRCLR[this->m_port] |= (1 << this->m_bit);
}

void Gpio::togglePin(void) {
	GPIO->NOT[this->m_port] |= (1 << this->m_bit);
}

bool Gpio::getPin(void) const {
	return	this->m_activity == HIGH ?
			((GPIO->PIN[this->m_port] >> this->m_bit) & 0x01) :
			!((GPIO->PIN[this->m_port] >> this->m_bit) & 0x01);
}

void Gpio::setPinMode(void) {
	uint8_t index = 0;
	if (this->m_port == PORT0) index = IOCON_INDEX_PIO0[this->m_bit];
	else if (this->m_port == PORT1) index = IOCON_INDEX_PIO1[this->m_bit];
	IOCON->PIO[index] &= ~(1 << 10);
	IOCON->PIO[index] |= (this->m_mode << 10);
}

void Gpio::setPinResistor(void) {
	uint8_t index = 0;
	if (this->m_port == PORT0) index = IOCON_INDEX_PIO0[this->m_bit];
	else if (this->m_port == PORT1) index = IOCON_INDEX_PIO1[this->m_bit];
	IOCON->PIO[index] &= ~0x180;
	IOCON->PIO[index] |= (this->m_mode << 3);
	// IOCON_PIO_OD(this->m_mode);
}

Gpio::error_t Gpio::setDir(void) {
	if (this->m_error == OK) {
		if (this->m_direction == INPUT) {
			// Sets a default pin mode in case it does not match the elements of the mode_input_t enumeration
			if (this->m_mode != INACTIVE && this->m_mode != PULLDOWN && this->m_mode != PULLUP && this->m_mode != REPEATER) this->m_mode = REPEATER;
            this->setDirInputs();
            this->setPinResistor();
		} else if (this->m_direction == OUTPUT) {
			// Sets a default pin mode in case it does not match the elements of the mode_output_t enumeration
			if (this->m_mode != PUSHPULL && this->m_mode != OPENCOLECTOR) this->m_mode = PUSHPULL;
            this->setDirOutputs();
            this->setPinMode();
			this->m_activity == LOW ? this->clearPin() : this->setPin();
		}
	}
	return this->m_error;
}

Gpio::error_t Gpio::toggleDir(void) {
	GPIO->DIRNOT[this->m_port] |= (1 << this->m_bit);
	if (this->m_direction == OUTPUT) this->m_direction = INPUT;
	else if (this->m_direction == INPUT) this->m_direction = OUTPUT;
	return this->setDir();
}
