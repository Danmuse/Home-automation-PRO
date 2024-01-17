/*!
 * @file CTimer.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 00:01:36
 * @brief Standard Counter/Timer Class API (Application Programming Interface).
 */

#ifndef CTIMER_H_
#define CTIMER_H_

#include "LPC845.h"

#if defined (__cplusplus)
extern "C" {
	void CTIMER0_IRQHandler(void);
}
#endif

class CTimer {
private:
	uint8_t m_pulse;
	uint32_t m_pulses[2];
protected:
	void bindCTOUT(void);

	void configMatch(void);
	uint8_t getPulseState(void);
	void setPulse(uint8_t value);
	uint32_t getPulseTime(void);
public:
	CTimer();
	virtual ~CTimer();
};

#endif /* CTIMER_H_ */
