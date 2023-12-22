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

MFRC522::MFRC522(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SSEL, const Gpio& hardRST) : SPI(SCK, MOSI, MISO, SyncCommSPI::FAST_FREQUENCY), Gpio(hardRST),
m_SSEL{SSEL}, m_statusRFID{RFID_OK} {
	this->enable();
	this->initPCD();
}

void MFRC522::send(const char *message) {
	if (!(this->m_statusRFID)) this->enableSSEL(this->m_slaveSelected);
	this->transmit(message);
	if (!(this->m_statusRFID)) this->disableSSEL(this->m_slaveSelected);
}

/*!
 * @brief Writes a number of bytes to the specified register in the MFRC522 chip.
 * @details The interface is described in the datasheet section 8.1.2.
 */
void MFRC522::writeRegisterPCD(uint8_t reg, uint8_t value) {
	uint8_t buffer[2] = { reg, value };
	if (this->m_statusRFID != RFID_SSEL_ERR) this->enableSSEL(this->m_slaveSelected);
	this->transmitBytes(buffer, sizeof(buffer));
	if (this->m_statusRFID != RFID_SSEL_ERR) this->disableSSEL(this->m_slaveSelected);
}

/*!
 * @brief Reads a byte from the specified register in the MFRC522 chip.
 * @details The interface is described in the datasheet section 8.1.2.
 */
uint8_t MFRC522::readRegisterPCD(uint8_t reg) {
	uint8_t value, buffer = (0x80 | reg);
	if (this->m_statusRFID != RFID_SSEL_ERR) this->enableSSEL(this->m_slaveSelected);
	this->transmitBytes(&buffer);	// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	this->receive(value);			// Read the value back.
	if (this->m_statusRFID != RFID_SSEL_ERR) this->disableSSEL(this->m_slaveSelected);
	return value;
}

/*!
 * @brief Sets the bits given in mask in register.
 */
void MFRC522::setRegisterBitMaskPCD(uint8_t reg, uint8_t mask) {
	uint8_t temporallyVariable;
	temporallyVariable = this->readRegisterPCD(reg);
	this->writeRegisterPCD(reg, temporallyVariable | mask);
}

/*!
 * @brief Clears the bits given in mask from register.
 */
void MFRC522::clearRegisterBitMaskPCD(uint8_t reg, uint8_t mask) {
	uint8_t temporallyVariable;
	temporallyVariable = this->readRegisterPCD(reg);
	this->writeRegisterPCD(reg, temporallyVariable & (~mask));
}

RFID_result_t MFRC522::PCD_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result) {

	return this->getStatus();
}

RFID_result_t MFRC522::PCD_TransceiveData(uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, bool checkCRC) {

	return this->getStatus();
}

RFID_result_t MFRC522::PCD_CommunicateWithPICC(uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, bool checkCRC) {

	return this->getStatus();
}

/*!
 * @brief Transmits REQA command.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 * @return RFID_OK on success.
 */
RFID_result_t MFRC522::PICC_REQA(uint8_t *bufferATQA, uint8_t *bufferSize) {
	uint8_t validBits, command = PICC_CMD_REQA;
	// The ATQA response is 2 bytes long.
	if (bufferATQA == nullptr || *bufferSize < 2) {
		this->m_statusRFID = RFID_NO_ROOM_ERR;
		return this->getStatus();
	}
	this->clearRegisterBitMaskPCD(RFID_COLLISION_ERR, 0x80); // ValuesAfterColl = 1 => Bits received after collision are cleared.
	validBits = 7;	// For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
	this->m_statusRFID = this->PCD_TransceiveData(&command, 1, bufferATQA, bufferSize, &validBits);
	if (this->m_statusRFID != RFID_OK) return this->getStatus();
	// ATQA must be exactly 16 bits.
	if (*bufferSize != 2 || validBits != 0) {
		this->m_statusRFID = RFID_UPDATE_ERR;
		return this->getStatus();
	}
	return this->getStatus();
}

/*!
 * @briefTransmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state by calling PICC_RequestA() or PICC_WakeupA().
 * On success:
 * 		- The chosen PICC is in state ACTIVE(*) and all other PICCs have returned to state IDLE/HALT. (Figure 7 of the ISO/IEC 14443-3 draft.)
 * 		- The UID size and value of the chosen PICC is returned in *uid along with the SAK.
 *
 * A PICC UID consists of 4, 7 or 10 bytes.
 * Only 4 bytes can be specified in a SELECT command, so for the longer UIDs two or three iterations are used:
 * 		UID size	Number of UID bytes		Cascade levels		Example of PICC
 * 		========	===================		==============		===============
 * 		single				 4						1				MIFARE Classic
 * 		double				 7						2				MIFARE Ultralight
 * 		triple				10						3				Not currently in use?
 *
 * @return RFID_OK on success.
 */
RFID_result_t MFRC522::PICC_Select(uint8_t validBits) {
	bool uidComplete, selectDone, useCascadeTag;
	uint8_t cascadeLevel = 1, count, checkBit, index;
	uint8_t uidIndex;				// The first index in uid->uidByte[] that is used in the current Cascade Level.
	int8_t currentLevelKnownBits;	// The number of known UID bits in the current Cascade Level.
	uint8_t buffer[9];				// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
	uint8_t bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
	uint8_t rxAlign;				// Used in BitFramingReg. Defines the bit position for the first bit received.
	uint8_t txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte.
	uint8_t *responseBuffer;
	uint8_t responseLength;

	// Description of buffer structure:
	//		Byte 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		Byte 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits.
	//		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
	//		Byte 3: UID-data
	//		Byte 4: UID-data
	//		Byte 5: UID-data
	//		Byte 6: BCC					Block Check Character - XOR of bytes 2-5
	//		Byte 7: CRC_A
	//		Byte 8: CRC_A
	// The BCC and CRC_A are only transmitted if we know all the UID bits of the current Cascade Level.
	//
	// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
	//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
	//		========	=============	=====	=====	=====	=====
	//		 4 bytes		1			uid0	uid1	uid2	uid3
	//		 7 bytes		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 bytes		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	if (validBits > 80) return RFID_INVALID; // Sanity checks

	// Prepare MFRC522
	this->clearRegisterBitMaskPCD(RC522_COLL_REG, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = false;
	while (!uidComplete) {
		// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
		switch (cascadeLevel) {
			case 1:
				buffer[0] = PICC_CMD_SEL_CL1;
				uidIndex = 0;
				// When we know that the UID has more than 4 bytes
				useCascadeTag = validBits && this->m_UID.size > 4;
				break;
			case 2:
				buffer[0] = PICC_CMD_SEL_CL2;
				uidIndex = 3;
				// When we know that the UID has more than 7 bytes
				useCascadeTag = validBits && this->m_UID.size > 7;
				break;
			case 3:
				buffer[0] = PICC_CMD_SEL_CL3;
				uidIndex = 6;
				// Never used in CL3.
				useCascadeTag = false;
				break;
			default:
				return RFID_INTERNAL_ERR;
				break;
		}

		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0) currentLevelKnownBits = 0;
		// Copy the known bits from uid->uidByte[] to buffer[]
		index = 2; // destination index in buffer[]
		if (useCascadeTag) buffer[index++] = PICC_CMD_CT;
		uint8_t bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
		if (bytesToCopy) {
			uint8_t maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
			if (bytesToCopy > maxBytes) bytesToCopy = maxBytes;
			for (count = 0; count < bytesToCopy; count++) buffer[index++] = this->m_UID.uidByte[uidIndex + count];
		}
		// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
		if (useCascadeTag) currentLevelKnownBits += 8;

		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
		selectDone = false;
		while (!selectDone) {
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
				//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				this->m_statusRFID = this->PCD_CalculateCRC(buffer, 7, &buffer[7]);
				if (this->m_statusRFID != RFID_OK) return this->getStatus();
				txLastBits		= 0; // 0 => All 8 bits are valid.
				bufferUsed		= 9;
				// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer	= &buffer[6];
				responseLength	= 3;
			} else { // This is an ANTICOLLISION.
				//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
				txLastBits		= currentLevelKnownBits % 8;
				count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
				index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
				buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
				bufferUsed		= index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer	= &buffer[index];
				responseLength	= sizeof(buffer) - index;
			}

			// Set bit adjustments
			rxAlign = txLastBits;											// Having a separate variable is overkill. But it makes the next line easier to read.
			this->writeRegisterPCD(RC522_BIT_FRAMING_REG, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

			// Transmit the buffer and receive the response.
			this->m_statusRFID = this->PCD_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign);
			if (this->m_statusRFID == RFID_COLLISION_ERR) { // More than one PICC in the field => collision.
				uint8_t valueOfCollReg = this->readRegisterPCD(RC522_COLL_REG); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				// Without a valid collision position we cannot continue
				if (valueOfCollReg & 0x20) {
					this->m_statusRFID = RFID_COLLISION_ERR;
					return this->getStatus();
				}
				uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0) collisionPos = 32;
				if (collisionPos <= currentLevelKnownBits) {
					this->m_statusRFID = RFID_INTERNAL_ERR;
					return this->getStatus();
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits	= collisionPos;
				count			= currentLevelKnownBits % 8; // The bit to modify
				checkBit		= (currentLevelKnownBits - 1) % 8;
				index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
				buffer[index]	|= (1 << checkBit);
			} else if (this->m_statusRFID != RFID_OK) return this->getStatus();
			else {
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					selectDone = true; // No more anticollision
					// We continue below outside the while.
				} else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		}

		// Copy the found UID bytes from buffer[] to uid->uidByte[]
		index = (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
		bytesToCopy = (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < bytesToCopy; count++) this->m_UID.uidByte[uidIndex + count] = buffer[index++];

		// Check response SAK (Select Acknowledge)
		// SAK must be exactly 24 bits (1 byte + CRC_A).
		if (responseLength != 3 || txLastBits != 0) return RFID_UPDATE_ERR;
		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
		this->m_statusRFID = this->PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
		if (this->m_statusRFID != RFID_OK) return this->getStatus();
		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
			this->m_statusRFID = RFID_CRC_WRONG;
			return this->getStatus();
		}
		if (responseBuffer[0] & 0x04) cascadeLevel++;
		else {
			uidComplete = true;
			this->m_UID.sak = responseBuffer[0];
		}
	}

	this->m_UID.size = 3 * cascadeLevel + 1;
	return this->getStatus();
}

RFID_result_t MFRC522::enable(void) {
	if (!(this->bindSSEL(this->m_SSEL, this->m_slaveSelected))) this->m_statusRFID = RFID_SSEL_ERR;
	return this->getStatus();
}

RFID_result_t MFRC522::disable(void) {
	if (!(this->unbindSSEL(this->m_slaveSelected))) this->m_statusRFID = RFID_SSEL_ERR;
	return this->getStatus();
}

RFID_result_t MFRC522::getStatus(void) const {
	return this->m_statusRFID;
}

/*!
 * @brief Initializes the MFRC522 chip.
 */
void MFRC522::initPCD(void) {
	bool hardRST = false;

	// First set the resetPowerDownPin as digital input, to check the MFRC522 power down mode.
	if (this->m_direction != Gpio::INPUT) this->toggleDir();

	if (!(this->getPin())) { // The MFRC522 chip is in power down mode.
		this->toggleDir(); // Now set the resetPowerDownPin as digital output.
		this->clearPin(); // Make sure we have a clean LOW state.
		// 8.8.1 Reset timing requirements says about 100ns.
		this->setPin();	// Exit power down mode. This triggers a hard reset.
		// Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74μs. Let us be generous: 50ms.
		delay(50);
		hardRST = true;
	}

	if (!hardRST) this->resetPCD(); // Perform a soft reset if we haven't triggered a hard reset above.

	// Reset baud rates
	this->writeRegisterPCD(RC522_TX_MODE_REG, 0x00);
	this->writeRegisterPCD(RC522_RX_MODE_REG, 0x00);
	// Reset RC522_MOD_WIDTH_REG
	this->writeRegisterPCD(RC522_MOD_WIDTH_REG, 0x26);

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
	this->writeRegisterPCD(RC522_T_MODE_REG, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
	this->writeRegisterPCD(RC522_T_PRESCALER_REG, 0xA9);	// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
	this->writeRegisterPCD(RC522_T_RELOAD_REG_H, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
	this->writeRegisterPCD(RC522_T_RELOAD_REG_L, 0xE8);

	this->writeRegisterPCD(RC522_TX_ASK_REG, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	this->writeRegisterPCD(RC522_MODE_REG, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
	this->antennaOnPCD();	// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
}

void MFRC522::resetPCD(void) {

}

void MFRC522::antennaOnPCD(void) {

}

void MFRC522::antennaOffPCD(void) {

}

/*!
 * @brief Returns true if a PICC responds to PICC_CMD_REQA.
 * @details Only "new" cards in state IDLE are invited. Sleeping cards in state HALT are ignored.
 */
RFID_result_t MFRC522::isCardPICC(void) {
	uint8_t bufferATQA[2], bufferSize = sizeof(bufferATQA);

	// Reset baud rates
	this->writeRegisterPCD(RC522_TX_MODE_REG, 0x00);
	this->writeRegisterPCD(RC522_RX_MODE_REG, 0x00);
	// Reset RC522_MOD_WIDTH_REG
	this->writeRegisterPCD(RC522_MOD_WIDTH_REG, 0x26);

	return this->PICC_REQA(bufferATQA, &bufferSize);
}

/*!
 * @brief Simple wrapper around PICC_Select.
 * @return Returns true if a UID could be read.
 * @note Remember to call PICC_IsNewCardPresent(), PICC_RequestA() or PICC_WakeupA() first.
 * @details The read UID is available in the class variable UID.
 */
bool MFRC522::readCardPICC(void) {
	return this->PICC_Select();
}

RFID_result_t MFRC522::haltPICC(void) {

	return this->getStatus();
}

RFID_result_t MFRC522::getUID(UID_st *UID) {

	return this->getStatus();
}

void MFRC522::dumpVersion(void) {

}

void MFRC522::dumpUID(UID_st *UID) {

}

void MFRC522::dumpDetails(UID_st *UID) {

}

char* MFRC522::printUID(void) {
	static char RFIDstr[RFID_STR_SIZE];
	RFIDstr[0] = 'U'; RFIDstr[1] = 'I'; RFIDstr[2] = 'D'; RFIDstr[3] = ':'; RFIDstr[4] = ' ';
	RFIDstr[5] = this->m_UID.uidByte[0] + '0'; RFIDstr[6] = this->m_UID.uidByte[1] + '0'; RFIDstr[7] = ' ';
	RFIDstr[8] = this->m_UID.uidByte[2] + '0'; RFIDstr[9] = this->m_UID.uidByte[3] + '0'; RFIDstr[10] = ' ';
	RFIDstr[11] = this->m_UID.uidByte[4] + '0'; RFIDstr[12] = this->m_UID.uidByte[5] + '0'; RFIDstr[13] = ' ';
	RFIDstr[14] = this->m_UID.uidByte[6] + '0'; RFIDstr[15] = this->m_UID.uidByte[7] + '0'; RFIDstr[16] = ' ';
	RFIDstr[17] = this->m_UID.uidByte[8] + '0'; RFIDstr[18] = this->m_UID.uidByte[9] + '0'; RFIDstr[19] = '\n'; RFIDstr[20] = '\0';
	return RFIDstr;
}

MFRC522::~MFRC522() {
	this->disable(); // According to the SPI implementation, the unbindSSEL function must be executed in the primitive classes.
}

//////////////////////////////
/// MFRC522 initialization ///
//////////////////////////////

void initRFID(void) {
	#if defined(SPI_DEBUG_PINS)

	static Gpio HARDWARE_RST(SPI_DEBUG_SSEL1);
	static MFRC522 rfid(SPI_DEBUG_SCK, SPI_DEBUG_MOSI, SPI_DEBUG_MISO, SPI_DEBUG_SSEL0, HARDWARE_RST);

	g_rfid = &rfid;

	#endif // defined(SPI_DEBUG_PINS)
}

