/*!
 * @file SPI.h
 * @authors Agustín Ordoñez
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @par Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 * @brief Serial Peripheral Interface Class API (Application Programming Interface).
 */

#ifndef SPI_H_
#define SPI_H_

#include <vector>
#include <algorithm>
#include "SyncCommSPI.h"
#include "GPIO.h"

#define SPI_MAX_CHANNELS 2
#define SPI0_MAX_SSEL 4
#define SPI1_MAX_SSEL 2

#define RECEIVE_BUFFER_SIZE 64
#define SEND_BUFFER_SIZE 64

#define SCK_IDX 0
#define MOSI_IDX 1
#define MISO_IDX 2

#define SPI_OFFSET_BASE (0x4000U)

#if defined (__cplusplus)
extern "C" {
	void SPI0_IRQHandler(void);
	void SPI1_IRQHandler(void);
}
#endif

class SPI : protected std::vector<Gpio>, public SyncCommSPI {
    private:
        SPI_Type* m_SPI;
        static uint8_t m_SPI0_SSELs;
        static uint8_t m_SPI1_SSELs;
        uint8_t m_bindSSELs;
        bitOrder_t m_bitOrder;
        // TODO: You need one per SSEL, also should change RX, TX accordingly.
        // Buffers are uint8_t because in TXDATCTL LEN is set on eight.
        uint8_t m_receiveBuffer[RECEIVE_BUFFER_SIZE];
        uint8_t m_receiveBufferIndexIn;
        uint8_t m_receiveBufferIndexOut;
        uint8_t m_sendBuffer[SEND_BUFFER_SIZE];
        uint8_t m_sendBufferIndexIn;
        uint8_t m_sendBufferIndexOut;
        bool m_isSending;
    public:
		enum channelSPI_t { SPI_CHANNEL0, SPI_CHANNEL1 };

		SPI(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, frequencyComm_t frequency = DEFAULT_FREQUENCY, channelSPI_t channel = SPI_CHANNEL0, bool master = true, bitOrder_t bitOrder = MSB_FIRST, mode_t mode = MODE0);
		bool bindSSEL(const Gpio& SSEL, uint8_t &bindedSSEL);
		bool unbindSSEL(uint8_t unbindedSSEL);
        void setBaudRate(frequencyComm_t frequency);
        void enableSSEL(uint8_t SSEL);
        void disableSSEL(uint8_t SSEL);
        void transmit(const char *message) override;
        void transmitBytes(uint8_t *message, uint8_t length = 1) override;
        bool receive(uint8_t *address, uint8_t &message) override;
        bool receive(uint8_t *address, uint8_t *message, uint8_t length) override;
        bool receive(uint8_t *address, char *message) override;
        ~SPI();
    private:
        void config(bool master, mode_t mode, frequencyComm_t frequency);
        void enableClock(void);
        void enableSWM(void);
        void pushReceive(uint8_t data) override;
        bool popReceive(uint8_t *data) override;
        void pushSend(uint8_t data) override;
        bool popSend(uint8_t *data) override;
        void enableReceiveInterrupt(void);
        void enableSendInterrupt(void);
        void disableReceiveInterrupt(void);
        void disableSendInterrupt(void);

        void SPI_IRQHandler(void) override;
};

#endif /* SPI_H_ */
