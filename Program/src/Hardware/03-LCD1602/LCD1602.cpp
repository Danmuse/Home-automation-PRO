/*/*!
 * @file LCD1602.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 27/08/2023 23:36:45
 */

#include "LCD1602.h"

LCD1602 *g_lcd1602 = nullptr;

LCD1602::LCD1602(std::vector<Gpio*> &outputs, const uint8_t rows, const uint8_t columns) : Callback(),
m_outputs{outputs},
m_mode{s_eigth_bits},
m_rows{rows},
m_columns{columns},
m_ticks{(uint16_t)(200 * (g_systick_freq / 1000))},
m_sweep(0),
m_position{0} {
	g_callback_list.push_back(this);
	if (g_systick_freq < 200) while (1);
	this->Initialize();
}

void LCD1602::Initialize(void) {
	this->m_outputs[RS]->ClearPin();
	this->m_outputs[D7]->ClearPin();
	this->m_outputs[D6]->ClearPin();
	this->m_outputs[D5]->SetPin();
	this->m_outputs[D4]->ClearPin();
	this->m_buffer = new uint8_t[this->m_rows * this->m_columns];
	for (uint8_t index = 0; index < this->m_rows * this->m_columns; index++) this->m_buffer[index] = ' ';
}

LCD1602& LCD1602::operator=(const int8_t *ptr_str) {
	this->Write(ptr_str);
	return *this;
}

void LCD1602::Write(const int8_t *ptr_str) {
	uint8_t index = 0;
	for (index = this->m_position; (index < this->m_rows * this->m_columns) && (ptr_str[index - this->m_position] != '\0'); index++)
		this->m_buffer[index] = ptr_str[index - this->m_position];
	this->m_position = index;
	if (this->m_position >= (this->m_columns * this->m_rows)) this->m_position = 0;
}

void LCD1602::Write(const int32_t value) {
	int32_t auxiliar;
	int8_t *ptr_number = new int8_t[12];
	uint8_t position = 0;

	if (value < 0) {
		ptr_number[0] = '-';
		position++;
	}
	for (int32_t i = 0; (i < 10) && (value != 0); i++) {
		auxiliar = (int32_t)(value / this->Pow(10, 9 - i));
		if (auxiliar < 0) auxiliar *= -1;
		if (auxiliar != 0) {
			ptr_number[position] = auxiliar % 10 + '0';
			position++;
		}
	}
	if (value == 0) {
		ptr_number[0] = '0';
		position++;
	}
	ptr_number[position] = '\0';
	this->Write(ptr_number);
	delete [] ptr_number;
}

void LCD1602::write(const int8_t *ptr_str, uint8_t row, uint8_t column) {
	if ((this->m_columns * row) + column <= this->m_columns * this->m_rows) {
		this->m_position = (this->m_columns * row) + column;
		this->Write(ptr_str);
	}
}

void LCD1602::write(const int32_t value, const uint8_t row, const uint8_t column) {
	if ((this->m_columns * row) + column <= this->m_columns * this->m_rows) {
		this->m_position = (this->m_columns * row) + column;
		this->Write(value);
	}
}

void LCD1602::clear(void) {
	for (uint8_t index = 0; index < (this->m_rows * this->m_columns); index++) this->m_buffer[index] = ' ';
	this->m_position = 0;
}

void LCD1602::CallbackMethod(void) {
	this->m_ticks--;
	if (this->m_ticks == 0) {
		switch (this->m_mode) {
		case s_print:
			this->WriteInstruction(this->m_buffer[this->m_sweep], Gpio::HIGH);
			this->m_ticks = 1 * (g_systick_freq / 1000);
			this->m_sweep++;
			if ((this->m_sweep == this->m_columns) || (this->m_sweep == (this->m_rows * this->m_columns)))
				this->m_mode = s_row;
			break;
		case s_row:
			if (this->m_sweep == (this->m_rows * this->m_columns)) {
				this->WriteInstruction(SET_DDRAM, Gpio::LOW);
				this->m_sweep = 0;
			} else this->WriteInstruction(SET_DDRAM | 0x40, Gpio::LOW);
			this->m_ticks = 1 * (g_systick_freq / 1000);
			this->m_mode = s_print;
			break;
		default:
		case s_eigth_bits:
			this->m_outputs[ENABLE]->SetPin();
			this->m_outputs[ENABLE]->ClearPin();
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_four_bits;
			break;
		case s_four_bits:
			this->WriteInstruction(FUNCTION_SET | (0 << 4) | (1 << 3) | (0 << 2), Gpio::LOW);	///DL	N	F
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_config_display;
			break;
		case s_config_display:
			this->WriteInstruction(DISPLAY_CONTROL | (1 << 2) | (1 << 1) | (0 << 0), Gpio::LOW); ///D	C	B
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_config_cursor;
			break;
		case s_config_cursor:
			this->WriteInstruction(ENTRY_MODE_SET | (1 << 1) | (0 << 0), Gpio::LOW);	///I/D	S
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_clear;
			break;
		case s_clear:
			this->WriteInstruction(CLEAR_DISPLAY, Gpio::LOW);
			this->m_ticks = 200 * (g_systick_freq / 1000);
			this->m_mode = s_print;
			break;
		}
	}
}

void LCD1602::WriteInstruction(const uint8_t data, const Gpio::activity_t mode) {
	if (mode == Gpio::HIGH) this->m_outputs[RS]->SetPin();
	else this->m_outputs[RS]->ClearPin();

	if ((data >> 7) & 1) this->m_outputs[D7]->SetPin();
	else this->m_outputs[D7]->ClearPin();
	if ((data >> 6) & 1) this->m_outputs[D6]->SetPin();
	else this->m_outputs[D6]->ClearPin();
	if ((data >> 5) & 1) this->m_outputs[D5]->SetPin();
	else this->m_outputs[D5]->ClearPin();
	if ((data >> 4) & 1) this->m_outputs[D4]->SetPin();
	else this->m_outputs[D4]->ClearPin();

	this->m_outputs[ENABLE]->SetPin();
	this->m_outputs[ENABLE]->ClearPin();

	if ((data >> 3) & 1) this->m_outputs[D7]->SetPin();
	else this->m_outputs[D7]->ClearPin();
	if ((data >> 2) & 1) this->m_outputs[D6]->SetPin();
	else this->m_outputs[D6]->ClearPin();
	if ((data >> 1) & 1) this->m_outputs[D5]->SetPin();
	else this->m_outputs[D5]->ClearPin();
	if ((data >> 0) & 1) this->m_outputs[D4]->SetPin();
	else this->m_outputs[D4]->ClearPin();

	this->m_outputs[ENABLE]->SetPin();
	this->m_outputs[ENABLE]->ClearPin();
}

uint32_t LCD1602::Pow(uint32_t base, uint32_t exponent) {
	int32_t auxiliar = 1;
	for (uint32_t index = 0; index < exponent && auxiliar != 0; index++) {
		if (auxiliar >= UINT32_MAX / base) auxiliar = 0;
		auxiliar *= base;
	}
	return auxiliar;
}

LCD1602::~LCD1602() { }

//////////////////////////////
/// LCD1602 initialization ///
//////////////////////////////

void initLCD1602(void) {
	#ifdef CN15_PINS

	static std::vector<Gpio*> LCD1602_GPIOs_list;
	LCD1602_GPIOs_list.push_back(&LCD_D7);
	LCD1602_GPIOs_list.push_back(&LCD_D6);
	LCD1602_GPIOs_list.push_back(&LCD_D5);
	LCD1602_GPIOs_list.push_back(&LCD_D4);
	LCD1602_GPIOs_list.push_back(&LCD_RS);
	LCD1602_GPIOs_list.push_back(&LCD_EN);

	static LCD1602 lcd1602(LCD1602_GPIOs_list, 2, 16);

	g_lcd1602 = &lcd1602;

	#endif // CN15_PINS
}
