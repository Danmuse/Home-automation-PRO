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

char* byteToHEX(char* cstring, uint8_t value) {
    size_t index = 0;
    for (index = 0; value > 0; index++) {
        if (value % 16 < 10) cstring[index] = (value % 16) + '0';
        else cstring[index] = (value % 16) - 10 + 'A';
        value /= 16;
    }
    cstring[index] = '\0';
    if (strlen(cstring) == 0) strcpy(cstring, "0");
    return strreverse(cstring);
}
