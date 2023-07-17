/*!
 * @file Timer.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 22:02:36
 */

#include <Drivers/06-Timer/Timer.h>

Timer::Timer() : Callback(),
m_TmrRun{0},
m_TmrEvent{false},
m_TmrHandler{nullptr},
m_TmrStandBy{RUN},
m_TmrBase{DEC} {
	g_callback_list.push_back(this);
}

Timer::Timer(const Timer_Handler handler, const bases_t base) : Callback(),
m_TmrRun{0},
m_TmrEvent{false},
m_TmrHandler{handler},
m_TmrStandBy{RUN},
m_TmrBase{base} {
	g_callback_list.push_back(this);
}

void Timer::TimerStart(uint32_t counter, const Timer_Handler handler, const bases_t base) {
	switch(base) {
	case DEC:
		counter *= DECIMALS;
	break;
	case SEC:
		counter *= (DECIMALS * SECONDS);
	break;
	case MIN:
		counter *= (DECIMALS * SECONDS * MINUTES);
	break;
	case HOUR:
		counter *= (DECIMALS * SECONDS * MINUTES * HOURS);
	break;
	case DAY:
		counter *= (DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
	break;
	default:
	break;
	}

	if (counter) {
		this->m_TmrRun = counter;
		this->m_TmrEvent = false;
	} else {
		this->m_TmrRun = 0;
		this->m_TmrEvent = true;
	}

	this->m_TmrHandler = handler;
	this->m_TmrBase = base;
}

void Timer::TimerStart(uint32_t counter) {
	switch(this->m_TmrBase) {
	case DEC:
		counter *= DECIMALS;
	break;
	case SEC:
		counter *= (DECIMALS * SECONDS);
	break;
	case MIN:
		counter *= (DECIMALS * SECONDS * MINUTES);
	break;
	case HOUR:
		counter *= (DECIMALS * SECONDS * MINUTES * HOURS);
	break;
	case DAY:
		counter *= (DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
	break;
	default:
	break;
	}

	if (counter) {
		this->m_TmrRun = counter;
		this->m_TmrEvent = false;
	} else {
		this->m_TmrRun = 0;
		this->m_TmrEvent = true;
	}
}

void Timer::SetTimer(uint32_t time) {
	switch(this->m_TmrBase) {
	case DEC:
		time *= DECIMALS;
	break;
	case SEC:
		time *= (DECIMALS * SECONDS);
	break;
	case MIN:
		time *= (DECIMALS * SECONDS * MINUTES);
	break;
	case HOUR:
		time *= (DECIMALS * SECONDS * MINUTES * HOURS);
	break;
	case DAY:
		time *= (DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
	break;
	default:
	break;
	}

	if (!time) this->m_TmrEvent = true;
	this->m_TmrRun = time;
}

void Timer::GetTimer(uint32_t &time) const {
	time = this->m_TmrRun;

	switch(this->m_TmrBase) {
	case DEC:
		time /= DECIMALS;
	break;
	case SEC:
		time /= (DECIMALS * SECONDS);
	break;
	case MIN:
		time /= (DECIMALS * SECONDS * MINUTES);
	break;
	case HOUR:
		time /= (DECIMALS * SECONDS * MINUTES * HOURS);
	break;
	case DAY:
		time /= (DECIMALS * SECONDS * MINUTES * HOURS * DAYS);
	break;
	default:
	break;
	}
}

uint32_t Timer::GetTimer(void) const {
	return this->m_TmrRun;
}

void Timer::SetmrStandBy(standby_t action) {
	this->m_TmrStandBy = action;
}

void Timer::TimerStop(void) {
	this->m_TmrRun = 0;
	this->m_TmrEvent = false;
	this->m_TmrHandler = nullptr;
	this->m_TmrStandBy = RUN;
}

Timer::error_t Timer::TimerEvent(void) {
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
	this->TimerStart(counter);
	return *this;
}

bool Timer::operator!() {
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

Timer::operator bool() {
	return this->m_TmrEvent;
}

void Timer::myCallback(void) {
	if (this->m_TmrRun) {
		if (this->m_TmrStandBy == RUN) {
			this->m_TmrRun--;
			if (!this->m_TmrRun) this->m_TmrEvent = true;
		}
	}
}

Timer::~Timer() { }
