/*!
 * @file KeyboardPassword.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 07/02/2024 01:22:33
 * @brief Keyboard password Class API (Application Programming Interface).
 */

#ifndef KEYBOARD_PASSWORD_H_
#define KEYBOARD_PASSWORD_H_

#include <cstring>
#include "Keyboard.h"

#define PASSWORD_TIMEOUT 2000

class KeyboardPassword : public Callback {
private:
	std::vector<uint8_t> m_password;
	Keyboard* m_keyboard;
	uint8_t m_passwordIndex;
	uint16_t m_timeoutCounter;
public:
	KeyboardPassword() = delete;
	KeyboardPassword(const char* password, Keyboard* keyboard);
	bool checkPassword(void);
	void callbackMethod(void) override;
	~KeyboardPassword() = default;
};

#endif // KEYBOARD_PASSWORD_H_
