/*!
 * @file LCD1602.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 27/08/2023 23:36:45
 * @brief Liquid Crystal LCD1602 Class API (Application Programming Interface).
 */

#ifndef LCD1602_H_
#define LCD1602_H_

#include <vector>
#include "systick.h"
#include "ProgramConfig.h"

#define CLEAR_DISPLAY			0b00000001 //!< Set CLEAR register
#define RETURN_HOME				0b00000010 //!< Set Cursor = 0 register
#define ENTRY_MODE_SET			0b00000100 //!< Set Input Writer register
#define DISPLAY_CONTROL			0b00001000 //!< Set Control register
#define CURSOR_DISPLAY_SHIFT	0b00010000 //!< Set Cursor register
#define FUNCTION_SET			0b00100000 //!< Set Operation Mode register
#define SET_CGRAM				0b01000000 //!< Set CGRAM register
#define SET_DDRAM				0b10000000 //!< Set DDRAM register

class LCD1602 : public Callback {
private:
	enum mode_LCD1602_t { s_eigth_bits, s_four_bits, s_config_display, s_config_cursor, s_clear, s_print, s_row };

	const std::vector<Gpio*> m_outputs;	//!< Vector of GPIO outputs
	mode_LCD1602_t m_mode;				//!< Current state of the display
	uint8_t *m_buffer;					//!< Display _write buffer
	const uint8_t m_rows;				//!< Number of display rows
	const uint8_t m_columns;			//!< Number of display columns
	uint8_t m_ticks;
	uint8_t m_sweep;					//!< Buffer _write position
	uint8_t m_position;					//!< User _write position

	void initialize(void);
	void writeInstruction(const uint8_t data, const Gpio::activity_t mode);
	uint32_t pow(uint32_t base, uint32_t exponent);
	void _write(const char* ptr_str);
	void _write(const int32_t value);
public:
	enum { D7, D6, D5, D4, RS, ENABLE };

	LCD1602() = delete;
	LCD1602(std::vector<Gpio*> &outputs, const uint8_t rows, const uint8_t columns);
	LCD1602& operator=(const char *ptr_str);
	void write(const char* ptr_str, const uint8_t row = 0, const uint8_t column = 0);
	void write(const int32_t value, const uint8_t row = 0, const uint8_t column = 0);
	void clear(void);
	void callbackMethod(void) override;
	virtual ~LCD1602() = default;
};

extern LCD1602 *g_lcd1602;

void initLCD1602(void);

#endif /* LCD1602_H_ */
