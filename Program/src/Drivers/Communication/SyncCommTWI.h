/*!
 * @file SyncCommTWI.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 * @brief Synchronous Communication TWI Abstract Class API (Application Programming Interface).
 */

#ifndef SYNC_COMM_TWI_H_
#define SYNC_COMM_TWI_H_

#include "utils.h"

#define MAX_ATTEMPTS_TWI 10
#define TWI_OFFSET_NONE 0

class SyncCommTWI {
public:
	// TIME_OUT_FAULT: Exceeds the attempts to connect with the slave device.
	// NACK_ADDR_FAULT: Negative acknowledgement Address. Slave NACKed address.
	// NACK_DATA_FAULT: Negative acknowledgement Data. Slave NACKed transmitted data.
	// STRETCH_FAULT: The slave function is not currently stretching the I2C bus clock.
	enum statusComm_t { TWI_SUCCESS, TWI_FAILURE, TIME_OUT_FAULT, NACK_ADDR_FAULT, NACK_DATA_FAULT, STRETCH_FAULT, UNREQUITED_ACTION };
	// READ_OFFSET_NONE: Avoid sending the positioning register data address when it's reading.
	// WRITE_OFFSET_NONE: Avoid sending the positioning register data address when it's writing.
	enum actionComm_t { READ, WRITE, READ_OFFSET_NONE, WRITE_OFFSET_NONE };
	// SLOW_FREQUENCY: Clock period at 100 Kb/s.
	// STD_FREQUENCY: Clock period at 400 Kb/s.
	// FAST_FREQUENCY: Clock period at 1 Kb/s.
	enum frequencyComm_t { SLOW_FREQUENCY = 100000, STD_FREQUENCY = 400000, FAST_FREQUENCY = 1000000 };
private:
	virtual statusComm_t prepareConditions(const uint8_t address, const uint8_t regOffset, actionComm_t action) = 0;
	virtual statusComm_t transmitStopBit(void) = 0;
protected:
	virtual statusComm_t transmitByte(const uint8_t address, const uint8_t regOffset, uint8_t value, actionComm_t action = WRITE) = 0;
	virtual statusComm_t receiveByte(const uint8_t address, const uint8_t regOffset, uint8_t* value, actionComm_t action = READ) = 0;
	virtual statusComm_t transmitBytes(const uint8_t address, const uint8_t regOffset, uint8_t values[], size_t numBytes, actionComm_t action = WRITE) = 0;
	virtual statusComm_t receiveBytes(const uint8_t address, const uint8_t regOffset, uint8_t values[], size_t numBytes, actionComm_t action = READ) = 0;
public:
	virtual void I2C_IRQHandler(void) = 0;
	virtual ~SyncCommTWI() = default;
};

#endif /* SYNC_COMM_TWI_H_ */
