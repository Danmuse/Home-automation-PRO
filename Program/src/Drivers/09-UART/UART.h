/*!
 * @file UART.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 * @brief Universal Asynchronous Receiver/Transmitter Class API (Application Programming Interface).
 */

#ifndef UART_H_
#define UART_H_

#include "AsyncComm.h"
#include "GPIO.h"

#if defined (__cplusplus)
extern "C" {
	void UART0_IRQHandler(void);
}
extern "C" {
	void UART1_IRQHandler(void);
}
extern "C" {
	void UART2_IRQHandler(void);
}
extern "C" {
	void UART3_IRQHandler(void);
}
extern "C" {
	void UART4_IRQHandler(void);
}
#endif

#define USART_USB	USART0
#define PORT_RX_USB	Gpio::PORT0
#define PORT_TX_USB	Gpio::PORT0
#define PIN_RX_USB	24
#define PIN_TX_USB	25

class UART : public AsyncComm {
private:
	const Gpio 	m_RX;
	const Gpio 	m_TX;
	USART_Type*	m_usart;							//!< Register to be used
	uint8_t*	m_bufferRX;							//!< Reception buffer
	uint32_t	m_indexRXIn, m_indexRXOut, m_maxRX;	//!< RX buffer input position - RX buffer output position - RX buffer size
	uint8_t*	m_bufferTX;							//!< Transmission buffer
	uint32_t	m_indexTXIn, m_indexTXOut, m_maxTX;	//!< TX buffer input position - TX buffer output position - TX buffer size
	bool 		m_flagTX;							//!< Error in sending data (Buffer overload)
public:
	enum parity_t 		{ none_parity, even = 2, odd };
	enum data_bits_t	{ seven_bits, eight_bits };

	UART(uint32_t baudrate, data_bits_t data_bits, parity_t parity, uint32_t maxRX, uint32_t maxTX);
	void Transmit(const char *message) override;
	void Transmit(const void *message, uint32_t n) override;
	void *Message(void *message, uint32_t n) override;
	void SetBaudRate(uint32_t baudrate);
	virtual ~UART();
private:
	void EnableSWM(void);
	void EnableClock(void);
	void Config(uint32_t baudrate, data_bits_t data_bits, parity_t parity);
	void UART_IRQHandler(void) override;

	void pushRX(uint8_t data) override;
	uint8_t popRX(uint8_t *data) override;
	void pushTX(uint8_t data) override;
	uint8_t popTX(uint8_t *data) override;
	void EnableInterupt(void);
	void DisableInterupt(void);
};

#endif /* UART_H_ */
