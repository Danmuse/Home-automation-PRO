//
// Created by Usuario on 06/01/2024.
//

#ifndef UARTIOT_H
#define UARTIOT_H


#include "IotConnection.h"
#include "UART.h"
#include "Callback.h"
//#include "Timer.h"

#define SERIAL_HEADER '$'
#define SERIAL_FOOTER '%'
#define SERIAL_TIMOUT 1000

#define MAX_MESSAGE_SIZE 50

class QTConnection : public IotConnection, public Callback {
    private:
        enum class SerialState {
                WAITING_HEADER,
                DECODING,
        };
        SerialState serialState = SerialState::WAITING_HEADER;
//        Timer timoutComunicacion;

        UART &uart;
        char recMessage[MAX_MESSAGE_SIZE];
        uint8_t recMessagePos;
        uint16_t timoutCounter;

        std::vector<IotListener*> listeners;

        void communicationTimeout();

    public:
        QTConnection(UART &uart);

        char* receiveMessage() override;

        void uploadVariable(IotVariable variable) override;

        void suscribeListener(IotListener* listener) override;

        void callbackMethod() override;


};


#endif //UARTIOT_H
