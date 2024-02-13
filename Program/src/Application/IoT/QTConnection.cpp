/*/*!
 * @file QTConnection.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 16:49:27
 */

#include "QTConnection.h"

QTConnection::QTConnection(UART& uart) : IoTConnection(), Callback(),
                                         m_uart{uart}, m_recMessagePos{0},
                                         m_timeoutCounter{(uint16_t) (SERIAL_TIMEOUT * (g_systick_freq / 1000))} {
    g_callback_list.push_back(this);
}

void QTConnection::receiveMessage(void) {
    this->m_recMessage[this->m_recMessagePos] = '\0';

    if (strcmp(this->m_recMessage + 1, "connect:ok") == 0) {
        this->establishConnection();
    }

    for (IoTListener* listener: this->m_listeners) {
        listener->processIoTMessage(this->m_recMessage + 1);
        //+1 to skip the header
    }

}

void QTConnection::establishConnection(void) {
    char message[MAX_MESSAGE_SIZE];

    message[0] = SERIAL_HEADER;
    strcpy(message + 1, "connect:ok");
    uint8_t size = strlen(message);
    message[size] = SERIAL_FOOTER;
    message[size + 1] = '\0';

    this->m_uart.transmit(message, strlen(message));

    bool confirm = true;

    for (IoTListener* listener: this->m_listeners) {
        confirm &= listener->initializeConnection();
        //Make sure all of then return true
    }
}

void QTConnection::uploadVariable(IoTVariable_st ioTVariable) {
    char message[MAX_MESSAGE_SIZE];

    message[0] = SERIAL_HEADER;
    strcpy(message + 1, ioTVariable.name);
    strcat(message, ":");

    if (ioTVariable.stringRepresent.empty()) {
        itoa(*((uint8_t*) ioTVariable.variable), message + strlen(message), 10);
    }
    else {
        uint8_t value = *((uint8_t*) ioTVariable.variable); //uint8 because if for booleans(if it is an integer :/)

        if (value <= ioTVariable.stringRepresent.size()) {
            strcat(message, ioTVariable.stringRepresent[value]);
        }
        else {
            strcat(message, ioTVariable.stringRepresent.back());
        }
    }

    uint8_t size = strlen(message);
    message[size] = SERIAL_FOOTER;
    message[size + 1] = '\0';

    this->m_uart.transmit(message, strlen(message));
}

void QTConnection::suscribeListener(IoTListener* listener) {
    this->m_listeners.push_back(listener);
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void QTConnection::callbackMethod(void) {
    bool newChar = this->m_uart.receive(this->m_recMessage + this->m_recMessagePos, 1);

    if (newChar) {
        this->m_timeoutCounter = SERIAL_TIMEOUT * (g_systick_freq / 1000);
        switch (this->m_serialState) {
            case WAITING_HEADER:
                // Pos should always be 0
                if (this->m_recMessage[this->m_recMessagePos] == SERIAL_HEADER) {
                    this->m_recMessagePos++;
                    this->m_serialState = DECODING;
                    // timeout
//                this->timeoutComunicacion.timerStart(1);
                }

                break;
            case DECODING:
                if (this->m_recMessage[this->m_recMessagePos] == SERIAL_FOOTER) {
//                this->timeoutComunicacion.timerStop();
                    this->m_serialState = WAITING_HEADER;
                    this->receiveMessage();
                    //Not needed, but just in case
                    memset(this->m_recMessage, 0, this->m_recMessagePos);

                    this->m_recMessagePos = 255; // So that when adding everything is equivalent to 0
                }

                this->m_recMessagePos++;
                this->m_recMessagePos %= MAX_MESSAGE_SIZE; // Overflow overlaps the message

                break;
        }

    }

    if (--this->m_timeoutCounter == 0) {
        this->communicationTimeout();
        this->m_timeoutCounter = SERIAL_TIMEOUT * (g_systick_freq / 1000);
    }
}

#pragma GCC pop_options

void QTConnection::communicationTimeout(void) {
    this->m_serialState = WAITING_HEADER;
    memset(this->m_recMessage, 0, this->m_recMessagePos);
    this->m_recMessagePos = 0;
}
