//
// Created by Usuario on 05/01/2024.
//

#ifndef LPC845ESQUELETO_IOTMANAGER_H
#define LPC845ESQUELETO_IOTMANAGER_H

#include <map>
#include "IotConnection.h"
#include "Callback.h"
#include "IotVariable.h"
#include "IotListener.h"

class IotManager : public Callback, public IotListener {

    private:
        std::map<char*, IotVariable> variablesToUpload;

        IotConnection* iotConection;

    public:
        IotManager(IotConnection* iotConection);

        void addVariableToUpload(char* name, int &variable, int uploadPeriod);

        void processIotMessage(char* message) override;

        void callbackMethod() override;

};


#endif //LPC845ESQUELETO_IOTMANAGER_H
