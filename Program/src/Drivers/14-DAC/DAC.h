/*!
 * @file DAC.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 * @brief Digital-Analog Converter Class API (Application Programming Interface).
 */

#ifndef DAC_H_
#define DAC_H_

#include "ProgramConfig.h"

#define MAX_DAC_CHANNELS 2

class DAC : protected Gpio {
public:
	//!< Digital-Analog Converter (<tt>DAC</tt>) channels.
	enum channelDAC_t { DAC_0, DAC_1 };
private:
	enum offset_t { FST_SWM_OUT = 26, SND_SWM_OUT = 27, FST_SYS_OUT = 27, SND_SYS_OUT = 1, IOCON_DAC = 16 };

	const channelDAC_t m_channel;	//!< Digital-Analog Converter (<tt>DAC</tt>) channel.
	uint16_t m_buffer;				//!< Digital-Analog Converter (<tt>DAC</tt>) buffer values.
	uint16_t m_max_range;			//!< Maximum range of values provided by the user.

	void EnablePower(void);
	void EnableClock(void);
	void EnableSWM(void);
	void EnableIOCON(void);

	void DisablePower(void);
	void DisableClock(void);
	void DisableSWM(void);
	void DisableIOCON(void);
public:
	DAC(channelDAC_t channel);
	void initialize(void);
	void analogWrite(uint16_t value);
	uint16_t readBuffer(void) const;

	void bindChannel(void);
	void unbindChannel(void);
	void setMaxRange(uint16_t max_range);
	uint16_t getMaxRange(void) const;
	virtual ~DAC();
};

extern DAC *g_dacExternal;

void initDAC(void);

#endif /* DAC_H_ */
