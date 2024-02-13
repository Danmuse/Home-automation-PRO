/*/*!
 * @file utilities.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 03/02/2024 22:15:43
 */

#include "utilities.h"

Timers g_timers_list;

void delay(uint32_t milliseconds) {
    static Timer delayTIMER;
    static bool flagPush = false;
    if (!flagPush) {
        flagPush = !flagPush;
        g_timers_list << delayTIMER;
    }
    delayTIMER.timerStart(milliseconds, nullptr, Timer::MILLI);
    while (delayTIMER.getTicks());
}

/*!
 * @brief Rverses a string.
 * @param[in,out] cstring String to be reversed.
 * @return Reversed string.
 */
char* strreverse(char* cstring) {
    size_t lenght = strlen(cstring);
    uint8_t left, right;
    for (left = 0, right = lenght - 1; left < (lenght / 2); left++, right--) {
        char aux = cstring[left];
        cstring[left] = cstring[right];
        cstring[right] = aux;
    }
    return cstring;
}

/*!
 * @brief Converts a uint8_t to a hexadecimal string.
 * @param[out] cstring String where the value will be stored.
 * @param value
 * @return
 */
void byteToHEX(char* cstring, uint8_t value) {
    size_t index = 0;
    for (index = 0; value > 0; index++) {
        if (value % 16 < 10) cstring[index] = (value % 16) + '0';
        else cstring[index] = (value % 16) - 10 + 'A';
        value /= 16;
    }
    cstring[index] = '\0';
    if (strlen(cstring) == 0) strcpy(cstring, "0");
    strreverse(cstring);
}

/*!
 * @brief Converts a uint16_t to an array of uint8_t.
 * @param value uint16_t value to be converted.
 * @param[out] array Pointer to the array where the value will be stored.
 */
void uint16ToArray(uint16_t value, uint8_t *array) {
    *array = (uint8_t)(value >> 8);
    *(array + 1) = (uint8_t)(value);
}

/*!
 * @brief Converts an array of uint8_t to a uint16_t.
 * @param array Array to be converted
 * @return uint16_t value.
 */
uint16_t arrayToUint16(uint8_t *array) {
    uint16_t value = *array;
    value <<= 8;
    value += *(array + 1);
    return value;
}

/*!
 * @brief Converts an epoch timestamp to a date.
 * @param epochTimestamp Timestamp to be converted.
 * @return Time structure.
 */
time_st epochToDate(uint32_t epochTimestamp) {
    const int SECONDS_IN_DAY = 86400, SECONDS_IN_HOUR = 3600, SECONDS_IN_MINUTE = 60;
    uint8_t hour, minute, second, day, month;
    uint16_t year;

    // Days since epoch (Jan 1, 1970)
    uint32_t daysSinceEpoch = epochTimestamp / SECONDS_IN_DAY;

    // Compute seconds remaining after whole days
    uint32_t secondsRemaining = epochTimestamp % SECONDS_IN_DAY;

    // Compute year
    uint8_t numYears = 0;
    uint32_t totalDays = 0;
    uint16_t yearDays = 0, yearStart = 1970;

    while (totalDays <= daysSinceEpoch) {
        yearDays = (yearStart % 4 == 0 && (yearStart % 100 != 0 || yearStart % 400 == 0)) ? 366 : 365;
        totalDays += yearDays;
        yearStart++;
        numYears++;
    }

    year = yearStart - 1; // Last year to meet condition

    // Compute month and day
    int16_t remainingDays = daysSinceEpoch - (totalDays - yearDays);
    static uint8_t daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    for (month = 0; month < 12; month++) {
    	daysInMonth[month] = (month == 1 && ((year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))) ? 29 : 28;
    	remainingDays -= daysInMonth[month];
		if (remainingDays < 0) break;
    }
    day = remainingDays + daysInMonth[month] + 1; // +1 because days start from 1
    month++;

    // Compute hours, minutes and seconds
    hour = secondsRemaining / SECONDS_IN_HOUR;
    secondsRemaining %= SECONDS_IN_HOUR;
    minute = secondsRemaining / SECONDS_IN_MINUTE;
    second = secondsRemaining % SECONDS_IN_MINUTE;

    return { year, month, day, hour, minute, second };
}
