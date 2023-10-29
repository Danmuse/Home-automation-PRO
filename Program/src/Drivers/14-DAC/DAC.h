/*!
 * @file DAC.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 * @brief Digital-Analog Converter Class API (Application Programming Interface).
 */

#ifndef DAC_H_
#define DAC_H_

#include "LPC845.h"
#include "GPIO.h"

#define MAX_DAC_CHANNELS 2
#define MAX_DAC_DEFAULT_VALUE 0x3FF

class DAC : protected Gpio {
public:
	//!< Digital-Analog Converter (<tt>DAC</tt>) channels.
	enum channel_t { FST_CHANNEL, SND_CHANNEL };
	//!< <tt>error_t</tt> enumeration reports any error. All thresholds will not be taken into account.
	enum error_t { OK, ERROR };
private:
	enum offset_t { FST_SWM_OUT = 26, SND_SWM_OUT = 27, FST_SYS_OUT = 27, SND_SYS_OUT = 1, IOCON_DAC = 16 };

	const channel_t m_channel;	//!< Digital-Analog Converter (<tt>DAC</tt>) channel.
	uint16_t m_buffer;			//!< Digital-Analog Converter (<tt>DAC</tt>) buffer values.
	uint16_t m_max_range;		//!< Maximum range of values provided by the user.
	error_t m_error;			//!< Member that indicates an error happened.

	void EnablePower(void);
	void EnableClock(void);
	void EnableSWM(void);
	void EnableIOCON(void);

	void DisablePower(void);
	void DisableClock(void);
	void DisableSWM(void);
	void DisableIOCON(void);
public:
	DAC(channel_t channel, uint16_t max_range = MAX_DAC_DEFAULT_VALUE);
	error_t initialize(void);
	void set(uint16_t value);
	uint16_t get(void) const;

	void setMaxRange(uint16_t max_range);
	uint16_t getMaxRange(void) const;

	DAC& operator=(uint16_t value);
	bool operator==(uint16_t value) const;
	bool operator<(uint16_t value) const;
	bool operator<=(uint16_t value) const;
	bool operator>(uint16_t value) const;
	bool operator>=(uint16_t value) const;
	bool operator!=(uint16_t value) const;

	virtual ~DAC();
};

#endif /* DAC_H_ */
