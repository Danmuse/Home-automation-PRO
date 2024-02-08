/*!
 * @file utilities.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 03/02/2024 22:15:43
 * @brief Utilities API (Application Programming Interface).
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <cstring>
#include "ProgramConfig.h"
#include "Timers.h"

typedef struct {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
} time_st;

extern Timers g_timers_list;

void delay(uint32_t milliseconds);

char* strreverse(char* cstring);

char* byteToHEX(char* cstring, uint8_t value);

time_st epochToDate(uint32_t epochTimestamp);


#endif // UTILITIES_H_
