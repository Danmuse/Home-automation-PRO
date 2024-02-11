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
#if RFID_USER_UID_SIZE == 4 && M24C16_USING_MODIFIERS == 1
    uint32_t userId = 0;
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) userId |= uid.uidByte[index] << (index * 8);
    for (size_t index = 0; index < userCount; index++) {
        uint32_t registeredUID = 0;
        result = g_eeprom->read(&registeredUID, M24C16::UINT32, index * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
        if (result != EEPROM_OK) return false;
        if (registeredUID == userId) isRegistered = true;
    }
#else
    for (size_t index = 0; index < userCount; index++) {
        uint8_t registeredUID[RFID_USER_UID_SIZE];
        for (size_t j_index = 0; j_index < RFID_USER_UID_SIZE; j_index++) {
            result = g_eeprom->read(&registeredUID[j_index], M24C16::UINT8, index * RFID_USER_UID_SIZE + USERS_INIT_POSITION + j_index);
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

    if (result != EEPROM_OK) return false;
#if RFID_USER_UID_SIZE == 4 && M24C16_USING_MODIFIERS == 1
    uint32_t userId = 0;
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) userId |= uid.uidByte[index] << (index * 8);
    result = g_eeprom->write(userId, userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
    if (result != EEPROM_OK) return false;
#else
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) {
        result = g_eeprom->write(uid.uidByte[index], userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION + index);
        if (result != EEPROM_OK) return false;
    }
#endif
    result = g_eeprom->write(++userCount, 0);
    if (result != EEPROM_OK) return false;
    return true;
}

KeyboardPassword keyBoardPassword("1222112", g_keyboard);
Timer userRegistrationTimer(nullptr, Timer::SEC);

void userRegistrationStateMachine(UserRegistrationState& state) {
    switch (state) {
        case UserRegistrationState::WAITING_FOR_PASSWORD:
            if (keyBoardPassword.checkPassword()) {
                state = UserRegistrationState::WAITING_FOR_USER;
                userRegistrationTimer.setTimer(30);
            }
            break;
        case UserRegistrationState::WAITING_FOR_USER:
            MFRC522::UID_st uuid;
            RFID_status_t result = RFID_BUSY;
            while (result == RFID_BUSY) result = g_rfid->getUID(&uuid);

            if (result == RFID_SUCCESS) {
                if (!isUserRegistered(uuid)) {
                    uint8_t registerAttempts = 0;
                    while (!registerNewUser(uuid) && registerAttempts < 3) registerAttempts++;
                }

                // TODO: After having exceeded the time between each state machine, a g_lcd->clear() must be performed

                g_lcd->write("Registered user");
                LED_GREEN.clearPin();
                state = UserRegistrationState::WAITING_FOR_PASSWORD;

                userRegistrationTimer.setTimer(5);
            } else LED_GREEN.setPin();

            if (userRegistrationTimer.getTicks() == 0) {
                LED_GREEN.clearPin();
                state = UserRegistrationState::WAITING_FOR_PASSWORD;

                // TODO: After having exceeded the time between each state machine, a g_lcd->clear() must be performed

                g_lcd->write("Timeout error");
            }
            break;
    }
}

Timer doorOpeningTimer(nullptr, Timer::SEC);
MFRC522::UID_st uuid;

void doorOpeningStateMachine(DoorOpeningState& state) {
    bool userRegistered = false;
    uint8_t checkCount = 0;

    switch (state) {
        case DoorOpeningState::WAITING_FOR_RFID:
            if (userRegistrationTimer.getTicks() == 0) {
                RFID_status_t result = RFID_BUSY;
                while (result == RFID_BUSY) result = g_rfid->getUID(&uuid);

                if (result == RFID_SUCCESS) state = DoorOpeningState::CHECKING_USER;
            }
            break;
        case DoorOpeningState::CHECKING_USER:
            while (!userRegistered && checkCount < 3) {
                userRegistered = isUserRegistered(uuid);
                checkCount++;
            }
            if (userRegistered) {
                g_servo->setAngle(90);
                state = DoorOpeningState::DOOR_OPEN;
                doorOpeningTimer.setTimer(3);
            } else state = DoorOpeningState::WAITING_FOR_RFID;
            break;
        case DoorOpeningState::DOOR_OPEN:
            if (doorOpeningTimer.getTicks() == 0) {
                g_servo->setAngle(0);
                state = DoorOpeningState::WAITING_FOR_RFID;
                userRegistrationTimer.setTimer(1);
            }
            break;
    }
}
