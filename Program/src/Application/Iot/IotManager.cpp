//
// Created by Usuario on 05/01/2024.
//

#include <cstring>
#include <cstdlib>
#include "IoTManager.h"

IoTManager::IoTManager(IoTConnection* ioTConnection) : ioTConection(ioTConnection) {

    ioTConnection->suscribeListener(this);

    g_callback_list.push_back(this);
}


void IoTManager::addVariableToUpload(char* name, int& variable, int uploadPeriod) {
    IoTVariable iotVariable = {name, variable, uploadPeriod, 0};

    variablesToUpload.insert(std::pair<char*, IoTVariable>(name, iotVariable));
}

void IoTManager::callbackMethod() {
    for (auto& [name, ioTVariable]: variablesToUpload) {
        if (ioTVariable.uploadCounter == ioTVariable.uploadPeriod) {
            ioTConection->uploadVariable(ioTVariable);
            ioTVariable.uploadCounter = 0;
        }
        ioTVariable.uploadCounter++;
    }
}

//Super lazy implementation
void IoTManager::processIoTMessage(char* message) {

    char* token = strtok(message, ",");

    if (strcmp(token, "setBright") == 0) {
        token = strtok(nullptr, ",");
        int bright = atoi(token);
        //TODO: Implement this
    }

}
