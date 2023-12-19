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
    this->m_SPI->INTENSET |= SPI_INTENSET_RXRDYEN_MASK; // Enable RXRDY interrupt
    //                    | SPI_INTENSET_SSAEN_MASK     // Enable when the Slave Select is asserted
    //                    | SPI_INTENSET_SSDEN_MASK     // Enable when the Slave Select is deasserted.
    this->m_SPI->TXDATCTL |= (0b1111 << SPI_TXDATCTL_TXSSEL0_N_SHIFT); // Not assert any SSELs
    this->m_SPI->CFG |= SPI_CFG_ENABLE_MASK; // Enable SPI channel
}

/* @brief Binds SSEL GPIO to the channel.
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

/* @brief Unbinds SSEL GPIO to the channel.
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

void SPI::setBaudRate(frequencyComm_t frequency) {
	this->m_SPI->CFG &= ~SPI_CFG_ENABLE_MASK; // Disable SPI channel
	this->m_SPI->DIV = (FREQ_CLOCK_MCU / frequency) - 1;
	this->m_SPI->CFG |= SPI_CFG_ENABLE_MASK; // Enable SPI channel
}

void SPI::enableSSEL(uint8_t SSEL) {
	this->m_SPI->TXCTL &= ~(1 << (SPI_TXCTL_TXSSEL0_N_SHIFT + SSEL));
}

void SPI::disableSSEL(uint8_t SSEL) {
	this->m_SPI->TXCTL |= (1 << (SPI_TXCTL_TXSSEL0_N_SHIFT + SSEL));
}

void SPI::transmit(const char *message) {
    while (*message) {
        pushSend(*message);
        message++;

        if (!this->m_isSending) {
        	this->m_isSending = true;
            enableSendInterrupt();
        }
    }
    // Software can set this bit to force an end to the current 0 transfer
    // when the transmitter finishes any activity already in progress,
    // as if the EOT flag had been set prior to the last transmission.
    this->m_SPI->STAT |= SPI_STAT_ENDTRANSFER_MASK;
}

void SPI::transmitBytes(uint8_t *message, uint8_t length) {
    for (size_t index = 0; index < length; index++) {
        pushSend(message[index]);
		if (!this->m_isSending) {
			this->m_isSending = true;
			enableSendInterrupt();
		}
    }
    // Software can set this bit to force an end to the current 0 transfer
    // when the transmitter finishes any activity already in progress,
    // as if the EOT flag had been set prior to the last transmission.
    this->m_SPI->STAT |= SPI_STAT_ENDTRANSFER_MASK;
}

bool SPI::receive(uint8_t &message) {
    return popReceive(&message);
}

bool SPI::receive(uint8_t *message, uint8_t length) {
    uint8_t data;
    static uint32_t cont = 0;
    while (this->popReceive(&data)) {
        message[cont++] = data;
        if (cont >= length) {
            cont = 0;
            return true;
        }
    }
    return false;
}

bool SPI::receive(char *message) {
    uint8_t data;
    static uint32_t cont = 0;
    while (this->popReceive(&data)) {
    	message[cont++] = data;
        if (data == '\r' || data == '\n') {
        	message[cont] = '\0';
            cont = 0;
            return true;
        }
    }
    return false;
}

void SPI::enableClock(void) {
    if (this->m_SPI == SPI0 && !(g_SPI[SPI_CHANNEL0])) {
    	g_SPI[SPI_CHANNEL0] = this;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 11); // Enable SPI channels
        SYSCON->PRESETCTRL0 &= ~(1 << 11);
        SYSCON->PRESETCTRL0 |= (1 << 11);
    } else if (this->m_SPI == SPI1 && !(g_SPI[SPI_CHANNEL1])) {
    	g_SPI[SPI_CHANNEL1] = this;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 12); // Enable SPI channels
        SYSCON->PRESETCTRL0 &= ~(1 << 12);
        SYSCON->PRESETCTRL0 |= (1 << 12);
    }

	// Select Clock PPAL.: FCLKSEL
    if (this->m_SPI == SPI0) SYSCON->FCLKSEL[9] = 1;
	else if (this->m_SPI == SPI1) SYSCON->FCLKSEL[10] = 1;
}

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

void SPI::pushReceive(uint8_t data) {
	this->m_receiveBuffer[this->m_receiveBufferIndexIn++] = data;
	this->m_receiveBufferIndexIn %= RECEIVE_BUFFER_SIZE;
}

bool SPI::popReceive(uint8_t *data) {
    if (this->m_receiveBufferIndexIn != this->m_receiveBufferIndexOut) {
        *data = this->m_receiveBuffer[this->m_receiveBufferIndexOut++];
        this->m_receiveBufferIndexOut %= RECEIVE_BUFFER_SIZE;
        return true;
    }
    return false;
}

void SPI::pushSend(uint8_t data) {
	this->m_sendBuffer[this->m_sendBufferIndexIn++] = data;
	this->m_sendBufferIndexIn %= SEND_BUFFER_SIZE;
}

bool SPI::popSend(uint8_t *data) {
    if (this->m_sendBufferIndexIn != this->m_sendBufferIndexOut) {
        *data = this->m_sendBuffer[this->m_sendBufferIndexOut++];
        this->m_sendBufferIndexOut %= SEND_BUFFER_SIZE;
        return true;
    }
    return false;
}

void SPI::enableSendInterrupt(void) {
	this->m_SPI->INTENSET = (1 << 1); // Enable TXRDY interrupt
}

void SPI::disableSendInterrupt(void) {
	this->m_SPI->INTENCLR = (1 << 1); // Disable TXRDY interrupt
}

void SPI::SPI_IRQHandler(void) {
    uint16_t stat = this->m_SPI->STAT; // Not 32bits because last 17 bits are reserved

    if (stat & (1 << 0)) {
        uint16_t data = this->m_SPI->RXDAT; // Only reading 8 bits because LEN is set to 8
        pushReceive((uint8_t)data);
    }
    if (stat & (1 << 1)) {
        uint16_t data;
        if (popSend((uint8_t*)(&data))) this->m_SPI->TXDAT = data;
        else {
        	this->m_isSending = false;
            disableSendInterrupt();
        }
    }
}

SPI::~SPI() {
	if (this->m_SPI == SPI0) {
		// Remember to execute the unbindSSEL function on primitive classes where they are protected or publicly inherited from SPI.
		this->m_SPI0_SSELs -= this->m_bindSSELs;
		if (!(this->m_SPI0_SSELs)) {
			this->disableSendInterrupt();
			NVIC->ISER[0] &= ~(1 << 0); // Disable SPI0_IRQ
		}
	} else if (this->m_SPI == SPI1) {
		// Remember to execute the unbindSSEL function on primitive classes where they are protected or publicly inherited from SPI.
		this->m_SPI1_SSELs -= this->m_bindSSELs;
		if (!(this->m_SPI1_SSELs)) {
			this->disableSendInterrupt();
			NVIC->ISER[0] &= ~(1 << 1); // Disable SPI1_IRQ
		}
	}
}

void SPI0_IRQHandler(void) {
	g_SPI[SPI::SPI_CHANNEL0]->SPI_IRQHandler();
}

void SPI1_IRQHandler(void) {
	g_SPI[SPI::SPI_CHANNEL1]->SPI_IRQHandler();
}
