/*/*!
 * @file M24C16.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/10/2023 16:36:21
 */

#include "M24C16.h"

pageBlock_t M24C16::m_pageBock = FIRST_PAGE_BLOCK;

M24C16 *g_eeprom = nullptr;

/****************** EEPROM Testing START ******************
int main(void) {
	initDevice();
	initM24C16();
	initUSB0();

	while (1) {
		// ...
		g_timers_list.TimerEvents();
		delay(1000);
	}
}
******************** EEPROM Testing END *******************/

/*
#include <cstdio>
#include <cstdint>

using namespace std;

// ----------------------------        FST OPTION
float getFloat(byteReg_ut reg) {
    return reg.Float;
}

uint32_t getUInt32(byteReg_ut reg) {
    return reg.UInt32;
}

int main() {
    byteReg_ut reg;
    reg.Float = 3.14; // Ejemplo de bits correspondientes al float 3.14

    uint32_t numeroUInt32 = getUInt32(reg);
    std::cout << numeroUInt32 << std::endl;
    reg.UInt32 = numeroUInt32;
    float numeroFloat = getFloat(reg);
    std::cout << numeroFloat << std::endl;

    return 0;
}

// ----------------------------        SND OPTION
float getFloat(byteReg_ut reg);

int main(void) {
    float foo = 3.14;
    byteReg_ut variableReg;
    variableReg.Float = foo;
    cout << "Float:" << variableReg.Float << endl;
    cout << "UInt32:" << getFloat(variableReg) << endl;
    cout << "UInt16[0]:" << variableReg.UInt16[0] << endl;
    cout << "UInt16[1]:" << variableReg.UInt16[1] << endl;
    cout << "UInt8[0]:" << (uint16_t)variableReg.UInt8[0] << endl;
    cout << "UInt8[1]:" << (uint16_t)variableReg.UInt8[1] << endl;
    cout << "UInt8[2]:" << (uint16_t)variableReg.UInt8[2] << endl;
    cout << "UInt8[3]:" << (uint16_t)variableReg.UInt8[3] << endl;
    return 0;
}

float getFloat(byteReg_ut reg) {
    float result = 0; // By defining this variable as static, you prevent the value passed as a reference from being lost.
	result = (float)(((uint32_t)reg.UInt16[1] << 16) | (uint32_t)reg.UInt16[0]); // (((((uint32_t)add) & 0x0000FFFF) << 16) | (((uint32_t)add) & 0xFFFF0000) >> 16);
	return result;
}
*/

M24C16::M24C16() : I2C(I2C0_SCL, I2C0_SDA),
m_statusEEPROM{EEPROM_OK} { }

SyncCommTWI::statusComm_t M24C16::acquire(uint8_t values[], size_t numBytes, uint16_t position) {
	//////////////////////////////////////////////
	// Check the corresponding fragment of code //
	//////////////////////////////////////////////
	if (!(this->receiveBytes(M24C16_ADDR_REG, position, values, numBytes)) && !(this->getStatus())) this->m_statusEEPROM = EEPROM_OK;
	else this->m_statusEEPROM = EEPROM_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

SyncCommTWI::statusComm_t M24C16::transmit(uint8_t values[], size_t numBytes, uint16_t position) {
	//////////////////////////////////////////////
	// Check the corresponding fragment of code //
	//////////////////////////////////////////////
	if (!(this->transmitBytes(M24C16_ADDR_REG, position, values, numBytes)) && !(this->getStatus())) this->m_statusEEPROM = EEPROM_OK;
	else this->m_statusEEPROM = EEPROM_UPDATE_ERR;
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

//SyncCommTWI::statusComm_t M24C16::acquireFloat(float *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::acquireInt32(int32_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::acquireUInt32(uint32_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::acquireInt16(int16_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::acquireUInt16(uint16_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::acquireInt8(int8_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::acquireByte(uint8_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

template <typename T> EEPROM_result_t M24C16::read(T *data, modifierType_t modifier, uint16_t position, pageBlock_t pageBlock, middleByte_t middleByte) {
	byteReg_ut reg;
	this->m_statusEEPROM = EEPROM_OK;

	if (sizeof(data) != 8) {
		this->m_statusEEPROM = EEPROM_INCORRECT_MODIFIER;
		return this->getStatus();
	}

	if (position > MAX_PAGE_BLOCK_BYTES - 1) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	} else if (position > MAX_PAGE_BLOCK_BYTES - 2 && (modifier == FLOAT || modifier == INT32 || modifier == UINT32)) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	}

	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;
	if (modifier == FLOAT || modifier == INT32 || modifier == UINT32) {
		uint8_t valuesFST[2], valuesSND[2];
		if (!(this->acquire(valuesFST, sizeof(valuesFST), position))) return this->getStatus();
		if (!(this->acquire(valuesSND, sizeof(valuesSND), position + 1))) return this->getStatus();
		reg.UInt8[0] = valuesFST[0]; reg.UInt8[1] = valuesFST[1];
		reg.UInt8[0] = valuesSND[2]; reg.UInt8[3] = valuesSND[1];
		if (modifier == FLOAT) data = reg.Float;
		else if (modifier == INT32) data = (int32_t)reg.UInt32;
		else if (modifier == UINT32) data = reg.UInt32;
	} else if (modifier == INT16 || modifier == UINT16) {
		uint8_t values[2];
		if (!(this->acquire(values, sizeof(values), position))) return this->getStatus();
		reg.UInt8[0] = values[0]; reg.UInt8[1] = values[1];
		if (modifier == INT16) data = (int16_t)reg.UInt16[0];
		else if (modifier == UINT16) data = reg.UInt16[0];
	} else if (modifier == INT8 || modifier == UINT8 || modifier == CHAR) {
		uint8_t values[2];
		if (!(this->acquire(values, sizeof(values), position))) return this->getStatus();
		if (middleByte == FST_QUARTER_BYTE) {
			if (modifier == INT8) data = (int8_t)values[0];
			else if (modifier == UINT8) data = values[0];
			else if (modifier == CHAR) data = (char)values[0];
		} else if (middleByte == SND_QUARTER_BYTE) {
			if (modifier == INT8) data = (int8_t)values[1];
			else if (modifier == UINT8) data = values[1];
			else if (modifier == CHAR) data = (char)values[1];
		}
	} else this->m_statusEEPROM = EEPROM_INCORRECT_MODIFIER;
	return this->getStatus();
}

template <typename T> EEPROM_result_t M24C16::write(T data, uint16_t position, pageBlock_t pageBlock, middleByte_t middleByte) {
	byteReg_ut reg;
	this->m_statusEEPROM = EEPROM_OK;

	if (position > MAX_PAGE_BLOCK_BYTES - 1) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	} else if (position > MAX_PAGE_BLOCK_BYTES - 2 && (std::is_same<T, float>::value || std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value)) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	}

	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;
	if (std::is_same<T, float>::value || std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value) {
		if (std::is_same<T, float>::value) reg.Float = data;
		else if (std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value) reg.UInt32 = data;
		uint8_t valuesFST[2] = { reg.UInt8[0], reg.UInt8[1] };
		uint8_t valuesSND[2] = { reg.UInt8[2], reg.UInt8[3] };
		if (!(this->transmit(valuesFST, sizeof(valuesFST), position))) return this->getStatus();
		if (!(this->transmit(valuesSND, sizeof(valuesSND), position + 1))) return this->getStatus();
	} else if (std::is_same<T, int>::value, std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value) {
		reg.UInt16[0] = data;
		uint8_t values[2] = { reg.UInt8[0], reg.UInt8[1] };
		if (!(this->transmit(values, sizeof(values), position))) return this->getStatus();
	} else if (std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value || std::is_same<T, char>::value) {
		uint8_t values[2];
		if (!(this->acquire(values, sizeof(values), position))) return this->getStatus();
		if (middleByte == FST_QUARTER_BYTE) {
			values[0] = data;
			if (!(this->transmit(values, sizeof(values), position))) return this->getStatus();
		} else if (middleByte == SND_QUARTER_BYTE) {
			values[1] = data;
			if (!(this->transmit(values, sizeof(values), position))) return this->getStatus();
		}
	} else this->m_statusEEPROM = EEPROM_INCORRECT_MODIFIER;
	return this->getStatus();
}

//SyncCommTWI::statusComm_t M24C16::transmitFloat(float value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::transmitInt32(int32_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::transmitUInt32(uint32_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::transmitInt16(int16_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::transmitUInt16(uint16_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::transmitInt8(int8_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }
//
//SyncCommTWI::statusComm_t M24C16::transmitByte(uint8_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

EEPROM_result_t M24C16::getStatus(void) const {
	return this->m_statusEEPROM;
}

M24C16::~M24C16() { }

///////////////////////////////
/// FM24C16U initialization ///
///////////////////////////////

void initM24C16(void) {
	#if defined(I2C0_PINS) || defined(I2C1_PINS) || defined(I2C2_PINS) || defined(I2C3_PINS)

	static M24C16 eeprom;

	g_eeprom = &eeprom;

	#endif // defined(I2C0_PINS) || defined(I2C1_PINS)
}
