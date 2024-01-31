/*!
 * @file IoTActions.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 29/91/2024 02:36:05
 * @brief IoT Actions Class API (Application Programming Interface).
 */

#ifndef IOT_ACTIONS_H_
#define IOT_ACTIONS_H_

#include "utils.h"

void manualLightControl(char* message);

void modeSelection(char* message);

void musicFlowControl(char* message);

void musicVolumeControl(char* message);

#endif // IOT_ACTIONS_H_
