/*/*!
 * @file MFRC522.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 22/18/2023 22:18:47
 */

#include "MFRC522.h"

MFRC522 *g_rfid = nullptr;

MFRC522::MFRC522(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL) : SPI(SCK, MOSI, MISO, std::vector<Gpio>({SSEL})) { }

MFRC522::~MFRC522() { }

//////////////////////////////
/// MFRC522 initialization ///
//////////////////////////////

void initRFID(void) {
	#if defined(SPI0_PINS)

	static MFRC522 rfid(SPI0_SCK, SPI0_MISO, SPI0_MOSI, SSEL0_PIN, 0);

	g_ds3231 = &rfid;

	#endif // defined(SPI0_PINS)
}

