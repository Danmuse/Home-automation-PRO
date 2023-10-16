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
	/** Canales del DAC */
	enum channel_t { DAC_CHANNEL_0, DAC_CHANNEL_1 };
	/** Error en la clase DAC  */
	enum error_t { OK, ERROR };
private:
	/** Offset of the registers */
	enum offset_t { SWM_DACOUT_0 = 26, SWM_DACOUT_1 = 27, SYS_DACOUT_0 = 27, SYS_DACOUT_1 = 1, IOCON_DAC = 16 };

	const channel_t m_channel;	/**< Canal del objeto DAC  */
	uint16_t m_buffer;			/**< Buffer del valor del DAC  */
	uint16_t m_max_range;		/**< Rango máximo de valores dado por el usuario  */
	error_t m_error;			/**< Error en la clase DAC  */

	void PowerDAC(void);
	void EnableClock(void);
	void EnableSWM(void);
	void EnableIOCONDAC(void);

	void UnPowerDAC(void);
	void DisableClock(void);
	void DisableSWM(void);
	void DisableIOCONDAC(void);
public:
	DAC(channel_t channel, uint16_t max_range = MAX_DAC_DEFAULT_VALUE);
	error_t Inicializar(void);
	void Set(uint16_t value);
	uint16_t Get(void) const;

	void SetMaxRange(uint16_t max_range);
	uint16_t GetMaxRange(void) const;

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
