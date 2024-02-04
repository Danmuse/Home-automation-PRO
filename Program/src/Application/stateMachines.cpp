/*/*!
 * @file stateMachines.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/08/2023 01:02:47
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

        for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) userId |= uid.uidByte[i] << (i * 8);

        for (uint8_t i = 0; i < userCount; i++) {
            uint32_t registeredUID = 0;
            result = g_eeprom->read(&registeredUID, M24C16::UINT32, i * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) return false;
            if (registeredUID == userId) isRegistered = true;
        }
    }
    else {
        for (uint8_t i = 0; i < userCount; i++) {
            uint8_t registeredUID[RFID_USER_UID_SIZE];
            for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) {
                result = g_eeprom->read(&registeredUID[i], M24C16::UINT8, i * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
                if (result != EEPROM_OK) {
                    return false;
                }
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

        for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) userId |= uid.uidByte[i] << (i * 8);

        result = g_eeprom->write(userId, userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
        if (result != EEPROM_OK) return false;
    }
    else {
        for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) {
            result = g_eeprom->write(uid.uidByte[i], userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) return false;
        }

    }
    return true;
}

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

Timer doorOpeningTimer(nullptr, Timer::SEC);
MFRC522::UID_st uuid;

void doorOpeningStateMachine(DoorOpeningState& state) {
    switch (state) {
        case DoorOpeningState::WAITING_FOR_RFID:
            RFID_result_t result;
            result = g_rfid->getUID(&uuid);
            if (result == RFID_OK) {
                state = DoorOpeningState::CHECKING_USER;
            }
            break;
        case DoorOpeningState::CHECKING_USER:
            if (isUserRegistered(uuid)) {
                g_servo->setAngle(90);
                state = DoorOpeningState::DOOR_OPEN;
                doorOpeningTimer.timerStart(3);
            }
            else {
                state = DoorOpeningState::WAITING_FOR_RFID;
            }
            break;
        case DoorOpeningState::DOOR_OPEN:
            if (doorOpeningTimer.getTicks() == 0) {
                g_servo->setAngle(0);
                state = DoorOpeningState::WAITING_FOR_RFID;
            }
            break;
    }
}
