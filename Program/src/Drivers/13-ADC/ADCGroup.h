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
	void ADC_SEQB_IRQHandler(void);
	void ADC_THCMP_IRQHandler(void);
	void ADC_OVR_IRQHandler(void);
}
#endif

#define MAX_ADC_CHANNELS 12
#define ADC_CLOCKS_PER_SAMPLE 25
#define ADC_STD_FREQUENCY 500000

class ADC_Group {
public:
	//!< <tt>adc_isr_t</tt> enumeration indicates the Analog-Digital Converter (<tt>ADC</tt>) interrupt types.
	enum adc_isr_t { SEQA_ISR, SEQB_ISR, THCMP_ISR, OVR_ISR };
	//!< <tt>irq_source_inten_t</tt> enumeration indicates the Analog-Digital Converter (<tt>ADC</tt>) interrupt source.
	enum irq_source_inten_t { ADC_SEQA_IRQ_INTEN, ADC_SEQB_IRQ_INTEN, ADC_OVR_IRQ_INTEN, INVALID_IRQ_INTEN = 0xFF };
	//!< <tt>error_t</tt> enumeration reports any error. All thresholds will not be taken into account.
	enum error_t { OK, ERROR };
private:
	//!< Nested Vectored Interrupt Controller (<tt>NVIC</tt>) interrupt source.
	enum irq_source_nvic_t { ADC_SEQA_IRQ = 16, ADC_SEQB_IRQ = 17, ADC_THCMP_IRQ = 18, ADC_OVR_IRQ = 19, INVALID_IRQ = 0xFF };
	//!< Analog-Digital Converter (<tt>ADC</tt>) settings for voltage mode.
	enum trm_voltage_config_t { HIGH_VOLTAGE, LOW_VOLTAGE };
	//!< Analog-Digital Converter (<tt>ADC</tt>) conversions types..
	enum conversion_mode_t { CONVERSION_INTERRUPT, SEQUENCE_INTERRUPT };

	uint32_t m_sample_rate;					//!< Sampling frequency of the Analog-Digital Converter (<tt>ADC</tt>).
	uint32_t m_clk_freq;					//!< Clock frequency of the Analog-Digital Converter (<tt>ADC</tt>).
	uint16_t m_enabled_channels;			//!< Channels enabled of the Analog-Digital Converter (<tt>ADC</tt>).
	uint32_t m_result[MAX_ADC_CHANNELS];	//!< Conversion results vector of the Analog-Digital Converter (<tt>ADC</tt>).
	bool m_result_ready[MAX_ADC_CHANNELS];	//!< Vector of the states for each prepared result of the Analog-Digital Converter (<tt>ADC</tt>).

	void initADC(void);
	uint32_t CalculateDivisor(uint32_t sample_rate);
	void ConfigVoltage(trm_voltage_config_t config);
	void EnablePower(void);
	void EnableClock(void);
	void EnableSWM(uint8_t channel, bool enable);
	void CalibrateADC(void);
	void Config(void);
	void bindSEQA(uint8_t channel);
	void bindSEQB(uint8_t channel);
	void unbindSEQA(uint8_t channel);
	void unbindSEQB(uint8_t channel);
	void setModeSEQA(conversion_mode_t mode);
	void setModeSEQB(conversion_mode_t mode);
	void EnableSEQA(void);
	void EnableSEQB(void);
	void DisableSEQA(void);
	void DisableSEQB(void);
	void ConfigSEQA(void);
	void EnableNVIC_IRQ(irq_source_nvic_t source);
	void DisableNVIC_IRQ(irq_source_nvic_t source);
	irq_source_nvic_t getNVIC_IRQ(irq_source_inten_t irq);
	void HandlerSEQA(void);
	uint16_t getResult(uint8_t channel);
public:
	ADC_Group(uint32_t clk_freq, uint32_t sample_rate, bool init_channel0 = false);
	void initialize(void);

	void setLowPowerMode(bool low_power);
	void setSampleRate(void);

	void EnableIRQ(irq_source_inten_t irq);
	void DisableIRQ(irq_source_inten_t irq);

	error_t bindChannel(uint8_t channel);
	error_t unbindChannel(uint8_t channel);
	void triggerSEQA(void);

	int32_t getValue(uint8_t channel);
	bool checkStatus(uint8_t channel) const;
	void Handler(adc_isr_t isr);
	virtual ~ADC_Group();
};

#endif /* ADC_GROUP_H */
