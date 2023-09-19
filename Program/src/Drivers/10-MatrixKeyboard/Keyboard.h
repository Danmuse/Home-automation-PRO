/*!
 * @file Keyboard.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 20/08/2023 12:55:47
 * @brief Matrix Keyboard Class API (Application Programming Interface).
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <vector>
#include "GPIO.h"
#include "Callback.h"

#define DEBOUNCE_TIME 4
#define NO_KEY 0xFF

class Keyboard : public Callback {
private:
	const std::vector<Gpio*> &m_columns; //!< WARNING: Direction of the GPIOs that constitute this member are "INPUT"
	const std::vector<Gpio*> &m_rows; 	 //!< WARNING: Direction of the GPIOs that constitute this member are "OUTPUT"

	uint8_t m_last_keyPressed;		//!< Status of the current key pressed
	uint32_t m_debounceCounter;		//!< Debounce counter
	uint8_t m_current_keyPressed;	//!< Key pressed pressed
	const uint8_t m_debounces;		//!< Maximum number of readings to debounce
public:
	Keyboard() = delete;
	Keyboard(std::vector<Gpio*> &columns, std::vector<Gpio*> &rows);
	void CallbackMethod(void);
	uint8_t	Get(void);
	virtual ~Keyboard();
private:
	uint8_t KeyboardHW(void);
	void KeyboardSW(uint8_t keyStatus);
};

#endif /* KEYBOARD_H_ */
