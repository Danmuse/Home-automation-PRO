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

extern Timers g_timers_list;

void delay(uint32_t milliseconds);

char* strreverse(char* cstring);
char* byteToHEX(char* cstring, uint8_t value);

#endif // UTILITIES_H_
