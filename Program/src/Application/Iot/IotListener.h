//
// Created by Usuario on 06/01/2024.
//

#ifndef LPC845ESQUELETO_IOTLISTENER_H
#define LPC845ESQUELETO_IOTLISTENER_H


class IoTListener {
    public:
        IoTListener() = default;
        virtual void processIoTMessage(char* message)=0;
};


#endif //LPC845ESQUELETO_IOTLISTENER_H
