/*!
 * @file Program.cpp
 * @authors
 * 	Agustin Ordoñez ~ <a href = "mailto: aordonez@frba.utn.edu.ar">aordonez@@frba.utn.edu.ar</a>
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 25/11/2023 17:43:24
 */

#include <cr_section_macros.h>
#include "stateMachines.h"

#if !DEBUG_MODE

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
//	initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
//	initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
    initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
    initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
    initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
	initDFPlayer();	// Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
    initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
    initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
    initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    QTConnection connection(*g_usb);
    IoTManager iotManager(&connection);
    uint8_t ledBrightness = 0;
    uint8_t songId = 0;
    bool isSongPlaying = false;
    uint8_t volume = DFPLAYER_INITIAL_VOLUME_PERCENTAGE;

    UserRegistrationState userRegistrationState = UserRegistrationState::WAITING_FOR_PASSWORD;
    DoorOpeningState doorOpeningState = DoorOpeningState::WAITING_FOR_RFID;

//    iotManager.registerAction("luz", manualLightControl);
//    iotManager.registerAction("automatic", modeSelection);
//    iotManager.registerAction("song", musicFlowControl);
//    iotManager.registerAction("volume", musicVolumeControl);
//    iotManager.registerAction("time", dateControl);
//
//    iotManager.registerState("song", songId);
//    iotManager.registerState("song", isSongPlaying, { "pause", "play" });
//    iotManager.registerState("luz", ledBrightness);
//    iotManager.registerState("automatic", automaticMode, { "off", "on" });
//    iotManager.registerState("volume", volume);

    g_leds->turnOn();

    // Initial condition to play the opening song.
    if (UserSwitch.getPin()) {
        g_lcd->write("*------------------*", 0, 0);
        g_lcd->write("|     The Home     |", 1, 0);
        g_lcd->write("|  Automation PRO  |", 2, 0);
        g_lcd->write("*------------------*", 3, 0);
		g_leds->setMode(S5050DJ::FADE);
    	g_dfplayer->play(1);
        delay(DFPLAYER_TIMEOUT_TICKS + 800); // Time lapse to engage the opening song.
        while (g_dfplayer->getStatus() != DFPLAYER_READY) { }
        g_lcd->clear();
    }

	g_leds->setColor(S5050DJ::WHITE);

    /// Code instructions to reset the registered users
//	#define CN6_PINS
//	!(g_eeprom->write((uint8_t) 0, 0)) ? LED_RED.clearPin() : LED_RED.setPin();

    while (true) {
        /// External LDR sensor acquisition
        if (automaticMode) {
			uint16_t currentBright = (uint16_t)((float)(g_adcExternal->analogRead()) / 40.9); // Range: 0 to 100 (Percentage)
			if (currentBright <= ledBrightness - 5 || currentBright >= ledBrightness + 5) {
				ledBrightness = currentBright;
				uint8_t brightness = ledBrightness;
				g_leds->setBrightness(brightness);
			}
        }

        /// Upgrade RTC timestamp legend
        g_ds3231->get();
        if (!(g_ds3231->getStatus())) g_lcd->write(g_ds3231->printTimestamp(false), 3, 0);

    	/// Machine states
		userRegistrationStateMachine(userRegistrationState);
		doorOpeningStateMachine(doorOpeningState);

		/// Upgrade the DFPLayer's volume
		volume = g_dfplayer->getVolume();

//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

    return EXIT_SUCCESS;
}

#else // DEBUG_MODE

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
//	initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
//	initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//  initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
//	initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    while (1) {

    	////////////////////////////////////////////
    	// Add the corresponding fragment of code //
    	////////////////////////////////////////////

//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

    return EXIT_SUCCESS;
}

#endif // DEBUG_MODE macro definition in precompile stage
