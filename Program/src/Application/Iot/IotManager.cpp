//
// Created by Usuario on 05/01/2024.
//

#include <cstring>
#include <cstdlib>
#include "IotManager.h"

IotManager::IotManager(IotConnection* iotConection) : iotConection(iotConection) {

    iotConection->suscribeListener(this);
}


void IotManager::addVariableToUpload(char* name, int &variable, int uploadPeriod) {
    IotVariable iotVariable = {name, variable, uploadPeriod, 0};

    variablesToUpload.insert(std::pair<char*, IotVariable>(name, iotVariable));
}

void IotManager::callbackMethod() {
    for (auto &[name, iotVariable]: variablesToUpload) {
        if (iotVariable.uploadCounter == iotVariable.uploadPeriod) {
            iotConection->uploadVariable(iotVariable);
            iotVariable.uploadCounter = 0;
        }
        iotVariable.uploadCounter++;
    }
}

//Super lazy implementation
void IotManager::processIotMessage(char* message) {

    char* token = strtok(message, ",");

    if (strcmp(token, "setBright") == 0) {
        token = strtok(nullptr, ",");
        int bright = atoi(token);
        //TODO: Implement this
    }

}
