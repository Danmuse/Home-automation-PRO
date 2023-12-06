/*!
 * @file SyncCommSPI.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 * @brief Synchronous Communication SPI Abstract Class API (Application Programming Interface).
 */

#ifndef SYNC_COMM_SPI_H_
#define SYNC_COMM_SPI_H_

#include "utils.h"

class SyncCommSPI {
public:
	virtual void SPI_IRQHandler(void) = 0;
	virtual ~SyncCommSPI() = default;
protected:
	virtual void sendData(uint32_t address, const uint8_t value) = 0;
	virtual void readData(uint32_t address, const uint8_t *value) = 0;
};

#endif /* SYNC_COMM_SPI_H_ */
