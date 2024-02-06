#include "KeyboardPassword.h"

KeyboardPassword::KeyboardPassword(const char* password, Keyboard* keyboard) : m_keyboard{keyboard}, m_passwordIndex{0},
                                                                               m_timeoutCounter{PASSWORD_TIMEOUT} {
    for (uint8_t i = 0; i < strlen(password); i++) {
        char entry = password[i];
        if (entry >= '1' && entry <= '6') { //TODO: change based on implementation
            m_password.push_back(password[i] - '1'); //-1 if because of offset
        }
    }
    g_callback_list.push_back(this);

}

bool KeyboardPassword::checkPassword() {
    bool succeed = false;
    uint8_t keyPressed = m_keyboard->get();

    if (keyPressed != NO_KEY) {
        if (keyPressed == m_password[m_passwordIndex]) {
            m_passwordIndex++;
            if (m_passwordIndex == m_password.size()) {
                m_passwordIndex = 0;
                succeed = true;
            }
        }
        else {
            m_passwordIndex = 0;
        }

        m_timeoutCounter = PASSWORD_TIMEOUT;
    }
    return succeed;
}

void KeyboardPassword::callbackMethod() {
    if (m_timeoutCounter > 0) {
        m_timeoutCounter--;
    }
    else {
        m_passwordIndex = 0;
    }

}
