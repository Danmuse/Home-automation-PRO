/*!
 * @file Timers.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 00:56:04
 * @brief Timers Class API (Application Programming Interface).
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "Timer.h"

class Timers {
private:
	std::list<Timer*> m_timers;
public:
	Timers();
	Timers& operator<<(Timer* timer);
	Timers& operator<<(Timer& timer);
	void TimerEvents(void);
	virtual ~Timers();
};

#endif /* TIMERS_H_ */
