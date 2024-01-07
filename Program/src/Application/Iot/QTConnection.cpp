//
// Created by Usuario on 06/01/2024.
//

#include <cstdio>
#include "QTConnection.h"
#include "IoTListener.h"

QTConnection::QTConnection(UART &uart) : uart{uart}, recMessagePos{0}, timoutCounter{SERIAL_TIMOUT} {

    g_callback_list.push_back(this);
}

char* QTConnection::receiveMessage() {

    this->recMessage[recMessagePos] = '\0';

    for (IoTListener* listener: listeners) {
        listener->processIoTMessage(recMessage + 1);
    }


}

void QTConnection::uploadVariable(IoTVariable variable) {

    char message[MAX_MESSAGE_SIZE];
    sprintf(message, "%c%s,%d%c", SERIAL_HEADER, variable.name, variable.variable, SERIAL_FOOTER);

    uart.transmit(message, strlen(message));
}

void QTConnection::suscribeListener(IoTListener* listener) {
    this->listeners.push_back(listener);
}

void QTConnection::callbackMethod() {

    bool newChar = uart.receive(this->recMessage + recMessagePos, 1);

    if (!newChar) {
        timoutCounter = SERIAL_TIMOUT;
        switch (serialState) {
            case SerialState::WAITING_HEADER:
                // Pos should always be 0
                if (recMessage[recMessagePos] == SERIAL_HEADER) {
                    recMessagePos++;
                    serialState = SerialState::DECODING;
                    //TODO: Add timout
//                timoutComunicacion.timerStart(1);
                }

                break;
            case SerialState::DECODING:
                if (recMessage[recMessagePos] == SERIAL_FOOTER) {
//                timoutComunicacion.timerStop();
                    serialState = SerialState::WAITING_HEADER;
                    receiveMessage();

                    recMessagePos = 255; //Para que al sumar sea todo 0
                }

                recMessagePos++;
                recMessagePos %= MAX_MESSAGE_SIZE;//Overflow overlaps the message

                break;
        }

    }

    if (--timoutCounter == 0) {
        communicationTimeout();
        timoutCounter = SERIAL_TIMOUT;
    }
}

void QTConnection::communicationTimeout() {
    this->serialState = SerialState::WAITING_HEADER;
    memset(this->recMessage, 0, this->recMessagePos);
    this->recMessagePos = 0;
}


