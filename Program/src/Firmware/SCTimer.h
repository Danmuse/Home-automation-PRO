/*!
 * @file SCTimer.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/07/2023 13:38:36
 * @brief Standard Command Timer Class API (Application Programming Interface).
 */

#ifndef SCTIMER_H_
#define SCTIMER_H_

#include "LPC845.h"

class SCTimer {
protected:
	void setTime(uint32_t time, uint32_t channel);
	void setUnify(bool status);
	void setAutoLimit(bool status);
	void bindSCTOUT(uint8_t bit, uint8_t port, uint8_t out_number);
	void startTimer(void);
	void stopTimer(void);
public:
	SCTimer() = delete;
	SCTimer(uint8_t period);
	virtual ~SCTimer() = default;
};

#endif /* SCTIMER_H_ */
