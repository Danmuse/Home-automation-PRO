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

#define DFPLAYER_RECEIVED_LENGTH 10
#define DFPLAYER_SEND_LENGTH 10

#define DFPLAYER_INITIAL_VOLUME_PERCENTAGE 70
#define DFPLAYER_TIMEOUT_TICKS 200

//#define TimeOut 0
//#define WrongStack 1
//#define DFPlayerCardInserted 2
//#define DFPlayerCardRemoved 3
//#define DFPlayerCardOnline 4
//#define DFPlayerPlayFinished 5
//#define DFPlayerError 6
//#define DFPlayerUSBInserted 7
//#define DFPlayerUSBRemoved 8
//#define DFPlayerUSBOnline 9
//#define DFPlayerCardUSBOnline 10
//#define DFPlayerFeedBack 11

#define Stack_Header 0
#define Stack_Version 1
#define Stack_Length 2
#define Stack_Command 3
#define Stack_ACK 4
#define Stack_Parameter 5
#define Stack_CheckSum 7
#define Stack_End 9

//! @brief <b>DFPlayer_result_t</b> enumeration reports all possible errors, conditions, warnings, and states in which the DFPlayer module operations can be found.
typedef enum {
	DFPLAYER_READY,		//!< Indicates that the player is ready to play a song.
	DFPLAYER_BUSY,		//!< Indicates that the player is busy playing a song.
	DFPLAYER_PAUSE		//!< Indicates that the player has paused a song.
//	DFPLAYER_FAILURE	//!< Indicates that the player could not be configured correctly due to some hardware incompatibility.
} DFPlayer_result_t;

class DFPlayer : protected Gpio, UART, Callback {
private:
	bool m_pauseState;
	uint8_t m_backupFile;
	uint8_t m_volume;

	uint16_t m_timeOutTimer;

	uint8_t m_received[DFPLAYER_RECEIVED_LENGTH];
	uint8_t m_sending[DFPLAYER_SEND_LENGTH] = { 0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF };

	DFPlayer_result_t m_statusDFPlayer;
	uint8_t m_receivedIndex;
	bool m_isAvailable;
	bool m_isSending;

	void uint16ToArray(uint16_t value, uint8_t *array);
	uint16_t arrayToUint16(uint8_t *array);
	uint16_t calculateCheckSum(uint8_t *buffer);
	void resetConditions(void);
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
	void begin(bool isACK = true, bool doReset = true);
protected:
	void callbackMethod(void) override;
public:
	enum equalizer_t { DFPLAYER_EQ_NORMAL, DFPLAYER_EQ_POP, DFPLAYER_EQ_ROCK, DFPLAYER_EQ_JAZZ, DFPLAYER_EQ_CLASSIC, DFPLAYER_EQ_BASS };

	DFPlayer() = delete;
	DFPlayer(const Gpio& busyPin, const Gpio& RX, const Gpio& TX, channelUART_t channel = UART0);

	void next(void);
	void previous(void);
	void play(uint8_t fileNumber = 0);
	void volume(uint8_t volume);
	void equalizer(equalizer_t equalizer);
	void loop(uint8_t fileNumber);
	void reset(void);
	void pause(void);
	void resume(void);

	void prepareSong(uint8_t fileNumber = 1);
	uint8_t getVolume(void) const;
	DFPlayer_result_t getStatus(void) const;

	virtual ~DFPlayer() = default;
};

extern DFPlayer *g_dfplayer;

void initDFPlayer(void);

#endif /* DFPLAYER_MINI_H_ */
