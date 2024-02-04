/*/*!
 * @file stateMachines.cpp
 * @par Author & Doxygen Editor
 *   Y  YO que no se editar esto
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/08/2023 01:02:47
 */

#include "stateMachines.h"

void userRegistrationStateMachine(UserRegistrationState& state) {
    switch (state) {
        case UserRegistrationState::WAITING_FOR_PASSWORD:
            //TODO: Password coso
            state = UserRegistrationState::WAITING_FOR_USER;
            break;
        case UserRegistrationState::WAITING_FOR_USER:
            MFRC522::UID_st uuid;
            RFID_result_t result = g_rfid->getUID(&uuid);

            if (result == RFID_OK) {
                if (isUserRegistered(uuid)) {
                    state = UserRegistrationState::WAITING_FOR_PASSWORD;
                }
                else {
                    registerNewUser(uuid);
                }
                //TODO: Mostrar en el LCD algo?
            }
            break;
    }
}

