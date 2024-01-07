//
// Created by Usuario on 06/01/2024.
//

#ifndef LPC845ESQUELETO_IOTLISTENER_H
#define LPC845ESQUELETO_IOTLISTENER_H


class IotListener {
    public:
        IotListener() = default;
        virtual void processIotMessage(char* message)=0;
};


#endif //LPC845ESQUELETO_IOTLISTENER_H
