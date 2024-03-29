/*!
 * @file IoTActions.h
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 29/91/2024 02:36:05
 * @brief IoT Actions API (Application Programming Interface).
 */

#ifndef IOT_ACTIONS_H_
#define IOT_ACTIONS_H_

#include "utils.h"
#include "S5050DJ.h"
#include "DFPlayerMini.h"
#include "DS3231.h"

extern uint8_t setVolume;

extern bool automaticMode;
typedef enum SongStatus_t {
        SONG_IDLE,
        SONG_PAUSE,
        SONG_PLAY,
        SONG_RESUME,
		SONG_VOLUME
} SongStatus_t;

extern SongStatus_t songStatus;

void manualLightControl(char* message);

void modeSelection(char* message);

void musicFlowControl(char* message);

void musicVolumeControl(char* message);

void dateControl(char* message);

#endif // IOT_ACTIONS_H_
