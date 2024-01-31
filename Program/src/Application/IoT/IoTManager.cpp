/*/*!
 * @file IoTManager.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/01/2024 12:23:56
 */

#include "IoTManager.h"

IoTManager::IoTManager(IoTConnection* ioTConnection) : IoTListener(), Callback(),
                                                       m_ioTConnection{ioTConnection} {
    ioTConnection->suscribeListener(this);
    g_callback_list.push_back(this);
}

void IoTManager::addVariableToUpload(const char* name, int& variable, int uploadPeriod) {
    IoTVariable_st iotVariable = {name, variable, uploadPeriod, 0};
    this->m_variablesToUpload.insert(std::pair<const char*, IoTVariable_st>(name, iotVariable));
}

void IoTManager::callbackMethod() {
    for (auto& variablePair: this->m_variablesToUpload) {
        // auto& name = variablePair.first;
        auto& ioTVariable = variablePair.second;
        if (ioTVariable.uploadCounter == ioTVariable.uploadPeriod) {
            this->m_ioTConnection->uploadVariable(ioTVariable);
            ioTVariable.uploadCounter = 0;
        }
        ioTVariable.uploadCounter++;
    }
}

// Super lazy implementation
void IoTManager::processIoTMessage(char* message) {
    char* token = strtok(message, ":");

    if (strcmp(token, "connect")) {
        initConectionState();
    }

    for (auto& actionPair: this->m_actions) {
        if (strcmp(actionPair.first, token) == 0) {
            actionPair.second(strtok(nullptr, ":"));
            return;//Only one function per action TODO: Maybe change?
        }
    }
}

void IoTManager::registerAction(const char* topic, ActionListener actionListener) {
    this->m_actions.insert(std::pair<const char*, ActionListener>(topic, actionListener));
}

void IoTManager::initConectionState() {
    for (auto& variablePair: this->m_variablesToUpload) {
        auto& ioTVariable = variablePair.second;
        ioTVariable.uploadCounter = 0;
    }

}

void IoTManager::registerState(const char* string, bool& variable, std::initializer_list<const char*> strings) {


}

void IoTManager::registerState(const char* string, int &variable) {

}

bool IoTManager::initializeConnection() {

    return false;
}
