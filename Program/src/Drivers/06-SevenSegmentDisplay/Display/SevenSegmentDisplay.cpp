/*/*!
 * @file SevenSegmentDisplay.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay(std::vector<DigitsGroup*> groups, CD4511B *segments_IC, CD4017 *sweep_IC, const uint8_t *relativePos, const Digit::code_t system) : Callback(),
m_groups{groups},
m_segments{segments_IC},
m_sweep{sweep_IC},
m_maxdigits{0},
m_ticks{0},
m_system{system},
m_relativePos{relativePos} {
	g_callback_list.push_back(this);
	for (uint8_t index = 0; index < this->m_groups.size(); index++) {
		this->m_maxdigits += this->m_groups[index]->m_quantity;
		for (uint8_t j_index = 0; j_index < this->m_groups[index]->m_quantity; j_index++)
			this->m_bufferDisplay.push_back(new Digit(this->m_system, Digit::TURNOFF));
	}
	this->m_index = 0;
}

void SevenSegmentDisplay::CallbackMethod(void) {
	this->m_ticks--;
	if (!this->m_ticks) {
		this->m_sweep->SetDigit();
		this->m_segments->SetSegments(Digit::TURNOFF);
		this->m_segments->SetSegments(this->m_bufferDisplay[this->m_index]->Get());
		this->m_ticks = UPDATE_TICKS * (g_systick_freq / 1000);
		this->m_index++;
		this->m_index %= this->m_maxdigits;
	}
}

void SevenSegmentDisplay::Set(uint32_t value, uint8_t display) {
	uint8_t auxiliar[this->m_maxdigits], index = 0;

	for (index = 0; index < this->m_groups[display]->m_quantity; index++, value /= 10)	auxiliar[index] = value % 10;
	for (index = 0; index < this->m_groups[display]->m_quantity; index++) this->m_bufferDisplay[this->m_relativePos[index + this->m_groups[display]->m_begin]]->Set(auxiliar[index]);
}

SevenSegmentDisplay::~SevenSegmentDisplay() {
	for (auto &digit : this->m_bufferDisplay) delete digit;
	this->m_bufferDisplay.clear();
}
