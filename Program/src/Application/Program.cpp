/*!
 * @file Program.cpp
 * @par Author and Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 25/11/2023 17:43:24
 */

#include <cr_section_macros.h>
#include "board.h"
#include "IoTActions.h"

#if !DEBUG_MODE

int main(void) {
    initDevice();	// Initializes the System Tick Timer and Phase Locked Loop modifying the FREQ_CLOCK_MCU macro in LPC845.h
//	initDisplay();	// Initializes the g_display     ~ Define the CN12_PINS macro in ProgramConfig.h {P0.23 - P0.22 - P0.21 - P0.20 - P0.18 - P0.19}
    initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
    initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
    initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
    initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
    initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

//  int exampleVariable = 0;
//	uint8_t keyPressed = 0;

//  QTConnection connection(*g_usb);

//  IoTManager iotManager(&connection);

//  iotManager.addVariableToUpload("example1", exampleVariable, 1000);
//  iotManager.addVariableToUpload("keyPressed", keyPressed, 1000);

    QTConnection connection(*g_usb);

    IoTManager iotManager(&connection);

    iotManager.registerAction("luz", manualLightControl);
    iotManager.registerAction("automatico", modeSelection);
    iotManager.registerAction("song", musicFlowControl);
    iotManager.registerAction("volume", musicVolumeControl);

    g_lcd1602->clear();
    g_lcd1602->write("The Home", 0, 4);
    g_lcd1602->write("Automation PRO", 1, 1);
    delay(1000);
    g_lcd1602->clear();

    g_leds->turnOn();
    delay(100); // (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);
    g_leds->setMode(S5050DJ::STROBE);
    delay(100); // (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);

    while (1) {
        /// External preset acquisition
        if (g_adcExternal != nullptr) {
            uint16_t val = (g_adcExternal->analogRead() / 40); // Range: 0 to 100 (Percentage)
            g_lcd1602->write("A0 ", 0, 0);
            g_lcd1602->write(val, 0, 3);
            g_leds->setSequenceSpeed(val);
        }

        delay(100);
        g_lcd1602->write("   ", 0, 4);
        g_lcd1602->write("   ", 1, 4);
        g_lcd1602->write("   ", 0, 12);
        g_lcd1602->write("   ", 1, 12);
//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

    g_leds->turnOff();
    delay((S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);

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
//	initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

	g_leds->turnOn();
	delay((S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30); // Minimum waiting time to avoid conflicts during the transmission of commands through the NEC protocol.

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
			delay(2000 + (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);
		}

		g_leds->setMode(S5050DJ::FLASH);
		delay(3000 + (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);
		g_leds->setMode(S5050DJ::STROBE);
		delay(3000 + (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);
		g_leds->setMode(S5050DJ::FADE);
		delay(3000 + (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);
		g_leds->setMode(S5050DJ::SMOOTH);
		delay(3000 + (S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30);
		break;

//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }

	g_leds->turnOff();
	delay((S5050DJ_COMMAND_PERIOD_TIME / 1000) + 30); // Minimum waiting time to avoid conflicts during the transmission of commands through the NEC protocol.

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
	initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

	bool statusFlag = false;
	uint8_t dutyCycle = 100, counterCycles = 0;
	PWM externPWM_FST(LED_RED, dutyCycle), externPWM_SND(LED_GREEN, dutyCycle), externPWM_TRD(LED_BLUE, dutyCycle);

	while (1) {
		while (counterCycles < 4) {
			g_lcd1602->write("Cycle counter: ");
			g_lcd1602->write(counterCycles, 0, 15);
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
		g_lcd1602->clear();
		counterCycles = 0;
    		g_timers_list.TimerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
	}
}

static void red_green(PWM &red, PWM &green) {
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("RED", 0, 7);
	red.setDuty(100);
	delay(2500);
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("YELLOW", 0, 7);
	red.setDuty(46);
	green.setDuty(50);
	delay(2500);
}

static void green_blue(PWM &green, PWM &blue) {
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("GREEN", 0, 7);
	green.setDuty(100);
	delay(2500);
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("SKY BLUE", 0, 7);
	green.setDuty(36);
	blue.setDuty(50);
	delay(2500);
}

static void blue_red(PWM &blue, PWM &red) {
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("BLUE", 0, 7);
	blue.setDuty(100);
	delay(2500);
	g_lcd1602->clear();
	g_lcd1602->write("Color: ");
	g_lcd1602->write("VIOLET", 0, 7);
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
//	initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
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
    	g_timers_list.TimerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
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
//	initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

	LED_RED.ClearPin();
	LED_GREEN.ClearPin();
	LED_BLUE.ClearPin();

	if (g_ds3231->set(0, 1, 1, 31, 12, 2023)) {
		LED_GREEN.SetPin();
		LED_RED.SetPin();
	} else LED_RED.ClearPin();

	RTC_st rtc;

	while (1) {
		rtc = g_ds3231->get();
		if (g_ds3231->getStatus()) {
			LED_GREEN.SetPin();
			LED_BLUE.setPin();
		} else LED_BLUE.clearPin();
		g_display->set(rtc.TIME.MIN, 0);
		g_display->set(rtc.TIME.SEC, 1);
		g_usb->transmit(g_ds3231->print());
    	g_timers_list.TimerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
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
	initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
//	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
//	initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

	DAC dacLocal(DAC::DAC_0);
	uint16_t val, aux;

    while (1) {
    	/// Internal preset acquisition
    	if (g_preset != nullptr && g_dacExternal != nullptr) {
    		val = g_preset->analogRead();
			g_lcd1602->write("A0 ", 0, 0);
			g_lcd1602->write(val, 0, 3);

			aux = val / 4;
			g_dacExternal->analogWrite(aux); // 12 bits into 10 bits
			g_lcd1602->write("D0 ", 1, 0);
			g_lcd1602->write(aux, 1, 3);
    	}

		/// External preset acquisition
		if (g_adcExternal != nullptr) {
			val = g_adcExternal->analogRead();
			g_lcd1602->write("A1 ", 0, 8);
			g_lcd1602->write(val, 0, 11);

			aux = val / 4;
			dacLocal.analogWrite(aux); // 12 bits into 10 bits
			g_lcd1602->write("D1 ", 1, 8);
			g_lcd1602->write(aux, 1, 11);
		}

		delay(100);
    	g_lcd1602->write("   ", 0, 4);
    	g_lcd1602->write("   ", 1, 4);
    	g_lcd1602->write("   ", 0, 12);
    	g_lcd1602->write("   ", 1, 12);
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
//	initLCD1602();	// Initializes the g_lcd1602     ~ Define the CN15_PINS macro in ProgramConfig.h {P0.13 - P0.11 - P0.10 - P0.09 - P0.01 - P0.14}
//	initKeyboard();	// Initializes the g_keyboard    ~ Define the CN16_PINS macro in ProgramConfig.h {P0.28 - P0.27 - P0.08 - P0.15 - P0.26}
//	initDS3231();	// Initializes the g_ds3231      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initM24C16();	// Initializes the g_eeprom      ~ Define the I2C0_PINS macro in ProgramConfig.h {P0.11 - P0.10}
//	initLEDs();		// Initializes the g_leds        ~ Define the LED_TRIP_PIN macro in ProgramConfig.h {P0.29}
	initUSB0();		// Initializes the g_usb         ~ Define the USB0_PINS macro in ProgramConfig.h {P0.24 - P0.25}
	initRFID();		// Initializes the g_rfid        ~ Define the SPI_DEBUG_PINS macro in ProgramConfig.h {P0.09 - P0.10 - P0.11 - P0.01}
//	initPreset();	// Initializes the g_preset      ~ Define the ANALOG_FST_CHANNEL_ENABLED macro in ProgramConfig.h {P0.07}
//	initADC();		// Initializes the g_adcExternal ~ Define the ANALOG_SND_CHANNEL_ENABLED macro in ProgramConfig.h {P0.06}
//	initDAC();		// Initializes the g_dacExternal ~ Define the CN7_PINS and DAC_SND_CHANNEL_ENABLED macros in ProgramConfig.h {P0.29}

    while (1) {
//    	g_timers_list.timerEvents(); // If only the "delay(milliseconds)" function is used in the program then this instruction will not be necessary.
    }
}

#endif // DEBUG_MODE macro definition in precompile stage