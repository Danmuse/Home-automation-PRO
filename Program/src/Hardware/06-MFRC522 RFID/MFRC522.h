/*!
 * @file MFRC522.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 22/18/2023 22:18:47
 * @brief RFID Reader MFRC522 Class API (Application Programming Interface).
 */

#ifndef MFRC522_H_
#define MFRC522_H_

#include "systick.h"
#include "SPI.h"
#include "utilities.h"

//////////////////////////////////////////
// Page 0: Command and Status Registers //
//////////////////////////////////////////
#define RC522_COMMAND_REG			(0x01U << 1)	//!< Starts and stops command execution
#define RC522_COM_IRQ_ENABLE_REG	(0x02U << 1)	//!< Enable and disable interrupt request control bits
#define RC522_DIV_IRQ_ENABLE_REG	(0x03U << 1)	//!< Enable and disable interrupt request control bits
#define RC522_COM_IRQ_REG			(0x04U << 1)	//!< Interrupt request bits
#define RC522_DIV_IRQ_REG			(0x05U << 1)	//!< Interrupt request bits
#define RC522_ERROR_REG				(0x06U << 1)	//!< Error bits showing the error status of the last command executed
#define RC522_STATUS1_REG			(0x07U << 1)	//!< Communication status bits
#define RC522_STATUS2_REG			(0x08U << 1)	//!< Receiver and transmitter status bits
#define RC522_FIFO_DATA_REG			(0x09U << 1)	//!< Input and output of 64 byte FIFO buffer
#define RC522_FIFO_LEVEL_REG		(0x0AU << 1)	//!< Number of bytes stored in the FIFO buffer
#define RC522_WATER_LEVEL_REG		(0x0BU << 1)	//!< Level for FIFO underflow and overflow warning
#define RC522_CONTROL_REG			(0x0CU << 1)	//!< Miscellaneous control registers
#define RC522_BIT_FRAMING_REG		(0x0DU << 1)	//!< Adjustments for bit-oriented frames
#define RC522_COLL_REG				(0x0EU << 1)	//!< Bit position of the first bit-collision detected on the RF interface

#define RC522_IdleIRq_MASK	(0x10U)	//!< CommandReg changes its value from any command to the Idle command
#define RC522_RxIRq_MASK	(0x20U)	//!< Receiver has detected the end of a valid data stream

///////////////////////////////
// Page 1: Command Registers //
///////////////////////////////
#define RC522_MODE_REG			(0x11U << 1)	//!< Defines general modes for transmitting and receiving
#define RC522_TX_MODE_REG		(0x12U << 1)	//!< Defines transmission data rate and framing
#define RC522_RX_MODE_REG		(0x13U << 1)	//!< Defines reception data rate and framing
#define RC522_TX_CONTROL_REG	(0x14U << 1)	//!< Controls the logical behavior of the antenna driver pins TX1 and TX2
#define RC522_TX_ASK_REG		(0x15U << 1)	//!< Controls the setting of the transmission modulation
#define RC522_TX_SEL_REG		(0x16U << 1)	//!< Selects the internal sources for the antenna driver
#define RC522_RX_SEL_REG		(0x17U << 1)	//!< Selects internal receiver settings
#define RC522_RX_THRESHOLD_REG	(0x18U << 1)	//!< Selects thresholds for the bit decoder
#define RC522_DEMOD_REG			(0x19U << 1)	//!< Defines demodulator settings
#define RC522_MF_TX_REG			(0x1CU << 1)	//!< Controls some MIFARE communication transmit parameters
#define RC522_MF_RX_REG			(0x1DU << 1)	//!< Controls some MIFARE communication receive parameters
#define RC522_SERIAL_SPEED_REG	(0x1FU << 1)	//!< Selects the speed of the serial UART interface

/////////////////////////////////////
// Page 2: Configuration Registers //
/////////////////////////////////////
#define RC522_CRC_RESULT_REG_H		(0x21U << 1)	//!< Shows the MSB and LSB values of the CRC calculation
#define RC522_CRC_RESULT_REG_L		(0x22U << 1)
#define RC522_MOD_WIDTH_REG			(0x24U << 1)	//!< Controls the ModWidth setting
#define RC522_RF_CFG_REG			(0x26U << 1)	//!< Configures the receiver gain
#define RC522_GS_N_REG				(0x27U << 1)	//!< Selects the conductance of the antenna driver pins TX1 and TX2 for modulation
#define RC522_CW_GS_P_REG			(0x28U << 1)	//!< Defines the conductance of the p-driver output during periods of no modulation
#define RC522_MOD_GS_P_REG			(0x29U << 1)	//!< Defines the conductance of the p-driver output during periods of modulation
#define RC522_T_MODE_REG			(0x2AU << 1)	//!< Defines settings for the internal timer
#define RC522_T_PRESCALER_REG		(0x2BU << 1)	//!< The lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
#define RC522_T_RELOAD_REG_H		(0x2CU << 1)	//!< Defines the 16-bit timer reload value
#define RC522_T_RELOAD_REG_L		(0x2DU << 1)
#define RC522_T_COUNTER_VALUE_REG_H	(0x2EU << 1)	//!< Shows the 16-bit timer value
#define RC522_T_COUNTER_VALUE_REG_L	(0x2FU << 1)

////////////////////////////
// Page 3: Test Registers //
////////////////////////////
#define RC522_TEST_SEL1_REG			(0x31U << 1)	//!< General test signal configuration
#define RC522_TEST_SEL2_REG			(0x32U << 1)	//!< General test signal configuration
#define RC522_TEST_PIN_ENABLE_REG	(0x33U << 1)	//!< Enables pin output driver on pins D1 to D7
#define RC522_TEST_PIN_VALUE_REG	(0x34U << 1)	//!< Defines the values for D1 to D7 when it is used as an I/O bus
#define RC522_TEST_BUS_REG			(0x35U << 1)	//!< Shows the status of the internal test bus
#define RC522_AUTO_TEST_REG			(0x36U << 1)	//!< Controls the digital self-test
#define RC522_VERSION_REG			(0x37U << 1)	//!< Shows the software version
#define RC522_ANALOG_TEST_REG		(0x38U << 1)	//!< Controls the pins AUX1 and AUX2
#define RC522_TEST_DAC1_REG			(0x39U << 1)	//!< Defines the test value for TestDAC1
#define RC522_TEST_DAC2_REG			(0x3AU << 1)	//!< Defines the test value for TestDAC2
#define RC522_TEST_ADC_REG			(0x3BU << 1)	//!< Shows the value of ADC I and Q channels

#define RFID_WAIT_IRQ	(RC522_IdleIRq_MASK | RC522_RxIRq_MASK)	// Interrupts of the CommandReg register used.

#define RFID_MF_ACK 		10	//!< The MIFARE Classic uses a 4 bit ACK/NAK. Any other value than 10 is NAK.
#define RFID_MF_KEY_SIZE	6	//!< A MIFARE Crypto1 key is 6 bytes.

#define MAXIMUM_UID_SIZE	10	//!< Maximum number of bytes in the UID.

//! @hideinitializer Number of characters to store the CRC (<tt>Cyclic Redundancy Check</tt>) captured by the PCD (<tt>Proximity Coupling Device</tt>).
#define RFID_CRC_SIZE	2	//!< <pre><strong>Value:</strong> 2

//! @hideinitializer Number of characters to store the UID captured by the PCD (<tt>Proximity Coupling Device</tt>).
#define RFID_STR_SIZE	18	//!< <pre><strong>Value:</strong> 18

//! @brief <b>RFID_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the RFID reader operations can be found.
typedef enum {
	RFID_OK,				//!< Successful operation.
	RFID_UPDATE_ERR,		//!< No valid data has been acquired or transmitted via SPI communication.
	RFID_SSEL_ERR,			//!< Error binding or unbinding SSEL GPIO.
	RFID_COLLISION_ERR,		//!< Collission detected.
	RFID_TIMEOUT_ERR,		//!< Timeout in communication.
	RFID_NO_ROOM_ERR,		//!< A buffer is not big enough.
	RFID_INTERNAL_ERR,		//!< Internal error in the program.
	RFID_INVALID,			//!< Indicates about an invalid argument.
	RFID_CRC_WRONG,			//!< The CRC (<tt>Cyclic Redundancy Check</tt>) does not match.
	RFID_MIFARE_NACK = 0xFF	//!< A MIFARE PICC responded with NAK.
} RFID_result_t;

//! @brief <b>RFID_status_t</b> enumeration indicates the status of the connection between the PICC (<tt>Proximity Integrated Circuit Card</tt>) and PCD (<tt>Proximity Coupling Device</tt>).
typedef enum {
	RFID_SUCCESS,	//!< Successful operation.
	RFID_FAILURE,	//!< Failure operation.
	RFID_BUSY		//!< Pending status of carrying out operations.
} RFID_status_t;

class MFRC522 : protected SPI, Gpio, Callback {
public:
	//! MIFARE_st: Structure used for passing a MIFARE Crypto1 key.
	typedef struct { uint8_t keyByte[RFID_MF_KEY_SIZE]; } MIFARE_st;
	//! UID_st: Structure used for passing the UID of a PICC.
	//! - The SAK (<tt>Select acknowledge</tt>) byte returned from the PICC after successful selection.
	typedef struct { uint8_t size; uint8_t uidByte[MAXIMUM_UID_SIZE]; uint8_t sak; } UID_st;

	//! @brief <b>RFID_PCD_Operation_t</b> enumeration is associated with the MFRC522 operations executed by the PCD (<tt>Proximity Coupling Device</tt>).
	typedef enum {
		COMMUNICATION_PICC,
		CALCULATE_CRC
	} RFID_PCD_Operation_t;

	//! @brief <b>RFID_PCD_Command_t</b> enumeration is associated with the MFRC522 commands sent to the PCD (<tt>Proximity Coupling Device</tt>).
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

	//! @brief <b>RFID_PICC_Command_t</b> enumeration is associated with the MFRC522 commands sent to the PICC (<tt>Proximity Integrated Circuit Card</tt>).
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
private:
	const Gpio m_SSEL;
	uint8_t m_slaveSelected;
	RFID_result_t m_statusRFID;
	uint8_t m_timeOut;
	RFID_PCD_Operation_t m_operation;
	bool m_operationCompleted;
	UID_st m_UID;

	void writeRegisterPCD(uint8_t reg, uint8_t value);
	void writeRegisterPCD(uint8_t reg, uint8_t count, uint8_t *values);
	uint8_t readRegisterPCD(uint8_t reg);
	void readRegisterPCD(uint8_t reg, uint8_t count, uint8_t *values, uint8_t rxAlign = 0);
	void setRegisterBitMaskPCD(uint8_t reg, uint8_t mask);
	void clearRegisterBitMaskPCD(uint8_t reg, uint8_t mask);

	RFID_result_t PCD_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result);
	RFID_result_t PCD_CommunicateWithPICC(uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen, uint8_t *backData = nullptr, uint8_t *backLen = nullptr, uint8_t *validBits = nullptr, uint8_t rxAlign = 0, bool checkCRC = false);
	RFID_result_t PICC_REQA(uint8_t *bufferATQA, uint8_t *bufferSize);
	virtual RFID_result_t PICC_Select(uint8_t validBits = 0);
protected:
	void callbackMethod(void) override;
public:
	MFRC522(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL, const Gpio& hardRST);

	RFID_result_t enable(void);
	RFID_result_t disable(void);
	RFID_result_t getStatus(void) const;
	void initPCD(void);
	void resetPCD(void);
	void antennaOnPCD(void);
	void antennaOffPCD(void);

	virtual bool isCardPICC(void);
	virtual bool readCardPICC(void);
	RFID_result_t haltPICC(void);
	RFID_status_t getUID(UID_st *UID = nullptr);

	// Support functions for debugging
	void dumpVersion(void);
	void dumpUID(UID_st* UID);
	void dumpDetails(UID_st* UID);
	char* printUID(void);

	virtual ~MFRC522();
};

extern MFRC522 *g_rfid;

void initRFID(void);

#endif /* MFRC522_H_ */
