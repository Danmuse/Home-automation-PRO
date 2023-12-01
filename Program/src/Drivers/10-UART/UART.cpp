/*/*!
 * @file UART.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 */

#include "UART.h"

AsyncComm *g_usart[MAX_USART_CHANNELS] = { nullptr, nullptr, nullptr, nullptr, nullptr };

UART *g_usb = nullptr;

UART::UART(const Gpio& RX, const Gpio& TX, channel_t channel, uint32_t baudrate, data_bits_t data_bits, parity_t parity, uint32_t maxRX, uint32_t maxTX) : std::vector<Gpio>({RX, TX}),
m_usart{(USART_Type *)(USART0_BASE + channel*USART_OFFSET_BASE)} {
	m_bufferRX = new uint8_t[maxRX];
	m_indexRXIn = m_indexRXOut = 0;
	m_maxRX = maxRX;

	m_bufferTX = new uint8_t[maxTX];
	m_indexTXIn = m_indexTXOut = 0;
	m_maxTX = maxTX;

	m_flagTX = false;

	this->EnableClock();
	this->EnableSWM();
	this->Config(baudrate, data_bits, parity);
}

void UART::pushRX(uint8_t data) {
	this->m_bufferRX[this->m_indexRXIn++] = data;
	this->m_indexRXIn %= this->m_maxRX;
}

bool UART::popRX(uint8_t *data) {
	if (this->m_indexRXIn != this->m_indexRXOut) {
		*data = this->m_bufferRX[this->m_indexRXOut++];
		this->m_indexRXOut %= this->m_maxRX;
		return true;
	}
	return false;
}

void UART::pushTX(uint8_t data) {
	this->m_bufferTX[this->m_indexTXIn++] = data;
	this->m_indexTXIn %= this->m_maxTX;
}

bool UART::popTX(uint8_t *data) {
	if (this->m_indexTXIn != this->m_indexTXOut) {
		*data = this->m_bufferTX[this->m_indexTXOut++];
		this->m_indexTXOut %= this->m_maxTX;
		return true;
	}
	return false;
}

void UART::Transmit(const char *message) {
	while (*message) {
		this->pushTX(*message);
		message++;

		if (!this->m_flagTX) {
			this->m_flagTX = true;
			this->EnableInterrupt();
		}
	}
}

void UART::Transmit(const char *message, uint32_t n) {
	for (uint32_t index = 0; index < n; index++) {
		this->pushTX(message[index]);
		if (!this->m_flagTX) {
			this->m_flagTX = true;
			this->EnableInterrupt();
		}
	}
}

bool UART::Receive(char *message, uint32_t n) {
	uint8_t data;
	static uint32_t cont = 0;
	char *ptr = message;
	while (this->popRX(&data)) {
		ptr[cont++] = data;
		if (cont >= n || (ptr[cont - 1] == '\r' || ptr[cont - 1] == '\n')) {
			ptr[cont] = '\0';
			cont = 0;
			return true;
		}
	}
	return false;
}

void UART::EnableInterrupt(void) {
	this->m_usart->INTENSET = (1 << 2); // Enable TX interruption.
}

void UART::DisableInterrupt(void) {
	this->m_usart->INTENCLR = (1 << 2); // Disable TX interruption.
}

void UART::EnableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_usart == USART0) SWM->PINASSIGN.PINASSIGN0 &= ((((at(TX_IDX).GetBit() + at(TX_IDX).GetPort() * 0x20) << 0) | ((at(RX_IDX).GetBit() + at(RX_IDX).GetPort() * 0x20) << 8)) | ~(0xFFFF << 0));
	if (this->m_usart == USART1) SWM->PINASSIGN.PINASSIGN1 &= ((((at(TX_IDX).GetBit() + at(TX_IDX).GetPort() * 0x20) << 8) | ((at(RX_IDX).GetBit() + at(RX_IDX).GetPort() * 0x20) << 16)) | ~(0xFFFF << 8));
	if (this->m_usart == USART2) SWM->PINASSIGN.PINASSIGN2 &= ((((at(TX_IDX).GetBit() + at(TX_IDX).GetPort() * 0x20) << 16) | ((at(RX_IDX).GetBit() + at(RX_IDX).GetPort() * 0x20) << 24)) | ~(0xFFFF << 16));
	if (this->m_usart == USART3) {
		SWM->PINASSIGN.PINASSIGN11 &= (((at(TX_IDX).GetBit() + at(TX_IDX).GetPort() * 0x20) << 24) | ~(0xFF << 24));
		SWM->PINASSIGN.PINASSIGN12 &= (((at(RX_IDX).GetBit() + at(RX_IDX).GetPort() * 0x20) << 0) | ~(0xFF << 0));
	}
	if (this->m_usart == USART4) SWM->PINASSIGN.PINASSIGN12 &= ((((at(TX_IDX).GetBit() + at(TX_IDX).GetPort() * 0x20) << 16) | ((at(RX_IDX).GetBit() + at(RX_IDX).GetPort() * 0x20) << 24)) | ~(0xFFFF << 16));
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void UART::Config(uint32_t baudrate, data_bits_t data_bits, parity_t parity) {
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
	if (this->m_usart == USART1) NVIC->ISER[0] |= (1 << 4);  // Enable UART1_IRQ
	if (this->m_usart == USART2) NVIC->ISER[0] |= (1 << 5);  // Enable UART2_IRQ
	if (this->m_usart == USART3) NVIC->ISER[0] |= (1 << 30); // Enable UART3_IRQ
	if (this->m_usart == USART4) NVIC->ISER[0] |= (1 << 31); // Enable UART4_IRQ

	this->m_usart->CFG |= (1 << 0); // Enable USART
}

void UART::SetBaudRate(uint32_t baudrate) {
	this->m_usart->CFG &= ~(1 << 0);	// Disable UART
	this->m_usart->BRG = ((FREQ_CLOCK_MCU / baudrate) / (this->m_usart->OSR + 1)) - 1;	// Change baudrate
	this->m_usart->CFG |= (1 << 0);		// Enable UART
}

void UART::EnableClock(void) {
	if (this->m_usart == USART0) {
		g_usart[0] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 14);
	}
	if (this->m_usart == USART1) {
		g_usart[1] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 15);
	}
	if (this->m_usart == USART2) {
		g_usart[2] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 16);
	}
	if (this->m_usart == USART3) {
		g_usart[3] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 30);
	}
	if (this->m_usart == USART4) {
		g_usart[4] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 31);
	}

	// Select Clock PPAL.: FCLKSEL
	if (this->m_usart == USART0) SYSCON->FCLKSEL[0] = 1;
	if (this->m_usart == USART1) SYSCON->FCLKSEL[1] = 1;
	if (this->m_usart == USART2) SYSCON->FCLKSEL[2] = 1;
	if (this->m_usart == USART3) SYSCON->FCLKSEL[3] = 1;
	if (this->m_usart == USART4) SYSCON->FCLKSEL[4] = 1;
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
			this->DisableInterrupt();
			this->m_flagTX = false;
		}
	}
}

UART::~UART() {
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
	g_usart[0]->UART_IRQHandler();
}

void UART1_IRQHandler(void) {
	g_usart[1]->UART_IRQHandler();
}

void UART2_IRQHandler(void) {
	g_usart[2]->UART_IRQHandler();
}

void UART3_IRQHandler(void) {
	g_usart[3]->UART_IRQHandler();
}

void UART4_IRQHandler(void) {
	g_usart[4]->UART_IRQHandler();
}
