/*!
 * @file QTConnection.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 16:49:27
 * @brief Qt Connection Class API (Application Programming Interface).
 */

#ifndef QT_CONNECTION_H_
#define QT_CONNECTION_H_

#include <cstdlib>
#include "systick.h"
#include "UART.h"
#include "IoTConnection.h"

#define SERIAL_HEADER '$'
#define SERIAL_FOOTER '%'
#define SERIAL_TIMEOUT 1000

#define MAX_MESSAGE_SIZE 50

class QTConnection : public IoTConnection, Callback {
    private:
        enum serialState_t {
                WAITING_HEADER, DECODING
        };
        serialState_t m_serialState = WAITING_HEADER;
//  Timer m_timeoutComunicacion;

        UART& m_uart;
        char m_recMessage[MAX_MESSAGE_SIZE];
        uint8_t m_recMessagePos;
        uint16_t m_timeoutCounter; // WARNING: Using this member as a "uint16_t" type could cause instabilities in the configured time period
        std::vector<IoTListener*> m_listeners;

        void communicationTimeout();

    public:
        QTConnection() = delete;

        QTConnection(UART& uart);

        void receiveMessage() override;

        void establishConnection() override;

        void uploadVariable(IoTVariable_st ioTVariable) override;

        void uploadLiteral(const char* literal) override;

        void suscribeListener(IoTListener* listener) override;

        void callbackMethod() override;

        virtual ~QTConnection() = default;
};

#endif // QT_CONNECTION_H_
