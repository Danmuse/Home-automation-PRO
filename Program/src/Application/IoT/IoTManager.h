/*!
 * @file IoTManager.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/01/2024 12:23:56
 * @brief IoT Manager Class API (Application Programming Interface).
 */

#ifndef IOT_MANAGER_H_
#define IOT_MANAGER_H_

#include <map>
#include <cstring>
#include <string>
#include <cstdlib>
#include "Callback.h"
#include "IoTConnection.h"

typedef void(* ActionListener)(char* message);

class IoTManager : public IoTListener, Callback {
    private:
        std::map<const char*, IoTVariable_st> m_variablesToUpload;
        IoTConnection* m_ioTConnection;
        std::map<const char*, ActionListener> m_actions;

    public:

        IoTManager() = delete;

        IoTManager(IoTConnection* ioTConnection);

        void addVariableToUpload(const char* name, int& variable, int uploadPeriod);

        bool initializeConnection() override;
        void processIoTMessage(char* message) override;

        void registerAction(const char* topic, ActionListener actionListener);

        void callbackMethod() override;

        virtual ~IoTManager() = default;

        void initConectionState();

        void registerState(const char* string, bool& variable, std::initializer_list<const char*> strings);

        void registerState(const char* string, int& vairable);
};

#endif // IOT_MANAGER_H_
