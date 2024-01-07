//
// Created by Usuario on 05/01/2024.
//

#ifndef IOTCONECTION_H
#define IOTCONECTION_H


#include "utils.h"
#include "IotVariable.h"
#include "IotListener.h"

class IotConnection {
    public:
        IotConnection() = default;

        virtual char *receiveMessage() = 0;

        virtual void uploadVariable(IotVariable variable) = 0;

        virtual void suscribeListener(IotListener *listener) = 0;

        ~IotConnection() = default;
};


#endif //IOTCONECTION_H
