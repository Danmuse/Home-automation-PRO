/*!
 * @file Callback.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 20:02:36
 * @brief Callback Class API (Application Programming Interface).
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <list>
#include "utils.h"

class Callback {
private:
	void (*m_callback)(void);
public:
	Callback(void (*function)(void) = nullptr);
	virtual void CallbackMethod(void);
	virtual ~Callback();
};

extern std::list<Callback*> g_callback_list;

#endif /* CALLBACK_H_ */
