/*/*!
 * @file DFPlayerMini.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/01/2024 13:51:53
 */

#include "DFPlayerMini.h"

DFPlayer *g_dfplayer = nullptr;

DFPlayer::DFPlayer(const Gpio& RX, const Gpio& TX, channelUART_t channel) : UART(RX, TX, channel) {
	this->begin();
}

void DFPlayer::enableACK(void) {
  this->m_sending[Stack_ACK] = 0x01;
}

void DFPlayer::disableACK(void) {
  this->m_sending[Stack_ACK] = 0x00;
}

/*
bool DFPlayer::waitAvailable(uint32_t duration) {
  unsigned long timer = millis();
  if (!duration) duration = this->m_timeOutDuration;
  while (!(this->available())){
    if (millis() - timer > duration) {
      return this->handleError(TimeOut);
    }
//    delay(0);
  }
  return true;
}
*/

bool DFPlayer::begin(bool isACK, bool doReset) {
	isACK ? this->enableACK() : this->disableACK();

	if (doReset) {
//		this->reset();
//		this->waitAvailable(2000);
		delay(200);
	} else this->m_handleType = DFPlayerCardOnline; // Assume same state as with reset(): online

	return (this->readType() == DFPlayerCardOnline) || (this->readType() == DFPlayerUSBOnline) || !isACK;
}

uint8_t DFPlayer::readType(void) {
  this->m_isAvailable = false;
  return this->m_handleType;
}

uint16_t DFPlayer::read(void) {
	this->m_isAvailable = false;
  return this->m_handleParameter;
}

///////////////////////////////////
/// DFPlayerMini initialization ///
///////////////////////////////////

void initDFPlayer(void) {
	#ifdef CN13_PINS

	static DFPlayer dfplayer(RX0_IN, TX0_OUT);

	g_dfplayer = &dfplayer;

	#endif // UART1_PINS
}
