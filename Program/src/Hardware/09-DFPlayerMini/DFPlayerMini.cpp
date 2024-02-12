/*/*!
 * @file DFPlayerMini.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/01/2024 13:51:53
 */

#include "DFPlayerMini.h"

DFPlayer *g_dfplayer = nullptr;

DFPlayer::DFPlayer(const Gpio& busyPin, const Gpio& RX, const Gpio& TX, channelUART_t channel) : Gpio(busyPin), UART(RX, TX, channel), Callback(),
m_pauseState{false}, m_backupFile{1}, m_volume{100}, m_timeOutTimer{(uint16_t)(0 * (g_systick_freq / 1000))}, m_statusDFPlayer{DFPLAYER_READY}, m_receivedIndex{0}, m_isAvailable{false}, m_isSending{false} {
	if (this->m_direction != Gpio::INPUT) this->toggleDir();
	g_callback_list.push_back(this);
	this->begin();
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

void DFPlayer::resetConditions(void) {
	this->m_receivedIndex = 0;
	this->m_isAvailable = true;
}

void DFPlayer::parseStack(void) {
	uint8_t handleCommand = *(this->m_received + Stack_Command);
	if (handleCommand == 0x41) { // Handle the 0x41 ACK feedback as a special case, in case the pollusion of _handleCommand, _handleParameter, and _handleType.
		this->m_isSending = false;
		return;
	}

//	uint16_t handleParameter = this->arrayToUint16(this->m_received + Stack_Parameter);

	switch (handleCommand) {
		case 0x3C:
		case 0x3D:
//			this->handleMessage(handleParameter); // DFPlayerPlayFinished
//			break;
		case 0x3F:
//			if (handleParameter & 0x01) this->handleMessage(handleParameter); // DFPlayerUSBOnline
//			else if (handleParameter & 0x02) this->handleMessage(handleParameter); // DFPlayerCardOnline
//			else if (handleParameter & 0x03) this->handleMessage(handleParameter); // DFPlayerCardUSBOnline
//			break;
		case 0x3A:
//			if (handleParameter & 0x01) this->handleMessage(handleParameter); // DFPlayerUSBInserted
//			else if (handleParameter & 0x02) this->handleMessage(handleParameter); // DFPlayerCardInserted
//			break;
		case 0x3B:
//			if (handleParameter & 0x01) this->handleMessage(handleParameter); // DFPlayerUSBRemoved
//			else if (handleParameter & 0x02) this->handleMessage(handleParameter); // DFPlayerCardRemoved
//			break;
		case 0x40:
//			this->handleMessage(handleParameter); // DFPlayerError
//			break;
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
//			this->handleMessage(handleParameter); // DFPlayerFeedBack
			this->resetConditions();
			break;
		default: // WrongStack
			this->resetConditions();
			this->m_isSending = false;
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
				if (this->m_received[Stack_Header] != 0x7E) {
					if (!(this->receive(bufferReceived, DFPLAYER_RECEIVED_LENGTH - 1))) {
						this->m_isSending = false;
						break;
					}
				} else this->m_receivedIndex++;
			} else {
				this->m_received[this->m_receivedIndex] = bufferReceived[this->m_receivedIndex];
				switch (this->m_receivedIndex) {
					case Stack_Version:
						if (this->m_received[this->m_receivedIndex] != 0xFF) { // WrongStack
							this->resetConditions();
							this->m_isSending = false;
							return false;
						}
						break;
					case Stack_Length:
						if (this->m_received[this->m_receivedIndex] != 0x06) { // WrongStack
							this->resetConditions();
							this->m_isSending = false;
							return false;
						}
						break;
					case Stack_End:
						if (this->m_received[this->m_receivedIndex] != 0xEF) { // WrongStack
							this->resetConditions();
							this->m_isSending = false;
							return false;
						} else {
							if (this->validateStack()) {
								this->parseStack();
								return this->m_isAvailable;
							} else { // WrongStack
								this->resetConditions();
								this->m_isSending = false;
								return false;
							}
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
			this->resetConditions();
			this->m_isSending = false;
			return false;
		}
	}
	return true;
}

void DFPlayer::begin(bool isACK, bool doReset) {
	isACK ? this->enableACK() : this->disableACK();
	if (doReset) {
		this->reset();
		this->waitAvailable(2000);
		delay(200);
	}
	this->m_isAvailable = false;
}

void DFPlayer::next(void) {
	this->sendStack(0x01);
	this->m_pauseState = false;
	this->m_statusDFPlayer = DFPLAYER_BUSY;
}

void DFPlayer::previous(void) {
	this->sendStack(0x02);
	this->m_pauseState = false;
	this->m_statusDFPlayer = DFPLAYER_BUSY;
}

void DFPlayer::play(uint8_t fileNumber) {
	if (!fileNumber) this->sendStack(0x03,this->m_backupFile);
	else this->sendStack(0x03, fileNumber);
	this->m_pauseState = false;
	this->m_statusDFPlayer = DFPLAYER_BUSY;
}

void DFPlayer::volume(uint8_t volume) {
	uint8_t result = volume > 100 ? 100 : volume;
	result /= (100 / (float)(DFPLAYER_MAX_VOLUME_VALUE));
	this->sendStack(0x06, result);
	this->m_volume = result;
}

void DFPlayer::equalizer(equalizer_t equalizer) {
	this->sendStack(0x07, (uint16_t)(equalizer));
}

void DFPlayer::loop(uint8_t fileNumber) {
	this->sendStack(0x08, fileNumber);
	this->m_pauseState = false;
	this->m_statusDFPlayer = DFPLAYER_BUSY;
}

void DFPlayer::reset(void) {
	this->sendStack(0x0C);
	this->m_pauseState = false;
	this->m_statusDFPlayer = DFPLAYER_READY;
}

void DFPlayer::pause(void) {
	if (this->getStatus() == DFPLAYER_BUSY && this->getPin()) {
		this->sendStack(0x0E);
		this->m_pauseState = true;
		this->m_statusDFPlayer = DFPLAYER_PAUSE;
	}
}

void DFPlayer::resume(void) {
	if (this->getStatus() == DFPLAYER_PAUSE && !(this->getPin())) {
		this->sendStack(0x0D);
		this->m_pauseState = false;
		this->m_statusDFPlayer = DFPLAYER_BUSY;
	}
}

void DFPlayer::prepareSong(uint8_t fileNumber) {
	this->m_backupFile = fileNumber;
}

uint8_t DFPlayer::getVolume(void) const {
	return this->m_volume;
}

DFPlayer_result_t DFPlayer::getStatus(void) const {
	return this->m_statusDFPlayer;
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void DFPlayer::callbackMethod(void) {
	if (this->getStatus() == DFPLAYER_BUSY && !(this->getPin())) this->m_statusDFPlayer = DFPLAYER_READY;
	else if (this->getStatus() == DFPLAYER_READY && this->getPin()) this->m_statusDFPlayer = DFPLAYER_BUSY;
	if (this->m_timeOutTimer) {
		this->m_timeOutTimer--;
		if (!(this->m_timeOutTimer)) this->m_isAvailable = false;
	}
}

#pragma GCC pop_options

///////////////////////////////////
/// DFPlayerMini initialization ///
///////////////////////////////////

void initDFPlayer(void) {
	#ifdef CN13_PINS

	static DFPlayer dfplayer(INT1_IN, RX0_IN, TX0_OUT);
	dfplayer.volume(DFPLAYER_INITIAL_VOLUME_PERCENTAGE);

	g_dfplayer = &dfplayer;

	#endif // CN13_PINS
}
