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
public:
	enum port_t				{ PORT0, PORT1 };
	enum max_bits_port_t	{ B_PORT0 = 31, B_PORT1 = 9 };
	enum direction_t		{ INPUT, OUTPUT };
	enum mode_output_t		{ PUSHPULL, OPENCOLECTOR };
	enum mode_input_t		{ INACTIVE, PULLDOWN, PULLUP, REPEATER };
	enum interrupt_mode_t 	{ RISING, FALLING, CHANGE, LOW_LEVEL, HIGH_LEVEL };
	enum activity_t			{ LOW, HIGH };
	enum error_t			{ OK, ERROR };
protected:
	const port_t m_port;
	const uint8_t m_bit;
	const activity_t m_activity;
	direction_t m_direction;

	error_t setDir(void);
	error_t toggleDir(void);
private:
	uint8_t m_mode;
	error_t m_error = OK;

	void setDirOutputs(void) override;
	void setDirInputs(void) override;
	void setPinMode(void) override;
	void setPinResistor(void) override;
public:
	Gpio() = delete;
	Gpio(const Gpio& original);
	Gpio(port_t port, uint8_t bit, uint8_t mode, direction_t direction, activity_t activity);
	void setPin(void) override;
	void clearPin(void) override;
	void togglePin(void) override;
	bool getPin(void) const override;
	// The following methods are used in synchronous or asynchronous communications
	port_t getPort(void) const { return this->m_port; }
	uint8_t getBit(void) const { return this->m_bit; }
	virtual ~Gpio() = default;
};

#endif /* GPIO_H_ */
