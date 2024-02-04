#ifndef PROYECTOINFO2_UTILITIES_H
#define PROYECTOINFO2_UTILITIES_H

#include "ProgramConfig.h"
#include "Timers.h"
#include <cstring>


extern Timers g_timers_list;

void delay(uint32_t milliseconds);

char* strreverse(char* cstring);
char* byteToHEX(char* cstring, uint8_t value);

#endif //PROYECTOINFO2_UTILITIES_H
