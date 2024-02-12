/*/*!
 * @file Timers.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 00:56:04
 */

#include "Timers.h"

Timers::Timers() { }

Timers& Timers::operator<<(Timer* timer) {
	this->m_timers.push_back(timer);
	return *this;
}

Timers& Timers::operator<<(Timer& timer) {
	this->m_timers.push_back(&timer);
	return *this;
}

void Timers::allStandBy(Timer::standby_t action) {
	for (Timer* q : this->m_timers) q->standBy(action);
}

/*!
 * @brief Stops all timers.
 */
void Timers::timerStops(void) {
	for (Timer* q : this->m_timers) q->timerStop();
}

/*!
 * @brief Calls the handler in all timers needed
 */
void Timers::timerEvents(void) {
	for (Timer* q : this->m_timers) q->timerEvent();
}
