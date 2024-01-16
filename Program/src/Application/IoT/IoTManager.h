//
// Created by Usuario on 05/01/2024.
//

#ifndef LPC845ESQUELETO_IOTMANAGER_H
#define LPC845ESQUELETO_IOTMANAGER_H

#include <map>
#include "Callback.h"
#include "IoTConnection.h"

class IoTManager : public Callback, public IoTListener {

    private:
        std::map<char*, IoTVariable> variablesToUpload;

        IoTConnection* ioTConection;

    public:
        IoTManager(IoTConnection* ioTConnection);

        void addVariableToUpload(char* name, int &variable, int uploadPeriod);

        void processIoTMessage(char* message) override;

        void callbackMethod() override;

};


#endif //LPC845ESQUELETO_IOTMANAGER_H
