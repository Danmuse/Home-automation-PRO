/*/*!
 * @file DFPlayerMini.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/01/2024 13:51:53
 */

#include "DFPlayerMini.h"

DFPlayer *g_dfplayer = nullptr;

//#pragma GCC push_options
//#pragma GCC optimize ("O0")

DFPlayer::DFPlayer(const Gpio& RX, const Gpio& TX, channelUART_t channel) : UART(RX, TX, channel), Callback(),
m_timeOutTimer{(uint16_t)(0 * (g_systick_freq / 1000))}, m_receivedIndex{0}, m_isAvailable{false}, m_isSending{false} {
    g_callback_list.push_back(this);
	if (!(this->begin())) { /* TODO: Make something */ }
}

void DFPlayer::uint16ToArray(uint16_t value, uint8_t *array) {
	*array = (uint8_t)(value >> 8);
	*(array + 1) = (uint8_t)(value);
}

uint16_t DFPlayer::arrayToUint16(uint8_t *array) {
	uint16_t value = *array;
	value <<= 8;
	value += *(array + 1);
	return value;
}

uint16_t DFPlayer::calculateCheckSum(uint8_t *buffer) {
	uint16_t sum = 0;
	for (size_t index = Stack_Version; index < Stack_CheckSum; index++) sum += buffer[index];
	return -sum;
}

void DFPlayer::parseStack(void) {
	uint8_t handleCommand = *(this->m_received + Stack_Command);
	if (handleCommand == 0x41) { // Handle the 0x41 ACK feedback as a special case, in case the pollusion of _handleCommand, _handleParameter, and _handleType.
		this->m_isSending = false;
		return;
	}

	this->m_handleCommand = handleCommand;
	this->m_handleParameter = this->arrayToUint16(this->m_received + Stack_Parameter);

	switch (this->m_handleCommand) {
		case 0x3C:
		case 0x3D:
			this->handleMessage(DFPlayerPlayFinished, this->m_handleParameter);
			break;
		case 0x3F:
			if (this->m_handleParameter & 0x01) this->handleMessage(DFPlayerUSBOnline, this->m_handleParameter);
			else if (this->m_handleParameter & 0x02) this->handleMessage(DFPlayerCardOnline, this->m_handleParameter);
			else if (this->m_handleParameter & 0x03) this->handleMessage(DFPlayerCardUSBOnline, this->m_handleParameter);
			break;
		case 0x3A:
			if (this->m_handleParameter & 0x01) this->handleMessage(DFPlayerUSBInserted, this->m_handleParameter);
			else if (this->m_handleParameter & 0x02) this->handleMessage(DFPlayerCardInserted, this->m_handleParameter);
			break;
		case 0x3B:
			if (this->m_handleParameter & 0x01) this->handleMessage(DFPlayerUSBRemoved, this->m_handleParameter);
			else if (this->m_handleParameter & 0x02) this->handleMessage(DFPlayerCardRemoved, this->m_handleParameter);
			break;
		case 0x40:
			this->handleMessage(DFPlayerError, this->m_handleParameter);
			break;
		case 0x3E:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
		case 0x48:
		case 0x49:
		case 0x4B:
		case 0x4C:
		case 0x4D:
		case 0x4E:
		case 0x4F:
			this->handleMessage(DFPlayerFeedBack, this->m_handleParameter);
			break;
		default:
			this->handleError(WrongStack);
			break;
	}
}

bool DFPlayer::validateStack(void) {
	return this->calculateCheckSum(this->m_received) == this->arrayToUint16(this->m_received + Stack_CheckSum);
}

void DFPlayer::sendStack(void) {
	// If the ACK mode is on wait until the last transmition
	if (this->m_sending[Stack_ACK]) while (this->m_isSending) this->waitAvailable();
	this->transmit(reinterpret_cast<const char*>(this->m_sending), DFPLAYER_SEND_LENGTH);
	this->m_isSending = this->m_sending[Stack_ACK];
	// If the ACK mode is off wait 10 ms after one transmition
	if (!(this->m_sending[Stack_ACK])) delay(10);
}

void DFPlayer::sendStack(uint8_t command) {
	this->sendStack(command, 0);
}

void DFPlayer::sendStack(uint8_t command, uint16_t argument) {
	this->m_sending[Stack_Command] = command;
	this->uint16ToArray(argument, this->m_sending + Stack_Parameter);
	this->uint16ToArray(calculateCheckSum(this->m_sending), this->m_sending + Stack_CheckSum);
	this->sendStack();
}

void DFPlayer::sendStack(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow) {
	uint16_t buffer = argumentHigh;
	buffer <<= 8;
	this->sendStack(command, buffer | argumentLow);
}

void DFPlayer::enableACK(void) {
	this->m_sending[Stack_ACK] = 0x01;
}

void DFPlayer::disableACK(void) {
	this->m_sending[Stack_ACK] = 0x00;
}

bool DFPlayer::available(void) {
	char bufferReceived[DFPLAYER_RECEIVED_LENGTH];
	if (this->receive(bufferReceived, DFPLAYER_RECEIVED_LENGTH - 1)) {
		while (this->m_receivedIndex < DFPLAYER_RECEIVED_LENGTH) {
			if (this->m_receivedIndex == 0) {
				this->m_received[Stack_Header] = bufferReceived[Stack_Header];
				if (this->m_received[Stack_Header] == 0x7E) this->m_receivedIndex++;
				else { if (!(this->receive(bufferReceived, DFPLAYER_RECEIVED_LENGTH))) break; }
			} else {
				this->m_received[this->m_receivedIndex] = bufferReceived[this->m_receivedIndex];
				switch (this->m_receivedIndex) {
					case Stack_Version:
						if (this->m_received[this->m_receivedIndex] != 0xFF) return this->handleError(WrongStack);
						break;
					case Stack_Length:
						if (this->m_received[this->m_receivedIndex] != 0x06) return this->handleError(WrongStack);
						break;
					case Stack_End:
						if (this->m_received[this->m_receivedIndex] != 0xEF) return this->handleError(WrongStack);
						else {
							if (this->validateStack()) {
								this->parseStack();
								return this->m_isAvailable;
							} else return this->handleError(WrongStack);
						}
						break;
					default:
						break;
				}
				this->m_receivedIndex++;
			}
		}
	}
	this->m_receivedIndex = 0;
	return this->m_isAvailable;
}

bool DFPlayer::waitAvailable(uint32_t duration) {
	static bool busyStatus = false;
	if (!(this->m_timeOutTimer) && !busyStatus) {
		this->m_timeOutTimer = duration ? (uint16_t)(duration * (g_systick_freq / 1000)) : (uint16_t)(DFPLAYER_TIMEOUT_TICKS * (g_systick_freq / 1000));
		busyStatus = true;
	}
	while (!(this->available())) {
		if (!(this->m_timeOutTimer)) {
			busyStatus = false;
			return this->handleError(TimeOut);
		}
	}
	return true;
}

bool DFPlayer::begin(bool isACK, bool doReset) {
	isACK ? this->enableACK() : this->disableACK();
	if (doReset) {
		this->reset();
		this->waitAvailable(2000);
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

bool DFPlayer::handleMessage(uint8_t type, uint16_t parameter) {
	this->m_receivedIndex = 0;
	this->m_handleType = type;
	this->m_handleParameter = parameter;
	this->m_isAvailable = true;
	return this->m_isAvailable;
}

bool DFPlayer::handleError(uint8_t type, uint16_t parameter) {
	this->handleMessage(type, parameter);
//	this->m_isAvailable = false; // TODO: Check this code instruction
	this->m_isSending = false;
	return false;
}

void DFPlayer::next(void) {
	this->sendStack(0x01);
}

void DFPlayer::previous(void) {
	this->sendStack(0x02);
}

void DFPlayer::play(uint8_t fileNumber) {
	this->sendStack(0x03, fileNumber);
}

void DFPlayer::volumeUp(void) {
	this->sendStack(0x04);
}

void DFPlayer::volumeDown(void) {
	this->sendStack(0x05);
}

void DFPlayer::volume(uint8_t volume) {
	uint8_t result = volume > 100 ? 100 : volume;
	result /= (100 / (float)(DFPLAYER_MAX_VOLUME_VALUE));
	this->sendStack(0x06, result);
}

void DFPlayer::equalizer(equalizer_t equalizer) {
	this->sendStack(0x07, (uint16_t)(equalizer));
}

void DFPlayer::loop(uint8_t fileNumber) {
	this->sendStack(0x08, fileNumber);
}

void DFPlayer::sleep(void) {
	this->sendStack(0x0A);
}

void DFPlayer::reset(void) {
	this->sendStack(0x0C);
}

void DFPlayer::start(void) {
	this->sendStack(0x0D);
}

void DFPlayer::pause(void) {
	this->sendStack(0x0E);
}

void DFPlayer::enableLoopAll(void) {
	this->sendStack(0x11, 0x01);
}

void DFPlayer::disableLoopAll(void) {
	this->sendStack(0x11, 0x00);
}

void DFPlayer::enableLoop(void) {
	this->sendStack(0x19, 0x00);
}

void DFPlayer::disableLoop(void) {
	this->sendStack(0x19, 0x01);
}

//#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("O1")

void DFPlayer::callbackMethod(void) {
	if (this->m_timeOutTimer) this->m_timeOutTimer--;
}

#pragma GCC pop_options

///////////////////////////////////
/// DFPlayerMini initialization ///
///////////////////////////////////

void initDFPlayer(void) {
	#ifdef CN13_PINS

	static DFPlayer dfplayer(RX0_IN, TX0_OUT);

	g_dfplayer = &dfplayer;

	#endif // CN13_PINS
}
