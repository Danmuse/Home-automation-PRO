/*!
 * @file S5050DJ.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 12:11:36
 * @brief LEDs S5050DJ with IR Transmitter Class API (Application Programming Interface).
 */

#ifndef S5050DJ_H_
#define S5050DJ_H_

#include "CTimer.h"

class S5050DJ : protected CTimer {
public:
	S5050DJ() = delete;
	S5050DJ(const Gpio &output);
	virtual ~S5050DJ();
};

#endif /* S5050DJ_H_ */
