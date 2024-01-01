/*!
 * @file SyncCommSPI.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 * @brief Synchronous Communication SPI Abstract Class API (Application Programming Interface).
 */

#ifndef SYNC_COMM_SPI_H_
#define SYNC_COMM_SPI_H_

#include "utils.h"

class SyncCommSPI {
public:
    enum bitOrder_t { MSB_FIRST, LSB_FIRST };
	// DEFAULT_FREQUENCY: Clock period at 1 Mb/s.
	enum frequencyComm_t { DEFAULT_FREQUENCY = 1000000, FAST_FREQUENCY = 4000000 };
    // MODE0: The SPI captures serial data on the first clock transition of the transfer. Data is changed on the following edge.
	// 		  The rest state of the clock is low.
	// MODE1: The SPI captures serial data on the first clock transition of the transfer. Data is changed on the following edge.
	// 		  The rest state of the clock is high.
	// MODE2: The SPI changes serial data on the first clock transition of the transfer. Data is changed on the following edge.
	// 		  The rest state of the clock is low.
	// MODE3: The SPI changes serial data on the first clock transition of the transfer. Data is changed on the following edge.
	// 		  The rest state of the clock is high.
    enum mode_t { MODE0, MODE1, MODE2, MODE3 };

    virtual void transmit(const char *message) = 0;
    virtual void transmitBytes(uint8_t *message, uint8_t length = 1) = 0;
    virtual bool receive(uint8_t *address, uint8_t &message) = 0;
    virtual bool receive(uint8_t *address, uint8_t *message, uint8_t length) = 0;
    virtual bool receive(uint8_t *address, char *message) = 0;
	virtual void SPI_IRQHandler(void) = 0;
	virtual ~SyncCommSPI() = default;
protected:
	virtual void pushReceive(uint8_t data) = 0;
	virtual bool popReceive(uint8_t *data) = 0;
	virtual void pushSend(uint8_t data) = 0;
	virtual bool popSend(uint8_t *data) = 0;
};

#endif /* SYNC_COMM_SPI_H_ */
