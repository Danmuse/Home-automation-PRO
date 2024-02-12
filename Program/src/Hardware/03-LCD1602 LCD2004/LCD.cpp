/*/*!
 * @file LCD.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 27/08/2023 23:36:45
 */

#include "LCD.h"

LCD *g_lcd = nullptr;

LCD::LCD(std::vector<Gpio*> &outputs, const uint8_t rows, const uint8_t columns) : Callback(),
m_outputs{outputs},
m_mode{s_eigth_bits},
m_rows{rows},
m_columns{columns},
m_ticks{(uint8_t)(100 * (g_systick_freq / 1000))},
m_sweep{0},
m_position{0} {
	g_callback_list.push_back(this);
    this->initialize();
}

void LCD::initialize(void) {
    this->m_outputs[RS]->clearPin();
    this->m_outputs[D7]->clearPin();
    this->m_outputs[D6]->clearPin();
    this->m_outputs[D5]->setPin();
    this->m_outputs[D4]->clearPin();
	this->m_buffer = new uint8_t[this->m_rows * this->m_columns];
	for (uint8_t index = 0; index < this->m_rows * this->m_columns; index++) this->m_buffer[index] = ' ';
}

LCD& LCD::operator=(const int8_t *ptr_str) {
    this->_write(ptr_str);
	return *this;
}

void LCD::_write(const int8_t *ptr_str) {
	uint8_t index = 0;
	for (index = this->m_position; (index < this->m_rows * this->m_columns) && (ptr_str[index - this->m_position] != '\0'); index++)
		this->m_buffer[index] = ptr_str[index - this->m_position];
	this->m_position = index;
	if (this->m_position >= (this->m_columns * this->m_rows)) this->m_position = 0;
}

void LCD::_write(const int32_t value) {
	int32_t auxiliar;
	int8_t ptr_number[12];
//	int8_t *ptr_number = new int8_t[12];
	uint8_t position = 0;

	if (value < 0) {
		ptr_number[0] = '-';
		position++;
	}
	for (int32_t i = 0; (i < 10) && (value != 0); i++) {
		auxiliar = (int32_t)(value / this->pow(10, 9 - i));
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
    this->_write(ptr_number);
}

/*!
 * @brief Writes a string in the LCD on the specified position.
 * @param ptr_str String to be written to the display.
 * @param row Row where the string will be written.
 * @param column Column where the string will be written.
 */
void LCD::write(const int8_t *ptr_str, uint8_t row, uint8_t column) {
	if ((this->m_columns * row) + column <= this->m_columns * this->m_rows) {
		this->m_position = (this->m_columns * row) + column;
        this->_write(ptr_str);
	}
}

/*!
 * @brief Writes a number in the LCD on the specified position.
 * @param value Number to be written to the display.
 * @param row Row where the number will be written.
 * @param column Column where the number will be written.
 */
void LCD::write(const int32_t value, const uint8_t row, const uint8_t column) {
	if ((this->m_columns * row) + column <= this->m_columns * this->m_rows) {
		this->m_position = (this->m_columns * row) + column;
        this->_write(value);
	}
}

/*!
 * @brief Clears the LCD display.
 */
void LCD::clear(void) {
	for (uint8_t index = 0; index < (this->m_rows * this->m_columns); index++) this->m_buffer[index] = ' ';
	this->m_position = 0;
}

#pragma GCC push_options
#pragma GCC optimize ("O2")

void LCD::callbackMethod(void) {
	this->m_ticks--;
	if (this->m_ticks == 0) {
		switch (this->m_mode) {
		case s_print:
            this->writeInstruction(this->m_buffer[this->m_sweep], Gpio::HIGH);
			this->m_ticks = 1 * (g_systick_freq / 1000);
			this->m_sweep++;
			if ((this->m_sweep == this->m_columns) ||
				(this->m_sweep == 2 * this->m_columns && this->m_columns == 20) ||
				(this->m_sweep == 3 * this->m_columns && this->m_columns == 20) ||
				(this->m_sweep == (this->m_rows * this->m_columns))) this->m_mode = s_row;
			break;
		case s_row:
			if (this->m_sweep == (this->m_rows * this->m_columns)) {
                this->writeInstruction(SET_DDRAM | DDRAM_FIRST_ROW_ADDR, Gpio::LOW);
				this->m_sweep = 0;
			} else if (this->m_sweep == this->m_columns) this->writeInstruction(SET_DDRAM | DDRAM_SECOND_ROW_ADDR, Gpio::LOW);
			else if (this->m_sweep == 2 * this->m_columns && this->m_columns == 20) this->writeInstruction(SET_DDRAM | DDRAM_THIRD_ROW_ADDR, Gpio::LOW);
			else if (this->m_sweep == 3 * this->m_columns && this->m_columns == 20) this->writeInstruction(SET_DDRAM | DDRAM_FOURTH_ROW_ADDR, Gpio::LOW);
			this->m_ticks = 1 * (g_systick_freq / 1000);
			this->m_mode = s_print;
			break;
		default:
		case s_eigth_bits:
            this->m_outputs[ENABLE]->setPin();
            this->m_outputs[ENABLE]->clearPin();
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_four_bits;
			break;
		case s_four_bits:
            this->writeInstruction(FUNCTION_SET | (0 << 4) | (1 << 3) | (0 << 2), Gpio::LOW);	///DL	N	F
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_config_display;
			break;
		case s_config_display:
            this->writeInstruction(DISPLAY_CONTROL | (1 << 2) | (1 << 1) | (0 << 0), Gpio::LOW); ///D	C	B
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_config_cursor;
			break;
		case s_config_cursor:
            this->writeInstruction(ENTRY_MODE_SET | (1 << 1) | (0 << 0), Gpio::LOW);	///I/D	S
			this->m_ticks = 5 * (g_systick_freq / 1000);
			this->m_mode = s_clear;
			break;
		case s_clear:
            this->writeInstruction(CLEAR_DISPLAY, Gpio::LOW);
			this->m_ticks = 100 * (g_systick_freq / 1000);
			this->m_mode = s_print;
			break;
		}
	}
}

#pragma GCC pop_options

void LCD::writeInstruction(const uint8_t data, const Gpio::activity_t mode) {
	if (mode == Gpio::HIGH) this->m_outputs[RS]->setPin();
	else this->m_outputs[RS]->clearPin();

	if ((data >> 7) & 1) this->m_outputs[D7]->setPin();
	else this->m_outputs[D7]->clearPin();
	if ((data >> 6) & 1) this->m_outputs[D6]->setPin();
	else this->m_outputs[D6]->clearPin();
	if ((data >> 5) & 1) this->m_outputs[D5]->setPin();
	else this->m_outputs[D5]->clearPin();
	if ((data >> 4) & 1) this->m_outputs[D4]->setPin();
	else this->m_outputs[D4]->clearPin();

    this->m_outputs[ENABLE]->setPin();
    this->m_outputs[ENABLE]->clearPin();

	if ((data >> 3) & 1) this->m_outputs[D7]->setPin();
	else this->m_outputs[D7]->clearPin();
	if ((data >> 2) & 1) this->m_outputs[D6]->setPin();
	else this->m_outputs[D6]->clearPin();
	if ((data >> 1) & 1) this->m_outputs[D5]->setPin();
	else this->m_outputs[D5]->clearPin();
	if ((data >> 0) & 1) this->m_outputs[D4]->setPin();
	else this->m_outputs[D4]->clearPin();

    this->m_outputs[ENABLE]->setPin();
    this->m_outputs[ENABLE]->clearPin();
}

uint32_t LCD::pow(uint32_t base, uint32_t exponent) {
    uint32_t result = 1;

    while(true) {
        if (exponent & 1) result *= base;
        exponent >>= 1;
        if (!exponent) break;
        base *= base;
    }

    return result;
}

LCD::~LCD() {
	delete [] this->m_buffer;
}

//////////////////////////////////////////
/// LCD1602 or LCD2004 initializations ///
//////////////////////////////////////////

void initLCD1602(void) {
	#if defined(CN15_PINS)

	static std::vector<Gpio*> LCD1602_GPIOs_list;
	LCD1602_GPIOs_list.push_back(&LCD_D7);
	LCD1602_GPIOs_list.push_back(&LCD_D6);
	LCD1602_GPIOs_list.push_back(&LCD_D5);
	LCD1602_GPIOs_list.push_back(&LCD_D4);
	LCD1602_GPIOs_list.push_back(&LCD_RS);
	LCD1602_GPIOs_list.push_back(&LCD_EN);

	static LCD lcd1602(LCD1602_GPIOs_list, 2, 16);

	g_lcd = &lcd1602;

	#endif // defined(CN15_PINS)
}

void initLCD2004(void) {
	#if defined(CN15_PINS)

	static std::vector<Gpio*> LCD2004_GPIOs_list;
	LCD2004_GPIOs_list.push_back(&LCD_D7);
	LCD2004_GPIOs_list.push_back(&LCD_D6);
	LCD2004_GPIOs_list.push_back(&LCD_D5);
	LCD2004_GPIOs_list.push_back(&LCD_D4);
	LCD2004_GPIOs_list.push_back(&LCD_RS);
	LCD2004_GPIOs_list.push_back(&LCD_EN);

	static LCD lcd2004(LCD2004_GPIOs_list, 4, 20);

	g_lcd = &lcd2004;

	#elif defined(LCD_DEBUG_PINS)

	static std::vector<Gpio*> LCD2004_GPIOs_list;
	LCD2004_GPIOs_list.push_back(&LCD_DEBUG_D7);
	LCD2004_GPIOs_list.push_back(&LCD_DEBUG_D6);
	LCD2004_GPIOs_list.push_back(&LCD_DEBUG_D5);
	LCD2004_GPIOs_list.push_back(&LCD_DEBUG_D4);
	LCD2004_GPIOs_list.push_back(&LCD_DEBUG_RS);
	LCD2004_GPIOs_list.push_back(&LCD_DEBUG_EN);

	static LCD lcd2004(LCD2004_GPIOs_list, 4, 20);

	g_lcd = &lcd2004;

	#endif
}
