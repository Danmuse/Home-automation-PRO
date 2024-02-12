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
	initUSB0();
	initRFID();

	while (1) {
		g_rfid->getUID();
    	if (g_rfid->getStatus() == RFID_OK && *(g_rfid->printUID()) != 0)
    		g_usb->transmit(g_rfid->printUID());
    	g_timers_list.timerEvents();
	}
}
******************** RFID Testing END *******************/

MFRC522 *g_rfid = nullptr;

MFRC522::MFRC522(const Gpio &SCK, const Gpio &MOSI, const Gpio &MISO, const Gpio &SSEL, const Gpio &hardRST) : SPI(SCK, MOSI, MISO, SyncCommSPI::FAST_FREQUENCY), Gpio(hardRST), Callback(),
m_SSEL{SSEL}, m_statusRFID{RFID_OK}, m_timeOut{0}, m_operationCompleted{false} {
    this->enable();
    this->initPCD();
    g_callback_list.push_back(this);
}

/*!
 * @brief Writes a byte to the specified register in the MFRC522 chip.
 * @details The interface is described in the datasheet section 8.1.2.
 */
void MFRC522::writeRegisterPCD(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
	this->enableSSEL(this->m_slaveSelected);
    this->transmitBytes(buffer, sizeof(buffer));
    this->disableSSEL(this->m_slaveSelected);
}

/*!
 * @brief Writes a number of bytes to the specified register in the MFRC522 chip.
 * @details The interface is described in the datasheet section 8.1.2.
 */
void MFRC522::writeRegisterPCD(uint8_t reg, uint8_t count, uint8_t *values) {
    uint8_t buffer[count + 1];
    buffer[0] = reg;
    for (uint8_t index = 0; index < count; index++) buffer[index + 1] = values[index];
    this->enableSSEL(this->m_slaveSelected);
    this->transmitBytes(buffer, sizeof(buffer));
    this->disableSSEL(this->m_slaveSelected);
}

/*!
 * @brief Reads a byte from the specified register in the MFRC522 chip.
 * @details The interface is described in the datasheet section 8.1.2.
 */
uint8_t MFRC522::readRegisterPCD(uint8_t reg) {
    uint8_t value, address = (0x80 | reg);
    this->enableSSEL(this->m_slaveSelected);
    // MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
    this->receiveBytes(&address, &value, sizeof(value)); // Read the value back.
    this->disableSSEL(this->m_slaveSelected);
    return value;
}

/*!
 * @brief Reads a number of bytes from the specified register in the MFRC522 chip.
 * @details The interface is described in the datasheet section 8.1.2.
 */
void MFRC522::readRegisterPCD(uint8_t reg, uint8_t count, uint8_t *values, uint8_t rxAlign) {
	uint8_t address = (0x80 | reg), index = 0;
	if (count == 0) return;
	this->enableSSEL(this->m_slaveSelected);
	count--; // One read is performed outside of the loop
	if (rxAlign) { // Only update bit positions rxAlign...7 in values[0]
		// Create bit mask for bit positions rxAlign...7
		uint8_t mask = (0xFF << rxAlign) & 0xFF;
		// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
		uint8_t value; // Read value and tell that we want to read the same address again.
		this->receiveBytes(&address, &value, sizeof(value)); // Read the value back.
		// Apply mask to both current value of values[0] and the new data in value.
		values[0] = (values[0] & ~mask) | (value & mask);
		index++;
	}
	// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	// Read value and tell that we want to read the same address again.
	while (index < count) {
//		if (!index) continue;
		this->receiveBytes(&address, &values[index++], 1);
	}
	this->receiveBytes(&address, &values[index], 1);
	this->disableSSEL(this->m_slaveSelected);
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

/*!
 * @brief Use the CRC coprocessor in the MFRC522 to calculate a CRC_A.
 * @return RFID_OK on success.
 */
RFID_result_t MFRC522::PCD_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result) {
	// Wait for the CRC calculation to complete.
	// Check for the register to indicate that the CRC calculation is complete in a loop.
	// If the calculation is not indicated as complete in ~90ms, then time out the operation.
	if (!(this->m_timeOut) && !(this->m_operationCompleted)) {
		this->writeRegisterPCD(RC522_COMMAND_REG, PCD_IDLE);			// Stop any active command
		this->writeRegisterPCD(RC522_DIV_IRQ_REG, 0x04);				// Clear the CRCIRq interrupt request bit
		this->writeRegisterPCD(RC522_FIFO_LEVEL_REG, 0x80);				// FlushBuffer = 1, FIFO initialization
		this->writeRegisterPCD(RC522_FIFO_DATA_REG, length, data);		// Write data to the FIFO
		this->writeRegisterPCD(RC522_COMMAND_REG, PCD_CALCULATE_CRC);	// Start the calculation

		this->m_operation = CALCULATE_CRC;
		this->m_timeOut = (uint8_t)(90 * (g_systick_freq / 1000));
	} else if (this->m_timeOut && !(this->m_operationCompleted) && this->m_operation == CALCULATE_CRC) {
		// DivIrqReg[7...0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
		uint8_t regValue = this->readRegisterPCD(RC522_DIV_IRQ_REG);
		if (regValue & 0x04) { // CRCIRq bit set - calculation done
			// Stop calculating CRC for new content in the FIFO.
			this->writeRegisterPCD(RC522_COMMAND_REG, PCD_IDLE);
			// Transfer the result from the registers to the result buffer
			result[0] = this->readRegisterPCD(RC522_CRC_RESULT_REG_L);
			result[1] = this->readRegisterPCD(RC522_CRC_RESULT_REG_H);
			this->m_timeOut = 0;
			this->m_statusRFID = RFID_OK;
			return this->getStatus();
		}
	}

	// 90ms passed and nothing happened. Communication with the MFRC522 might be down.
	this->m_statusRFID = RFID_TIMEOUT_ERR;
	return this->getStatus();
}

/*!
 * @brief Transfers data to the MFRC522 FIFO, executes a command, waits for completion and transfers data back from the FIFO.
 * @details CRC validation can only be done if backData and backLen are specified.
 * @return RFID_OK on success.
 */
RFID_result_t MFRC522::PCD_CommunicateWithPICC(uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, bool checkCRC) {
    // Prepare values for BitFramingReg
    uint8_t txLastBits = validBits ? *validBits : 0;
    uint8_t bitFraming = (rxAlign << 4) + txLastBits; // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

    static uint8_t *local_backData;
    static uint8_t local_backLen;

	// In PCD_Init() we set the TAuto flag in TModeReg.
    // This means the timer automatically starts when the PCD stops transmitting.
	//
	// Wait here for the command to complete. The bits specified in the `waitIRq` parameter define what bits constitute a completed command.
	// When they are set in the ComIrqReg register, then the command is considered complete.
    // If the command is not indicated as complete in ~36ms, then consider the command as timed out.
    if (!(this->m_timeOut) && !(this->m_operationCompleted)) {
		this->writeRegisterPCD(RC522_COMMAND_REG, PCD_IDLE);           	// Stop any active command.
		this->writeRegisterPCD(RC522_COM_IRQ_REG, 0x7F);               	// Clear all seven interrupt request bits
		this->writeRegisterPCD(RC522_FIFO_LEVEL_REG, 0x80);            	// FlushBuffer = 1, FIFO initialization
		this->writeRegisterPCD(RC522_FIFO_DATA_REG, sendLen, sendData);	// Write sendData to the FIFO
		this->writeRegisterPCD(RC522_BIT_FRAMING_REG, bitFraming);     	// Bit adjustments
		this->writeRegisterPCD(RC522_COMMAND_REG, command);            	// Execute the command

		if (command == PCD_TRANSCEIVE) this->setRegisterBitMaskPCD(RC522_BIT_FRAMING_REG, 0x80);    // StartSend=1, transmission of data starts

		this->m_operation = COMMUNICATION_PICC;
		this->m_timeOut = (uint8_t)(36 * (g_systick_freq / 1000));
    } else if (this->m_timeOut && !(this->m_operationCompleted) && this->m_operation == COMMUNICATION_PICC) {
    	// ComIrqReg[7...0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
    	uint8_t regValue = this->readRegisterPCD(RC522_COM_IRQ_REG);
    	if (regValue & RFID_WAIT_IRQ) { // One of the interrupts that signal success has been set.
    		this->m_operationCompleted = true;
    		this->m_timeOut = 0;
    	}
    	// Timer interrupt - nothing received in 25ms
    	if (regValue & 0x01 && !(this->m_operationCompleted)) this->m_timeOut = 0;
    } else if (this->m_timeOut && !(this->m_operationCompleted) && this->m_operation == CALCULATE_CRC) {
    	backData = &local_backData[0];
    	*backLen = local_backLen;
    	// Verify CRC_A - do our own calculation and store the control in controlBuffer.
    	uint8_t controlBuffer[RFID_CRC_SIZE];
		this->PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
		if (this->m_statusRFID != RFID_OK) return this->getStatus();
		if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
			this->m_statusRFID = RFID_CRC_WRONG;
			return this->getStatus();
		}
		return this->getStatus();
    } else if (this->m_operationCompleted && this->m_operation == COMMUNICATION_PICC) {
		this->m_operationCompleted = false;
		this->m_statusRFID = RFID_OK;

		// ErrorReg[7...0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
		uint8_t errorRegValue = this->readRegisterPCD(RC522_ERROR_REG);
		if (errorRegValue & 0x13) { // BufferOvfl ParityErr ProtocolErr
			this->m_statusRFID = RFID_UPDATE_ERR;
			return this->getStatus();
		}

		uint8_t _validBits = 0;
		// If the caller wants data back, get it from the MFRC522.
		if (backData && backLen) {
			uint8_t n = this->readRegisterPCD(RC522_FIFO_LEVEL_REG); // Number of bytes in the FIFO
			if (n > *backLen) {
				this->m_statusRFID = RFID_NO_ROOM_ERR;
				return this->getStatus();
			}
			*backLen = n; // Number of bytes returned
			this->readRegisterPCD(RC522_FIFO_DATA_REG, n, backData, rxAlign); // Get received data from FIFO
			_validBits = this->readRegisterPCD(RC522_CONTROL_REG) & 0x07; // RxLastBits[2:0] indicates the number of valid bits in the last received byte. If this value is 000b, the whole byte is valid.
			if (validBits) *validBits = _validBits;

			local_backData = &backData[0];
			local_backLen = *backLen;
		}

		if (errorRegValue & 0x08) { // Tell about collisions
			this->m_statusRFID = RFID_COLLISION_ERR;
			return this->getStatus();
		}

		// The buffer must be at least 18 bytes because a CRC_A is also returned.
		// Checks the CRC_A before returning RFID_OK.
		if (backData && backLen && checkCRC) {
			// In this case a MIFARE Classic NAK is not OK.
			if (*backLen == 1 && _validBits == 4) {
				this->m_statusRFID = RFID_MIFARE_NACK;
				return this->getStatus();
			}
			// We need at least the CRC_A value and all 8 bits of the last byte must be received.
			if (*backLen < 2 || _validBits != 0) {
				this->m_statusRFID = RFID_CRC_WRONG;
				return this->getStatus();
			}
			// Verify CRC_A - do our own calculation and store the control in controlBuffer.
			uint8_t controlBuffer[RFID_CRC_SIZE];
			this->PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
			if (this->m_statusRFID != RFID_OK) return this->getStatus();
			if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
				this->m_statusRFID = RFID_CRC_WRONG;
				return this->getStatus();
			}
		}
		return this->getStatus();
	}

	// 36ms passed and nothing happened. Communication with the MFRC522 might be down.
	this->m_statusRFID = RFID_TIMEOUT_ERR;
	return this->getStatus();
}

/*!
 * @brief Transmits REQA command.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 * @return RFID_OK on success.
 */
RFID_result_t MFRC522::PICC_REQA(uint8_t *bufferATQA, uint8_t *bufferSize) {
    uint8_t validBits, command = PICC_CMD_REQA;

    if (this->m_timeOut || this->m_operationCompleted) {
    	validBits = 7; // For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
    	this->PCD_CommunicateWithPICC(PCD_TRANSCEIVE, RFID_WAIT_IRQ, &command, 1, bufferATQA, bufferSize, &validBits);
    	if (this->m_statusRFID != RFID_OK) return this->getStatus();
		// ATQA must be exactly 16 bits.
		if (*bufferSize != 2 || validBits != 0) {
			this->m_statusRFID = RFID_UPDATE_ERR;
			return this->getStatus();
		}
		return this->getStatus();
    }

    // The ATQA response is 2 bytes long.
    if (bufferATQA == nullptr || *bufferSize < 2) {
        this->m_statusRFID = RFID_NO_ROOM_ERR;
        return this->getStatus();
    }
    this->clearRegisterBitMaskPCD(RC522_COLL_REG, 0x80); // ValuesAfterColl = 1 => Bits received after collision are cleared.
    validBits = 7; // For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
    this->PCD_CommunicateWithPICC(PCD_TRANSCEIVE, RFID_WAIT_IRQ, &command, 1, bufferATQA, bufferSize, &validBits);
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
    bool uidComplete = false, selectDone = false, loopBack = false;
    static bool useCascadeTag;
    static uint8_t cascadeLevel = 1, count, checkBit, index;
    static uint8_t uidIndex;				// The first index in uid->uidByte[] that is used in the current Cascade Level.
    static int8_t currentLevelKnownBits;	// The number of known UID bits in the current Cascade Level.
    static uint8_t buffer[9];				// The SELECT/ANTICOLLISION commands uses a 7 byte standard frame + 2 bytes CRC_A
    static uint8_t bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
    static uint8_t rxAlign;				    // Used in BitFramingReg. Defines the bit position for the first bit received.
    static uint8_t txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted byte.
    static uint8_t *responseBuffer;
    static uint8_t responseLength;

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

    static bool calculatingCRC = true;
    static bool followingSectionFST = false;
    static bool followingSectionSND = false;
    static bool followingSectionTRD = false;
    static bool followingSectionFRD = false;

    static bool local_selectDone = false;

    if ((this->m_timeOut || this->m_operationCompleted) && calculatingCRC && !local_selectDone) followingSectionFST = true;
    else if ((this->m_timeOut || this->m_operationCompleted) && !calculatingCRC && !local_selectDone) followingSectionSND = true;
    else if ((this->m_timeOut || this->m_operationCompleted) && !calculatingCRC && local_selectDone) followingSectionTRD = true;

    ////////////////////////////////////////////////////
    // Check debugging the following fragment of code //
    ////////////////////////////////////////////////////
    while ((this->m_timeOut || this->m_operationCompleted || loopBack) && (followingSectionFST || followingSectionSND || followingSectionTRD)) {
    	loopBack = false;
    	if (followingSectionFST) {
			followingSectionFST = false;
			calculatingCRC = true;

			buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
			// Calculate BCC - Block Check Character
			buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
			// Calculate CRC_A
			this->PCD_CalculateCRC(buffer, 7, &buffer[7]);
			if (this->m_statusRFID != RFID_OK) return this->getStatus();
			calculatingCRC = false;
			txLastBits = 0; // 0 => All 8 bits are valid.
			bufferUsed = 9;
			// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
			// local_responseBufferData = &buffer[6];
			responseBuffer = &buffer[6];
			responseLength = 3;
			// Set bit adjustments
			rxAlign = txLastBits; // Having a separate variable is overkill. But it makes the next line easier to read.
			this->writeRegisterPCD(RC522_BIT_FRAMING_REG, (rxAlign << 4) + txLastBits);    // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

			followingSectionSND = true;
    	}
    	if (followingSectionSND) {
			followingSectionSND = false;
			calculatingCRC = false;

			this->m_operation = COMMUNICATION_PICC; // WARNING: Force action. Avoid enter to the CRC calculation.
			// Transmit the buffer and receive the response.
			this->PCD_CommunicateWithPICC(PCD_TRANSCEIVE, RFID_WAIT_IRQ, buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign);
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
				currentLevelKnownBits = collisionPos;
				count = currentLevelKnownBits % 8; // The bit to modify
				checkBit = (currentLevelKnownBits - 1) % 8;
				index = 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
				buffer[index] |= (1 << checkBit);

				if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
					followingSectionFST = true;
					loopBack = true;
				} else { // This is an ANTICOLLISION.
					txLastBits = currentLevelKnownBits % 8;
					count = currentLevelKnownBits / 8;    // Number of whole bytes in the UID part.
					index = 2 + count;                    // Number of whole bytes: SEL + NVB + UIDs
					buffer[1] = (index << 4) + txLastBits;    // NVB - Number of Valid Bits
					bufferUsed = index + (txLastBits ? 1 : 0);
					// Store response in the unused part of buffer
					responseBuffer = &buffer[index];
					responseLength = sizeof(buffer) - index;
					// Set bit adjustments
					rxAlign = txLastBits; // Having a separate variable is overkill. But it makes the next line easier to read.
					this->writeRegisterPCD(RC522_BIT_FRAMING_REG, (rxAlign << 4) + txLastBits);    // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

					followingSectionSND = true;
					loopBack = true;
				}
			} else if (this->m_statusRFID != RFID_OK) return this->getStatus();
			else {
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					local_selectDone = true;
					selectDone = true; // No more anticollision
					// We continue below outside the while.
					followingSectionTRD = true;
				} else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
					followingSectionFST = true;
					loopBack = true;
				}
			}
		}
    	if (followingSectionTRD) {
			followingSectionTRD = false;
	        // Copy the found UID bytes from buffer[] to uid->uidByte[]
	        index = (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
	        uint8_t bytesToCopy = (buffer[2] == PICC_CMD_CT) ? 3 : 4;
	        for (count = 0; count < bytesToCopy; count++) this->m_UID.uidByte[uidIndex + count] = buffer[index++];

	        // Verify CRC_A - do our own calculation and store the control in buffer[2...3] - those bytes are not needed anymore.
	        this->PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
	        if (this->m_statusRFID != RFID_OK) return this->getStatus();
	        if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
	            this->m_statusRFID = RFID_CRC_WRONG;
	            return this->getStatus();
	        }
	        if (responseBuffer[0] & 0x04) cascadeLevel++;
	        else {
	            uidComplete = true;
	            this->m_UID.sak = responseBuffer[0];
	            this->m_UID.size = 3 * cascadeLevel + 1;
				return this->getStatus();
	        }
    	}
    	if (followingSectionFRD) {
			followingSectionFRD = false;
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
					this->m_statusRFID = RFID_INTERNAL_ERR;
					return this->getStatus();
					break;
			}

			// How many UID bits are known in this Cascade Level?
			currentLevelKnownBits = validBits - (8 * uidIndex);
			if (currentLevelKnownBits < 0) currentLevelKnownBits = 0;
			// Copy the known bits from uid->uidByte[] to buffer[]
			index = 2; // Destination index in buffer[]
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
			local_selectDone = false;
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
				followingSectionFST = true;
				loopBack = true;
			} else { // This is an ANTICOLLISION.
				txLastBits = currentLevelKnownBits % 8;
				count = currentLevelKnownBits / 8;    // Number of whole bytes in the UID part.
				index = 2 + count;                    // Number of whole bytes: SEL + NVB + UIDs
				buffer[1] = (index << 4) + txLastBits;    // NVB - Number of Valid Bits
				bufferUsed = index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer = &buffer[index];
				responseLength = sizeof(buffer) - index;
				// Set bit adjustments
				rxAlign = txLastBits; // Having a separate variable is overkill. But it makes the next line easier to read.
				this->writeRegisterPCD(RC522_BIT_FRAMING_REG, (rxAlign << 4) + txLastBits);    // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

				followingSectionSND = true;
				loopBack = true;
			}
    	}
    }

    if (validBits > 80) { // Sanity checks
    	this->m_statusRFID = RFID_INVALID;
    	return this->getStatus();
    }

    // Prepare MFRC522
    this->clearRegisterBitMaskPCD(RC522_COLL_REG, 0x80); // ValuesAfterColl = 1 => Bits received after collision are cleared.

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
            	this->m_statusRFID = RFID_INTERNAL_ERR;
                return this->getStatus();
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

    	calculatingCRC = true;
        // Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
        local_selectDone = false;
        selectDone = false;
        while (!selectDone) {
            // Find out how many bits and bytes to send and receive.
            if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
                calculatingCRC = true;
                buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
                // Calculate BCC - Block Check Character
                buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
                // Calculate CRC_A
                this->PCD_CalculateCRC(buffer, 7, &buffer[7]);
                if (this->m_statusRFID != RFID_OK) return this->getStatus();
                calculatingCRC = false;
                txLastBits = 0; // 0 => All 8 bits are valid.
                bufferUsed = 9;
                // Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
                // local_responseBufferData = &buffer[6];
                responseBuffer = &buffer[6];
                responseLength = 3;
            } else { // This is an ANTICOLLISION.
                txLastBits = currentLevelKnownBits % 8;
                count = currentLevelKnownBits / 8;    // Number of whole bytes in the UID part.
                index = 2 + count;                    // Number of whole bytes: SEL + NVB + UIDs
                buffer[1] = (index << 4) + txLastBits;    // NVB - Number of Valid Bits
                bufferUsed = index + (txLastBits ? 1 : 0);
                // Store response in the unused part of buffer
                responseBuffer = &buffer[index];
                responseLength = sizeof(buffer) - index;
            }

            // Set bit adjustments
            rxAlign = txLastBits; // Having a separate variable is overkill. But it makes the next line easier to read.
            this->writeRegisterPCD(RC522_BIT_FRAMING_REG, (rxAlign << 4) + txLastBits);    // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
            calculatingCRC = false;

            this->m_operation = COMMUNICATION_PICC; // WARNING: Force action. Avoid enter to the CRC calculation.
            // Transmit the buffer and receive the response.
            this->PCD_CommunicateWithPICC(PCD_TRANSCEIVE, RFID_WAIT_IRQ, buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign);
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
                currentLevelKnownBits = collisionPos;
                count = currentLevelKnownBits % 8; // The bit to modify
                checkBit = (currentLevelKnownBits - 1) % 8;
                index = 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
                buffer[index] |= (1 << checkBit);
            } else if (this->m_statusRFID != RFID_OK) return this->getStatus();
            else {
                if (currentLevelKnownBits >= 32) { // This was a SELECT.
                	local_selectDone = true;
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

        // Verify CRC_A - do our own calculation and store the control in buffer[2...3] - those bytes are not needed anymore.
        this->PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
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

    if (this->m_statusRFID == RFID_SSEL_ERR) return;

    // First set the resetPowerDownPin as digital input, to check the MFRC522 power down mode.
    if (this->m_direction != Gpio::INPUT) this->toggleDir();

    if (!(this->getPin())) { // The MFRC522 chip is in power down mode.
        this->toggleDir(); // Now set the resetPowerDownPin as digital output.
        this->clearPin(); // Make sure we have a clean LOW state.
        // 8.8.1 Reset timing requirements says about 100ns.
        this->setPin();    // Exit power down mode. This triggers a hard reset.
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
    this->writeRegisterPCD(RC522_T_MODE_REG, 0x80);			// TAuto = 1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    this->writeRegisterPCD(RC522_T_PRESCALER_REG, 0xA9);	// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
    this->writeRegisterPCD(RC522_T_RELOAD_REG_H, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    this->writeRegisterPCD(RC522_T_RELOAD_REG_L, 0xE8);

    this->writeRegisterPCD(RC522_TX_ASK_REG, 0x40);			// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
    this->writeRegisterPCD(RC522_MODE_REG, 0x3D);			// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
    this->antennaOnPCD();	// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
}

/*!
 * @brief Performs a soft reset on the MFRC522 chip and waits for it to be ready again.
 */
void MFRC522::resetPCD(void) {
	uint8_t count = 0;
	if (this->m_statusRFID == RFID_SSEL_ERR) return;
	this->writeRegisterPCD(RC522_COMMAND_REG, PCD_SOFTWARE_RESET); // Issue the SoftReset command.
	// The datasheet does not mention how long the SoftRest command takes to complete.
	// But the MFRC522 might have been in soft power-down mode (triggered by bit 4 of CommandReg)
	// Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74μs. Let us be generous: 50ms.
	while ((this->readRegisterPCD(RC522_COMMAND_REG) & RC522_IdleIRq_MASK) && (count++) < 3) delay(50); // Wait for the PowerDown bit in CommandReg to be cleared (max 3x50ms)
}

/*!
 * @brief Turns the antenna on by enabling pins TX1 and TX2.
 * @details After a reset these pins are disabled.
 */
void MFRC522::antennaOnPCD(void) {
	if (this->m_statusRFID == RFID_SSEL_ERR) return;
    uint8_t value = this->readRegisterPCD(RC522_TX_CONTROL_REG);
    if ((value & 0x03) != 0x03) this->writeRegisterPCD(RC522_TX_CONTROL_REG, value | 0x03);
}

/*!
 * @brief Turns the antenna off by disabling pins TX1 and TX2.
 */
void MFRC522::antennaOffPCD(void) {
	if (this->m_statusRFID == RFID_SSEL_ERR) return;
    this->clearRegisterBitMaskPCD(RC522_TX_CONTROL_REG, 0x03);
}

/*!
 * @brief Returns true if a PICC responds to PICC_CMD_REQA.
 * @details Only "new" cards in state IDLE are invited. Sleeping cards in state HALT are ignored.
 */
bool MFRC522::isCardPICC(void) {
    uint8_t bufferATQA[2], bufferSize = sizeof(bufferATQA);

    if (this->m_statusRFID == RFID_SSEL_ERR) return false;

    if (this->m_timeOut || this->m_operationCompleted) return this->PICC_REQA(bufferATQA, &bufferSize) == RFID_OK ? true : false;

    // Reset baud rates
    this->writeRegisterPCD(RC522_TX_MODE_REG, 0x00);
    this->writeRegisterPCD(RC522_RX_MODE_REG, 0x00);
    // Reset RC522_MOD_WIDTH_REG
    this->writeRegisterPCD(RC522_MOD_WIDTH_REG, 0x26);

    return this->PICC_REQA(bufferATQA, &bufferSize) == RFID_OK ? true : false;
}

/*!
 * @brief Simple wrapper around PICC_Select.
 * @return Returns true if a UID could be read.
 * @note Remember to call PICC_IsNewCardPresent(), PICC_RequestA() or PICC_WakeupA() first.
 * @details The read UID is available in the class variable UID.
 */
bool MFRC522::readCardPICC(void) {
	if (this->m_statusRFID == RFID_SSEL_ERR) return false;
    return this->PICC_Select() == RFID_OK ? true : false;
}

/*!
 * @brief Instructs a PICC in state ACTIVE(*) to go to state HALT.
 * @return RFID_OK on success.
 */
RFID_result_t MFRC522::haltPICC(void) {
	uint8_t buffer[4];

	static bool calculatingCRC = true;
	static uint8_t *local_backData;
	static uint8_t local_backLen;

	if ((this->m_timeOut && !calculatingCRC) || (this->m_operationCompleted && !calculatingCRC)) {
		// Send the command.
		// The standard says:
		//	If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
		//	HLTA command, this response shall be interpreted as 'not acknowledge'.
		// IMPORTANT: We interpret that this way: Only STATUS_TIMEOUT is a success.
		this->PCD_CommunicateWithPICC(PCD_TRANSCEIVE, RFID_WAIT_IRQ, local_backData, local_backLen, nullptr, 0);
		if (this->m_statusRFID == RFID_OK) this->m_statusRFID = RFID_UPDATE_ERR;
		if (this->m_statusRFID == RFID_TIMEOUT_ERR) this->m_statusRFID = RFID_OK;
		return this->getStatus();
	}

	if (this->m_statusRFID != RFID_SSEL_ERR) {
		// Build command buffer
		calculatingCRC = true;
		buffer[0] = PICC_CMD_HLTA;
		buffer[1] = 0;
		// Calculate CRC_A
		this->PCD_CalculateCRC(buffer, 2, &buffer[2]);
		if (this->m_statusRFID != RFID_OK) return this->getStatus();
		local_backData = &buffer[0];
		local_backLen = sizeof(buffer);
		calculatingCRC = false;

		// Send the command.
		// The standard says:
		//	If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
		//	HLTA command, this response shall be interpreted as 'not acknowledge'.
		// IMPORTANT: We interpret that this way: Only STATUS_TIMEOUT is a success.
		this->PCD_CommunicateWithPICC(PCD_TRANSCEIVE, RFID_WAIT_IRQ, buffer, sizeof(buffer), nullptr, 0);
		if (this->m_statusRFID == RFID_OK) this->m_statusRFID = RFID_UPDATE_ERR;
		if (this->m_statusRFID == RFID_TIMEOUT_ERR) this->m_statusRFID = RFID_OK;
	}

    return this->getStatus();
}

RFID_status_t MFRC522::getUID(UID_st *UID) {
	static bool readingCard = false, releasingPCD = false;
	if (this->m_statusRFID != RFID_SSEL_ERR) {
		if (!(this->m_timeOut) && !(this->m_operationCompleted)) {
			readingCard = false;
			releasingPCD = false;
		}
		if (!releasingPCD) {
			if (!readingCard) {
				if (!(this->isCardPICC())) return RFID_FAILURE; // this->getStatus();
				readingCard = true;
			}
			if (!(this->readCardPICC())) return RFID_BUSY; // this->getStatus();
			if (UID != nullptr) *UID = this->m_UID;
			releasingPCD = true;
		}
		// The following instruction will be useful depending on the implementation of the program.
		// if (!(this->haltPICC())) return RFID_BUSY; // this->getStatus();
	}

    return this->getStatus() == RFID_OK ? RFID_SUCCESS : RFID_FAILURE;
}

void MFRC522::dumpVersion(void) {
	if (this->m_statusRFID == RFID_SSEL_ERR) return;
	// TODO (Minor priority): Not implemented yet
}

void MFRC522::dumpUID(UID_st* UID) {
	if (this->m_statusRFID == RFID_SSEL_ERR) return;
	// TODO (Minor priority): Not implemented yet
}

void MFRC522::dumpDetails(UID_st* UID) {
	if (this->m_statusRFID == RFID_SSEL_ERR) return;
	// TODO (Minor priority): Not implemented yet
}


char *MFRC522::printUID(bool appendLineFeed) {
    static char RFIDstr[RFID_STR_SIZE];
    RFIDstr[0] = 'U';
    RFIDstr[1] = 'I';
    RFIDstr[2] = 'D';
    RFIDstr[3] = ':';
    RFIDstr[4] = ' ';
    byteToHEX(&RFIDstr[5], this->m_UID.uidByte[0]);
    RFIDstr[7] = ' ';
    byteToHEX(&RFIDstr[8], this->m_UID.uidByte[1]);
    RFIDstr[10] = ' ';
    byteToHEX(&RFIDstr[11], this->m_UID.uidByte[2]);
    RFIDstr[13] = ' ';
    byteToHEX(&RFIDstr[14], this->m_UID.uidByte[3]);
    if (appendLineFeed) { RFIDstr[16] = '\n'; RFIDstr[17] = '\0'; }
    else RFIDstr[16] = '\0';
    return RFIDstr;
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void MFRC522::callbackMethod(void) {
    if (this->m_timeOut && this->m_statusRFID != RFID_SSEL_ERR) this->m_timeOut--;
}

#pragma GCC pop_options

MFRC522::~MFRC522() {
    this->disable(); // According to the SPI implementation, the unbindSSEL function must be executed in the primitive classes.
}

//////////////////////////////
/// MFRC522 initialization ///
//////////////////////////////

void initRFID(void) {
	#if defined(SPI1_DEBUG_PINS)

    static Gpio HARDWARE_RST(SPI1_DEBUG_SSEL1);
    static MFRC522 rfid(SPI1_DEBUG_SCK, SPI1_DEBUG_MOSI, SPI1_DEBUG_MISO, SPI1_DEBUG_SSEL0, HARDWARE_RST);

    g_rfid = &rfid;

	#endif // defined(SPI1_DEBUG_PINS)
}
