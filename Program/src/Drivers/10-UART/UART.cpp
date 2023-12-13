/*/*!
 * @file UART.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 */

#include "UART.h"

AsyncComm *g_usart[MAX_USART_CHANNELS] = { nullptr, nullptr, nullptr, nullptr, nullptr };

UART *g_usb = nullptr;

UART::UART(const Gpio& RX, const Gpio& TX, channelUART_t channel, uint32_t baudrate, data_bits_t data_bits, parity_t parity) : std::vector<Gpio>({RX, TX}),
m_usart{(USART_Type *)(USART0_BASE + channel * USART_OFFSET_BASE)},
m_indexRXIn{0}, m_indexRXOut{0}, m_indexTXIn{0}, m_indexTXOut{0}, m_flagTX{false} {
	if (this->m_usart == USART0 && g_usart[UART0] != nullptr);
	else if (this->m_usart == USART1 && g_usart[UART1] != nullptr);
	else if (this->m_usart == USART2 && g_usart[UART2] != nullptr);
	else if (this->m_usart == USART3 && g_usart[UART3] != nullptr);
	else if (this->m_usart == USART3 && g_usart[UART4] != nullptr);
	else { // Initialize the corresponding USART channel.
		this->m_bufferRX = new uint8_t[RX_BUFFER_SIZE];
		this->m_bufferTX = new uint8_t[TX_BUFFER_SIZE];

		this->enableClock();
		this->enableSWM();
		this->config(baudrate, data_bits, parity);
	}
}

void UART::pushRX(uint8_t data) {
	this->m_bufferRX[this->m_indexRXIn++] = data;
	this->m_indexRXIn %= RX_BUFFER_SIZE;
}

bool UART::popRX(uint8_t *data) {
	if (this->m_indexRXIn != this->m_indexRXOut) {
		*data = this->m_bufferRX[this->m_indexRXOut++];
		this->m_indexRXOut %= RX_BUFFER_SIZE;
		return true;
	}
	return false;
}

void UART::pushTX(uint8_t data) {
	this->m_bufferTX[this->m_indexTXIn++] = data;
	this->m_indexTXIn %= TX_BUFFER_SIZE;
}

bool UART::popTX(uint8_t *data) {
	if (this->m_indexTXIn != this->m_indexTXOut) {
		*data = this->m_bufferTX[this->m_indexTXOut++];
		this->m_indexTXOut %= TX_BUFFER_SIZE;
		return true;
	}
	return false;
}

void UART::transmit(const char *message) {
	while (*message) {
		this->pushTX(*message);
		message++;

		if (!this->m_flagTX) {
			this->m_flagTX = true;
			this->enableInterrupt();
		}
	}
}

void UART::transmit(const char *message, uint8_t length) {
	for (uint8_t index = 0; index < length; index++) {
		this->pushTX(message[index]);
		if (!this->m_flagTX) {
			this->m_flagTX = true;
            this->enableInterrupt();
		}
	}
}

bool UART::receive(char *message, uint8_t length) {
	uint8_t data;
	static uint8_t cont = 0;
	char *ptr = message;
	while (this->popRX(&data)) {
		ptr[cont++] = data;
		if (cont >= length || (ptr[cont - 1] == '\r' || ptr[cont - 1] == '\n')) {
			ptr[cont] = '\0';
			cont = 0;
			return true;
		}
	}
	return false;
}

void UART::enableInterrupt(void) {
	this->m_usart->INTENSET = (1 << 2); // Enable TX interruption.
}

void UART::disableInterrupt(void) {
	this->m_usart->INTENCLR = (1 << 2); // Disable TX interruption.
}

void UART::enableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_usart == USART0) SWM->PINASSIGN.PINASSIGN0 &= ((((at(TX_IDX).getBit() + at(TX_IDX).getPort() * 0x20) << 0) | ((at(RX_IDX).getBit() + at(RX_IDX).getPort() * 0x20) << 8)) | ~(0xFFFF << 0));
	else if (this->m_usart == USART1) SWM->PINASSIGN.PINASSIGN1 &= ((((at(TX_IDX).getBit() + at(TX_IDX).getPort() * 0x20) << 8) | ((at(RX_IDX).getBit() + at(RX_IDX).getPort() * 0x20) << 16)) | ~(0xFFFF << 8));
	else if (this->m_usart == USART2) SWM->PINASSIGN.PINASSIGN2 &= ((((at(TX_IDX).getBit() + at(TX_IDX).getPort() * 0x20) << 16) | ((at(RX_IDX).getBit() + at(RX_IDX).getPort() * 0x20) << 24)) | ~(0xFFFF << 16));
	else if (this->m_usart == USART3) {
		SWM->PINASSIGN.PINASSIGN11 &= (((at(TX_IDX).getBit() + at(TX_IDX).getPort() * 0x20) << 24) | ~(0xFF << 24));
		SWM->PINASSIGN.PINASSIGN12 &= (((at(RX_IDX).getBit() + at(RX_IDX).getPort() * 0x20) << 0) | ~(0xFF << 0));
	} else if (this->m_usart == USART4) SWM->PINASSIGN.PINASSIGN12 &= ((((at(TX_IDX).getBit() + at(TX_IDX).getPort() * 0x20) << 16) | ((at(RX_IDX).getBit() + at(RX_IDX).getPort() * 0x20) << 24)) | ~(0xFFFF << 16));
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void UART::config(uint32_t baudrate, data_bits_t data_bits, parity_t parity) {
	this->m_usart->CFG = ((0 << 0) // 0 = DISABLE 		1 = ENABLE
	| (data_bits << 2) 		  	   // 0 = 7 BITS 		1 = 8 BITS 		2 = 9 BITS
	| (parity << 4)		 		   // 0 = NOPARITY 		2 = PAR 		3 = IMPAR
	| (0 << 6)			 		   // 0 = 1 BIT STOP  	1 = 2 BIT STOP
	| (0 << 9)			 		   // 0 = NOFLOWCONTROL 1 = FLOWCONTROL
	| (0 << 11)			 		   // 0 = ASINCRONICA 	1 = SINCRONICA
	| (0 << 14));				   // 0 = SLAVE 		1 = MASTER

	// The default value of the OSR register is 16
	this->m_usart->BRG = ((FREQ_CLOCK_MCU / baudrate) / (this->m_usart->OSR + 1)) - 1;
	this->m_usart->CTL = 0;

	this->m_usart->INTENSET = (1 << 0);	// Enable RX interruption.

	if (this->m_usart == USART0) NVIC->ISER[0] |= (1 << 3);  // Enable UART0_IRQ
	else if (this->m_usart == USART1) NVIC->ISER[0] |= (1 << 4);  // Enable UART1_IRQ
	else if (this->m_usart == USART2) NVIC->ISER[0] |= (1 << 5);  // Enable UART2_IRQ
	else if (this->m_usart == USART3) NVIC->ISER[0] |= (1 << 30); // Enable UART3_IRQ
	else if (this->m_usart == USART4) NVIC->ISER[0] |= (1 << 31); // Enable UART4_IRQ

	this->m_usart->CFG |= (1 << 0); // Enable USART
}

void UART::setBaudRate(uint32_t baudrate) {
	this->m_usart->CFG &= ~(1 << 0);	// Disable UART
	this->m_usart->BRG = ((FREQ_CLOCK_MCU / baudrate) / (this->m_usart->OSR + 1)) - 1;	// Change baudrate
	this->m_usart->CFG |= (1 << 0);		// Enable UART
}

void UART::enableClock(void) {
	if (this->m_usart == USART0) {
		g_usart[UART0] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 14);
	} else if (this->m_usart == USART1) {
		g_usart[UART1] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 15);
	} else if (this->m_usart == USART2) {
		g_usart[UART2] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 16);
	} else if (this->m_usart == USART3) {
		g_usart[UART3] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 30);
	} else if (this->m_usart == USART4) {
		g_usart[UART4] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 31);
	}

	// Select Clock PPAL.: FCLKSEL
	if (this->m_usart == USART0) SYSCON->FCLKSEL[0] = 1;
	else if (this->m_usart == USART1) SYSCON->FCLKSEL[1] = 1;
	else if (this->m_usart == USART2) SYSCON->FCLKSEL[2] = 1;
	else if (this->m_usart == USART3) SYSCON->FCLKSEL[3] = 1;
	else if (this->m_usart == USART4) SYSCON->FCLKSEL[4] = 1;
}

void UART::UART_IRQHandler(void) {
	uint8_t endTransmission, data;
	uint32_t stat = this->m_usart->STAT;

	if (stat & (1 << 0)) {
		data = (uint8_t) this->m_usart->RXDAT;
		this->pushRX(data);
	}

	if (stat & (1 << 2)) {
		endTransmission = this->popTX(&data);
		if (endTransmission) this->m_usart->TXDAT = (uint8_t)data;
		else {
            this->disableInterrupt();
			this->m_flagTX = false;
		}
	}
}

UART::~UART() {
	this->disableInterrupt();
	if (this->m_usart == USART0) NVIC->ISER[0] &= ~(1 << 3);  // Disable UART0_IRQ
	else if (this->m_usart == USART1) NVIC->ISER[0] &= ~(1 << 4);  // Disable UART1_IRQ
	else if (this->m_usart == USART2) NVIC->ISER[0] &= ~(1 << 5);  // Disable UART2_IRQ
	else if (this->m_usart == USART3) NVIC->ISER[0] &= ~(1 << 30); // Disable UART3_IRQ
	else if (this->m_usart == USART4) NVIC->ISER[0] &= ~(1 << 31); // Disable UART4_IRQ
	delete[] this->m_bufferRX;
	delete[] this->m_bufferTX;
}

///////////////////////////
/// USB0 initialization ///
///////////////////////////

void initUSB0(void) {
	#ifdef USB0_PINS

	static UART usb(RX1_IN, TX1_OUT, UART::UART1);

	g_usb = &usb;

	#endif // USB0_PINS
}

void UART0_IRQHandler(void) {
	g_usart[UART::UART0]->UART_IRQHandler();
}

void UART1_IRQHandler(void) {
	g_usart[UART::UART1]->UART_IRQHandler();
}

void UART2_IRQHandler(void) {
	g_usart[UART::UART2]->UART_IRQHandler();
}

void UART3_IRQHandler(void) {
	g_usart[UART::UART3]->UART_IRQHandler();
}

void UART4_IRQHandler(void) {
	g_usart[UART::UART4]->UART_IRQHandler();
}
