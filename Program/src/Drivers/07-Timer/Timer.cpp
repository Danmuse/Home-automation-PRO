/*/*!
 * @file Timer.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 22:02:36
 */

#include "Timer.h"

Timer::Timer() : Callback(),
m_TmrRun{0},
m_TmrEvent{false},
m_TmrHandler{nullptr},
m_TmrStandBy{RUN},
m_TmrBase{DEC} {
	g_callback_list.push_back(this);
}

Timer::Timer(const TimerHandler handler, const bases_t base) : Callback(),
m_TmrRun{0},
m_TmrEvent{false},
m_TmrHandler{handler},
m_TmrStandBy{RUN},
m_TmrBase{base} {
	g_callback_list.push_back(this);
}

void Timer::timerStart(uint32_t counter, const TimerHandler handler, const bases_t base) {
	switch (base) {
	case MILLI:
			counter *= MILLIS;
		break;
	case DEC:
			counter *= (MILLIS * DECIMALS);
		break;
	case SEC:
			counter *= (MILLIS * DECIMALS * SECONDS);
		break;
	case MIN:
			counter *= (MILLIS * DECIMALS * SECONDS * MINUTES);
		break;
	case HOUR:
			counter *= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS);
		break;
	case DAY:
			counter *= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
		break;
	default:
		break;
	}

	if (counter) {
		this->m_TmrRun = counter * (g_systick_freq / 1000);
		this->m_TmrEvent = false;
	} else {
		this->m_TmrRun = 0;
		this->m_TmrEvent = true;
	}

	this->m_TmrHandler = handler;
	this->m_TmrBase = base;
}

void Timer::timerStart(uint32_t counter) {
	switch (this->m_TmrBase) {
	case MILLI:
			counter *= MILLIS;
		break;
	case DEC:
			counter *= (MILLIS * DECIMALS);
		break;
	case SEC:
			counter *= (MILLIS * DECIMALS * SECONDS);
		break;
	case MIN:
			counter *= (MILLIS * DECIMALS * SECONDS * MINUTES);
		break;
	case HOUR:
			counter *= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS);
		break;
	case DAY:
			counter *= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
		break;
	default:
		break;
	}

	if (counter) {
		this->m_TmrRun = counter * (g_systick_freq / 1000);
		this->m_TmrEvent = false;
	} else {
		this->m_TmrRun = 0;
		this->m_TmrEvent = true;
	}
}

void Timer::setTimer(uint32_t time) {
	switch (this->m_TmrBase) {
	case MILLI:
			time *= MILLIS;
		break;
	case DEC:
			time *= (MILLIS * DECIMALS);
		break;
	case SEC:
			time *= (MILLIS * DECIMALS * SECONDS);
		break;
	case MIN:
			time *= (MILLIS * DECIMALS * SECONDS * MINUTES);
		break;
	case HOUR:
			time *= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS);
		break;
	case DAY:
			time *= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
		break;
	default:
		break;
	}

	if (!time) this->m_TmrEvent = true;
	this->m_TmrRun = time * (g_systick_freq / 1000);
}

uint32_t Timer::getTimer(void) const {
	uint32_t time = this->m_TmrRun;

	switch(this->m_TmrBase) {
	case MILLI:
			time /= MILLIS;
		break;
	case DEC:
			time /= (MILLIS * DECIMALS);
		break;
	case SEC:
			time /= (MILLIS * DECIMALS * SECONDS);
		break;
	case MIN:
			time /= (MILLIS * DECIMALS * SECONDS * MINUTES);
		break;
	case HOUR:
			time /= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS);
		break;
	case DAY:
			time /= (MILLIS * DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
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
	this->timerStart(counter);
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

void Timer::callbackMethod(void) {
	if (this->m_TmrRun) {
		if (this->m_TmrStandBy == RUN) {
			this->m_TmrRun--;
			if (!this->m_TmrRun) this->m_TmrEvent = true;
		}
	}
}

Timer::~Timer() { }
