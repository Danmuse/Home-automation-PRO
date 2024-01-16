//
// Created by Usuario on 06/01/2024.
//

#ifndef QT_CONNECTION_H
#define QT_CONNECTION_H


#include "Callback.h"
#include "UART.h"
#include "IoTConnection.h"
#include <cstdlib>

#define SERIAL_HEADER '$'
#define SERIAL_FOOTER '%'
#define SERIAL_TIMOUT 1000

#define MAX_MESSAGE_SIZE 50

class QTConnection : public IoTConnection, public Callback {
    private:
        enum class SerialState {
                WAITING_HEADER,
                DECODING,
        };
        SerialState serialState = SerialState::WAITING_HEADER;
//      Timer timoutComunicacion;

        UART& uart;
        char recMessage[MAX_MESSAGE_SIZE];
        uint8_t recMessagePos;
        uint16_t timoutCounter;

        std::vector<IoTListener*> listeners;

        void communicationTimeout();

    public:
        QTConnection(UART& uart);

        char* receiveMessage() override;

        void uploadVariable(IoTVariable variable) override;

        void suscribeListener(IoTListener* listener) override;

        void callbackMethod() override;
};


#endif //QT_CONNECTION_H
