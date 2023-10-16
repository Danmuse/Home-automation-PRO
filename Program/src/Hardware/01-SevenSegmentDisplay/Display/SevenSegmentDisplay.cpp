/*/*!
 * @file SevenSegmentDisplay.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 */

#include "SevenSegmentDisplay.h"

SevenSegmentDisplay *g_display = nullptr;

SevenSegmentDisplay::SevenSegmentDisplay(std::vector<DigitsGroup*> groups, CD4511B *segments_IC, CD4017 *sweep_IC, const uint8_t *relativePos, const Digit::code_t system) : Callback(),
m_groups{groups},
m_segments{segments_IC},
m_sweep{sweep_IC},
m_maxdigits{0},
m_ticks{0},
m_blink_ticks_on{(uint8_t)(BLINK_TICKS_ON * (g_systick_freq / 1000))},
m_blink_ticks_off{(uint8_t)(BLINK_TICKS_OFF * (g_systick_freq / 1000))},
m_blink_status{false},
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
	if (!this->m_blink_status) this->m_blink_ticks_on--;
	else this->m_blink_ticks_off--;
	if (!this->m_ticks) {
		if (!this->m_blink_ticks_on) {
			this->m_blink_ticks_on = BLINK_TICKS_ON * (g_systick_freq / 1000);
			this->m_blink_status = !this->m_blink_status;
		} else if (!this->m_blink_ticks_off) {
			this->m_blink_ticks_off = BLINK_TICKS_OFF * (g_systick_freq / 1000);
			this->m_blink_status = !this->m_blink_status;
		}
		this->m_sweep->SetDigit();
		this->m_segments->SetSegments(Digit::TURNOFF);
		if (!this->m_blink_status || this->m_bufferDisplay[this->m_index]->GetMode() != Digit::BLINK)
			this->m_segments->SetSegments(this->m_bufferDisplay[this->m_index]->Get());
		this->m_ticks = UPDATE_TICKS * (g_systick_freq / 1000);
		this->m_index++;
		this->m_index %= this->m_maxdigits;
	}
}

void SevenSegmentDisplay::set(uint32_t value, uint8_t display) {
	uint16_t auxiliar[this->m_maxdigits], index = 0;
	for (index = 0; index < this->m_groups[display]->m_quantity; index++, value /= 10) auxiliar[index] = value % 10;
	for (index = 0; index < this->m_groups[display]->m_quantity; index++) this->m_bufferDisplay[this->m_relativePos[index + this->m_groups[display]->m_begin]]->Set(auxiliar[index]);
}

void SevenSegmentDisplay::clear(uint8_t display) {
	for (uint8_t index = 0; index < this->m_groups[display]->m_quantity; index++) this->m_bufferDisplay[this->m_relativePos[index + this->m_groups[display]->m_begin]]->Clear();
}

void SevenSegmentDisplay::mode(Digit::mode_t mode, uint8_t display) {
	if (mode == Digit::BLINK) for (uint8_t index = 0; index < this->m_groups[display]->m_quantity; index++) this->m_bufferDisplay[this->m_relativePos[index + this->m_groups[display]->m_begin]]->BlinkBind();
	else if (mode == Digit::NONE) for (uint8_t index = 0; index < this->m_groups[display]->m_quantity; index++) this->m_bufferDisplay[this->m_relativePos[index + this->m_groups[display]->m_begin]]->BlinkUnbind();
	else for (uint8_t index = 0; index < this->m_groups[display]->m_quantity; index++) this->m_bufferDisplay[this->m_relativePos[index + this->m_groups[display]->m_begin]]->Clear();
}

SevenSegmentDisplay::~SevenSegmentDisplay() {
	for (auto &digit : this->m_bufferDisplay) delete digit;
	this->m_bufferDisplay.clear();
}

////////////////////////////////////////////
/// Seven Segment Display initialization ///
////////////////////////////////////////////

void initDisplay(void) {
	#ifdef CN12_PINS

	static std::vector<DigitsGroup*> sevenSegments_list;
	static DigitsGroup tripletDisplayFST(0, 3);
	static DigitsGroup tripletDisplaySND(3, 3);
	sevenSegments_list.push_back(&tripletDisplayFST);
	sevenSegments_list.push_back(&tripletDisplaySND);

	static std::vector<Gpio*> segments_GPIOs_list;
	segments_GPIOs_list.push_back(&BCDA);
	segments_GPIOs_list.push_back(&BCDB);
	segments_GPIOs_list.push_back(&BCDC);
	segments_GPIOs_list.push_back(&BCDD);
	static CD4511B segments_IC(segments_GPIOs_list);

	static std::vector<Gpio*> sweep_GPIOs_list;
	sweep_GPIOs_list.push_back(&BCD_RST);
	sweep_GPIOs_list.push_back(&BCD_CLK);
	static CD4017 sweep_IC(sweep_GPIOs_list, 6);

	static uint8_t relativePositions[] = { 2, 1, 0, 5, 4, 3 };

	static SevenSegmentDisplay Display(sevenSegments_list, &segments_IC, &sweep_IC, relativePositions, Digit::BCD);

	g_display = &Display;

	#endif // CN12_PINS
}
