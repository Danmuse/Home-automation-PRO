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
#include "DigitsGroup.h"
#include "CD4511B.h"
#include "CD4017.h"

#define UPDATE_TICKS 3	//!< Writing speed of the displays expressed as milliseconds

class SevenSegmentDisplay : public Callback {
private:
	std::vector<DigitsGroup*> m_groups;	//!< Dynamic list of digits to be used
	CD4511B *m_segments;				//!< Pointer to Segments class
	CD4017 *m_sweep;						//!< Pointer to segment Sweep class
	uint8_t	m_maxdigits;				//!< Number of digits
	uint8_t	m_index;					//!< Position index
	uint8_t	m_ticks;					//!< Tick counter

	std::vector<Digit*> m_bufferDisplay;	//!< Write buffer
	const Digit::code_t m_system;		//!< Usage system to be used
	const uint8_t *m_relativePos;		//!< Vector with user positions
public:
	SevenSegmentDisplay() = delete;
	SevenSegmentDisplay(std::vector<DigitsGroup*> groups, CD4511B *segments_IC, CD4017 *sweep_IC, const uint8_t *relativePos, const Digit::code_t system);
	void CallbackMethod(void);
	void Set(uint32_t value, uint8_t display);
	virtual ~SevenSegmentDisplay();
};

#endif /* SEVEN_SEGMENT_DISPLAY_H_ */