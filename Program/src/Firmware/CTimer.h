/*!
 * @file CTimer.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 00:01:36
 * @brief Standard Counter/Timer Class API (Application Programming Interface).
 */

#ifndef CTIMER_H_
#define CTIMER_H_

#include "GPIO.h"

#define MAX_CTIMER0_MATCH_CHANNELS 4
#define MAX_CTIMER0_CAPTURE_CHANNELS 3

#if defined (__cplusplus)
extern "C" {
	void CTIMER0_IRQHandler(void);
}
#endif

class CTimer : protected Gpio {
public:
	enum channelFunction_t { CTIMER_MATCH, CTIMER_CAPTURE };
	enum matchInterruption_t { MAT0INT, MAT1INT, MAT2INT, MAT3INT };
	enum captureInterruption_t { CAP0INT, CAP1INT, CAP2INT, CAP3INT };
	enum actionInterruption_t { INACTIVE, CLEAR, SET, TOGGLE };
private:
	const channelFunction_t m_channelFunction;
	const uint8_t m_match_channel;
	const uint8_t m_capture_channel;
	static uint8_t m_match_quantity;
	static uint8_t m_capture_quantity;

	void initCTimer(void) const;
	void enableSWM(void) const;
	void disableSWM(void) const;
protected:
//	uint8_t getPulseState(void);
//	void setPulse(uint8_t value);
//	uint32_t getPulseTime(void);
	void changePrescaler(uint32_t value);
	void configMatch(uint32_t timeTicks, actionInterruption_t actionInterruption, matchInterruption_t matchInterruption = MAT0INT);
public:
	CTimer() = delete;
	CTimer(const Gpio &output, channelFunction_t channelFunction);
	void CTIMER_IRQHandler(void); // NOTE: IT IS PRIVATE!!
	virtual ~CTimer();
};

#endif /* CTIMER_H_ */
