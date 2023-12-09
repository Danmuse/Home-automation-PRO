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

class MFRC522 : protected SPI {
public:
	MFRC522(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL);
	virtual ~MFRC522();
};

extern MFRC522 *g_rfid;

void initRFID(void);

#endif /* MFRC522_H_ */
