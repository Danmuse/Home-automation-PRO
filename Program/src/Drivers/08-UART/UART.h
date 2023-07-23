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

class UART : public AsyncComm {
public:
	enum parity_t 			{ NoParidad, par = 2, impar };
	enum bits_de_datos		{ siete_bits, ocho_bits };

	#define PORT_TX_USB		Gpio::port0
	#define PORT_RX_USB		Gpio::port0
	#define PIN_TX_USB		25
	#define PIN_RX_USB		24
	#define USART_USB		USART0
private:
	const Gpio 	m_TX;
	const Gpio 	m_RX;
	USART_Type*	m_usart;							/**< Registro a utilizar */
	uint8_t*	m_bufferRX;							/**< Buffer de recepcion */
	uint32_t	m_indexRXIn, m_indexRXOut, m_maxRX;	/**< Posicion de entrada del buffer RX */ /**< Posicion de salida del buffer RX */ /**< Tamaño buffer RX */
	uint8_t*	m_bufferTX;							/**< Buffer de transmision */
	uint32_t	m_indexTXIn, m_indexTXOut, m_maxTX;	/**< Posicion de entrada del buffer TX */ /**< Posicion de salida del buffer TX */ /**< Tamaño buffer TX */
	bool 		m_flagTX;							/**< error en el envio de datos. Buffer sobrepasado */
public:
	UART(Gpio::port_t portTX, uint8_t pinTX, Gpio::port_t portRX,	uint8_t pinRX, USART_Type *usart, uint32_t baudrate, bits_de_datos BitsDeDatos,	parity_t parity, uint32_t maxRX, uint32_t maxTX);
	void Transmit(const char *message) override;
	void Transmit(const void *message, uint32_t n) override;
	void *Message(void *message, uint32_t n) override;
	void SetBaudRate(uint32_t baudrate);
	virtual ~UART();
private:
	void EnableSW(void);
	void EnableClock(void);
	void Config(uint32_t baudrate, bits_de_datos BitsDeDatos, parity_t parity);
	void UART_IRQHandler(void) override;

	void pushRX(uint8_t data) override;
	uint8_t popRX(uint8_t *data) override;
	void pushTX(uint8_t data) override;
	uint8_t popTX(uint8_t *data) override;
	void EnableInterupt(void);
	void DisableInterupt(void);
};

#endif /* UART_H_ */
