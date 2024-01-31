/*/*!
 * @file IoTActions.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 29/91/2024 02:36:05
 */

#include <cstdlib>
#include <cstring>
#include "IoTActions.h"

static uint8_t brightnessLevel = 0;

void manualLightControl(char* message) {
    int brightness = atoi(message);

    if (brightness > 0) {
        if (brightnessLevel < brightness) {
            for (uint8_t i = brightnessLevel; i < brightness; i++) {
                //TODO: Increase brightness
            }
        }
        else {
            for (uint8_t i = brightnessLevel; i > brightness; i--) {
                //TODO: Decrase brightness
            }
        }
        brightnessLevel = brightness;
    }
    else {
        if (strcmp(message, "red") == 0) {
            //TODO: Lights on red
        }
        else if (strcmp(message, "white") == 0) {
            //TODO: Lights on white
        }
        else if (strcmp(message, "green") == 0) {
            //TODO: Lights on green
        }
        else if (strcmp(message, "blue") == 0) {
            //TODO: Lights on blue
        }
    }
}

void modeSelection(char* message) {
    if (strcmp(message, "on") == 0) {
        //TODO: ENABLE Light Automatic mode
    }
    else {
        //TODO: ENABLE Light Manual mode
    }
}

void musicFlowControl(char* message) {

    int songId = atoi(message);

    if (songId != 0) {
        //TODO: Select Music

    }
    else if (strcmp(message, "pause") == 0) {
        //TODO: Pause/stop

    }
    else if (strcmp(message, "play") == 0) {
        //TODO: Play music
    }
}

void musicVolumeControl(char* message) {
    int volume = atoi(message);

    //TODO: Set volume
}
