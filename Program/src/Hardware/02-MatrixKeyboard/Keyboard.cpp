/*/*!
 * @file Keyboard.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 20/08/2023 12:55:47
 */

#include "Keyboard.h"

Keyboard *g_keyboard = nullptr;

Keyboard::Keyboard(std::vector<Gpio*> &columns, std::vector<Gpio*> &rows) : Callback(),
m_columns{columns},
m_rows{rows},
m_debounces{DEBOUNCE_TIME} {
	this->m_debounceCounter = 0;
	this->m_last_keyPressed = NO_KEY;
	this->m_current_keyPressed = NO_KEY;
	g_callback_list.push_back(this);
}

void Keyboard::callbackMethod(void) {
	uint8_t keyPressed;
	keyPressed = this->keyboardHW();
    this->keyboardSW(keyPressed);
}

uint8_t	Keyboard::get(void) {
	uint8_t keyPressed = this->m_current_keyPressed;
	this->m_current_keyPressed = NO_KEY;
	return keyPressed;
}

uint8_t Keyboard::keyboardHW(void) {
	for (uint8_t index = 0; index < this->m_rows.size(); index++) {
		for (uint8_t j_index = 0; j_index < this->m_rows.size(); j_index++)
            this->m_rows[j_index]->setPin();
        this->m_rows[index]->clearPin(); // Enable the row's pin to be checked
		for (uint8_t j_index = 0; j_index < this->m_columns.size(); j_index++)
			if (this->m_columns[j_index]->getPin()) return j_index + index * this->m_columns.size();
	}
	return NO_KEY;
}

void Keyboard::keyboardSW(uint8_t keyStatus) {
	if (keyStatus == NO_KEY) {
		this->m_debounceCounter = 0;
		this->m_last_keyPressed = NO_KEY;
	} else if (this->m_debounceCounter == 0) {
		this->m_last_keyPressed = keyStatus;
		this->m_debounceCounter++;
	} else if (this->m_last_keyPressed == keyStatus) {
		if (this->m_debounceCounter < this->m_debounces) this->m_debounceCounter++;
		else if (this->m_debounceCounter == this->m_debounces) {
			this->m_current_keyPressed = this->m_last_keyPressed;
			this->m_debounceCounter++;
		}
	} else {
		this->m_debounceCounter = 0;
		this->m_last_keyPressed = NO_KEY;
	}
}

//////////////////////////////////////
/// Matrix Keyboard initialization ///
//////////////////////////////////////

void initKeyboard(void) {
	#ifdef CN16_PINS

	static std::vector<Gpio*> columns_GPIOs_list;
	columns_GPIOs_list.push_back(&COL0_IN);
//	columns_GPIOs_list.push_back(&COL1_IN);
//	columns_GPIOs_list.push_back(&COL2_IN);

	static std::vector<Gpio*> rows_GPIOs_list;
	rows_GPIOs_list.push_back(&ROW0_OUT);
	rows_GPIOs_list.push_back(&ROW1_OUT);

	static Keyboard keyboard(columns_GPIOs_list, rows_GPIOs_list);

	g_keyboard = &keyboard;

	#endif // CN16_PINS
}
