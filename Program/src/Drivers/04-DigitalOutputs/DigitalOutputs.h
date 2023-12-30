/*!
 * @file DigitalOutputs.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 * @brief Digital Outputs Class API (Application Programming Interface).
 */

#ifndef DIGITAL_OUTPUTS_H_
#define DIGITAL_OUTPUTS_H_

#include "systick.h"
#include "GPIO.h"

class DigitalOutput : protected Gpio, public Callback {
public:
	DigitalOutput() = delete;
	DigitalOutput(const Gpio& output, Gpio::activity_t state);
	void set(void);
	void clear(void);
	DigitalOutput& operator=(Gpio::activity_t state);
	void callbackMethod(void) override;
	virtual ~DigitalOutput();
};

#endif /* DIGITAL_OUTPUTS_H_ */
