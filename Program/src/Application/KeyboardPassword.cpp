/*/*!
 * @file KeyboardPassword.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 07/02/2024 01:22:33
 */

#include "KeyboardPassword.h"

KeyboardPassword::KeyboardPassword(const char* password, Keyboard* keyboard) : Callback(),
m_keyboard{keyboard}, m_passwordIndex{0}, m_timeoutCounter{PASSWORD_TIMEOUT} {
    for (uint8_t i = 0; i < strlen(password); i++) {
        char entry = password[i];
        if (entry >= '1' && entry <= '2') { // TODO: Change based on implementation
            this->m_password.push_back(password[i] - '1'); //-1 if because of offset
        }
    }
    g_callback_list.push_back(this);
}

bool KeyboardPassword::checkPassword(void) {
    bool succeed = false;
    uint8_t keyPressed = this->m_keyboard->get();

    if (keyPressed != NO_KEY) {
        if (keyPressed == this->m_password[this->m_passwordIndex]) {
        	this->m_passwordIndex++;
            if (this->m_passwordIndex == this->m_password.size()) {
                this->m_passwordIndex = 0;
                succeed = true;
            }
        } else this->m_passwordIndex = 0;
        this->m_timeoutCounter = PASSWORD_TIMEOUT;
    }
    return succeed;
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void KeyboardPassword::callbackMethod(void) {
    this->m_timeoutCounter > 0 ? this->m_timeoutCounter-- : this->m_passwordIndex = 0;
}

#pragma GCC pop_options