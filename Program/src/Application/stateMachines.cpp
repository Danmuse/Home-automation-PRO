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

    if (result != EEPROM_OK) { return false; }
#if RFID_USER_UID_SIZE == 7
    uint32_t userId = 0;
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) { userId |= uid.uidByte[index] << (index * 8); }
    for (size_t index = 0; index < userCount; index++) {
        uint32_t registeredUID = 0;
        result = g_eeprom->read(&registeredUID, M24C16::UINT32, index * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
        if (result != EEPROM_OK) { return false; }
        if (registeredUID == userId) { isRegistered = true; }
    }
#else
    for (size_t index = 0; index < userCount; index++) {
        uint8_t registeredUID[RFID_USER_UID_SIZE];
        for (size_t j_index = 0; j_index < RFID_USER_UID_SIZE; j_index++) {
            result = g_eeprom->read(&registeredUID[j_index], M24C16::UINT8, index * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) return false;
        }
        if (memcmp(registeredUID, uid.uidByte, RFID_USER_UID_SIZE) == 0) isRegistered = true;
    }
#endif
    return isRegistered;
}

static bool registerNewUser(const MFRC522::UID_st& uid) {
    uint8_t userCount = 0;
    EEPROM_result_t result = g_eeprom->read(&userCount, M24C16::UINT8, 0);

    if (result != EEPROM_OK) { return false; }
#if RFID_USER_UID_SIZE == 4
    uint32_t userId = 0;
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) { userId |= uid.uidByte[index] << (index * 8); }
    result = g_eeprom->write(userId, userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
    if (result != EEPROM_OK) { return false; }
#else
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) {
        result = g_eeprom->write(uid.uidByte[index], userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
        if (result != EEPROM_OK) return false;
    }
#endif
    return true;
}

KeyboardPassword keyBoardPassword("1222112", g_keyboard);
Timer userRegistrationTimer(nullptr, Timer::SEC);

void userRegistrationStateMachine(UserRegistrationState& state) {
    switch (state) {
        case UserRegistrationState::WAITING_FOR_PASSWORD:
            if (keyBoardPassword.checkPassword()) {
                state = UserRegistrationState::WAITING_FOR_USER;
                userRegistrationTimer.timerStart(30);
            }
            break;
        case UserRegistrationState::WAITING_FOR_USER:
            MFRC522::UID_st uuid;
            RFID_result_t result = g_rfid->getUID(&uuid);

            if (result == RFID_OK) {
                if (!isUserRegistered(uuid)) {
                    registerNewUser(uuid);
                }
                g_lcd->write("Usuario registrado", 0, 0);
                LED_GREEN.clearPin();
                state = UserRegistrationState::WAITING_FOR_PASSWORD;
                userRegistrationTimer.setTimer(0);
//				g_lcd->write(g_rfid->printUID(), 0, 0);
            }
            else {
                LED_GREEN.setPin();
            }

            if (userRegistrationTimer.getTicks() == 0) {
                LED_GREEN.clearPin();
                state = UserRegistrationState::WAITING_FOR_PASSWORD;
                g_lcd->write("Timout", 0, 0);
            }
            break;
    }
}

Timer doorOpeningTimer(nullptr, Timer::SEC);
MFRC522::UID_st uuid;


void doorOpeningStateMachine(DoorOpeningState& state) {
    switch (state) {
        case DoorOpeningState::WAITING_FOR_RFID:
            if (userRegistrationTimer.getTicks() == 0) {
                RFID_result_t result;
                result = g_rfid->getUID(&uuid);

                if (result == RFID_OK) {
                    state = DoorOpeningState::CHECKING_USER;
                }
            }
            break;
        case DoorOpeningState::CHECKING_USER:
            if (isUserRegistered(uuid)) {
                g_servo->setAngle(90);
                state = DoorOpeningState::DOOR_OPEN;
                doorOpeningTimer.timerStart(3);
            }
            else { state = DoorOpeningState::WAITING_FOR_RFID; }
            break;
        case DoorOpeningState::DOOR_OPEN:
            if (doorOpeningTimer.getTicks() == 0) {
                g_servo->setAngle(0);
                state = DoorOpeningState::WAITING_FOR_RFID;
            }
            break;
    }
}