/*!
 * @file ADC.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 * @brief Analog-Digital Converter Class API (Application Programming Interface).
 */

#ifndef ADC_H_
#define ADC_H_

#include "ProgramConfig.h"

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
#define ADC_STD_SAMPLE_RATE 500000
#define ADC_SLOW_SAMPLE_RATE 100000

class ADC : protected Gpio {
public:
	//!< Analog-Digital Converter (<tt>ADC</tt>) channels.
	enum channelADC_t { ADC_0, ADC_1, ADC_2, ADC_3, ADC_4, ADC_5, ADC_6, ADC_7, ADC_8, ADC_9, ADC_10, ADC_11 };
	//!< <tt>adc_sequence_t</tt> enumeration indicates the Analog-Digital Converter (<tt>ADC</tt>) sequences.
	enum adc_sequence_t { ADC_SEQA, ADC_SEQB };
	//!< <tt>adc_isr_t</tt> enumeration indicates the Analog-Digital Converter (<tt>ADC</tt>) interrupt types.
	enum adc_isr_t { SEQA_ISR, SEQB_ISR, THCMP_ISR, OVR_ISR };
	//!< <tt>irq_source_inten_t</tt> enumeration indicates the Analog-Digital Converter (<tt>ADC</tt>) interrupt source.
	enum irq_source_inten_t { ADC_SEQA_IRQ_INTEN, ADC_SEQB_IRQ_INTEN, ADC_OVR_IRQ_INTEN, INVALID_IRQ_INTEN = 0xFF };
private:
	//!< Nested Vectored Interrupt Controller (<tt>NVIC</tt>) interrupt source.
	enum irq_source_nvic_t { ADC_SEQA_IRQ = 16, ADC_SEQB_IRQ = 17, ADC_THCMP_IRQ = 18, ADC_OVR_IRQ = 19, INVALID_IRQ = 0xFF };
	//!< Analog-Digital Converter (<tt>ADC</tt>) settings for voltage mode.
	enum trm_voltage_config_t { HIGH_VOLTAGE, LOW_VOLTAGE };
	//!< Analog-Digital Converter (<tt>ADC</tt>) conversions types.
	enum conversion_mode_t { CONVERSION_INTERRUPT, SEQUENCE_INTERRUPT };

	const channelADC_t m_channel; 			//!< Analog-Digital Converter (<tt>ADC</tt>) channel.
	uint32_t m_sample_rate;					//!< Sampling rate of the Analog-Digital Converter (<tt>ADC</tt>).
	uint32_t m_clk_freq;					//!< Clock frequency of the Analog-Digital Converter (<tt>ADC</tt>).
	adc_sequence_t m_sequence;				//!< Sequence of the Analog-Digital Converter (<tt>ADC</tt>).
	uint32_t m_result;						//!< Result of the Analog-Digital Converter (<tt>ADC</tt>) conversion.
	bool m_acquisition_ready;				//!< Status of the Analog-Digital Converter (<tt>ADC</tt>) conversion.

	void initADC(void);
	uint32_t CalculateDivisor(void);
	void ConfigVoltage(trm_voltage_config_t config);
	void EnablePower(void);
	void EnableClock(void);
	void EnableSWM(void);
	void DisableSWM(void);
	void CalibrateADC(void);
	void Config(void);
	void setSampleRate(void);
	void bindSEQA(void);
	void bindSEQB(void);
	void unbindSEQA(void);
	void unbindSEQB(void);
	void setModeSEQA(conversion_mode_t mode);
	void setModeSEQB(conversion_mode_t mode);
	void EnableSEQA(void);
	void EnableSEQB(void);
	void DisableSEQA(void);
	void DisableSEQB(void);
	void ConfigSEQA(void);
	void ConfigSEQB(void);
	void EnableNVIC_IRQ(irq_source_nvic_t source);
	void DisableNVIC_IRQ(irq_source_nvic_t source);
	irq_source_nvic_t getNVIC_IRQ(irq_source_inten_t irq);

	void triggerSEQ(void);
	void HandlerSEQA(void);
	void HandlerSEQB(void);
	uint16_t getResult(void);
public:
	ADC(uint32_t clk_freq, channelADC_t channel, uint32_t sample_rate, adc_sequence_t sequence = ADC_SEQA);
	void initialize(void);
	void setLowPowerMode(bool low_power);
	void bindChannel(void);
	void unbindChannel(void);
	void EnableIRQ(irq_source_inten_t irq);
	void DisableIRQ(irq_source_inten_t irq);
	void Handler(adc_isr_t isr);
	bool checkResult(void);
	uint16_t analogRead(void);
	virtual ~ADC();
};

extern ADC *g_preset;
extern ADC *g_adcExternal;

void initPreset(void);
void initADC(void);

#endif /* ADC_H_ */
