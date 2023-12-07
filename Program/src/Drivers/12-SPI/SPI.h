/*!
 * @file SPI.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 * @brief Serial Peripheral Interface Class API (Application Programming Interface).
 */

#ifndef SPI_H_
#define SPI_H_

#include "LPC845.h"
#include "GPIO.h"
#include <algorithm>
#include <vector>

#define SPI_MAX_CHANNELS 2
#define SPI0_CLK_REG 11
#define SPI1_CLK_REG 12
#define SPI0_MAX_SSEL 4
#define SPI1_MAX_SSEL 2

#define SEND_BUFFER_SIZE 64
#define RECEIVE_BUFFER_SIZE 64

#if defined (__cplusplus)
extern "C" {
	void SPI0_IRQHandler(void);
	void SPI1_IRQHandler(void);
}
#endif

class SPI {
    public:
        enum bitOrder_t { MSB_FIRST, LSB_FIRST };
        enum mode_t { MODE0, MODE1, MODE2, MODE3 };
    private:
        SPI_Type *m_SPI;
        Gpio m_SCK;
        Gpio m_MISO;
        Gpio m_MOSI;
        std::vector<Gpio> m_SSEL;
        const uint8_t m_channel; /**Channel number, from 0 to MAX non inclusively*/
        const bool m_master;
        bitOrder_t m_bitOrder;
        uint8_t m_maxSSELSize;

        //TODO: You need one per SSEL, also should change rx,tx acordingly

        //Buffers are uint8_t because in TXDATCTL LEN is set on 8
        uint8_t m_receiveBuffer[RECEIVE_BUFFER_SIZE];
        uint8_t m_receiveBufferIndexIn;
        uint8_t m_receiveBufferIndexOut;
        uint8_t m_sendBuffer[SEND_BUFFER_SIZE];
        uint8_t m_sendBufferIndexIn;
        uint8_t m_sendBufferIndexOut;

        bool m_isSending;
    public:
        SPI(Gpio &SCK, Gpio &MISO, Gpio &MOSI, std::vector<Gpio> SSEL, uint32_t bps, uint8_t channel = 0,
               bool master = true, bitOrder_t bitOrder = MSB_FIRST, mode_t mode = MODE0);
        SPI(Gpio &SCK, Gpio &MISO, Gpio &MOSI, uint32_t bps, uint8_t channel);
        bool bindSSEL(Gpio &SSEL, uint8_t &SSELNumber);
        void setBaudRate(uint32_t bps) const;
        void enableSSEL(uint8_t SSEL) const;
        void disableSSEL(uint8_t SSEL) const;
        void transmit(uint8_t *message, uint8_t length = 1);
        void transmit(char *cString);
        bool receive(uint8_t &message);
        bool receive(uint8_t *message, uint8_t length);
        bool receive(char *cString);
        // TODO: Make protected the following method
        void SPI_IRQHandler(void);

        ~SPI();
    private:
        void config(bool master, mode_t mode, bitOrder_t bitOrder, uint32_t bps) const;
        void enableClock() const;
        void enableSWM() const;
        void enableTxInterrupt() const;
        void disableSendInterrupt() const;
    protected:
        void pushReceive(uint8_t data);
        bool popReceive(uint8_t *data);
        void pushSend(uint8_t data);
        bool popSend(uint8_t *data);
};

#endif /* SPI_H_ */
