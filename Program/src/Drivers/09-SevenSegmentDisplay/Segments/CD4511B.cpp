/*/*!
 * @file CD4511B.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "CD4511B.h"

CD4511B::CD4511B(const std::list<Gpio*> &BCD) :
m_BCD{BCD} { }

void CD4511B::SetSegments(uint16_t value) {
	Gpio* pinBCDs[MAX_BCD_PINS];
	uint8_t index = 0;

	for (const auto &pin : this->m_BCD) {
		pinBCDs[index] = pin;
		if (index < MAX_BCD_PINS) index++;
		else break;
	}

	if (!(value & 1)) pinBCDs[0]->ClearPin();
	else pinBCDs[0]->SetPin();

	if (!((value >> 1) & 1)) pinBCDs[1]->ClearPin();
	else pinBCDs[1]->SetPin();

	if (!((value >> 2) & 1)) pinBCDs[2]->ClearPin();
	else pinBCDs[2]->SetPin();

	if (!((value >> 3) & 1)) pinBCDs[3]->ClearPin();
	else pinBCDs[3]->SetPin();
}

void CD4511B::InitSegments(void) {
	for (const auto &pin : this->m_BCD) pin->SetDir();
}

CD4511B::~CD4511B() { }
