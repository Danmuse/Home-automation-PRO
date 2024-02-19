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
SongStatus_t songStatus = SONG_IDLE;
uint8_t setVolume = 0;

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

    uint8_t backupSong = g_dfplayer->getBackupSong();
    if (songId != 0) g_dfplayer->prepareSong(songId);
    else if (strcmp(message, "pause") == 0) songStatus = SONG_PAUSE;
    else if (strcmp(message, "play") == 0) {
        DFPlayer_result_t status = g_dfplayer->getStatus();
        if (status == DFPLAYER_READY || (status == DFPLAYER_PAUSE && backupSong != lastId)) {
        	songStatus = SONG_PLAY;
        	lastId = backupSong;
        } else songStatus = SONG_RESUME;
    }
}

void musicVolumeControl(char* message) {
    setVolume = atoi(message);
	songStatus = SONG_VOLUME;
}

void dateControl(char* message) {
    uint32_t timestamp = atoi(message);
    timestamp -= 10800; // The result is expressed as GMT-3
    time_st date = epochToDate(timestamp);

    g_ds3231->set(date.second, date.minute, date.hour, date.day, date.month, date.year);
}
