/*/*!
 * @file CD4511B.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "CD4511B.h"

CD4511B::CD4511B(const std::vector<Gpio*> &BCD) :
m_BCD{BCD} { }

void CD4511B::setSegments(uint16_t value) {
	if (!(value & 1)) this->m_BCD[BCDA_INDEX]->clearPin();
	else this->m_BCD[0]->setPin();

	if (!((value >> 1) & 1)) this->m_BCD[BCDB_INDEX]->clearPin();
	else this->m_BCD[1]->setPin();

	if (!((value >> 2) & 1)) this->m_BCD[BCDC_INDEX]->clearPin();
	else this->m_BCD[2]->setPin();

	if (!((value >> 3) & 1)) this->m_BCD[BCDD_INDEX]->clearPin();
	else this->m_BCD[3]->setPin();
}

CD4511B::~CD4511B() { }
