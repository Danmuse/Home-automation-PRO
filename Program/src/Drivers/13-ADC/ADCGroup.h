/*!
 * @file ADCGroup.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 * @brief Analog-Digital Converter Class Group API (Application Programming Interface).
 */

#ifndef ADC_GROUP_H_
#define ADC_GROUP_H_

#include "LPC845.h"

#if defined (__cplusplus)
extern "C" {
	void ADC_SEQA_IRQHandler(void);
}
extern "C" {
	void ADC_SEQB_IRQHandler(void);
}
extern "C" {
	void ADC_THCMP_IRQHandler(void);
}
extern "C" {
	void ADC_OVR_IRQHandler(void);
}
#endif

#define MAX_ADC_CHANNELS 12
#define CLOCKS_PER_SAMPLE 25
#define CLK_500KHz 500000

class ADC_Group {
public:
	/** Tipo de interrupciones del ADC */
	enum adc_isr_t { SEQA_ISR, SEQB_ISR, THCMP_ISR, OVR_ISR };
	/** Fuente de la interrupcion del ADC */
	enum irq_source_inten_t { ADC_SEQA_IRQ_INTEN, ADC_SEQB_IRQ_INTEN, ADC_OVR_IRQ_INTEN, INVALID_IRQ_INTEN = 0xFF };
	// Todas las de threshold no serán tenidas en cuenta
	enum error_t { OK, ERROR };
private:
	/** Fuente de interrupcion NVIC */
	enum irq_source_nvic_t { ADC_SEQA_IRQ = 16, ADC_SEQB_IRQ = 17, ADC_THCMP_IRQ = 18, ADC_OVR_IRQ = 19, INVALID_IRQ = 0xFF };
	/** Configuracion de modo de voltaje */
	enum trm_voltage_config_t { HIGH_VOLTAGE, LOW_VOLTAGE };
	/** Tipo de conversion del ADC */
	enum conversion_mode_t { CONVERSION_INTERRUPT, SEQUENCE_INTERRUPT };

	uint32_t m_sample_rate;					/**< Frecuencia de muestreo del ADC  */
	uint32_t m_clk_freq;					/**< Frecuencia del clock del ADC  */
	uint16_t m_enabled_channels;			/**< Canales habilitados del ADC  */
	uint32_t m_result[MAX_ADC_CHANNELS];	/**< Vector con resultados de conversión del ADC */
	bool m_result_ready[MAX_ADC_CHANNELS];	/**< Vector con flags de resultados listos  */

	void InitADC();
	uint32_t CalculateDivisor(uint32_t sample_rate);
	void SetADCVoltage(trm_voltage_config_t config);
	void EnableADCPower(void);
	void EnableADCClock(void);
	void ConfigSWM(uint8_t channel, bool enable);
	void CalibrateADC(void);
	void ADCConfig(void);
	void AddChannel_to_SequenceA(uint8_t channel);
	void AddChannel_to_SequenceB(uint8_t channel);
	void RemoveChannelOfSequenceA(uint8_t channel);
	void RemoveChannelOfSequenceB(uint8_t channel);
	void SetSeqAMode(conversion_mode_t mode);
	void SetSeqBMode(conversion_mode_t mode);
	void EnableSeqA(void);
	void EnableSeqB(void);
	void DisableSeqA(void);
	void DisableSeqB(void);
	void SetUpSeqA(void);
	void EnableNvicADCInterrupt(irq_source_nvic_t source);
	void DisableNvicADCInterrupt(irq_source_nvic_t source);
	irq_source_nvic_t GetNvicIrq(irq_source_inten_t irq);
	void handlerSeqA(void);
	uint16_t GetResult(uint8_t channel);
public:
	ADC_Group(uint32_t clk_freq, uint32_t sample_rate, bool init_channel0 = false);
	void Inicializar(void);

	void SetLowPowerMode(bool low_power);
	void SetSampleRate(void);

	void EnableIrq(irq_source_inten_t irq);
	void DisableIrq(irq_source_inten_t irq);

	ADC_Group::error_t InitADCChanel(uint8_t channel);		// Adds a channel to the ADC
	ADC_Group::error_t RemoveADCChanel(uint8_t channel);	// Removes a channel to the ADC
	void TriggerStartSeqA(void);

	int32_t GetValue(uint8_t channel);
	bool IsResultReady(uint8_t channel) const;
	void Handler(adc_isr_t isr);
	virtual ~ADC_Group();
};

#endif /* ADC_GROUP_H */
