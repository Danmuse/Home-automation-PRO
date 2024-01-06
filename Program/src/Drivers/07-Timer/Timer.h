/*!
 * @file Timer.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 22:02:36
 * @brief Timer Class API (Application Programming Interface).
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "systick.h"

typedef void (*TimerHandler)(void);

class Timer : public Callback {
public:
	enum bases_t	{ MILLI, DEC, SEC, MIN, HOUR, DAY };
	enum ticks_t	{ MILLIS = 1, DECIMALS = 100, SECONDS = 10, MINUTES = 60, HOURS = 60, DAYS = 24 };
	enum error_t	{ OK, ERROR };
	enum standby_t	{ RUN, PAUSE };
protected:
	volatile uint32_t	m_TmrRun;		// Counts
	volatile bool		m_TmrEvent;		// Terminated timer (Shot a flag)
	TimerHandler		m_TmrHandler;	// Function to be executed when it's terminated
	volatile standby_t	m_TmrStandBy; 	// Pause the timer
	volatile uint8_t	m_TmrBase;		// Timer unit (DEC - SEG - MIN)
public:
	Timer();
	Timer(const TimerHandler handler, const bases_t base);
	void timerStart(uint32_t counter, const TimerHandler handler, const bases_t base);
	void TimerStart(uint32_t counter);
	void setTimer(uint32_t time);
	uint32_t getTimer(void) const;
	uint32_t getTicks(void) const;
	void standBy(standby_t action);
	void timerStop(void);
	error_t timerEvent(void);
	Timer& operator=(uint32_t counter);
	bool operator!(void);
	bool operator==(bool checkEvent);
	friend bool operator==(bool checkEvent, Timer& timer);
	explicit operator bool(void);
	void callbackMethod(void) override;
	virtual ~Timer();
};

#endif /* TIMER_H_ */
