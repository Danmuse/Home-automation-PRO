/*/*!
 * @file Digit.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "Digit.h"

Digit::Digit(code_t system, uint8_t value) :
m_system{system},
m_value{value} { }

bool Digit::Set(uint16_t value) {
	switch (this->m_system) {
	case BCD:
		if (value < 10) {
			this->m_value = value & 0x0F;
			return true;
		}
		break;
	case SEGMENTS:
		if (value <= U) {
			this->m_value = tableBCD[value];
			return true;
		}
		break;
	case ASCII:
		if (value < 10) {
			this->m_value = value + '0';
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

uint8_t Digit::Get(void) const {
	return this->m_value;
}

void Digit::Clear(void) {
	this->m_value = TURNOFF;
}

Digit::~Digit() { }
