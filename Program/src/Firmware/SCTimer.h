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

#define CLK_SWM 7
#define CLK_SCT 8

class SCTimer {
public:
	SCTimer();
	void setTime(uint32_t time, uint32_t channel);
	void setUnify(bool status);
	void setAutoLimit(bool status);
	void setSwitchMatrixSCTOUT(uint8_t bit, uint8_t port, uint8_t out_number);
	void startTimer(void);
	void StopTimer(void);

	virtual ~SCTimer();
};

#endif /* SCTIMER_H_ */
