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

static void clearHandler(void);
static bool isUserRegistered(const MFRC522::UID_st& uid);

static bool registerNewUser(const MFRC522::UID_st& uid);
static bool manageUserLocation(const MFRC522::UID_st& uid);

uint8_t registeredUsers = 0;
uint8_t loggedInUsers = 0;

static bool isUserRegistered(const MFRC522::UID_st& uid) {
    uint8_t userCount = registeredUsers;
    EEPROM_result_t result;
    bool isRegistered = false;

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
    uint8_t userCount = registeredUsers;
    EEPROM_result_t result;

    if(userCount >= MAX_USERS) return false;

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

    result = g_eeprom->write((uint8_t)0, USER_ENTERED_POSITION);
    if (result != EEPROM_OK) return false;
    result = g_eeprom->write(++userCount, USER_COUNT_POSITION);
    if (result != EEPROM_OK) return false;
    registeredUsers = userCount;
    return true;
}

bool manageUserLocation(const MFRC522::UID_st& uid) {
    uint8_t userCount = registeredUsers;
    EEPROM_result_t result;

	#if RFID_USER_UID_SIZE == 4 && M24C16_USING_MODIFIERS == 1
    uint32_t userId = 0;
    for (size_t index = 0; index < RFID_USER_UID_SIZE; index++) userId |= uid.uidByte[index] << (index * 8);
    for (size_t index = 0; index < userCount; index++) {
        uint32_t registeredUID = 0;
        result = g_eeprom->read(&registeredUID, M24C16::UINT32, index * (RFID_USER_UID_SIZE + USER_ENTERED_SIZE) + USERS_INIT_POSITION);
        if (result != EEPROM_OK) return false;
        if (registeredUID == userId) {
            uint8_t location = 0;
            result = g_eeprom->read(&location, M24C16::UINT8, index * RFID_USER_UID_SIZE - USER_ENTERED_POSITION);

            if (!location){
                g_dfplayer->play(8);
                loggedInUsers++;
            }
            else {
                g_dfplayer->play(9);
                loggedInUsers--;
            }

            result = g_eeprom->write((uint8_t)!location, index * RFID_USER_UID_SIZE - USER_ENTERED_POSITION);
            if (result != EEPROM_OK) return false;
        }
    }
	#else
    for (size_t index = 0; index < userCount; index++) {
        uint8_t registeredUID[RFID_USER_UID_SIZE];
        for (size_t j_index = 0; j_index < RFID_USER_UID_SIZE; j_index++) {
            result = g_eeprom->read(&registeredUID[j_index], M24C16::UINT8, index * (RFID_USER_UID_SIZE + USER_ENTERED_SIZE) + USERS_INIT_POSITION + j_index);
            if (result != EEPROM_OK) return false;
        }
        if (memcmp(registeredUID, uid.uidByte, RFID_USER_UID_SIZE) == 0){
            uint8_t location = 0;
            result = g_eeprom->read(&location, M24C16::UINT8, index * (RFID_USER_UID_SIZE + USER_ENTERED_SIZE) + USER_ENTERED_POSITION);
            if (result != EEPROM_OK) return false;

            if (!location){
                g_dfplayer->play(8);
                loggedInUsers++;
            }
            else {
                g_dfplayer->play(9);
                loggedInUsers--;
            }
            result = g_eeprom->write((uint8_t)!location, index * (RFID_USER_UID_SIZE + USER_ENTERED_SIZE) + USER_ENTERED_POSITION);
            if (result != EEPROM_OK) return false;
        }
    }
	#endif
    return true;
}

void clearHandler(void) {
    g_lcd->clear();
}

KeyboardPassword keyBoardPassword("1222112", g_keyboard);
Timer userRegistrationTimer(nullptr, Timer::SEC);
Timer clearCommandTimer(clearHandler, Timer::SEC);

void userRegistrationStateMachine(UserRegistrationState& state) {
    clearCommandTimer.timerEvent();
    switch (state) {
        case UserRegistrationState::WAITING_FOR_PASSWORD:
        	if (!(clearCommandTimer.getTicks())) {
				g_lcd->write("Registered users: ", 0, 0);
				g_lcd->write(registeredUsers, 0, 18);
				if (registeredUsers< 10) g_lcd->write(" ", 0, 19);
        		g_lcd->write("Logged in users:  ", 1, 0);
        		g_lcd->write(loggedInUsers, 1, 18);
        		if (loggedInUsers< 10) g_lcd->write(" ", 1, 19);
        	}
            if (keyBoardPassword.checkPassword()) {
            	g_lcd->clear();
                state = UserRegistrationState::WAITING_FOR_USER;
                userRegistrationTimer.setTimer(20);
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
                    if(registerAttempts==3) g_lcd->write("Error while registering", 1, 0);
                    else g_lcd->write("Success registering!", 1, 0);
                }
                else {
					g_lcd->write("The user has already", 0, 0);
                    g_lcd->write("been registered!", 1, 2);
                }

                state = UserRegistrationState::WAITING_FOR_PASSWORD;
                clearCommandTimer.setTimer(6);
                userRegistrationTimer.setTimer(6);
            } else g_lcd->write("Present the RFID", 1, 2);
            if (userRegistrationTimer.getTicks() == 0) {
                state = UserRegistrationState::WAITING_FOR_PASSWORD;
                g_lcd->write("Timeout, try again", 1, 1);
                clearCommandTimer.setTimer(5);
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
            if (!(userRegistrationTimer.getTicks())) {
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

                checkCount = 0;
                while (manageUserLocation(uuid) && checkCount < 3) checkCount++;
            } else state = DoorOpeningState::WAITING_FOR_RFID;
            break;
        case DoorOpeningState::DOOR_OPEN:
            if (!(doorOpeningTimer.getTicks())) {
                g_servo->setAngle(0);
                state = DoorOpeningState::WAITING_FOR_RFID;
                userRegistrationTimer.setTimer(1);
            }
            break;
    }
}

void initUsers() {
    bool actionConfirmed = false;
    uint8_t retryCount = 0;
    EEPROM_result_t result;


    while (!actionConfirmed && retryCount < 3) {
        result = g_eeprom->read(&registeredUsers, M24C16::UINT8, USER_COUNT_POSITION);
        retryCount++;
        if(result==EEPROM_OK) actionConfirmed = true;
    }
    actionConfirmed = false;
    retryCount = 0;

    for(uint8_t i = 0; i < registeredUsers; i++) {
        uint8_t location = 0;

        while (!actionConfirmed && retryCount < 3) {
            result = g_eeprom->read(&location, M24C16::UINT8, i * (RFID_USER_UID_SIZE + USER_ENTERED_SIZE) + USER_ENTERED_POSITION);
            retryCount++;
            if (result != EEPROM_OK) actionConfirmed = true;
        }
        if (location) loggedInUsers++;
    }
}
