/*!
 * @file Timer.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 22:02:36
 * @brief Timer Class API (Application Programming Interface).
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Callback.h"

typedef void (*Timer_Handler)(void);

class Timer : public Callback {
public:
	enum bases_t	{ DEC, SEC, MIN, HOUR, DAY };
	enum ticks_t	{ DECIMALS = 100, SECONDS = 10, MINUTES = 60, HOURS = 60, DAYS = 24 };
	enum error_t	{ OK, ERROR };
	enum standby_t	{ RUN, PAUSE };
protected:
	volatile uint32_t	m_TmrRun;		// Counts
	volatile bool		m_TmrEvent;		// Terminated timer (Shot a flag)
	Timer_Handler		m_TmrHandler;	// Function to be executed when it's terminated
	volatile standby_t	m_TmrStandBy; 	// Pause the timer
	volatile uint8_t	m_TmrBase;		// Timer unit (DEC - SEG - MIN)
public:
	Timer();
	Timer(const Timer_Handler handler, const bases_t base);
	void TimerStart(uint32_t counter, const Timer_Handler handler, const bases_t base);
	void TimerStart(uint32_t counter);
	void SetTimer(uint32_t time);
	void GetTimer(uint32_t &time) const;
	uint32_t GetTimer(void) const;
	void StandBy(standby_t action);
	void TimerStop(void);
	error_t TimerEvent(void);
	Timer& operator=(uint32_t counter);
	bool operator!();
	bool operator==(bool checkEvent);
	friend bool operator==(bool checkEvent, Timer& timer);
	explicit operator bool();
	void myCallback(void);
	virtual ~Timer();
};

#endif /* TIMER_H_ */
