/*!
 * @file DigitalInputs.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 * @brief Digital Inputs Class API (Application Programming Interface).
 */

#ifndef DIGITAL_INPUTS_H_
#define DIGITAL_INPUTS_H_

#include "systick.h"
#include "GPIO.h"

#define BOUNCE_TIME 4

class DigitalInput : protected Gpio, public Callback {
private:
	const uint8_t m_maxBounces;
	uint8_t m_bounceCounter;
	bool m_lastPressed;		//!< Status of the current key pressed
	bool m_currentPressed;	//!< Key pressed pressed

	bool getHW(void);
	void getSW(bool statusPressed);
public:
	DigitalInput() = delete;
	DigitalInput(const Gpio& input, uint8_t maxBounces = BOUNCE_TIME);
	bool get(void);
	explicit operator bool(void);
	bool operator!(void);
	void CallbackMethod(void);
	virtual ~DigitalInput();
};

#endif /* DIGITAL_INPUTS_H_ */
