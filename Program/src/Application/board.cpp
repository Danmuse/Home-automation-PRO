/*!
 * @file board.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "board.h"

void initDevice(void) {
#if FREQ_CLOCK_MCU != 12000000UL
    initPhaseLockedLoop();
#endif // FREQ_CLOCK_MCU != 12000000UL
    // Setting the systick to less than 200 milliseconds will produce serious instabilities in the program.
    // The most appropriate values are from 500 milliseconds to 1000 milliseconds.
    if ((FREQ_SYSTICK < 200 && FREQ_CLOCK_MCU == 12000000UL) ||
        (FREQ_SYSTICK < 100 && FREQ_CLOCK_MCU == 24000000UL))
        while (1);
    initSysTick(FREQ_SYSTICK);
}

bool isUserRegistered(MFRC522::UID_st& uid) {
    uint8_t userCount = 0;
    EEPROM_result_t result = g_eeprom->read(&userCount, M24C16::UINT8, 0);
    bool isRegistered = false;

    if (result != EEPROM_OK) {
        return false;
    }
    if (RFID_USER_UID_SIZE == 4) {
        uint32_t userId;

        for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) {
            userId |= uid.uidByte[i] << (i * 8);
        }

        for (uint8_t i = 0; i < userCount; i++) {
            uint32_t registeredUID = 0;
            result = g_eeprom->read(&registeredUID, M24C16::UINT32, i * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) {
                return false;
            }
            if (registeredUID == userId) {
                isRegistered = true;
            }
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
            if (memcmp(registeredUID, uid.uidByte, RFID_USER_UID_SIZE) == 0) {
                isRegistered = true;
            }
        }
    }
    return isRegistered;
}

bool registerNewUser(MFRC522::UID_st& uid) {
    uint8_t userCount = 0;


    EEPROM_result_t result = g_eeprom->read(&userCount, M24C16::UINT8, 0);

    if (result != EEPROM_OK) {
        return false;
    }

    if (RFID_USER_UID_SIZE == 4) {
        uint32_t userId;

        for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) {
            userId |= uid.uidByte[i] << (i * 8);
        }

        result = g_eeprom->write(userId, userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
        if (result != EEPROM_OK) {
            return false;
        }
    }
    else {
        for (uint8_t i = 0; i < RFID_USER_UID_SIZE; i++) {
            result = g_eeprom->write(uid.uidByte[i], userCount * RFID_USER_UID_SIZE + USERS_INIT_POSITION);
            if (result != EEPROM_OK) {
                return false;
            }
        }

    }
    return true;
}
