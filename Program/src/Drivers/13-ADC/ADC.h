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
#include "GPIO.h"

class ADC : protected Gpio {
private:
	static ADC_Group* m_global_adc;		/**< Grupo de ADCs. Realiza la conversión y configuración del periférico ADC por completo. */
	uint8_t m_channel;					/**< Canal del ADC actual*/
public:
	ADC(uint8_t channel, uint32_t clk_freq = 0, uint32_t sample_rate = 0);
	int32_t Get(void);
	bool IsResultReady(void);
	void Trigger(void);
	void Inicializar(void);
	virtual ~ADC();
};

#endif /* ADC_H_ */
