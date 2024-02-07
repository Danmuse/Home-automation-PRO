/*/*!
 * @file stateMachines.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/02/2024 17:06:04
 */

#include "stateMachines.h"

static bool isUserRegistered(const MFRC522::UID_st& uid);

static bool registerNewUser(const MFRC522::UID_st& uid);

static bool isUserRegistered(const MFRC522::UID_st& uid) {
    uint8_t userCount = 0;
    EEPROM_result_t result = g_eeprom->read(&userCount, M24C16::UINT8, 0);
    bool isRegistered = false;

    if (result != EEPROM_OK) return false;
    if (RFID_USER_UID_SIZE == 4) {
        uint32_t userId = 0;
        for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) userId |= uid.uidByte[index] << (index * 8);
        for (size_t index = 0; index < userCount; index++) {
            uint32_t registeredUID = 0;
            result = g_eeprom->read(&registeredUID, M24C16::UINT32, index * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) return false;
            if (registeredUID == userId) isRegistered = true;
        }
    }
    else {
    	// TODO: CHECK THIS FRAGMENT OF CODE
    	// IS THE SAME INDEX ITERATING WITHIN THE SAME FOR LOOP??
        for (uint8_t i = 0; i < userCount; i++) {
            uint8_t registeredUID[RFID_USER_UID_SIZE];
            for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) {
                result = g_eeprom->read(&registeredUID[i], M24C16::UINT8, i * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
                if (result != EEPROM_OK) return false;
            }
            if (memcmp(registeredUID, uid.uidByte, RFID_USER_UID_SIZE) == 0) isRegistered = true;
        }
    }
    return isRegistered;
}

static bool registerNewUser(const MFRC522::UID_st& uid) {
    uint8_t userCount = 0;
    EEPROM_result_t result = g_eeprom->read(&userCount, M24C16::UINT8, 0);

    if (result != EEPROM_OK) return false;
    if (RFID_USER_UID_SIZE == 4) {
        uint32_t userId = 0;
        for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) userId |= uid.uidByte[index] << (index * 8);
        result = g_eeprom->write(userId, userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
        if (result != EEPROM_OK) return false;
    } else {
        for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) {
            result = g_eeprom->write(uid.uidByte[index], userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) return false;
        }

    }
    return true;
}
KeyboardPassword keyBoardPassword("5542", g_keyboard);

void userRegistrationStateMachine(UserRegistrationState& state) {
    switch (state) {
        case UserRegistrationState::WAITING_FOR_PASSWORD:
            if (keyBoardPassword.checkPassword()) state = UserRegistrationState::WAITING_FOR_USER;
            break;
        case UserRegistrationState::WAITING_FOR_USER:
            MFRC522::UID_st uuid;
            RFID_result_t result = g_rfid->getUID(&uuid);

            if (result == RFID_OK) {
                if (isUserRegistered(uuid)) state = UserRegistrationState::WAITING_FOR_PASSWORD;
                else registerNewUser(uuid);
                // TODO: Mostrar en el LCD algo?
            }
            break;
    }
}

Timer doorOpeningTimer(nullptr, Timer::SEC);
MFRC522::UID_st uuid;

void doorOpeningStateMachine(DoorOpeningState& state) {
    switch (state) {
        case DoorOpeningState::WAITING_FOR_RFID:
            RFID_result_t result;
            result = g_rfid->getUID(&uuid);
            if (result == RFID_OK) state = DoorOpeningState::CHECKING_USER;
            break;
        case DoorOpeningState::CHECKING_USER:
            if (isUserRegistered(uuid)) {
                g_servo->setAngle(90);
                state = DoorOpeningState::DOOR_OPEN;
                doorOpeningTimer.timerStart(3);
            } else state = DoorOpeningState::WAITING_FOR_RFID;
            break;
        case DoorOpeningState::DOOR_OPEN:
            if (doorOpeningTimer.getTicks() == 0) {
                g_servo->setAngle(0);
                state = DoorOpeningState::WAITING_FOR_RFID;
            }
            break;
    }
}
