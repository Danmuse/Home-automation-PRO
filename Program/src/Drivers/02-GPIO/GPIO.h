/*!
 * @file GPIO.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 * @brief GPIO API (Application Programming Interface).
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "Inputs.h"
#include "Outputs.h"
#include "LPC845.h"

#define CLK_GPIO0 6
#define CLK_GPIO1 18
#define CLK_IOCON 20

class Gpio : public Inputs, public Outputs {
protected:
	const uint8_t m_port;
	const uint8_t m_bit;
	const uint8_t m_mode;
	uint8_t m_direction;
	const uint8_t m_activity;
	uint8_t m_error;
public:
	enum port_t				{ PORT0, PORT1 };
	enum max_bits_port_t	{ B_PORT0 = 31, B_PORT1 = 9 };
	enum direction_t		{ INPUT, OUTPUT };
	enum power_t			{ OFF, ON };
	enum mode_output_t		{ PUSHPULL, OPENCOLECTOR };
	enum mode_input_t		{ INACTIVE, PULLDOWN, PULLUP, REPEATER };
	enum activity_t			{ LOW, HIGH };
	enum error_t			{ OK, ERROR };

	Gpio() = delete;
	Gpio(port_t port, uint8_t bit, uint8_t mode, direction_t direction, activity_t activity);
	void SetPin(void) override;
	void ClearPin(void) override;
	void SetDirOutputs(void) override;
	void SetDirInputs(void) override;
	void SetTogglePin(void) override;
	uint8_t GetPin(void) const override;
	void SetPinMode(void) override;
	void SetPinResistor(void) override;
	uint8_t SetToggleDir(void);
	uint8_t SetDir(void);
	~Gpio();
};

#endif /* GPIO_H_ */
