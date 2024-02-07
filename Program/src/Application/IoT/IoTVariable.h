/*!
 * @file IoTVariable.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/01/2024 12:23:56
 * @brief IoT Variable Structure API (Application Programming Interface).
 */

#ifndef IOT_VARIABLE_H_
#define IOT_VARIABLE_H_

#include "utils.h"

/*
typedef struct {
    const char *name;
    // TODO: MAYBE NOT INTEGRER
    int &variable;
    int uploadPeriod;
    int uploadCounter;
} IoTVariable_st;
*/
typedef struct {
        const char* name;
        void* variable; //NOTE: only int(or derivate) and bool are supported
        std::vector<const char*> stringRepresent;
        int uploadPeriod;
        int uploadCounter;
} IoTVariable_st;
#endif // IOT_VARIABLE_H_
