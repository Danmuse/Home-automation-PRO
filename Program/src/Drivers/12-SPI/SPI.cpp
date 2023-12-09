/*!
 * @file SPI.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 */

#include "SPI.h"

SyncCommSPI *g_SPI[SPI_MAX_CHANNELS] = { nullptr, nullptr };

SPI::SPI(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, std::vector<Gpio> SSEL, frequencyComm_t frequency, channelSPI_t channel, bool master, bitOrder_t bitOrder, mode_t mode) : std::vector<Gpio>({SCK, MOSI, MISO}),
m_SPI{(SPI_Type *)(SPI0_BASE + channel * SPI_OFFSET_BASE)}, m_bitOrder{bitOrder}, m_receiveBufferIndexIn{0}, m_receiveBufferIndexOut{0}, m_sendBufferIndexIn{0}, m_sendBufferIndexOut{0}, m_isSending{false} {
	// Maybe do something if it already exists
	if (g_SPI[channel] == nullptr) g_SPI[channel] = this;

    if (channel == FST_SPI) this->m_maxSSELSize = SPI0_MAX_SSEL;
    else if (channel == SND_SPI) this->m_maxSSELSize = SPI1_MAX_SSEL;

    uint8_t size = std::min((int) this->m_SSEL.size(), (int) this->m_maxSSELSize);
    std::copy(this->m_SSEL.begin(), this->m_SSEL.begin() + size, std::back_inserter(this->m_SSEL));

    config(master, mode, frequency);
    enableClock();
    enableSWM();
}

SPI::SPI(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, frequencyComm_t frequency, channelSPI_t channel) :
SPI(SCK, MOSI, MISO, std::vector<Gpio>(), frequency, channel) { }

void SPI::config(bool master, mode_t mode, frequencyComm_t frequency) const {
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

    this->m_SPI->CFG |= ((0 << 0)     // Disable SPI channel
    | (master << 2) 				  // If true master mode is set
	| (this->m_bitOrder << 3)		  // LSB first
	| (CPHA << 4) 					  // Clock phase
	| (CPOL << 5)); 				  // Clock polarity
    // Configure SSEL polarity on device implementations
    // TODO: Maybe configure delays
    this->m_SPI->DIV = (FREQ_CLOCK_MCU / frequency) - 1;

    this->m_SPI->TXDATCTL |= (0b111 << 24); // 8 bits per transfer

    if (this->m_SPI == SPI0) NVIC->ISER[0] |= (1 << 0); // Enable SPI0_IRQ
    else if (this->m_SPI == SPI1) NVIC->ISER[0] |= (1 << 1); // Enable SPI1_IRQ
    this->m_SPI->INTENSET |= (1 << 0); // Enable RXRDY interrupt
    //                    | (1 << 4)   // Enable when the Slave Select is asserted
    //                    | (1 << 5)   // Enable when the Slave Select is deasserted.

    this->m_SPI->TXDATCTL |= (0b1111 << 16); // Not assert any SSELs

    this->m_SPI->CFG |= (1 << 0); // Enable SPI channel
}

/* @brief Adds SSEL GPIO to the channel
 * @param SSEL GPIO to be added
 * @param SSELNumber (output) Number of the SSEL, use it for transmission and reception
 * @return whether or not the SSEL was added
 */
bool SPI::bindSSEL(const Gpio &SSEL, uint8_t &SSELNumber) {
    if (this->m_SSEL.size() < this->m_maxSSELSize) {
        SSELNumber = this->m_SSEL.size();
        this->m_SSEL.push_back(SSEL);
        if (this->m_SPI == SPI0) {
            SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
			for (size_t i = 0; i < this->m_SSEL.size() + 1; i++) {
				if (i == 0) SWM->PINASSIGN.PINASSIGN4 &= (((this->m_SSEL[i].getBit() + this->m_SSEL[i].getPort() * 0x20) << 16) | ~(0xFF << 16));
				else if (i == 1) SWM->PINASSIGN.PINASSIGN4 &= (((this->m_SSEL[i].getBit() + this->m_SSEL[i].getPort() * 0x20) << 24) | ~(0xFF << 24));
				else if (i == 2) SWM->PINASSIGN.PINASSIGN5 &= (((this->m_SSEL[i].getBit() + this->m_SSEL[i].getPort() * 0x20) << 0) | ~(0xFF << 0));
				else if (i == 3) SWM->PINASSIGN.PINASSIGN5 &= (((this->m_SSEL[i].getBit() + this->m_SSEL[i].getPort() * 0x20) << 8) | ~(0xFF << 8));
			}
		    SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
        } else if (this->m_SPI == SPI1) {
            SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
			for (size_t i = 0; i < this->m_SSEL.size() + 1; i++) {
				if (i == 0) SWM->PINASSIGN.PINASSIGN6 &= (((this->m_SSEL[i].getBit() + this->m_SSEL[i].getPort() * 0x20) << 8) | ~(0xFF << 8));
				else if (i == 1) SWM->PINASSIGN.PINASSIGN6 &= (((this->m_SSEL[i].getBit() + this->m_SSEL[i].getPort() * 0x20) << 16) | ~(0xFF << 16));
			}
		    SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
        }
        return true;
    }
    return false;
}

void SPI::setBaudRate(frequencyComm_t frequency) const {
	this->m_SPI->CFG &= ~(1 << 0); // Enable SPI channel
	this->m_SPI->DIV = (FREQ_CLOCK_MCU / frequency) - 1;
	this->m_SPI->CFG |= (1 << 0); // Enable SPI channel
}

void SPI::enableSendInterrupt(void) const {
	this->m_SPI->INTENSET |= (1 << 1); // Enable TXRDY interrupt
}

void SPI::disableSendInterrupt(void) const {
	this->m_SPI->INTENCLR |= (1 << 1); // Disable TXRDY interrupt
}

void SPI::enableClock(void) const {
    if (this->m_SPI == SPI0) {
        SYSCON->SYSAHBCLKCTRL0 |= (1 << SPI0_CLK_REG); // Enable SPI channels
        // Full reset of the module
        SYSCON->PRESETCTRL0 &= ~(1 << SPI0_CLK_REG);
        SYSCON->PRESETCTRL0 |= (1 << SPI0_CLK_REG);
    } else if (this->m_SPI == SPI1) {
        SYSCON->SYSAHBCLKCTRL0 |= (1 << SPI1_CLK_REG); // Enable SPI channels
        // Full reset of the module
        SYSCON->PRESETCTRL0 &= ~(1 << SPI1_CLK_REG);
        SYSCON->PRESETCTRL0 |= (1 << SPI1_CLK_REG);
    }
}

void SPI::enableSWM(void) const {
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_SPI == SPI0) {
		SWM->PINASSIGN.PINASSIGN3 &= (((at(SCK_IDX).getBit() + at(SCK_IDX).getPort() * 0x20) << 24) | ~(0xFF << 24));
		SWM->PINASSIGN.PINASSIGN4 &= ((((at(MOSI_IDX).getBit() + at(MOSI_IDX).getPort() * 0x20) << 0) | ((at(MISO_IDX).getBit() + at(MISO_IDX).getPort() * 0x20) << 8)) | ~(0xFFFF << 0));
	} else if (this->m_SPI == SPI1) {
		SWM->PINASSIGN.PINASSIGN5 &= ((((at(SCK_IDX).getBit() + at(SCK_IDX).getPort() * 0x20) << 16) | ((at(MOSI_IDX).getBit() + at(MOSI_IDX).getPort() * 0x20) << 24)) | ~(0xFFFF << 16));
		SWM->PINASSIGN.PINASSIGN6 &= (((at(MISO_IDX).getBit() + at(MISO_IDX).getPort() * 0x20) << 0) | ~(0xFF << 0));
	}
    SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void SPI::enableSSEL(uint8_t SSEL) const {
	this->m_SPI->TXDATCTL &= ~(1 << (16 + SSEL));
}

void SPI::disableSSEL(uint8_t SSEL) const {
	this->m_SPI->TXDATCTL |= (1 << (16 + SSEL)); // Maybe I should use EOT to set the end
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

void SPI::transmit(uint8_t *message, uint8_t length) {
    for (int i = 0; i < length; i++) {
        pushSend(message[i]);
		if (!this->m_isSending) {
			this->m_isSending = true;
			enableSendInterrupt();
		}
    }
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

void SPI::SPI_IRQHandler(void) {
    uint16_t stat = this->m_SPI->STAT; // Not 32bits because last 17 bits are reserved

    if (stat & (1 << 0)) {
        uint8_t data = this->m_SPI->RXDAT; // Only reading 8 bits because LEN is set to 8
        pushReceive(data);
    }
    if (stat & (1 << 1)) {
        uint8_t data;
        if (popSend(&data)) {
        	this->m_SPI->TXDAT = data;
        } else {
        	this->m_isSending = false;
            disableSendInterrupt();
        }
    }
}

SPI::~SPI() {
    this->disableSendInterrupt();
    if (this->m_SPI == SPI0) NVIC->ISER[0] &= ~(1 << 0); // Disable SPI0_IRQ
    else if (this->m_SPI == SPI1) NVIC->ISER[0] &= ~(1 << 1); // Disable SPI1_IRQ
}

void SPI0_IRQHandler(void) {
	g_SPI[0]->SPI_IRQHandler();
}

void SPI1_IRQHandler(void) {
	g_SPI[1]->SPI_IRQHandler();
}
