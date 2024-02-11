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
    int ledBrightness = 0;
    int songId = 0;
    bool isSongPlaying = false;
    int volume = DFPLAYER_INITIAL_VOLUME_PERCENTAGE;

    UserRegistrationState userRegistrationState = UserRegistrationState::WAITING_FOR_PASSWORD;
    DoorOpeningState doorOpeningState = DoorOpeningState::WAITING_FOR_RFID;

    iotManager.registerAction("luz", manualLightControl);
    iotManager.registerAction("automatic", modeSelection);
    iotManager.registerAction("song", musicFlowControl);
    iotManager.registerAction("volume", musicVolumeControl);
    iotManager.registerAction("time", dateControl);

    iotManager.registerState("song", songId);
    iotManager.registerState("song", isSongPlaying, { "pause", "play" });
    iotManager.registerState("luz", ledBrightness);
    iotManager.registerState("automatic", automaticMode, { "off", "on" });
    iotManager.registerState("volume", volume);

    g_leds->turnOn();

    // Initial condition to play the opening song.
    if (UserSwitch.getPin()) {
        g_lcd->write("*------------------*", 0, 0);
        g_lcd->write("|     The Home     |", 1, 0);
        g_lcd->write("|  Automation PRO  |", 2, 0);
        g_lcd->write("*------------------*", 3, 0);
		g_leds->setMode(S5050DJ::FADE);
    	g_dfplayer->play(1);
    	// NOTE: The opening song during approximately 30 seconds
        delay(1000); // TODO: Replace this instruction by the method that corresponds.
    }

    g_lcd->clear();
	g_leds->setColor(S5050DJ::WHITE);

    while (true) {
        /// External preset acquisition
        if (automaticMode) {
			uint16_t currentBright = (uint16_t)((float)(g_adcExternal->analogRead()) / 40.9); // Range: 0 to 100 (Percentage)
//			g_lcd->write("ANALOG: ", 0, 0);
//			g_lcd->write(currentBright, 0, 8);

			if (currentBright <= ledBrightness - 5 || currentBright >= ledBrightness + 5) {
				ledBrightness = currentBright;
				uint8_t brightness = ledBrightness;
				g_leds->setBrightness(brightness);
//				g_lcd->write("BRIGHT: ", 1, 0);
//				brightness = g_leds->getBrightness();
//				g_lcd->write(brightness, 1, 8);
//				g_lcd->write("ERRCODE ", 2, 0);
//				g_lcd->write(g_ds3231->getStatus(), 2, 8);
			}
//			else {
//				g_lcd->write("BRIGHT: ", 1, 0);
//				g_lcd->write(ledBrightness, 1, 8);
//				g_lcd->write("ERRCODE ", 2, 0);
//				g_lcd->write(g_ds3231->getStatus(), 2, 8);
//			}
        }

    	/// Machine states
		userRegistrationStateMachine(userRegistrationState);
		doorOpeningStateMachine(doorOpeningState);

        //Todo: volume= g_dfplayer->getVolume();


//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

    g_leds->turnOff();
    delay(S5050DJ_COMMAND_SLOT_TICKS); // Minimum waiting time to avoid conflicts during the transmission of commands through the NEC protocol.

    return EXIT_SUCCESS;
}

#else // DEBUG_MODE

///////////////////////////////
/// RGB LEDs implementation ///
///////////////////////////////

/*

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
//	initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
//	initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
    initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
//	initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    g_leds->turnOn();

    while (1) {
        for (uint8_t index = 0; index < 15; index++) {
            if (index == 0) g_leds->setColor(S5050DJ::RED);
            else if (index == 1) g_leds->setColor(S5050DJ::FST_RED);
            else if (index == 2) g_leds->setColor(S5050DJ::SND_RED);
            else if (index == 3) g_leds->setColor(S5050DJ::TRD_RED);
            else if (index == 4) g_leds->setColor(S5050DJ::VIOLET);
            else if (index == 5) g_leds->setColor(S5050DJ::BLUE);
            else if (index == 6) g_leds->setColor(S5050DJ::FST_BLUE);
            else if (index == 7) g_leds->setColor(S5050DJ::SND_BLUE);
            else if (index == 8) g_leds->setColor(S5050DJ::TRD_BLUE);
            else if (index == 9) g_leds->setColor(S5050DJ::LIGHT_BLUE);
            else if (index == 10) g_leds->setColor(S5050DJ::GREEN);
            else if (index == 11) g_leds->setColor(S5050DJ::FST_GREEN);
            else if (index == 12) g_leds->setColor(S5050DJ::SND_GREEN);
            else if (index == 13) g_leds->setColor(S5050DJ::TRD_GREEN);
            else if (index == 14) g_leds->setColor(S5050DJ::YELLOW);
//			delay(2000 + S5050DJ_COMMAND_SLOT_TICKS);
        }

        g_leds->setMode(S5050DJ::FLASH);
        delay(3000 + S5050DJ_COMMAND_SLOT_TICKS);
        g_leds->setMode(S5050DJ::STROBE);
        delay(3000 + S5050DJ_COMMAND_SLOT_TICKS);
        g_leds->setMode(S5050DJ::FADE);
        delay(3000 + S5050DJ_COMMAND_SLOT_TICKS);
        g_leds->setMode(S5050DJ::SMOOTH);
        delay(3000 + S5050DJ_COMMAND_SLOT_TICKS);
        break;

//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

    g_leds->turnOff();
    delay(S5050DJ_COMMAND_SLOT_TICKS); // Minimum waiting time to avoid conflicts during the transmission of commands through the NEC protocol.

    return EXIT_SUCCESS;
}

*/

//////////////////////////
/// PWM implementation ///
//////////////////////////

/*
static void red_green(PWM &red, PWM &green);
static void green_blue(PWM &green, PWM &blue);
static void blue_red(PWM &blue, PWM &red);

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
//	initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
    initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
    initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    bool statusFlag = false;
    uint8_t dutyCycle = 100, counterCycles = 0;
    PWM externPWM_FST(LED_RED, dutyCycle), externPWM_SND(LED_GREEN, dutyCycle), externPWM_TRD(LED_BLUE, dutyCycle);

    while (1) {
        while (counterCycles < 4) {
            g_lcd->write("Cycle counter: ");
            g_lcd->write(counterCycles, 0, 15);
            if (!statusFlag) {
                if (counterCycles % 2) {
                    externPWM_TRD.setDuty(0);
                    externPWM_FST.setDuty(dutyCycle--);
                    externPWM_SND.setDuty(100 - dutyCycle);
                } else {
                    externPWM_SND.setDuty(0);
                    externPWM_FST.setDuty(dutyCycle--);
                    externPWM_TRD.setDuty(100 - dutyCycle);
                }
                if (dutyCycle < 1) statusFlag = !statusFlag;
                delay(10);
            } else {
                if (counterCycles % 2) {
                    externPWM_TRD.setDuty(0);
                    externPWM_FST.setDuty(dutyCycle++);
                    externPWM_SND.setDuty(100 - dutyCycle);
                } else {
                    externPWM_SND.setDuty(0);
                    externPWM_FST.setDuty(dutyCycle++);
                    externPWM_TRD.setDuty(100 - dutyCycle);
                }
                if (dutyCycle > 100 - 1) {
                    statusFlag = !statusFlag;
                    counterCycles++;
                }
                delay(10);
            }
        }
        externPWM_FST.setDuty(0);
        externPWM_SND.setDuty(0);
        externPWM_TRD.setDuty(0);
        red_green(externPWM_FST, externPWM_SND);
        externPWM_FST.setDuty(0);
        externPWM_SND.setDuty(0);
        green_blue(externPWM_SND, externPWM_TRD);
        externPWM_SND.setDuty(0);
        externPWM_TRD.setDuty(0);
        blue_red(externPWM_TRD, externPWM_FST);
        externPWM_TRD.setDuty(0);
        externPWM_FST.setDuty(0);
        g_lcd->clear();
        counterCycles = 0;
        g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }
}

static void red_green(PWM &red, PWM &green) {
    g_lcd->clear();
    g_lcd->write("Color: ");
    g_lcd->write("RED", 0, 7);
    red.setDuty(100);
    delay(2500);
    g_lcd->clear();
    g_lcd->write("Color: ");
    g_lcd->write("YELLOW", 0, 7);
    red.setDuty(46);
    green.setDuty(50);
    delay(2500);
}

static void green_blue(PWM &green, PWM &blue) {
    g_lcd->clear();
    g_lcd->write("Color: ");
    g_lcd->write("GREEN", 0, 7);
    green.setDuty(100);
    delay(2500);
    g_lcd->clear();
    g_lcd->write("Color: ");
    g_lcd->write("SKY BLUE", 0, 7);
    green.setDuty(36);
    blue.setDuty(50);
    delay(2500);
}

static void blue_red(PWM &blue, PWM &red) {
    g_lcd->clear();
    g_lcd->write("Color: ");
    g_lcd->write("BLUE", 0, 7);
    blue.setDuty(100);
    delay(2500);
    g_lcd->clear();
    g_lcd->write("Color: ");
    g_lcd->write("VIOLET", 0, 7);
    blue.setDuty(50);
    red.setDuty(45);
    delay(2500);
}
*/

/////////////////////////////
/// M24C16 implementation ///
/////////////////////////////

/*
int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
    initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
//	initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
    initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
    initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    uint8_t value = 0, currentValue = 0, position = 0;

    while (1) {
        if (!(g_eeprom->write(value, position))) LED_RED.ClearPin();
        else LED_RED.SetPin();
        if (!(g_eeprom->read(&currentValue, M24C16::UINT8, position))) LED_BLUE.ClearPin();
        else LED_BLUE.SetPin();
        g_display->set(value, 0);
        g_display->set(currentValue, 1);
        value++; position++; // These are may overflow...
        g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
        delay(100);
    }
}
*/

/////////////////////////////
/// DS3231 implementation ///
/////////////////////////////

/*

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
    initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
//	initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
    initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
//	initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
    initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    LED_RED.clearPin();
    LED_GREEN.clearPin();
    LED_BLUE.clearPin();

    if (g_ds3231->set(0, 1, 1, 31, 12, 2023)) {
        LED_GREEN.setPin();
        LED_RED.setPin();
    } else LED_RED.clearPin();

    RTC_st rtc;

    while (1) {
        rtc = g_ds3231->get();
        if (g_ds3231->getStatus()) {
            LED_GREEN.setPin();
            LED_BLUE.setPin();
        } else LED_BLUE.clearPin();
        g_display->set(rtc.TIME.MIN, 0);
        g_display->set(rtc.TIME.SEC, 1);
        g_usb->transmit(g_ds3231->printTimestamp());
        g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
        delay(1000);
    }
}

*/

////////////////////////////////
/// ADC & DAC implementation ///
////////////////////////////////

/*

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
//	initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
    initLCD1602();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initLCD2004();	// Initializes the g_lcd         ~ Define the CN15_PINS or LCD_DEBUG_PINS macros in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14} or {P0.13 - P0.15 - P0.26 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS or KEYBOARD_DEBUG_PINS macros in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26} or {P0.28 - P0.27 - P0.08}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initServo();	// Initializes the g_servo       ~ Define the SG90S_SERVO_PIN macro in ProgramConfig.h {P0.23}
    initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
    initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
    initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
    initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    DAC dacLocal(DAC::DAC_0);
    uint16_t val, aux;

    while (1) {
        /// Internal preset acquisition
        if (g_preset != nullptr && g_dacExternal != nullptr) {
            val = g_preset->analogRead();
            g_lcd->write("A0 ", 0, 0);
            g_lcd->write(val, 0, 3);

            aux = val / 4;
            g_dacExternal->analogWrite(aux); // 12 bits into 10 bits
            g_lcd->write("D0 ", 1, 0);
            g_lcd->write(aux, 1, 3);
        }

        /// External preset acquisition
        if (g_adcExternal != nullptr) {
            val = g_adcExternal->analogRead();
            g_lcd->write("A1 ", 0, 8);
            g_lcd->write(val, 0, 11);

            aux = val / 4;
            dacLocal.analogWrite(aux); // 12 bits into 10 bits
            g_lcd->write("D1 ", 1, 8);
            g_lcd->write(aux, 1, 11);
        }

        delay(100);
        g_lcd->write("   ", 0, 4);
        g_lcd->write("   ", 1, 4);
        g_lcd->write("   ", 0, 12);
        g_lcd->write("   ", 1, 12);
//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }
}
*/

/////////////////////
/// General Tests ///
/////////////////////

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
	initDFPlayer(); // Initializes the g_dfplayer    ~ Define the CN13_PINS macro in ProgramConfig.h {P0.17 - P0.16 - P0.00}
	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
	initRFID();		// Initializes the g_rfid        ~ Define the SPI1_DEBUG_PINS macro in ProgramConfig.h {P0.19 - P0.22 - P0.21 - P0.20 - P0.18}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    UserRegistrationState userRegistrationState = UserRegistrationState::WAITING_FOR_PASSWORD;
    DoorOpeningState doorOpeningState = DoorOpeningState::WAITING_FOR_RFID;

	g_lcd->write("*------------------*", 0, 0);
	g_lcd->write("|     The Home     |", 1, 0);
	g_lcd->write("|  Automation PRO  |", 2, 0);
	g_lcd->write("*------------------*", 3, 0);
	delay(1000);

//	char bufferUSB[4];
//	bufferUSB[1] = '\n';
//	bufferUSB[2] = '\n';
//	bufferUSB[3] = '\0';
//
//	LED_RED.clearPin();
//	LED_GREEN.clearPin();
//	LED_BLUE.clearPin();

//	if (g_ds3231->set(0, 38, 20, 4, 2, 2024)) {
//		LED_GREEN.setPin();
//		LED_RED.setPin();
//	} else LED_RED.clearPin();

	g_dfplayer->volume(50); // Set volume value. From 0% to 100%
	g_dfplayer->play(1); // Play the first mp3

	uint8_t volume = 50;
	uint8_t keyPressed = NO_KEY;

    while (1) {
    	/*/// DS3231 RTC

    	g_ds3231->get();
    	g_lcd->write(g_ds3231->printTimestamp(), 0, 0);

    	*/

       	/*/// SevenSegmentDisplay

		static uint8_t value = 0;
		static bool toggleDisplay = false;

		if (!(value)) {
			g_display->set(value, 0);
			g_display->set(value, 1);
		}

		if (!(value % 10)) toggleDisplay = !toggleDisplay;
		if (toggleDisplay) {
			g_display->mode(Digit::NONE, 0);
    		g_display->mode(Digit::BLINK, 1);
    		g_display->set(value, 0);
		} else {
			g_display->mode(Digit::NONE, 1);
			g_display->mode(Digit::BLINK, 0);
			g_display->set(value, 1);
		}

		value++;
		if (value == 255) break;

    	delay(500);

		*/

    	/*/// FM24C16 EEPROM

		static uint32_t value = 0;
		static uint32_t currentValue = 0;
		static uint32_t position = 0;

		static char bufferFST[10];
		static char bufferSND[9];

		bufferFST[0] = 'W';
		bufferFST[1] = 'r';
		bufferFST[2] = 'i';
		bufferFST[3] = 't';
		bufferFST[4] = 'e';
		bufferFST[5] = ' ';
		bufferFST[7] = ' ';
		bufferFST[8] = ' ';
		bufferFST[9] = '\0';

		bufferSND[0] = 'R';
		bufferSND[1] = 'e';
		bufferSND[2] = 'a';
		bufferSND[3] = 'd';
		bufferSND[4] = ' ';
		bufferSND[6] = ' ';
		bufferSND[7] = ' ';
		bufferSND[8] = '\0';

        !(g_eeprom->write(value, position)) ? LED_RED.clearPin() : LED_RED.setPin();
        !(g_eeprom->read(&currentValue, M24C16::UINT8, position)) ? LED_BLUE.clearPin() : LED_BLUE.setPin();
        if (value < 10) bufferFST[6] = value + '0';
        else if (value >= 10 && value < 100) {
        	bufferFST[6] = (value / 10) + '0';
        	bufferFST[7] = (value % 10) + '0';
        } else if (value >= 100 && value < 1000) {
        	bufferFST[6] = (value / 100) + '0';
        	bufferFST[7] = ((value / 10) % 10) + '0';
        	bufferFST[8] = (value % 10) + '0';
        } else {
        	bufferFST[6] = ((value / 100) % 10) + '0';
			bufferFST[7] = ((value / 10) % 10) + '0';
			bufferFST[8] = (value % 10) + '0';
        }

        if (currentValue < 10) bufferSND[5] = currentValue + '0';
        else if (currentValue >= 10 && currentValue < 100) {
        	bufferSND[5] = (currentValue / 10) + '0';
        	bufferSND[6] = (currentValue % 10) + '0';
        } else if(currentValue >= 100 && currentValue < 1000) {
        	bufferSND[5] = (currentValue / 100) + '0';
        	bufferSND[6] = ((currentValue / 10) % 10) + '0';
        	bufferSND[7] = (currentValue % 10) + '0';
        } else {
        	bufferSND[5] = ((currentValue / 100) % 10) + '0';
        	bufferSND[6] = ((currentValue / 10) % 10) + '0';
        	bufferSND[7] = (currentValue % 10) + '0';
        }

        g_lcd->write(bufferFST, 0, 0);
        g_lcd->write(bufferSND, 1, 0);
        value++; position++; // These are may overflow...
        if (value == 65535) {
        	value = 0;
			bufferFST[7] = ' ';
			bufferFST[8] = ' ';
		}
        if (position == 65535) {
        	position = 0;
			bufferSND[6] = ' ';
			bufferSND[7] = ' ';
		}
        delay(100);

    	*/

    	/*/// MG90S SERVO

    	g_servo->setAngle(0);
    	delay(500);
		g_servo->setAngle(90);
    	delay(500);

		*/

//    	bufferUSB[0] = g_ds3231->getStatus() + '0';
//    	g_usb->transmit("ERRCODE ");
//    	g_usb->transmit(bufferUSB);
//
//		g_lcd->write("ERRCODE ", 1, 0);
//		g_lcd->write(bufferUSB[0] - '0', 1, 8);
//
//		delay(1000);

       	///*/// DFPlayer

    	keyPressed = g_keyboard->get();
    	if (keyPressed == 0) {
    		volume += 10;
    		g_dfplayer->volume(volume > 100 ? 100 : volume);
    		if (volume == 100) volume = 90;
    	} else if (keyPressed == 1) {
    		volume -= 10;
    		g_dfplayer->volume(volume);
    		if (volume == 0) volume = 10;
    	}

    	if (UserSwitch.getPin()) {
    		LED_GREEN.setPin();
    		g_dfplayer->play(0);
    	} else LED_GREEN.clearPin();

    	g_lcd->clear();

		//*/

//    	userRegistrationStateMachine(userRegistrationState);
//    	doorOpeningStateMachine(doorOpeningState);

//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

    return EXIT_SUCCESS;
}

#endif // DEBUG_MODE macro definition in precompile stage
