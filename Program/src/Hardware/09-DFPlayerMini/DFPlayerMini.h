/*!
 * @file DFPlayerMini.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/01/2024 13:51:53
 * @brief DFPlayer Module Class API (Application Programming Interface).
 */

#ifndef DFPLAYER_MINI_H_
#define DFPLAYER_MINI_H_

#include "UART.h"
#include "Semaphore.h"

#define DFPLAYER_EQ_NORMAL 0
#define DFPLAYER_EQ_POP 1
#define DFPLAYER_EQ_ROCK 2
#define DFPLAYER_EQ_JAZZ 3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASS 5

#define DFPLAYER_DEVICE_U_DISK 1
#define DFPLAYER_DEVICE_SD 2
#define DFPLAYER_DEVICE_AUX 3
#define DFPLAYER_DEVICE_SLEEP 4
#define DFPLAYER_DEVICE_FLASH 5

#define DFPLAYER_RECEIVED_LENGTH 10
#define DFPLAYER_SEND_LENGTH 10

#define TimeOut 0
#define WrongStack 1
#define DFPlayerCardInserted 2
#define DFPlayerCardRemoved 3
#define DFPlayerCardOnline 4
#define DFPlayerPlayFinished 5
#define DFPlayerError 6
#define DFPlayerUSBInserted 7
#define DFPlayerUSBRemoved 8
#define DFPlayerUSBOnline 9
#define DFPlayerCardUSBOnline 10
#define DFPlayerFeedBack 11

#define Busy 1
#define Sleeping 2
#define SerialWrongStack 3
#define CheckSumNotMatch 4
#define FileIndexOut 5
#define FileMismatch 6
#define Advertise 7

#define Stack_Header 0
#define Stack_Version 1
#define Stack_Length 2
#define Stack_Command 3
#define Stack_ACK 4
#define Stack_Parameter 5
#define Stack_CheckSum 7
#define Stack_End 9

class DFPlayer : protected UART {
private:
	uint8_t m_received[DFPLAYER_RECEIVED_LENGTH];
	uint8_t m_sending[DFPLAYER_SEND_LENGTH] = {0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF};

	uint8_t m_receivedIndex = 0;

//	uint8_t m_device = DFPLAYER_DEVICE_SD;
	uint8_t m_handleType;
	uint8_t m_handleCommand;
	uint16_t m_handleParameter;
	bool m_isAvailable = false;
	bool m_isSending = false;

	void enableACK(void);
	void disableACK(void);
	bool waitAvailable(uint32_t duration);
	bool begin(bool isACK = true, bool doReset = true);
	uint8_t readType(void);
	uint16_t read(void);
public:
	DFPlayer(const Gpio& RX, const Gpio& TX, channelUART_t channel = UART0);

	virtual ~DFPlayer() = default;
};

extern DFPlayer *g_dfplayer;

void initDFPlayer(void);

#endif /* DFPLAYER_MINI_H_ */
