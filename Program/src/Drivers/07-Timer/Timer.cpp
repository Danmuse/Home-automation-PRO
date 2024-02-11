/*/*!
 * @file Timer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 22:02:36
 */

#include "Timer.h"

Timer::Timer() : Callback(),
m_TmrRun{0}, m_TmrEvent{false}, m_TmrHandler{nullptr}, m_TmrStandBy{RUN}, m_TmrBase{DEC} {
	g_callback_list.push_back(this);
}

Timer::Timer(const TimerHandler handler, const bases_t base) : Callback(),
m_TmrRun{0}, m_TmrEvent{false}, m_TmrHandler{handler}, m_TmrStandBy{RUN}, m_TmrBase{base} {
	g_callback_list.push_back(this);
}

void Timer::timerStart(uint32_t counter, const TimerHandler handler, const bases_t base) {
	switch (base) {
		case DAY:
			counter *= DAYS;
		case HOUR:
			counter *= HOURS;
		case MIN:
			counter *= MINUTES;
		case SEC:
			counter *= SECONDS;
		case DEC:
			counter *= DECIMALS;
		case MILLI:
			counter *= MILLIS;
			break;
		default:
			break;
	}

	this->m_TmrRun = counter * (g_systick_freq / 1000);
	this->m_TmrEvent = !counter;

	this->m_TmrHandler = handler;
	this->m_TmrBase = base;
}

void Timer::setTimer(uint32_t counter) {
	switch (this->m_TmrBase) {
		case DAY:
			counter *= DAYS;
		case HOUR:
			counter *= HOURS;
		case MIN:
			counter *= MINUTES;
		case SEC:
			counter *= SECONDS;
		case DEC:
			counter *= DECIMALS;
		case MILLI:
			counter *= MILLIS;
			break;
		default:
			break;
	}

	this->m_TmrRun = counter * (g_systick_freq / 1000);
	this->m_TmrEvent = !counter;
}

uint32_t Timer::getTimer(void) const {
	uint32_t time = this->m_TmrRun;

	switch (this->m_TmrBase) {
		case DAY:
			time /= DAYS;
		case HOUR:
			time /= HOURS;
		case MIN:
			time /= MINUTES;
		case SEC:
			time /= SECONDS;
		case DEC:
			time /= DECIMALS;
		case MILLI:
			time /= MILLIS;
			break;
		default:
			break;
	}
	return time;
}

uint32_t Timer::getTicks(void) const {
	return this->m_TmrRun;
}

void Timer::standBy(standby_t action) {
	this->m_TmrStandBy = action;
}

void Timer::timerStop(void) {
	this->m_TmrRun = 0;
	this->m_TmrEvent = false;
	this->m_TmrHandler = nullptr;
	this->m_TmrStandBy = RUN;
}

Timer::error_t Timer::timerEvent(void) {
	if (this->m_TmrHandler != nullptr) {
		if (this->m_TmrEvent) {
			this->m_TmrEvent = false;
			this->m_TmrHandler();
			return OK;
		}
	}
	return ERROR;
}

Timer& Timer::operator=(uint32_t counter) {
	this->setTimer(counter);
	return *this;
}

bool Timer::operator!(void) {
	return !this->m_TmrEvent;
}

bool Timer::operator==(bool checkEvent) {
	if (this->m_TmrEvent == checkEvent) {
		this->m_TmrEvent = false;
		return true;
	}
	return false;
}

bool operator==(bool checkEvent, Timer& timer) {
	if (timer.m_TmrEvent == checkEvent) {
		timer.m_TmrEvent = false;
		return true;
	}
	return false;
}

Timer::operator bool(void) {
	return this->m_TmrEvent;
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void Timer::callbackMethod(void) {
	if (this->m_TmrRun) {
		if (this->m_TmrStandBy == RUN) {
			this->m_TmrRun--;
			if (!this->m_TmrRun) this->m_TmrEvent = true;
		}
	}
}

#pragma GCC pop_options
