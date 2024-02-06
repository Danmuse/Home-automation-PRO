#ifndef KEYBOARDPASSWORD_H
#define KEYBOARDPASSWORD_H

#include "Keyboard.h"
#include <cstring>

#define PASSWORD_TIMEOUT 2000

class KeyboardPassword : public Callback {
    private:
        std::vector<uint8_t> m_password;
        Keyboard* m_keyboard;
        uint8_t m_passwordIndex;
        uint16_t m_timeoutCounter;

    public:
        KeyboardPassword(const char* password, Keyboard* keyboard);

        bool checkPassword();

        void callbackMethod() override;
};


#endif //KEYBOARDPASSWORD_H
