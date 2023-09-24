/*!
 * @file SyncComm.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 * @brief Synchronous Communication Class API (Application Programming Interface).
 */

#ifndef SYNC_COMM_H_
#define SYNC_COMM_H_

#include "utils.h"

class SyncComm {
public:
	SyncComm();
	virtual void SendData(uint32_t address, const uint8_t value) = 0;
	virtual void ReadData(uint32_t address, const uint8_t *value) = 0;
	virtual void I2C_IRQHandler(void) = 0;
	virtual void SPI_IRQHandler(void) = 0;
	virtual ~SyncComm();
};

#endif /* SYNC_COMM_H_ */
