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
#define I2C_STAT_MSTSTATE_NACK_ADDR_MASK (0x3U << I2C_STAT_MSTSTATE_SHIFT)
#define I2C_STAT_MSTSTATE_NACK_DATA_MASK (0x4U << I2C_STAT_MSTSTATE_SHIFT)

class SyncCommTWI {
public:
	// ACK_FAULT: Exceeds the attempts to connect with the slave device.
	// NACK_ADDR_FAULT: Negative acknowledgement Address. Slave NACKed address.
	// NACK_DATA_FAULT: Negative acknowledgement Data. Slave NACKed transmitted data.
	// STRETCH_FAULT: The slave function is not currently stretching the I2C bus clock.
	enum statusComm_t { TWI_SUCCESS, TWI_FAILURE, ACK_FAULT, NACK_ADDR_FAULT, NACK_DATA_FAULT, STRETCH_FAULT };
	enum actionComm_t { READ, WRITE };

	virtual void I2C_IRQHandler(void) = 0;
	virtual ~SyncCommTWI() = default;
protected:
	virtual statusComm_t SendByte(uint32_t address, const uint8_t value) = 0;
	virtual statusComm_t ReadByte(uint32_t address, uint8_t *value) = 0;
	virtual statusComm_t SendBytes(uint32_t address, const uint8_t *value) = 0;
	virtual statusComm_t ReadBytes(uint32_t address, uint8_t *value) = 0;
};

#endif /* SYNC_COMM_TWI_H_ */
