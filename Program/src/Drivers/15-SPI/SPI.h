/*!
 * @file SPI.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 * @brief Serial Peripheral Interface Class API (Application Programming Interface).
 */

#ifndef SPI_H_
#define SPI_H_

#include <vector>
#include "SyncComm.h"
#include "GPIO.h"

class SPI : protected std::vector<Gpio>, public SyncComm {
public:
	SPI();
	virtual ~SPI();
};

#endif /* SPI_H_ */
