//
// Created by Usuario on 05/01/2024.
//

#ifndef IOTCONECTION_H
#define IOTCONECTION_H

#include "utils.h"
#include "IoTListener.h"
#include "IoTVariable.h"

class IoTConnection {
    public:
        IoTConnection() = default;

        virtual char *receiveMessage() = 0;

        virtual void uploadVariable(IoTVariable variable) = 0;

        virtual void suscribeListener(IoTListener *listener) = 0;

        ~IoTConnection() = default;
};


#endif //IOTCONECTION_H
