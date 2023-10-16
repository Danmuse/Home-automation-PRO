/*/*!
 * @file M24C16.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/10/2023 16:36:21
 */

#include "M24C16.h"

pageBlock_t M24C16::m_pageBock = FIRST_PAGE_BLOCK;

M24C16 *g_eeprom = nullptr;

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

SyncCommTWI::statusComm_t M24C16::acquireFloat(float *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::acquireInt32(int32_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::acquireUInt32(uint32_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::acquireInt16(int16_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::acquireUInt16(uint16_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::acquireInt8(int8_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::acquireByte(uint8_t *value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

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

SyncCommTWI::statusComm_t M24C16::transmitFloat(float value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::transmitInt32(int32_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::transmitUInt32(uint32_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::transmitInt16(int16_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::transmitUInt16(uint16_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::transmitInt8(int8_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

SyncCommTWI::statusComm_t M24C16::transmitByte(uint8_t value) { return this->getStatus() ? TWI_FAILURE : TWI_SUCCESS; }

EEPROM_result_t M24C16::read(uint8_t *value, const uint16_t position, pageBlock_t pageBlock) {
	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;

	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	return this->getStatus();
}

EEPROM_result_t M24C16::write(uint8_t value, const uint16_t position, pageBlock_t pageBlock) {
	if (this->m_pageBock != pageBlock) this->m_pageBock = pageBlock;

	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	return this->getStatus();
}

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
