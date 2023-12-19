/*!
 * @file M24C16.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/10/2023 16:36:21
 * @brief EEPROM Memory FM24C16U Class API (Application Programming Interface).
 */

#ifndef M24C16_H_
#define M24C16_H_

#include "I2C.h"
#include "ProgramConfig.h"

#define M24C16_ADDR_REG (0x50U)

#define MAX_PAGE_BLOCKS 8
#define MAX_PAGE_BLOCK_BITS (REG_BYTES_SIZE * BYTE_SIZE) * BYTE_SIZE // 2048 Bits
#define MAX_EEPROM_BITS MAX_PAGE_BLOCK_BITS * MAX_PAGE_BLOCKS // 16384 Bits

//! @brief <b>EEPROM_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the EEPROM memory operations can be found.
typedef enum {
	EEPROM_OK,					//!< Successful operation.
	EEPROM_UPDATE_ERR,			//!< No valid data has been acquired or transmitted via I2C communication.
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
	enum modifierType_t { CHAR, UINT8, INT8, UINT16, INT16, UINT32, INT32, FLOAT };
	enum pageBlock_t { FIRST_PAGE_BLOCK = (0x0U), SECOND_PAGE_BLOCK = (0x4U), THIRD_PAGE_BLOCK = (0x2U),
		FOURTH_PAGE_BLOCK = (0x6U), FIFTH_PAGE_BLOCK = (0x1U), SIXTH_PAGE_BLOCK = (0x3U), SEVENTH_PAGE_BLOCK = (0x5U), EIGHTH_PAGE_BLOCK = (0x7U) };
private:
	static pageBlock_t m_pageBlock;
	EEPROM_result_t m_statusEEPROM;

	statusComm_t acquire(uint8_t values[], size_t numBytes, uint8_t position, pageBlock_t pageBlock);
	statusComm_t transmit(uint8_t values[], size_t numBytes, uint8_t position, pageBlock_t pageBlock);
public:
	M24C16(const Gpio& SCL, const Gpio& SDA, channelTWI_t channel);
	template <typename T> EEPROM_result_t read(T* data, modifierType_t modifier, uint8_t position, pageBlock_t pageBlock = FIRST_PAGE_BLOCK, middleByte_t middleByte = FST_QUARTER_BYTE);
	template <typename T> EEPROM_result_t write(T data, uint8_t position, pageBlock_t pageBlock = FIRST_PAGE_BLOCK, middleByte_t middleByte = FST_QUARTER_BYTE);

	EEPROM_result_t getStatus(void) const;
	virtual ~M24C16();
};

extern M24C16 *g_eeprom;

void initM24C16(void);

#endif /* M24C16_H_ */
