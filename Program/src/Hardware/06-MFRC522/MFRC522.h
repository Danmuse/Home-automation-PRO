/*!
 * @file MFRC522.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 22/18/2023 22:18:47
 * @brief RFID Reader MFRC522 Class API (Application Programming Interface).
 */

#ifndef MFRC522_H_
#define MFRC522_H_

#include "SPI.h"
#include "ProgramConfig.h"

//// Version 1.0 (0x91)
//// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 self-test
//const uint8_t MFRC522_firmware_referenceV1_0[] = {
//	0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
//	0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
//	0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
//	0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
//	0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
//	0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
//	0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
//	0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
//};
//// Version 2.0 (0x92)
//// NXP Semiconductors; Rev. 3.8 - 17 September 2014; 16.1.1 self-test
//const uint8_t MFRC522_firmware_referenceV2_0[] = {
//	0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
//	0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
//	0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
//	0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
//	0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
//	0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
//	0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
//	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
//};

//////////////////////////////////////////
// Page 0: Command and Status Registers //
//////////////////////////////////////////
#define RC522_COMMAND_REG			(0x01U)	//!< Starts and stops command execution
#define RC522_COM_IRQ_ENABLE_REG	(0x02U)	//!< Enable and disable interrupt request control bits
#define RC522_DIV_IRQ_ENABLE_REG	(0x03U)	//!< Enable and disable interrupt request control bits
#define RC522_COM_IRQ_REG			(0x04U)	//!< Interrupt request bits
#define RC522_DIV_IRQ_REG			(0x05U)	//!< Interrupt request bits
#define RC522_ERROR_REG				(0x06U)	//!< Error bits showing the error status of the last command executed
#define RC522_STATUS1_REG			(0x07U)	//!< Communication status bits
#define RC522_STATUS2_REG			(0x08U)	//!< Receiver and transmitter status bits
#define RC522_FIFO_DATA_REG			(0x09U)	//!< Input and output of 64 byte FIFO buffer
#define RC522_FIFO_LEVEL_REG		(0x0AU)	//!< Number of bytes stored in the FIFO buffer
#define RC522_WATER_LEVEL_REG		(0x0BU)	//!< Level for FIFO underflow and overflow warning
#define RC522_CONTROL_REG			(0x0CU)	//!< Miscellaneous control registers
#define RC522_BIT_FRAMING_REG		(0x0DU)	//!< Adjustments for bit-oriented frames
#define RC522_COLL_REG				(0x0EU)	//!< Bit position of the first bit-collision detected on the RF interface

///////////////////////////////
// Page 1: Command Registers //
///////////////////////////////
#define RC522_MODE_REG			(0x11U)	//!< Defines general modes for transmitting and receiving
#define RC522_TX_MODE_REG		(0x12U)	//!< Defines transmission data rate and framing
#define RC522_RX_MODE_REG		(0x13U)	//!< Defines reception data rate and framing
#define RC522_TX_CONTROL_REG	(0x14U)	//!< Controls the logical behavior of the antenna driver pins TX1 and TX2
#define RC522_TX_ASK_REG		(0x15U)	//!< Controls the setting of the transmission modulation
#define RC522_TX_SEL_REG		(0x16U)	//!< Selects the internal sources for the antenna driver
#define RC522_RX_SEL_REG		(0x17U)	//!< Selects internal receiver settings
#define RC522_RX_THRESHOLD_REG	(0x18U)	//!< Selects thresholds for the bit decoder
#define RC522_DEMOD_REG			(0x19U)	//!< Defines demodulator settings
#define RC522_MF_TX_REG			(0x1CU)	//!< Controls some MIFARE communication transmit parameters
#define RC522_MF_RX_REG			(0x1DU)	//!< Controls some MIFARE communication receive parameters
#define RC522_SERIAL_SPEED_REG	(0x1FU)	//!< Selects the speed of the serial UART interface

/////////////////////////////////////
// Page 2: Configuration Registers //
/////////////////////////////////////
#define RC522_CRC_RESULT_REG_H		(0x21U)	//!< Shows the MSB and LSB values of the CRC calculation
#define RC522_CRC_RESULT_REG_L		(0x22U)
#define RC522_MOD_WIDTH_REG			(0x24U)	//!< Controls the ModWidth setting
#define RC522_RF_CFG_REG			(0x26U)	//!< Configures the receiver gain
#define RC522_GS_N_REG				(0x27U)	//!< Selects the conductance of the antenna driver pins TX1 and TX2 for modulation
#define RC522_CW_GS_P_REG			(0x28U)	//!< Defines the conductance of the p-driver output during periods of no modulation
#define RC522_MOD_GS_P_REG			(0x29U)	//!< Defines the conductance of the p-driver output during periods of modulation
#define RC522_T_MODE_REG			(0x2AU)	//!< Defines settings for the internal timer
#define RC522_T_PRESCALER_REG		(0x2BU)	//!< The lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
#define RC522_T_RELOAD_REG_H		(0x2CU)	//!< Defines the 16-bit timer reload value
#define RC522_T_RELOAD_REG_L		(0x2DU)
#define RC522_T_COUNTER_VALUE_REG_H	(0x2EU)	//!< Shows the 16-bit timer value
#define RC522_T_COUNTER_VALUE_REG_L	(0x2FU)

////////////////////////////
// Page 3: Test Registers //
////////////////////////////
#define RC522_TEST_SEL1_REG			(0x31U)	//!< General test signal configuration
#define RC522_TEST_SEL2_REG			(0x32U)	//!< General test signal configuration
#define RC522_TEST_PIN_ENABLE_REG	(0x33U)	//!< Enables pin output driver on pins D1 to D7
#define RC522_TEST_PIN_VALUE_REG	(0x34U)	//!< Defines the values for D1 to D7 when it is used as an I/O bus
#define RC522_TEST_BUS_REG			(0x35U)	//!< Shows the status of the internal test bus
#define RC522_AUTO_TEST_REG			(0x36U)	//!< Controls the digital self-test
#define RC522_VERSION_REG			(0x37U)	//!< Shows the software version
#define RC522_ANALOG_TEST_REG		(0x38U)	//!< Controls the pins AUX1 and AUX2
#define RC522_TEST_DAC1_REG			(0x39U)	//!< Defines the test value for TestDAC1
#define RC522_TEST_DAC2_REG			(0x3AU)	//!< Defines the test value for TestDAC2
#define RC522_TEST_ADC_REG			(0x3BU)	//!< Shows the value of ADC I and Q channels

#define RFID_MF_ACK 		10	//!< The MIFARE Classic uses a 4 bit ACK/NAK. Any other value than 10 is NAK.
#define RFID_MF_KEY_SIZE	6	//!< A Mifare Crypto1 key is 6 bytes.

//! brief <b>RFID_PCD_Command_t</b> enumeration is associated with the MFRC522 commands.
typedef enum {
	PCD_IDLE				= 0x00,	//!< No action, cancels current command execution
	PCD_MEMORY				= 0x01,	//!< Stores 25 bytes into the internal buffer
	PCD_GENERATE_RANDOM_ID	= 0x02,	//!< Generates a 10-byte random ID number
	PCD_CALCULATE_CRC		= 0x03,	//!< Activates the CRC coprocessor or performs a self-test
	PCD_TRANSMIT			= 0x04,	//!< Transmits data from the FIFO buffer
	PCD_NO_COMMAND_CHANGE	= 0x07,	//!< No command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
	PCD_RECEIVE				= 0x08,	//!< Activates the receiver circuits
	PCD_TRANSCEIVE 			= 0x0C,	//!< Transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
	PCD_MF_AUTHENT 			= 0x0E,	//!< Performs the MIFARE standard authentication as a reader
	PCD_SOFTWARE_RESET		= 0x0F	//!< Resets the MFRC522
} RFID_PCD_Command_t;

//! brief <b>RFID_PICC_Command_t</b> enumeration is associated with the commands sent to the PICC.
typedef enum {
	// The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
	PICC_CMD_REQA			= 0x26,	//!< REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
	PICC_CMD_WUPA			= 0x52,	//!< Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
	PICC_CMD_CT				= 0x88,	//!< Cascade Tag. Not really a command, but used during anti collision.
	PICC_CMD_SEL_CL1		= 0x93,	//!< Anti collision/Select, Cascade Level 1
	PICC_CMD_SEL_CL2		= 0x95,	//!< Anti collision/Select, Cascade Level 2
	PICC_CMD_SEL_CL3		= 0x97,	//!< Anti collision/Select, Cascade Level 3
	PICC_CMD_HLTA			= 0x50,	//!< HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
	PICC_CMD_RATS           = 0xE0,	//!< Request command for Answer To Reset.
	// The commands used for MIFARE Classic (from http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9)
	// Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
	// The read/write commands can also be used for MIFARE Ultralight.
	PICC_CMD_MF_AUTH_KEY_A	= 0x60,	//!< Perform authentication with Key A
	PICC_CMD_MF_AUTH_KEY_B	= 0x61,	//!< Perform authentication with Key B
	PICC_CMD_MF_READ		= 0x30,	//!< Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
	PICC_CMD_MF_WRITE		= 0xA0,	//!< Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
	PICC_CMD_MF_DECREMENT	= 0xC0,	//!< Decrements the contents of a block and stores the result in the internal data register.
	PICC_CMD_MF_INCREMENT	= 0xC1,	//!< Increments the contents of a block and stores the result in the internal data register.
	PICC_CMD_MF_RESTORE		= 0xC2,	//!< Reads the contents of a block into the internal data register.
	PICC_CMD_MF_TRANSFER	= 0xB0,	//!< Writes the contents of the internal data register to a block.
	// The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
	// The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
	PICC_CMD_UL_WRITE		= 0xA2	//!< Writes one 4 byte page to the PICC.
} RFID_PICC_Command_t;

//! @brief <b>RFID_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the RFID reader operations can be found.
typedef enum {
	RFID_OK,				//!< Successful operation.
	RFID_UPDATE_ERR,		//!< No valid data has been acquired or transmitted via SPI communication.
	RFID_SSEL_ERR,			//!< Error binding or unbinding SSEL GPIO.
	RFID_COLLISION_ERR,		//!< Collission detected.
	RFID_TIMEOUT_ERR,		//!< Timeout in communication.
	RFID_NO_ROOM_ERR,		//!< A buffer is not big enough.
	RFID_CRC_WRONG,			//!< The CRC (<tt>Cyclic Redundancy Check</tt>) does not match.
	RFID_MIFARE_NACK = 0xFF	//!< A MIFARE PICC responded with NAK.
} RFID_result_t;

class MFRC522 : protected SPI {
private:
	const Gpio m_SSEL;
	uint8_t m_slaveSelected;
	RFID_result_t m_statusRFID;
public:
	// MIFARE_st: Structured used for passing a MIFARE Crypto1 key.
	struct MIFARE_st { uint8_t keyByte[RFID_MF_KEY_SIZE]; };

	MFRC522(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL);
	void send(const char *message); // WARNING: DEBUG METHOD!!
	RFID_result_t enable(void);
	RFID_result_t disable(void);
	RFID_result_t getStatus(void) const;
	virtual ~MFRC522();
};

extern MFRC522 *g_rfid;

void initRFID(void);

#endif /* MFRC522_H_ */
