/*!
 * @file M24C16.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/10/2023 16:36:21
 * @brief EEPROM Memory FM24C16U Class API (Application Programming Interface).
 */

#ifndef M24C16_H_
#define M24C16_H_

// #include <type_traits>
#include "I2C.h"
#include "ProgramConfig.h"

#define M24C16_ADDR_REG (0x50U)

#define MAX_PAGE_BLOCK_BYTES 2048
#define MAX_EEPROM_BYTES MAX_PAGE_BLOCK_BYTES * BYTE_SYZE

typedef enum {
	FIRST_PAGE_BLOCK,
	SECOND_PAGE_BLOCK,
	THIRD_PAGE_BLOCK,
	FOURTH_PAGE_BLOCK,
	FIFTH_PAGE_BLOCK,
	SIXTH_PAGE_BLOCK,
	SEVENTH_PAGE_BLOCK,
	EIGHTH_PAGE_BLOCK
} pageBlock_t;

//! @brief <b>EEPROM_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the EEPROM memory operations can be found.
typedef enum {
	EEPROM_OK,					//!< Successful operation.
	EEPROM_UPDATE_ERR,			//!< No valid data has been acquired or transmitted via i2C communication
	EEPROM_PAGE_BLOCK_INVALID,	//!< Exceeds the limit of allowed pages block.
	EEPROM_OVERFLOW_INVALID,	//!< Exceeds the limit of allowed bytes into the desired page block.
	EEPROM_INCORRECT_MODIFIER	//!< The method parameter does not match the supported modifiers.
} EEPROM_result_t;

typedef union {
    float Float;
    uint32_t UInt32;
    uint16_t UInt16[2];
    uint8_t UInt8[4];
} byteReg_ut;

class M24C16 : protected I2C {
public:
	// FST_QUARTER_BYTE: Indicates a zero bit shift to the left of the 16-bit register. Useful for storing 8-bit values ​​in EEPROM memory.
	// SND_QUARTER_BYTE: Indicates an 8-bit shift to the left of the 16-bit register. Useful for storing 8-bit values ​​in EEPROM memory.
	enum middleByte_t { FST_QUARTER_BYTE, SND_QUARTER_BYTE };
private:
	static pageBlock_t m_pageBock;
	EEPROM_result_t m_statusEEPROM;

	template <typename T> statusComm_t acquire(T* values, const uint16_t position);
	template <typename T> statusComm_t transmit(T values, const uint16_t position);
public:
	M24C16();
	template <typename T> EEPROM_result_t read(T* data, const uint16_t position, pageBlock_t pageBlock = FIRST_PAGE_BLOCK, middleByte_t middleByte = FST_QUARTER_BYTE);
	template <typename T> EEPROM_result_t write(T data, const uint16_t position, pageBlock_t pageBlock = FIRST_PAGE_BLOCK, middleByte_t middleByte = FST_QUARTER_BYTE);

	EEPROM_result_t getStatus(void) const;
	virtual ~M24C16();
};

//if (std::is_same<T, float>::value) {
//	std::cout << "Tipo: float" << std::endl;
//}


/*
class M24C16 : protected I2C {
private:
	static pageBlock_t m_pageBock;
	EEPROM_result_t m_statusEEPROM;
	statusComm_t acquireFloat(float *value);
	statusComm_t acquireInt32(int32_t *value);
	statusComm_t acquireUInt32(uint32_t *value);
	statusComm_t acquireInt16(int16_t *value);
	statusComm_t acquireUInt16(uint16_t *value);
	statusComm_t acquireInt8(int8_t *value);
	statusComm_t acquireByte(uint8_t *value);

	statusComm_t transmitFloat(float value);
	statusComm_t transmitInt32(int32_t value);
	statusComm_t transmitUInt32(uint32_t value);
	statusComm_t transmitInt16(int16_t value);
	statusComm_t transmitUInt16(uint16_t value);
	statusComm_t transmitInt8(int8_t value);
	statusComm_t transmitByte(uint8_t value);
public:
	M24C16();
	// @param *value: USE TEMPLATES.
	EEPROM_result_t read(uint8_t *value, const uint16_t position, pageBlock_t pageBlock = FIRST_PAGE_BLOCK);
	// @param value: USE TEMPLATES.
	EEPROM_result_t write(uint8_t value, const uint16_t position, pageBlock_t pageBlock = FIRST_PAGE_BLOCK);
	EEPROM_result_t getStatus(void) const;
	virtual ~M24C16();
};
*/

extern M24C16 *g_eeprom;

void initM24C16(void);

#endif /* M24C16_H_ */
