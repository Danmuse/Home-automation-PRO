/*!
 * @file AsyncComm.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 * @brief Asynchronous Communication Class API (Application Programming Interface).
 */

#ifndef ASYNC_COMM_H_
#define ASYNC_COMM_H_

#include "utils.h"

class AsyncComm {
public:
	virtual void transmit(const char *msg) = 0;
	virtual void transmit(const char *msg, uint32_t n) = 0;
	virtual bool receive(char *message, uint32_t n) = 0;
	virtual void UART_IRQHandler(void) = 0;
	virtual ~AsyncComm() = default;
protected:
	virtual void pushRX(uint8_t data) = 0;
	virtual bool popRX(uint8_t *data) = 0;
	virtual void pushTX(uint8_t data) = 0;
	virtual bool popTX(uint8_t *data) = 0;
};

#endif /* ASYNC_COMM_H_ */
