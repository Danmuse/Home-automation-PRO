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
    IoTVariable_st iotVariable = { name, &variable, { }, uploadPeriod, 0} ;
    this->m_variablesToUpload.insert(std::pair<const char*, IoTVariable_st>(name, iotVariable));
}

void IoTManager::processIoTMessage(char* message) {
    char* token = strtok(message, ":");

    for (auto& actionPair: this->m_actions) {
        if (strcmp(actionPair.first, token) == 0) {
            actionPair.second(strtok(nullptr, ":")); //Maybe split this inside connection and not get message
            return;//Only one function per action TODO: Maybe change?
        }
    }
}

void IoTManager::registerAction(const char* topic, ActionListener actionListener) {
    this->m_actions.insert(std::pair<const char*, ActionListener>(topic, actionListener));
}

bool IoTManager::initializeConnection(void) {
    for (auto& state: states) this->m_ioTConnection->uploadVariable(state);
    return true;
}

void IoTManager::registerState(const char* name, bool& variable, std::initializer_list<const char*> strings) {
    states.push_back({name, &variable, strings, 0, 0});
}

void IoTManager::registerState(const char* name, uint8_t& variable) {
    states.push_back({ name, &variable, { }, 0, 0 });
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void IoTManager::callbackMethod(void) {
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

#pragma GCC pop_options
