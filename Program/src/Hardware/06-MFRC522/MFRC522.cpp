/*/*!
 * @file MFRC522.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 22/18/2023 22:18:47
 */

#include "MFRC522.h"

/****************** RFID Testing START ******************
int main(void) {
	initDevice();
	initDisplay();
	initRFID();

	while (1) {
		if (g_rfid->getStatus()) return EXIT_FAILURE;
		g_timers_list.timerEvents();
		delay(1000);
	}
}
******************** RFID Testing END *******************/

MFRC522 *g_rfid = nullptr;

MFRC522::MFRC522(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL) : SPI(SCK, MOSI, MISO),
m_SSEL{SSEL}, m_statusRFID{RFID_OK} {
//	this->enable();
}

void MFRC522::send(const char *message) {
//	if (!(this->m_statusRFID)) this->enableSSEL(this->m_slaveSelected);
	this->enable();
	this->transmit(message);
	this->disable();
//	if (!(this->m_statusRFID)) this->disableSSEL(this->m_slaveSelected);
}

RFID_result_t MFRC522::enable(void) {
	if (!(this->bindSSEL(this->m_SSEL, this->m_slaveSelected))) this->m_statusRFID = RFID_SSEL_ERR;
	if (!(this->m_statusRFID)) this->enableSSEL(this->m_slaveSelected);
	return this->getStatus();
}

RFID_result_t MFRC522::disable(void) {
	if (!(this->unbindSSEL(this->m_slaveSelected))) this->m_statusRFID = RFID_SSEL_ERR;
	if (!(this->m_statusRFID)) this->disableSSEL(this->m_slaveSelected);
	return this->getStatus();
}

RFID_result_t MFRC522::getStatus(void) const {
	return this->m_statusRFID;
}

MFRC522::~MFRC522() {
	this->disable(); // According to the SPI implementation, the unbindSSEL function must be executed in the primitive classes.
}

//////////////////////////////
/// MFRC522 initialization ///
//////////////////////////////

void initRFID(void) {
	#if defined(SPI_DEBUG_PINS)

	static MFRC522 rfid(SPI_DEBUG_SCK, SPI_DEBUG_MOSI, SPI_DEBUG_MISO, SPI_DEBUG_SSEL0);

	g_rfid = &rfid;

	#endif // defined(SPI_DEBUG_PINS)
}

