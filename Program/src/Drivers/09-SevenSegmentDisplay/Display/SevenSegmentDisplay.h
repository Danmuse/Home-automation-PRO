/*!
 * @file SevenSegmentDisplay.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief Seven Segment Display Class API (Application Programming Interface).
 */

#ifndef SEVEN_SEGMENT_DISPLAY_H_
#define SEVEN_SEGMENT_DISPLAY_H_

#include "systick.h"
#include "Sweep.h"
#include "Segments.h"
#include "DigitsGroup.h"

class SevenSegmentDisplay : public Callback {
private:
	#define UPDATE_TICKS 3				//!< Writing speed of the displays expressed as milliseconds

	std::list<DigitsGroup*> m_groups;	//!< Dynamic list of digits to be used
	Segments *m_segments;				//!< Pointer to Segments class
	Sweep *m_sweep;						//!< Pointer to segment Sweep class
	uint8_t	m_maxdigits;				//!< Number of digits
	uint8_t	m_index;					//!< Position index
	uint8_t	m_ticks;					//!< Tick counter

	std::list<Digit*> m_bufferDisplay;	//!< Write buffer
	const Digit::code_t m_system;		//!< Usage system to be used
	const uint8_t *m_relativePos;		//!< Vector with user positions
public:
	SevenSegmentDisplay() = delete;
	SevenSegmentDisplay(std::list<DigitsGroup*> groups, Segments *segments, Sweep *sweep, const uint8_t *relativePos, const Digit::code_t system);
	void CallbackDisplay(void);
	void Set(uint32_t value, uint8_t display);
	virtual ~SevenSegmentDisplay();
};

#endif /* SEVEN_SEGMENT_DISPLAY_H_ */
