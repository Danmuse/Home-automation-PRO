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

/*!
 * @brief Adds a variable to upload to the IoT platform.
 * @param name Variable name.
 * @param variable Reference for the variable to upload.
 * @param uploadPeriod Period(in ms) in which variable will be uploaded
 * @Note Variable should be updated normally, the manager will upload get its value when it is time to upload it
 */
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

/*!
 * @brief Registers an action to be executed when a message is received from the IoT platform.
 * @param topic The topic to listen to.
 * @param actionListener The function to be executed when a message of specified topic is received.
 */
void IoTManager::registerAction(const char* topic, ActionListener actionListener) {
    this->m_actions.insert(std::pair<const char*, ActionListener>(topic, actionListener));
}

bool IoTManager::initializeConnection(void) {
    for (auto& state: states) this->m_ioTConnection->uploadVariable(state);
    return true;
}

/*!
 * @brief Registers a variable to be uploaded when the IoT platform initializes the connection.
 * @param name Name of the variable to be uploaded.
 * @param variable Reference to the variable to be uploaded.
 * @param strings String representation for the variable, first is for 'false' condition. (e.g. {"OFF", "ON"})
 */
void IoTManager::registerState(const char* name, bool& variable, std::initializer_list<const char*> strings) {
    states.push_back({name, &variable, strings, 0, 0});
}

/*!
 * @brief Registers a variable to be uploaded when the IoT platform initializes the connection.
 * @param name Name of the variable to be uploaded.
 * @param variable Reference to the variable to be uploaded.
 */
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
