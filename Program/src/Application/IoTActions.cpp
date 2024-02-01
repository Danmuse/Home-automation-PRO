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
#include "DFRobotDFPlayerMini.h"

void manualLightControl(char* message) {
    int brightness = atoi(message);

    if (brightness > 0) {
        g_leds->setBrightness(brightness*10);
    }
    else {
        if (strcmp(message, "red") == 0) {
            g_leds->setColor(S5050DJ::RED);
        }
        else if (strcmp(message, "white") == 0) {
            g_leds->setColor(S5050DJ::WHITE);
        }
        else if (strcmp(message, "green") == 0) {
            g_leds->setColor(S5050DJ::GREEN);
        }
        else if (strcmp(message, "blue") == 0) {
            g_leds->setColor(S5050DJ::BLUE);
        }
    }
}

void modeSelection(char* message) {//Automatic/manual
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
