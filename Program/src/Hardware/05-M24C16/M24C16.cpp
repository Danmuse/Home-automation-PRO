/*/*!
 * @file M24C16.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/10/2023 16:36:21
 */

#include "M24C16.h"

pageBlock_t M24C16::m_pageBock = FIRST_PAGE_BLOCK;

M24C16 *g_eeprom = nullptr;

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

M24C16::M24C16() : I2C(TWI_CHANNEL, I2C_SCL, I2C_SDA),
m_statusEEPROM{EEPROM_OK} { }

/*** EXAMPLE OF CODE ***
statusI2C_t readM24C02Byte(uint8_t reg, uint8_t * readValue){
	uint8_t valueAux;
	statusI2C_t errors;

	i2c0_start(M24C02_address,write);
	errors = i2c0_writeLastByte(reg);
	if (errors) return errors;

	i2c0_start(M24C02_address,read);
	errors = i2c0_readNACK(&valueAux);
	if (errors) return errors;
	(*readValue) = valueAux;
	return NoError;
}

statusI2C_t readM24C02Float(uint8_t reg, float *value){
	statusI2C_t status;
	uint8_t i=0;
	uint8_t auxRead=0;
	for(i=0; i<4; i++){
		status = readM24C02Byte(reg+i, &auxRead);
		if (status) return status;
		((uint8_t *)value)[i]=auxRead;
	}
	return NoError;
}

statusI2C_t readM24C02Uint32(uint8_t reg, uint32_t *value){
	statusI2C_t status;
	uint8_t i=0;
	uint8_t auxRead=0;
	for(i=0; i<4; i++){
		status = readM24C02Byte(reg+i, &auxRead);
		if (status) return status;
		((uint8_t *)value)[i]=auxRead;
	}
	return NoError;
}
***/

template <typename T> SyncCommTWI::statusComm_t M24C16::acquire(T *values, const uint16_t position) {
	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////
	return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS;
}

template <typename T> SyncCommTWI::statusComm_t M24C16::transmit(T values, const uint16_t position) {
	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////
	if (!(this->transmitByte(M24C16_ADDR_REG, position, values)) && !(this->getStatus())) this->m_statusEEPROM = EEPROM_OK;
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

/*** EXAMPLE OF CODE ***
statusI2C_t writeM24C02Byte(uint8_t reg, uint8_t value){
	statusI2C_t errors;
	i2c0_start(M24C02_address,write);
	//point to the desired register
	errors = i2c0_write(reg);
	if (errors) return errors;
	errors = i2c0_writeLastByte(value);
	if (errors) return errors;
	NOp10M();
	return NoError;
}

statusI2C_t writeM24C02Float(uint8_t reg, float value){
	statusI2C_t status;
	uint8_t i=0;
	for(i=0; i<4; i++){
		status = writeM24C02Byte(reg+i,((uint8_t *)&value)[i]);
		if (status) return status;
	}
	return NoError;
}

statusI2C_t writeM24C02Uint32(uint8_t reg, uint32_t value){
	statusI2C_t status;
	uint8_t i=0;
	for(i=0; i<4; i++){
		status = writeM24C02Byte(reg+i,((uint8_t *)&value)[i]);
		if (status) return status;
	}
	return NoError;
}
***/

template <typename T> EEPROM_result_t M24C16::read(T *data, const uint16_t position, pageBlock_t pageBlock, middleByte_t middleByte) {
	this->m_statusEEPROM = EEPROM_OK;

	if (position > MAX_PAGE_BLOCK_BYTES - 1) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	}

	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;

	return this->getStatus();
}

template <typename T> EEPROM_result_t M24C16::write(T data, const uint16_t position, pageBlock_t pageBlock, middleByte_t middleByte) {
	byteReg_ut reg;
	this->m_statusEEPROM = EEPROM_OK;

	if (position > MAX_PAGE_BLOCK_BYTES - 1) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	} else if (position > MAX_PAGE_BLOCK_BYTES - 2 && std::is_same<T, float>::value) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	} else if (position > MAX_PAGE_BLOCK_BYTES - 2 && std::is_same<T, uint32_t>::value) {
		this->m_statusEEPROM = EEPROM_OVERFLOW_INVALID;
		return this->getStatus();
	}

	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;
	if (std::is_same<T, float>::value) {
		reg.Float = data;
		uint8_t valuesFST[2] = { reg.UInt8[0], reg.UInt8[1] };
		uint8_t valuesSND[2] = { reg.UInt8[2], reg.UInt8[3] };
		if (!(this->transmit(valuesFST, position))) return this->getStatus();
		if (!(this->transmit(valuesSND, position + 1))) return this->getStatus();
		// if (!(this->transmit(reg.UInt16[0], position))) return this->getStatus();
		// if (!(this->transmit(reg.UInt16[1], position + 1))) return this->getStatus();
	} else if (std::is_same<T, uint32_t>::value) {
		reg.UInt32 = data;
		uint8_t valuesFST[2] = { reg.UInt8[0], reg.UInt8[1] };
		uint8_t valuesSND[2] = { reg.UInt8[2], reg.UInt8[3] };
		if (!(this->transmit(valuesFST, position))) return this->getStatus();
		if (!(this->transmit(valuesSND, position + 1))) return this->getStatus();
		// if (!(this->transmit(reg.UInt16[0], position))) return this->getStatus();
		// if (!(this->transmit(reg.UInt16[1], position + 1))) return this->getStatus();
	// } else if (std::is_same<T, uint16_t>::value) this->transmit(data, position);
	} else if (std::is_same<T, uint16_t>::value) {
		reg.UInt16 = data;
		uint8_t values[2] = { reg.UInt8[0], reg.UInt8[1] };
		this->transmit(values, position);
	} else if (std::is_same<T, uint8_t>::value) {
		if (!(this->acquire(&(reg.UInt16[0]), position))) return this->getStatus();
		// if (middleByte == FST_QUARTER_BYTE) reg.UInt8[0] = data; 
		if (middleByte == FST_QUARTER_BYTE) {
			reg.UInt8[0] = data;
			uint8_t values[1] = { reg.UInt8[0] };
			this->transmit(values, position);
		// if (middleByte == SND_QUARTER_BYTE) reg.UInt8[1] = data;
		} else if (middleByte == SND_QUARTER_BYTE) {
			reg.UInt8[1] = data;
			uint8_t values[1] = { reg.UInt8[1] };
			this->transmit(values, position);
		}
		// this->transmit(reg.UInt16[0], position);
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

//EEPROM_result_t M24C16::read(uint8_t *value, const uint16_t position, pageBlock_t pageBlock) {
//	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;
//
//	////////////////////////////////////////////
//	// Add the corresponding fragment of code //
//	////////////////////////////////////////////
//
//	return this->getStatus();
//}
//
//EEPROM_result_t M24C16::write(uint8_t value, const uint16_t position, pageBlock_t pageBlock) {
//	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;
//
//	////////////////////////////////////////////
//	// Add the corresponding fragment of code //
//	////////////////////////////////////////////
//
//	return this->getStatus();
//}

EEPROM_result_t M24C16::getStatus(void) const {
	return this->m_statusEEPROM;
}

M24C16::~M24C16() { }

///////////////////////////////
/// FM24C16U initialization ///
///////////////////////////////

void initM24C16(void) {
	#if defined(I2C0_PINS) || defined(I2C1_PINS)

	static M24C16 eeprom;

	g_eeprom = &eeprom;

	#endif // defined(I2C0_PINS) || defined(I2C1_PINS)
}
