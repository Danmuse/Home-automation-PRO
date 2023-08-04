/*/*!
 * @file CD4017.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "CD4017.h"

CD4017::CD4017(const std::list<Gpio*> &pins4017, uint8_t maxOutputs) :
m_index{0},
m_pins4017{pins4017},
m_maxOutputs{(uint8_t) (maxOutputs <= 10 ? maxOutputs : 10)} { }

void CD4017::SetDigit(void) {
	if (this->m_index < this->m_maxOutputs) SetClock();
	else SetReset();
	this->m_index++;
}

void CD4017::InitSweep(void) {
	for (const auto &pin : this->m_pins4017) pin->SetDir();
}

void CD4017::SetReset(void) {
	this->m_pins4017.front()->ClearPin();
	this->m_pins4017.front()->SetPin();
	this->m_pins4017.front()->ClearPin();
	this->m_index = 0;
}

void CD4017::SetClock(void) {
	this->m_pins4017.front()->ClearPin();
	this->m_pins4017.back()->ClearPin();
	this->m_pins4017.back()->SetPin();
	this->m_pins4017.back()->ClearPin();
}

CD4017::~CD4017() { }
