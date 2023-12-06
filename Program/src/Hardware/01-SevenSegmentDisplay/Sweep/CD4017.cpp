/*/*!
 * @file CD4017.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "CD4017.h"

CD4017::CD4017(const std::vector<Gpio*> &pins4017, uint8_t maxOutputs) :
m_index{maxOutputs},
m_pins4017{pins4017},
m_maxOutputs{(uint8_t)(maxOutputs <= 10 ? maxOutputs : 10)} { }

void CD4017::setDigit(void) {
	if (this->m_index < this->m_maxOutputs) setClock();
	else setReset();
	this->m_index++;
}

void CD4017::setReset(void) {
    this->m_pins4017[RST_INDEX]->clearPin();
    this->m_pins4017[RST_INDEX]->setPin();
    this->m_pins4017[RST_INDEX]->clearPin();
	this->m_index = 0;
}

void CD4017::setClock(void) {
    this->m_pins4017[RST_INDEX]->clearPin();
    this->m_pins4017[CLK_INDEX]->setPin();
    this->m_pins4017[CLK_INDEX]->clearPin();
}

CD4017::~CD4017() { }
