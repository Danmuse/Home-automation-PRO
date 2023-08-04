/*/*!
 * @file SevenSegmentDisplay.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay(std::list<DigitsGroup*> groups, Segments *segments, Sweep *sweep, const uint8_t *relativePos, const Digit::code_t system) : Callback(),
m_groups{groups},
m_segments{segments},
m_sweep{sweep},
m_maxdigits{0},
m_ticks{0},
m_system{system},
m_relativePos{relativePos} {
	for (auto &display : this->m_groups) {
		this->m_maxdigits += display->m_quantity;
		for (uint8_t index = 0; index < display->m_quantity; index++)
			this->m_bufferDisplay.push_back(new Digit(this->m_system, Digit::TURNOFF));
	}
	this->m_index = 0;
}

void SevenSegmentDisplay::CallbackDisplay(void) {
	auto iterator = this->m_bufferDisplay.begin();
	std::advance(iterator, this->m_index);
	Digit* digitPointer = *iterator;

	this->m_ticks--;
	if (!this->m_ticks) {
		this->m_ticks = UPDATE_TICKS * (g_systick_freq / 1000);
		this->m_sweep->SetDigit();
		this->m_segments->SetSegments(digitPointer->Get());
		this->m_index++;
		this->m_index %= this->m_maxdigits;
	}
}

void SevenSegmentDisplay::Set(uint32_t value, uint8_t display) {
	auto iteratorFST = this->m_groups.begin();
	std::advance(iteratorFST, display);
	DigitsGroup* displayPointer = *iteratorFST;

	auto iteratorSND = this->m_bufferDisplay.begin();
	Digit* digitPointer = nullptr;

	for (uint8_t index = 0; index < displayPointer->m_quantity; index++, value /= 10) {
		std::advance(iteratorSND, this->m_relativePos[index + displayPointer->m_begin]);
		digitPointer = *iteratorSND;
		digitPointer->Set(value % 10);
	}
}

SevenSegmentDisplay::~SevenSegmentDisplay() {
	for (auto &digit : this->m_bufferDisplay) delete digit;
	this->m_bufferDisplay.clear();
}
