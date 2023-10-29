/*!
 * @file ADC.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 * @brief Analog-Digital Converter Class API (Application Programming Interface).
 */

#ifndef ADC_H_
#define ADC_H_

#include "ADCGroup.h"
#include "ProgramConfig.h"

class ADC : protected Gpio {
private:
	static ADC_Group* m_global_adc;		//!< Covers the conversion and configuration of the Analog-Digital Converter (<tt>ADC</tt>) peripheral completely.
	uint8_t m_channel;					//!< Current Analog-Digital Converter (<tt>ADC</tt>) channel.
public:
	ADC(const Gpio& ADC, uint32_t clk_freq = 0, uint32_t sample_rate = 0);
	void initialize(void);
	int32_t get(void);
	void trigger(void);
	bool getStatus(void);
	virtual ~ADC();
};

extern ADC *g_preset;

void initPreset(void);

#endif /* ADC_H_ */
