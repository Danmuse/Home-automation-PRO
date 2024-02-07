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
#include "utilities.h"

#define DFPLAYER_MAX_VOLUME_VALUE 30
#define DFPLAYER_MIN_VOLUME_VALUE 0

//#define DFPLAYER_EQ_NORMAL 0
//#define DFPLAYER_EQ_POP 1
//#define DFPLAYER_EQ_ROCK 2
//#define DFPLAYER_EQ_JAZZ 3
//#define DFPLAYER_EQ_CLASSIC 4
//#define DFPLAYER_EQ_BASS 5

//#define DFPLAYER_DEVICE_U_DISK 1
//#define DFPLAYER_DEVICE_SD 2
//#define DFPLAYER_DEVICE_AUX 3
//#define DFPLAYER_DEVICE_SLEEP 4
//#define DFPLAYER_DEVICE_FLASH 5

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

//#define Busy 1
//#define Sleeping 2
//#define SerialWrongStack 3
//#define CheckSumNotMatch 4
//#define FileIndexOut 5
//#define FileMismatch 6
//#define Advertise 7

#define Stack_Header 0
#define Stack_Version 1
#define Stack_Length 2
#define Stack_Command 3
#define Stack_ACK 4
#define Stack_Parameter 5
#define Stack_CheckSum 7
#define Stack_End 9

/*

TODO (Medium priority): Complete the enumeration and modify the parameters of the methods involved in said definitions.

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

//! @brief <b>DFPlayer_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the DFPlayer module operations can be found.
typedef enum {
	DFPLAYER_OK,	//!< Successful operation.
} DFPlayer_result_t;

*/

class DFPlayer : protected UART, Callback {
private:
	uint16_t m_timeOutTimer;

	uint8_t m_received[DFPLAYER_RECEIVED_LENGTH];
	uint8_t m_sending[DFPLAYER_SEND_LENGTH] = { 0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF };

//	DFPlayer_result_t m_statusDFPlayer;
	uint8_t m_receivedIndex;
	uint8_t m_handleType;
	uint8_t m_handleCommand;
	uint16_t m_handleParameter;
	bool m_isAvailable;
	bool m_isSending;

	void uint16ToArray(uint16_t value, uint8_t *array);
	uint16_t arrayToUint16(uint8_t *array);
	uint16_t calculateCheckSum(uint8_t *buffer);
	void parseStack(void);
	bool validateStack(void);
	void sendStack(void);
	void sendStack(uint8_t command);
	void sendStack(uint8_t command, uint16_t argument);
	void sendStack(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow);
	void enableACK(void);
	void disableACK(void);
	bool available(void);
	bool waitAvailable(uint32_t duration = 0);
	bool begin(bool isACK = true, bool doReset = true);
	uint8_t readType(void);
	uint16_t read(void);
	bool handleMessage(uint8_t type, uint16_t parameter = 0);
	bool handleError(uint8_t type, uint16_t parameter = 0);
public:
	DFPlayer() = delete;
	DFPlayer(const Gpio& RX, const Gpio& TX, channelUART_t channel = UART0);

	void next(void);
	void previous(void);
	void play(uint8_t fileNumber = 1);
	void volumeUp(void);
	void volumeDown(void);
	void volume(uint8_t volume);
	void equalizer(uint8_t equalizer);
	void loop(uint8_t fileNumber);
	void sleep(void);
	void reset(void);
	void start(void);
	void pause(void);
	void enableLoopAll(void);
	void disableLoopAll(void);
	void enableLoop(void);
	void disableLoop(void);
//	DFPlayer_result_t getStatus(void) const;

	void callbackMethod(void) override;
	virtual ~DFPlayer() = default;
};

extern DFPlayer *g_dfplayer;

void initDFPlayer(void);

#endif /* DFPLAYER_MINI_H_ */
