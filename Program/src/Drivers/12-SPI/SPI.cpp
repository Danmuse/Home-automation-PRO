/*!
 * @file SPI.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 */

#include "SPI.h"

uint8_t SPI::m_SPI0_SSELs = 0;
uint8_t SPI::m_SPI1_SSELs = 0;

SyncCommSPI *g_SPI[SPI_MAX_CHANNELS] = { nullptr, nullptr };

SPI::SPI(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, frequencyComm_t frequency, channelSPI_t channel, bool master, bitOrder_t bitOrder, mode_t mode) : std::vector<Gpio>({SCK, MOSI, MISO}),
m_SPI{(SPI_Type *)(SPI0_BASE + channel * SPI_OFFSET_BASE)}, m_bindSSELs{0}, m_bitOrder{bitOrder}, m_receiveBufferIndexIn{0}, m_receiveBufferIndexOut{0}, m_sendBufferIndexIn{0}, m_sendBufferIndexOut{0}, m_isSending{false} {
	if (this->m_SPI == SPI0 && g_SPI[SPI_CHANNEL0] != nullptr);
	else if (this->m_SPI == SPI1 && g_SPI[SPI_CHANNEL1] != nullptr);
	else { // Initialize the corresponding SPI channel.
		enableClock();
		enableSWM();
		config(master, mode, frequency);
	}
}

/*!
 * @brief Configures the SPI channel.
 * @param master Whether the SPI is master or slave.
 * @param mode Mode of the SPI communication
 * @param frequency Frequency for the SPI communication
 */
void SPI::config(bool master, mode_t mode, frequencyComm_t frequency) {
    uint8_t CPHA = 0, CPOL = 0;

    switch (mode) {
    	// The SPI captures serial data on the first clock transition of the transfer. Data is changed on the following edge.
    	// The rest state of the clock is low.
        case MODE0: // CPOL = 0, CPHA = 0
        	break;
		// The SPI captures serial data on the first clock transition of the transfer. Data is changed on the following edge.
		// The rest state of the clock is high.
        case MODE1: // CPOL = 0, CPHA = 1
            CPHA = 1;
            break;
		// The SPI changes serial data on the first clock transition of the transfer. Data is changed on the following edge.
		// The rest state of the clock is low.
        case MODE2: // CPOL = 1, CPHA = 0
            CPOL = 1;
            break;
		// The SPI changes serial data on the first clock transition of the transfer. Data is changed on the following edge.
		// The rest state of the clock is high.
        case MODE3: // CPOL = 1, CPHA = 1
            CPHA = 1;
            CPOL = 1;
            break;
    }

    if (this->m_SPI->CFG & SPI_CFG_ENABLE_MASK) this->m_SPI->CFG &= ~SPI_CFG_ENABLE_MASK; // Disable SPI channel

    this->m_SPI->CFG |= ((0 << SPI_CFG_ENABLE_SHIFT)	// Disable SPI channel
    | (master << SPI_CFG_MASTER_SHIFT) 					// If true master mode is set
	| (this->m_bitOrder << SPI_CFG_LSBF_SHIFT)			// Most/Least Significant Bit
	| (CPHA << SPI_CFG_CPHA_SHIFT) 						// Clock phase
	| (CPOL << SPI_CFG_CPOL_SHIFT)		 				// Positive/Negative clock polarity
	| ((CPOL * 0b1111) << SPI_CFG_SPOL0_SHIFT));	 	// Positive/Negative SSELs polarity

    this->m_SPI->DIV = (FREQ_CLOCK_MCU / frequency) - 1;
    this->m_SPI->TXDATCTL |= (0b111 << SPI_TXDATCTL_LEN_SHIFT); // 8 bits per transfer
    if (this->m_SPI == SPI0) NVIC->ISER[0] |= (1 << 0);      // Enable SPI0_IRQ
    else if (this->m_SPI == SPI1) NVIC->ISER[0] |= (1 << 1); // Enable SPI1_IRQ
    this->m_SPI->TXDATCTL |= (0b1111 << SPI_TXDATCTL_TXSSEL0_N_SHIFT); // Not assert any SSELs
    this->m_SPI->CFG |= SPI_CFG_ENABLE_MASK; // Enable SPI channel
}

/*!
 * @brief Binds SSEL GPIO to the SPI channel.
 * @param SSEL GPIO to be binded.
 * @param bindedSSEL Number of the SSEL, use it for transmission and reception.
 * @return Whether or not the SSEL was added.
 */
bool SPI::bindSSEL(const Gpio &SSEL, uint8_t &bindedSSEL) {
    if (this->m_SPI == SPI0) {
    	if (this->m_SPI0_SSELs > SPI0_MAX_SSEL - 1) return false;
    	if (this->m_SPI0_SSELs < 2 && ((SWM->PINASSIGN.PINASSIGN4 & (0xFF << (!(this->m_SPI0_SSELs) ? 16 : 24))) >> (!(this->m_SPI0_SSELs) ? 16 : 24)) != 0xFF) return false;
    	else if (this->m_SPI0_SSELs >= 2 && ((SWM->PINASSIGN.PINASSIGN5 & (0xFF << (!(this->m_SPI0_SSELs - 2) ? 0 : 8))) >> (!(this->m_SPI0_SSELs - 2) ? 0 : 8)) != 0xFF) return false;
    	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
		if (this->m_SPI0_SSELs < 2) SWM->PINASSIGN.PINASSIGN4 &= (((SSEL.getBit() + SSEL.getPort() * 0x20) << (!(this->m_SPI0_SSELs) ? 16 : 24)) | ~(0xFF << (!(this->m_SPI0_SSELs) ? 16 : 24)));
		else SWM->PINASSIGN.PINASSIGN5 &= (((SSEL.getBit() + SSEL.getPort() * 0x20) << (!(this->m_SPI0_SSELs - 2) ? 0 : 8)) | ~(0xFF << (!(this->m_SPI0_SSELs - 2) ? 0 : 8)));
		SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
		bindedSSEL = this->m_SPI0_SSELs++;
		this->m_bindSSELs++;
	} else if (this->m_SPI == SPI1) {
    	if (this->m_SPI1_SSELs > SPI1_MAX_SSEL - 1) return false;
    	if (((SWM->PINASSIGN.PINASSIGN6 & (0xFF << (!(this->m_SPI1_SSELs) ? 8 : 16))) >> (!(this->m_SPI1_SSELs) ? 8 : 16)) != 0xFF) return false;
		SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
		SWM->PINASSIGN.PINASSIGN6 &= (((SSEL.getBit() + SSEL.getPort() * 0x20) << (!(this->m_SPI1_SSELs) ? 8 : 16)) | ~(0xFF << (!(this->m_SPI1_SSELs) ? 8 : 16)));
		SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
    	bindedSSEL = this->m_SPI1_SSELs++;
    	this->m_bindSSELs++;
	}
    return true;
}

/*!
 * @brief Unbinds SSEL GPIO to the channel.
 * @param SSEL GPIO to be unbinded.
 * @param unbindedSSEL Number of the SSEL, use it for transmission and reception.
 * @return Whether or not the SSEL was added.
 */
bool SPI::unbindSSEL(uint8_t unbindedSSEL) {
    if (this->m_SPI == SPI0) {
    	if (!(this->m_SPI0_SSELs)) return false;
    	if (unbindedSSEL < 2 && ((SWM->PINASSIGN.PINASSIGN4 & (0xFF << (!unbindedSSEL ? 16 : 24))) >> (!unbindedSSEL ? 16 : 24)) == 0xFF) return false;
		else if (unbindedSSEL >= 2 && ((SWM->PINASSIGN.PINASSIGN5 & (0xFF << (!(unbindedSSEL - 2) ? 0 : 8))) >> (!(unbindedSSEL - 2) ? 0 : 8)) == 0xFF) return false;
		SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
		if (unbindedSSEL < 2) SWM->PINASSIGN.PINASSIGN4 |= (0xFF << (!unbindedSSEL ? 16 : 24));
		else SWM->PINASSIGN.PINASSIGN5 |= (0xFF << (!(unbindedSSEL - 2) ? 0 : 8));
		SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
		this->m_SPI0_SSELs--;
		this->m_bindSSELs--;
	} else if (this->m_SPI == SPI1) {
    	if (!(this->m_SPI1_SSELs)) return false;
    	if (((SWM->PINASSIGN.PINASSIGN6 & (0xFF << (!unbindedSSEL ? 8 : 16))) >> (!unbindedSSEL ? 8 : 16)) == 0xFF) return false;
    	SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
		SWM->PINASSIGN.PINASSIGN6 |= (0xFF << (!unbindedSSEL ? 8 : 16));
		SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
    	this->m_SPI1_SSELs--;
    	this->m_bindSSELs--;
	}
    return true;
}

/*!
 * @brief Changes the baud rate for the SPI communication.
 * @param frequency New frequency for the SPI communication.
 */
void SPI::setBaudRate(frequencyComm_t frequency) {
	this->m_SPI->CFG &= ~SPI_CFG_ENABLE_MASK; // Disable SPI channel
	this->m_SPI->DIV = (FREQ_CLOCK_MCU / frequency) - 1;
	this->m_SPI->CFG |= SPI_CFG_ENABLE_MASK; // Enable SPI channel
}

/*!
 * @brief Enables the Slave Select for the SPI communication.
 * @param SSEL Number of the Slave.
 */
void SPI::enableSSEL(uint8_t SSEL) {
	if (this->m_SPI == SPI0) g_SPI[SPI_CHANNEL0] = this;
	if (this->m_SPI == SPI1) g_SPI[SPI_CHANNEL1] = this;
//	this->m_SPI->CFG &= ~SPI_CFG_ENABLE_MASK; // Disable SPI channel
//	this->m_SPI->TXDATCTL &= ~(1 << (SPI_TXDATCTL_TXSSEL0_N_SHIFT + SSEL));
	this->m_SPI->TXCTL &= ~(1 << (SPI_TXCTL_TXSSEL0_N_SHIFT + SSEL));
//	this->m_SPI->CFG |= SPI_CFG_ENABLE_MASK; // Enable SPI channel
}

/*!
 * @brief Disables the Slave Select for the SPI communication.
 * @param SSEL Number for the slave.
 */
void SPI::disableSSEL(uint8_t SSEL) {
	if (this->m_SPI == SPI0) g_SPI[SPI_CHANNEL0] = this;
	if (this->m_SPI == SPI1) g_SPI[SPI_CHANNEL1] = this;
//	this->m_SPI->CFG &= ~SPI_CFG_ENABLE_MASK; // Disable SPI channel
//	this->m_SPI->TXDATCTL |= (1 << (SPI_TXDATCTL_TXSSEL0_N_SHIFT + SSEL));
	this->m_SPI->TXCTL |= (1 << (SPI_TXCTL_TXSSEL0_N_SHIFT + SSEL));
//	this->m_SPI->CFG |= SPI_CFG_ENABLE_MASK; // Enable SPI channel
}

/*!
 * @brief Transmits a message via SPI.
 * @param message A pointer to the null-terminated character array (C-string) to be transmitted.
 *
 * @note The function sets a bit in the SPI status register to force an end to the current zero transfer when the transmitter completes any ongoing activity. This simulates the behavior as if the End of Transfer (EOT) flag had been set before the last transmission.
 * @warning This function assumes that the SPI instance (`m_SPI`) and related variables are properly initialized before calling.
 */
void SPI::transmit(const char *message) {
    while (*message) {
        this->pushSend(*message);
        message++;

        if (!this->m_isSending) {
        	this->m_isSending = true;
        	this->enableSendInterrupt();
        }
    }
    // Software can set this bit to force an end to the current 0 transfer
    // when the transmitter finishes any activity already in progress,
    // as if the EOT flag had been set prior to the last transmission.
    this->m_SPI->STAT |= SPI_STAT_ENDTRANSFER_MASK;
}

/*!
 * @brief Transmits an array of bytes using SPI communication.
 * @param[in] message Pointer to the array of bytes to be transmitted.
 * @param[in] length  The number of bytes to transmit from the message array.
 *
 * @note This function assumes that the SPI instance (m_SPI) is properly configured before calling.
 *       It also relies on the existence of certain member functions such as pushSend, enableSendInterrupt, and
 *       the presence of m_isSending and m_SPI->STAT.
 */
void SPI::transmitBytes(uint8_t *message, uint8_t length) {
    for (size_t index = 0; index < length; index++) {
    	this->pushSend(message[index]);
		if (!this->m_isSending) {
			this->m_isSending = true;
			this->enableSendInterrupt();
		}
    }
    // Software can set this bit to force an end to the current 0 transfer
    // when the transmitter finishes any activity already in progress,
    // as if the EOT flag had been set prior to the last transmission.
    this->m_SPI->STAT |= SPI_STAT_ENDTRANSFER_MASK;
}

/*!
 * @brief Receive data over SPI communication.
 * @param address Pointer to the address for communication.
 * @param message Reference to a variable where the received message will be stored.
 * @return Boolean indicating the success of the communication. 'true' if successful, 'false' otherwise.
 *
 * @note The function relies on the SPI object's methods for communication and interrupts.
 * @see SPI::enableReceiveInterrupt(), SPI::transmitBytes(), SPI::popReceive(), SPI::disableReceiveInterrupt()
 */
bool SPI::receive(uint8_t *address, uint8_t &message) {
    bool resultCommunication = false;
	this->enableReceiveInterrupt();
	this->transmitBytes(address);
	resultCommunication = this->popReceive(&message);
    this->disableReceiveInterrupt();
    return resultCommunication;
}

bool SPI::receiveBytes(uint8_t *address, uint8_t *message, uint8_t length, bool transmitStopByte) {
    uint8_t data, stopByte = 0x00;
    bool stopByteTransmitted = false;
    static uint8_t counter = 0;
    this->enableReceiveInterrupt();
	this->transmitBytes(address);
    while (this->popReceive(&data) && length) {
    	if (!stopByteTransmitted && transmitStopByte && counter == length - 1) {
			this->transmitBytes(&stopByte);
			stopByteTransmitted = true;
			continue;
		}
        message[counter++] = data;
        if (counter >= length) {
        	counter = 0;
            this->disableReceiveInterrupt();
            return true;
        }
    }
    this->disableReceiveInterrupt();
    return false;
}

bool SPI::receive(uint8_t *address, char *message) {
    uint8_t data;
    static uint32_t counter = 0;
    this->enableReceiveInterrupt();
	this->transmitBytes(address);
    while (this->popReceive(&data)) {
    	message[counter++] = data;
        if (data == '\r' || data == '\n') {
        	message[counter] = '\0';
        	counter = 0;
            this->disableReceiveInterrupt();
            return true;
        }
    }
    this->disableReceiveInterrupt();
    return false;
}

void SPI::enableClock(void) {
    if (this->m_SPI == SPI0) {
    	g_SPI[SPI_CHANNEL0] = this;
        SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SPI0_MASK; // Enable SPI channels
        SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_SPI0_RST_N_MASK;
        SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_SPI0_RST_N_MASK;
    } else if (this->m_SPI == SPI1) {
    	g_SPI[SPI_CHANNEL1] = this;
        SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SPI1_MASK; // Enable SPI channels
        SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_SPI1_RST_N_MASK;
        SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_SPI1_RST_N_MASK;
    }

	// Select Clock PPAL.: FCLKSEL
    if (this->m_SPI == SPI0) SYSCON->FCLKSEL[9] = 1;
	else if (this->m_SPI == SPI1) SYSCON->FCLKSEL[10] = 1;
}

/*!
 * @brief Enable the Switch Matrix (SWM) for SPI configuration.
 * @details This function enables the Switch Matrix (SWM) for configuring the SPI (Serial Peripheral Interface).
 * It sets the necessary pin assignments for SCK, MOSI, and MISO based on the selected SPI instance (SPI0 or SPI1).
 *
 * @note This function assumes that the SPI instance has been previously configured.
 */
void SPI::enableSWM(void) {
    SYSCON->SYSAHBCLKCTRL0 |= SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
	if (this->m_SPI == SPI0) {
		SWM->PINASSIGN.PINASSIGN3 &= (((at(SCK_IDX).getBit() + at(SCK_IDX).getPort() * 0x20) << 24) | ~(0xFF << 24));
		SWM->PINASSIGN.PINASSIGN4 &= ((((at(MOSI_IDX).getBit() + at(MOSI_IDX).getPort() * 0x20) << 0) | ((at(MISO_IDX).getBit() + at(MISO_IDX).getPort() * 0x20) << 8)) | ~(0xFFFF << 0));
	} else if (this->m_SPI == SPI1) {
		SWM->PINASSIGN.PINASSIGN5 &= ((((at(SCK_IDX).getBit() + at(SCK_IDX).getPort() * 0x20) << 16) | ((at(MOSI_IDX).getBit() + at(MOSI_IDX).getPort() * 0x20) << 24)) | ~(0xFFFF << 16));
		SWM->PINASSIGN.PINASSIGN6 &= (((at(MISO_IDX).getBit() + at(MISO_IDX).getPort() * 0x20) << 0) | ~(0xFF << 0));
	}
    SYSCON->SYSAHBCLKCTRL0 &= ~SYSCON_SYSAHBCLKCTRL0_SWM_MASK;
}

/*!
 * @brief Pushes data to the receive buffer.
 * @param data Data to be pushed.
 */
void SPI::pushReceive(uint8_t data) {
	this->m_receiveBuffer[this->m_receiveBufferIndexIn++] = data;
	this->m_receiveBufferIndexIn %= RECEIVE_BUFFER_SIZE;
}

/*!
 * @brief Pops data from the receive buffer.
 * @param[out] data Pointer to where the data will be stored.
 * @return Whether or not there was data to pop.
 */
bool SPI::popReceive(uint8_t *data) {
    if (this->m_receiveBufferIndexIn != this->m_receiveBufferIndexOut) {
        *data = this->m_receiveBuffer[this->m_receiveBufferIndexOut++];
        this->m_receiveBufferIndexOut %= RECEIVE_BUFFER_SIZE;
        return true;
    }
    return false;
}

/*!
 * @brief Pushes data to the send buffer.
 * @param data Data to be pushed.
 */
void SPI::pushSend(uint8_t data) {
	this->m_sendBuffer[this->m_sendBufferIndexIn++] = data;
	this->m_sendBufferIndexIn %= SEND_BUFFER_SIZE;
}

/*!
 * @brief Pops data from the send buffer.
 * @param[out] data Pointer to where the data will be stored.
 * @return Whether or not there was data to pop.
 */
bool SPI::popSend(uint8_t *data) {
    if (this->m_sendBufferIndexIn != this->m_sendBufferIndexOut) {
        *data = this->m_sendBuffer[this->m_sendBufferIndexOut++];
        this->m_sendBufferIndexOut %= SEND_BUFFER_SIZE;
        return true;
    }
    return false;
}

void SPI::enableReceiveInterrupt(void) {
	this->m_SPI->INTENSET |= SPI_INTENSET_RXRDYEN_MASK; // Enable RXRDY interrupt
	//                    | SPI_INTENSET_SSAEN_MASK     // Enable when the Slave Select is asserted
	//                    | SPI_INTENSET_SSDEN_MASK     // Enable when the Slave Select is deasserted
}

void SPI::enableSendInterrupt(void) {
	this->m_SPI->INTENSET |= SPI_INTENSET_TXRDYEN_MASK; // Enable TXRDY interrupt
}

void SPI::disableReceiveInterrupt(void) {
	this->m_SPI->INTENCLR |= SPI_INTENCLR_RXRDYEN_MASK; // Disable RXRDY interrupt
	//                    | SPI_INTENCLR_SSAEN_MASK     // Disable when the Slave Select is asserted
	//                    | SPI_INTENCLR_SSDEN_MASK     // Disable when the Slave Select is deasserted
}

void SPI::disableSendInterrupt(void) {
	this->m_SPI->INTENCLR |= SPI_INTENCLR_TXRDYEN_MASK; // Disable TXRDY interrupt
}

void SPI::SPI_IRQHandler(void) {
    if (this->m_SPI->STAT & SPI_STAT_RXRDY_MASK) {
        uint16_t data = this->m_SPI->RXDAT;
        // Only reading 8 bits because TXDATCTL.LEN register is set to 8
        if (this->m_SPI->INTENSET & SPI_INTENSET_RXRDYEN_MASK) this->pushReceive((uint8_t)data);
    }
    if (this->m_SPI->STAT & SPI_STAT_TXRDY_MASK) {
        uint16_t data;
        // Only writing 8 bits because TXDATCTL.LEN register is set to 8
        if (this->popSend((uint8_t*)(&data))) this->m_SPI->TXDAT = data;
        else {
        	this->m_isSending = false;
            this->disableSendInterrupt();
        }
    }
}

SPI::~SPI() {
	if (this->m_SPI == SPI0) {
		// Remember to execute the unbindSSEL function on primitive classes where they are protected or publicly inherited from SPI.
		this->m_SPI0_SSELs -= this->m_bindSSELs;
		if (!(this->m_SPI0_SSELs)) {
			this->disableReceiveInterrupt();
			this->disableSendInterrupt();
			NVIC->ICER[0] |= (1 << 0); // Disable SPI0_IRQ
		}
	} else if (this->m_SPI == SPI1) {
		// Remember to execute the unbindSSEL function on primitive classes where they are protected or publicly inherited from SPI.
		this->m_SPI1_SSELs -= this->m_bindSSELs;
		if (!(this->m_SPI1_SSELs)) {
			this->disableReceiveInterrupt();
			this->disableSendInterrupt();
			NVIC->ICER[0] |= (1 << 1); // Disable SPI1_IRQ
		}
	}
}

void SPI0_IRQHandler(void) {
	g_SPI[SPI::SPI_CHANNEL0]->SPI_IRQHandler();
}

void SPI1_IRQHandler(void) {
	g_SPI[SPI::SPI_CHANNEL1]->SPI_IRQHandler();
}
