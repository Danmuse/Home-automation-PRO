/*!
 * @file PinInterrupt.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/09/2023 10:58:37
 * @brief Pin Interrupt Class API (Application Programming Interface).
 */

#ifndef PIN_INTERRUPT_H_
#define PIN_INTERRUPT_H_

#include "LPC845.h"
#include "GPIO.h"

#if defined (__cplusplus)
extern "C" {
	void PININT0_IRQHandler(void);
	void PININT1_IRQHandler(void);
	void PININT2_IRQHandler(void);
	void PININT3_IRQHandler(void);
	void PININT4_IRQHandler(void);
	void PININT5_IRQHandler(void);
	void PININT6_IRQHandler(void);
	void PININT7_IRQHandler(void);
}
#endif

#define MAX_PIN_INTERRUPTS 8

class PinInterrupt : protected Gpio {
private:
	static uint8_t m_quantity; //!< Number of PIN INTERRUPTS created globally
	const interrupt_mode_t m_interrupt_mode; //!< PIN INTERRUPT type
protected:
	const uint8_t m_interrupt_number; //!< Unique PIN INTERRUPT number created
public:
	PinInterrupt() = delete;
	PinInterrupt(const Gpio& interrupt, interrupt_mode_t interrupt_mode);
	void initInterrupt(void);
	void enableInterupt(void);
	void disableInterupt(void);
	// Function that is executed when performing the interrupt
	virtual void gpioHandler(void) = 0;
	virtual ~PinInterrupt();
private:
	void EnableClockInterrupt(void);
};

extern PinInterrupt *g_gpiohandler[MAX_PIN_INTERRUPTS];

#endif /* PIN_INTERRUPT_H_ */
