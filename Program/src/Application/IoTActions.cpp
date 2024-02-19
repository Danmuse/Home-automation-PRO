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

bool automaticMode = true;
//bool play = false;
//bool resume = false;
//bool pause = false;

uint8_t songStatus = 0;


void manualLightControl(char* message) {
    int brightness = atoi(message);

    if (brightness > 0) g_leds->setBrightness(brightness);
    else if(message[0]=='0') g_leds->setBrightness(0);
    else {
        if (strcmp(message, "red") == 0) g_leds->setColor(S5050DJ::RED);
        else if (strcmp(message, "white") == 0) g_leds->setColor(S5050DJ::WHITE);
        else if (strcmp(message, "green") == 0) g_leds->setColor(S5050DJ::GREEN);
        else if (strcmp(message, "blue") == 0) g_leds->setColor(S5050DJ::BLUE);
    }
}

void modeSelection(char* message) { // Automatic/manual control
    strcmp(message, "on") == 0 ? automaticMode = true : automaticMode = false;
}

void musicFlowControl(char* message) {
	static uint8_t lastId = 1;
	int songId = atoi(message);

    if (songId != 0){
    	lastId = g_dfplayer->getBackupSong();
    	g_dfplayer->prepareSong(songId);
    }
    else if (strcmp(message, "pause") == 0)
    	songStatus = 1;
//    	g_dfplayer->pause();
    else if (strcmp(message, "play") == 0) {
        DFPlayer_result_t status = g_dfplayer->getStatus();
        if (status == DFPLAYER_READY || (status == DFPLAYER_PAUSE && g_dfplayer->getBackupSong() != lastId)) {
        	songStatus = 2;
        	lastId = g_dfplayer->getBackupSong();
        	//g_dfplayer->play();
//        	play = true;
        }
        else{
        	songStatus = 3;
        }
//        	resume = true;
        //g_dfplayer->resume();
    }
}

void musicVolumeControl(char* message) {
    int volume = atoi(message);
    g_dfplayer->volume(volume);
}

void dateControl(char* message) {
    uint32_t timestamp = atoi(message);
    timestamp -= 10800; // The result is expressed as GMT-3
    time_st date = epochToDate(timestamp);

    g_ds3231->set(date.second, date.minute, date.hour, date.day, date.month, date.year);
}
