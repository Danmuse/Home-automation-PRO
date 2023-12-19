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

//! @brief <b>RFID_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the RFID reader operations can be found.
typedef enum {
	RFID_OK,			//!< Successful operation.
	RFID_UPDATE_ERR,	//!< No valid data has been acquired or transmitted via SPI communication.
	RFID_SSEL_ERR		//!< Error binding or unbinding SSEL GPIO.
} RFID_result_t;

class MFRC522 : protected SPI {
private:
	const Gpio m_SSEL;
	uint8_t m_slaveSelected;
	RFID_result_t m_statusRFID;
public:
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
