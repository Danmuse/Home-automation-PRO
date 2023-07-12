/*!
 * @file Callback.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 20:02:36
 * @brief Callback Class API (Application Programming Interface).
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "systick.h"

class Callback {
private:
	void (*m_callback)(void);
public:
	Callback(void (*function)(void) = nullptr);
	virtual void myCallback(void);
	~Callback();
};

#endif /* CALLBACK_H_ */
