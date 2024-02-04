/*/*!
 * @file S5050DJ.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 12:11:36
 */

#include "S5050DJ.h"

S5050DJ *g_leds = nullptr;

CTimer *g_ctimer_instance = nullptr;
CTimer::actionInterruption_t externalOutput_instance;
bool repeatCommand_instance;
uint8_t repeatCounts_instance;
uint32_t command_instance;

S5050DJ::S5050DJ(const Gpio &output, actionInterruption_t idleExternalOutput) : CTimer(output, CTIMER_MATCH),
m_brightnessPoint{S5050DJ_MAX_BRIGHTNESS_SPEED_POINT},
m_speedPoint{S5050DJ_MAX_BRIGHTNESS_SPEED_POINT},
m_externalOutput{idleExternalOutput},
m_externalActivity{HIGH} {
	if (idleExternalOutput == INACTIVE) this->m_externalOutput = CLEAR;
	else if (idleExternalOutput == TOGGLE) this->m_externalOutput = this->m_activity == LOW ? SET : CLEAR;
	changeExternalOutput(this->m_externalOutput);
}

void S5050DJ::transmitCommand(void) {
	static uint8_t indexBit = 0;
	static bool activityBit, idleStateBit = false;
	if (indexBit == 0) {
		indexBit++;
		g_ctimer_instance->configMatch(S5050DJ_COMMAND_START_BIT_TIME, externalOutput_instance == CLEAR ? CLEAR : SET);
	} else if (indexBit == 1) {
		indexBit++;
		g_ctimer_instance->configMatch(S5050DJ_COMMAND_SLOT_BIT_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
	} else if (indexBit > 1 && indexBit < (S5050DJ_COMMAND_LENGTH + 2)) {
		if (!idleStateBit) activityBit = ((command_instance >> ((S5050DJ_COMMAND_LENGTH + 1) - indexBit)) & 0x01);
		if (activityBit && !idleStateBit) {
			idleStateBit = true;
			g_ctimer_instance->configMatch(S5050DJ_COMMAND_HIGH_ACTIVE_BIT_TIME, externalOutput_instance == CLEAR ? CLEAR : SET);
		} else if (!activityBit && !idleStateBit) {
			idleStateBit = true;
			g_ctimer_instance->configMatch(S5050DJ_COMMAND_LOW_ACTIVE_BIT_TIME, externalOutput_instance == CLEAR ? CLEAR : SET);
		} else {
			indexBit++;
			idleStateBit = false;
			if (activityBit) g_ctimer_instance->configMatch(S5050DJ_COMMAND_HIGH_IDLE_BIT_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
			else g_ctimer_instance->configMatch(S5050DJ_COMMAND_LOW_IDLE_BIT_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
		}
	} else if (indexBit == (S5050DJ_COMMAND_LENGTH + 2)) {
		indexBit++;
		g_ctimer_instance->configMatch(S5050DJ_COMMAND_STOP_BIT_TIME, externalOutput_instance == CLEAR ? CLEAR : SET);
	} else if (indexBit > (S5050DJ_COMMAND_LENGTH + 2) && !repeatCommand_instance) {
		// Avoid conflicts with other classes that inherit from CTimer class.
		// Stabilized the output pin by connecting it to the idle state.
		g_ctimer_instance->bindHandler(nullptr);
		g_ctimer_instance->configMatch(S5050DJ_DEBOUNCE_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
		g_ctimer_instance = nullptr;
		indexBit = 0;
	} else if (indexBit == (S5050DJ_COMMAND_LENGTH + 3) && repeatCommand_instance) {
		indexBit++;
		g_ctimer_instance->configMatch(S5050DJ_REPEAT_COMMAND_DELAY - S5050DJ_COMMAND_PERIOD_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
	} else if (indexBit == (S5050DJ_COMMAND_LENGTH + 4) && repeatCommand_instance) {
		indexBit++;
		g_ctimer_instance->configMatch(S5050DJ_REPEAT_COMMAND_START_BIT_TIME, externalOutput_instance == CLEAR ? CLEAR : SET);
	} else if (indexBit == (S5050DJ_COMMAND_LENGTH + 5) && repeatCommand_instance) {
		indexBit++;
		g_ctimer_instance->configMatch(S5050DJ_REPEAT_COMMAND_SLOT_BIT_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
	} else if (indexBit == (S5050DJ_COMMAND_LENGTH + 6) && repeatCommand_instance) {
		if (repeatCounts_instance) {
			indexBit = S5050DJ_COMMAND_LENGTH + 4;
			repeatCounts_instance--;
			g_ctimer_instance->configMatch(S5050DJ_REPEAT_COMMAND_DELAY - S5050DJ_REPEAT_COMMAND_PERIOD_TIME, externalOutput_instance == CLEAR ? SET : CLEAR);
		} else {
			repeatCommand_instance = false;
			repeatCounts_instance = 0;
			g_ctimer_instance->configMatch(S5050DJ_REPEAT_COMMAND_STOP_BIT_TIME, externalOutput_instance == CLEAR ? CLEAR : SET);
		}
	}
}

void S5050DJ::prepareConditions(void) {
	g_ctimer_instance = this;
	externalOutput_instance = this->m_externalOutput;
	// Modularize this implementation with the interruption of the CTimer interface.
	this->bindHandler(transmitCommand);
	this->adjustTimeCount(1, MICROS);
	this->configMatch(S5050DJ_DEBOUNCE_TIME, this->m_externalOutput == CLEAR ? SET : CLEAR);
}

void S5050DJ::changeExternalOutput(actionInterruption_t actionInterrupt) {
	// Modularize this implementation with the interruption of the CTimer interface.
	this->bindHandler(nullptr);
	this->adjustTimeCount(1, MICROS);
	this->configMatch(S5050DJ_DEBOUNCE_TIME, actionInterrupt);
}

bool S5050DJ::setAction(actionSetting_t action) {
	// Avoid conflicts with other classes that inherit from CTimer class.
	if (g_ctimer_instance == nullptr) {
		if (action == TURNON_LEDS) this->m_externalActivity = HIGH;
		else if (action == TURNOFF_LEDS) this->m_externalActivity = LOW;
		command_instance = ((S5050DJ_ADDR << (2 * BYTE_SIZE)) | action);
		this->prepareConditions();
		return true;
	} else return false;
}

bool S5050DJ::setColor(colorSetting_t color) {
	// Avoid conflicts with other classes that inherit from CTimer class.
	if (g_ctimer_instance == nullptr) {
		repeatCommand_instance = false;
		command_instance = ((S5050DJ_ADDR << (2 * BYTE_SIZE)) | color);
		this->prepareConditions();
		return true;
	} else return false;
}

bool S5050DJ::setMode(modeSetting_t mode) {
	// Avoid conflicts with other classes that inherit from CTimer class.
	if (g_ctimer_instance == nullptr) {
		repeatCommand_instance = false;
		command_instance = ((S5050DJ_ADDR << (2 * BYTE_SIZE)) | mode);
		this->prepareConditions();
		return true;
	} else return false;
}

bool S5050DJ::setBrightness(uint8_t percentage) {
	uint8_t offsetThresholdPercentage = 10, result = percentage > 100 ? 100 : percentage;
//	if (!((command_instance == FLASH) || (command_instance == STROBE) || (command_instance == FADE) || (command_instance == SMOOTH)) && g_ctimer_instance == nullptr) {
	if (g_ctimer_instance == nullptr) {
		if (result > (100 / (float)(S5050DJ_MAX_BRIGHTNESS_SPEED_POINT)) + (offsetThresholdPercentage / (100 / (float)(S5050DJ_MAX_BRIGHTNESS_SPEED_POINT)))) {
			// NOTE: The incorrect conversion of this result to float would produce instabilities in the program.
			result /= (100 / (float)(S5050DJ_MAX_BRIGHTNESS_SPEED_POINT));
			if (this->m_brightnessPoint < result && this->m_externalActivity == LOW) {
//				this->m_brightnessPoint++;
				this->turnOn();
				// The following code fragment is correct,
				// but due to the implementation there will be instabilities in the program.
//				this->turnOn();
//				repeatCommand_instance = result - this->m_brightnessPoint;
//				this->m_brightnessPoint = result;
//				this->setAction(INCREASE_BRIGHTNESS);
			} else if (this->m_brightnessPoint < result) {
				this->m_brightnessPoint++; // TODO: Check this code instruction!!
				this->m_speedPoint = this->m_brightnessPoint;
				// The following code fragment is correct,
				// but due to the implementation there will be instabilities in the program.
//				repeatCommand_instance = true;
//				repeatCounts_instance = result - this->m_brightnessPoint;
//				this->m_brightnessPoint = result;
				this->setAction(INCREASE_BRIGHTNESS);
			} else if (this->m_brightnessPoint > result) {
				this->m_brightnessPoint--; // TODO: Check this code instruction!!
				this->m_speedPoint = this->m_brightnessPoint;
				// The following code fragment is correct,
				// but due to the implementation there will be instabilities in the program.
//				repeatCommand_instance = true;
//				repeatCounts_instance = this->m_brightnessPoint - result;
//				this->m_brightnessPoint = result;
				this->setAction(DECREASE_BRIGHTNESS);
			}
		} else {
//			this->m_brightnessPoint--;
			this->turnOff();
		}
		return true;
	} else return false;
}

bool S5050DJ::setSequenceSpeed(uint8_t percentage) {
	uint8_t result = percentage > 100 ? 100 : percentage;
//	if (((command_instance == FLASH) || (command_instance == STROBE) || (command_instance == FADE) || (command_instance == SMOOTH)) && g_ctimer_instance == nullptr) {
	if (g_ctimer_instance == nullptr) {
		// NOTE: The incorrect conversion of this result to float would produce instabilities in the program.
		result /= (100 / (float)(S5050DJ_MAX_BRIGHTNESS_SPEED_POINT));
		if (this->m_externalActivity == LOW) this->turnOn();
		else if (this->m_speedPoint < result) {
			this->m_speedPoint++; // TODO: Check this code instruction!!
			this->m_brightnessPoint = this->m_speedPoint;
			// The following code fragment is correct,
			// but due to the implementation there will be instabilities in the program.
//			repeatCommand_instance = true;
//			repeatCounts_instance = result - this->m_speedPoint;
//			this->m_speedPoint = result;
			this->setAction(INCREASE_SPEED);
		} else if (this->m_speedPoint > result) {
			this->m_speedPoint--; // TODO: Check this code instruction!!
			this->m_brightnessPoint = this->m_speedPoint;
			// The following code fragment is correct,
			// but due to the implementation there will be instabilities in the program.
//			repeatCommand_instance = true;
//			repeatCounts_instance = this->m_speedPoint - result;
//			this->m_speedPoint = result;
			this->setAction(DECREASE_SPEED);
		}
		return true;
	} else return false;
}

bool S5050DJ::turnOn(void) {
	repeatCommand_instance = false;
	return this->setAction(TURNON_LEDS);
}

bool S5050DJ::turnOff(void) {
	repeatCommand_instance = false;
	return this->setAction(TURNOFF_LEDS);
}

uint8_t S5050DJ::getBrightness(void) const {
	// NOTE: The incorrect conversion of this result to float would produce instabilities in the program.
    return (uint8_t)(this->m_brightnessPoint * (100 / (float)(S5050DJ_MAX_BRIGHTNESS_SPEED_POINT)));
}

uint8_t S5050DJ::getSpeed(void) const {
	// NOTE: The incorrect conversion of this result to float would produce instabilities in the program.
    return (uint8_t)(this->m_speedPoint * (100 / (float)(S5050DJ_MAX_BRIGHTNESS_SPEED_POINT)));
}

///////////////////////////////
/// RGB LEDs initialization ///
///////////////////////////////

void initLEDs(void) {
	#if defined(LED_TRIP_PIN)

    static S5050DJ leds(LED_TRIP, CTimer::SET);
    leds.turnOff();

    g_leds = &leds;

	#endif // defined(LED_TRIP_PIN)
}
