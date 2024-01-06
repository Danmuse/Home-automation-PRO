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

class CTimer {
protected:
	void bindCTOUT(void);
public:
	CTimer();
	virtual ~CTimer();
};

#endif /* CTIMER_H_ */
