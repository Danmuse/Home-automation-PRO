/*/*!
 * @file ADC.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "ADC.h"

ADC *g_adc[MAX_ADC_CHANNELS] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

ADC *g_preset = nullptr;
ADC *g_adcExternal = nullptr;

uint8_t channelIndex[MAX_ADC_CHANNELS] = { 7, 6, 14, 23, 22, 21, 20, 19, 18, 17, 13, 4 };

ADC::ADC(uint32_t clk_freq, channelADC_t channel, uint32_t sample_rate, adc_sequence_t sequence) : Gpio(Gpio::PORT0, channelIndex[channel], Gpio::PULLUP, Gpio::INPUT, Gpio::LOW),
m_channel{channel},
m_sample_rate{sample_rate},
m_clk_freq{clk_freq},
m_sequence{sequence} {
	this->initialize();
	g_adc[channel] = this;
}

void ADC::initialize(void) {
	this->initADC();
	if (this->m_sequence == ADC_SEQA) this->configSEQA();
	else if (this->m_sequence == ADC_SEQB) this->configSEQB();
}

void ADC::enableSwm(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	SWM->PINENABLE0 &= ~(1 << (this->m_channel + 14)); // Bits 14 to 25 for each channel.
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void ADC::disableSwm(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	SWM->PINENABLE0 |= (1 << (this->m_channel + 14)); // Bits 14 to 25 for each channel.
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

uint32_t ADC::calculateDivisor(void) {
	return (uint32_t)(((this->m_clk_freq / ADC_STD_SAMPLE_RATE) - 1) / ADC_CLOCKS_PER_SAMPLE);
}

void ADC::configVoltage(trm_voltage_config_t config) {
	if (config) ADC0->TRM |= (1 << ADC_TRM_VRANGE_SHIFT);
	else ADC0->TRM &= ~(1 << ADC_TRM_VRANGE_SHIFT);
}

void ADC::enablePower(void) {
	// Turn on the Analog-Digital Converter (<tt>ADC</tt>).
	// To turn off tied it on low consumption.
	SYSCON->PDRUNCFG &= ~(1 << 4);
}

void ADC::enableClock(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);
	SYSCON->ADCCLKDIV &= ~ADC_CTRL_CLKDIV_MASK;
	SYSCON->ADCCLKDIV |= 1; // ADCCLK = FRO
//	SYSCON->ADCCLKSEL It could be changed if it's necessary.
//	SYSCON->ADCCLKDIV It could be changed if it's necessary.
}

void ADC::EnableNVIC_IRQ(irq_source_nvic_t source) {
	NVIC->ISER[0] |= (1 << source);
}

void ADC::DisableNVIC_IRQ(irq_source_nvic_t source) {
	NVIC->ICER[0] |= (1 << source);
}

void ADC::calibrateAdc(void) {
	uint32_t original_ctrl = ADC0->CTRL;
	uint32_t new_ctrl = (original_ctrl) & ~(ADC_CTRL_CLKDIV_MASK | (1 << ADC_CTRL_LPWRMODE_SHIFT) | (1 << ADC_SEQ_CTRL_MODE_SHIFT)); // CLKDIV = 0, LP = 0, CALMODE = 0
	new_ctrl |= (this->calculateDivisor() | (1 << ADC_SEQ_CTRL_MODE_SHIFT)); // CALMODE = 1
	ADC0->CTRL = new_ctrl; // The register must be wrote in once instruction.

	while ((ADC0->CTRL & (1 << ADC_SEQ_CTRL_MODE_SHIFT))); // Await for CALMODE = 0
	// Other way is generate 300us and ask for it, then return an error if it's not tied to zero.
	ADC0->CTRL = original_ctrl;
}

void ADC::setLowPowerMode(bool low_power) {
	if (low_power) ADC0->CTRL |= (1 << ADC_CTRL_LPWRMODE_SHIFT);
	else ADC0->CTRL &= ~(1 << ADC_CTRL_LPWRMODE_SHIFT);
}

void ADC::setSampleRate(void) {
	uint32_t clkdiv = this->calculateDivisor();
	uint32_t temp = ADC0->CTRL & ~ADC_CTRL_CLKDIV_MASK;
	ADC0->CTRL = temp | clkdiv;
}

void ADC::config(void) {
	ADC0->INTEN = 0;
	this->setLowPowerMode(false);
	this->setSampleRate();
}

void ADC::bindSEQA(void) {
	if (ADC0->SEQ_CTRL[0] & (1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT)) this->disableSEQA();
	ADC0->SEQ_CTRL[0] |= (1 << this->m_channel);
    this->enableSEQA();
}

void ADC::bindSEQB(void) {
	if (ADC0->SEQ_CTRL[1] & (1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT)) this->disableSEQB();
	ADC0->SEQ_CTRL[1] |= (1 << this->m_channel);
    this->enableSEQB();
}

void ADC::unbindSEQA(void) {
	if (ADC0->SEQ_CTRL[0] & (1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT)) this->disableSEQA();
	ADC0->SEQ_CTRL[0] &= ~(1 << this->m_channel);
    this->enableSEQA();
}

void ADC::unbindSEQB(void) {
	if (ADC0->SEQ_CTRL[1] & (1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT)) this->disableSEQB();
	ADC0->SEQ_CTRL[1] &= ~(1 << this->m_channel);
    this->enableSEQB();
}

void ADC::setModeSEQA(conversion_mode_t mode) {
	if (mode) ADC0->SEQ_CTRL[0] |= (1 << ADC_SEQ_CTRL_MODE_SHIFT);
	else ADC0->SEQ_CTRL[0] &= ~(1 << ADC_SEQ_CTRL_MODE_SHIFT);
}

void ADC::setModeSEQB(conversion_mode_t mode) {
	if (mode) ADC0->SEQ_CTRL[1] |= (1 << ADC_SEQ_CTRL_MODE_SHIFT);
	else ADC0->SEQ_CTRL[1] &= ~(1 << ADC_SEQ_CTRL_MODE_SHIFT);
}

void ADC::enableSEQA(void) {
	ADC0->SEQ_CTRL[0] |= (1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT);
}

void ADC::enableSEQB(void) {
	ADC0->SEQ_CTRL[1] |= (1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT);
}

void ADC::disableSEQA(void) {
	ADC0->SEQ_CTRL[0] &= ~(1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT);
}

void ADC::disableSEQB(void) {
	ADC0->SEQ_CTRL[1] &= ~(1 << ADC_SEQ_CTRL_SEQ_ENA_SHIFT);
}

ADC::irq_source_nvic_t ADC::getNVIC_IRQ(irq_source_inten_t irq) {
	switch (irq) {
	case ADC_SEQA_IRQ_INTEN:
		return ADC_SEQA_IRQ;
	case ADC_SEQB_IRQ_INTEN:
		return ADC_SEQB_IRQ;
	case ADC_OVR_IRQ_INTEN:
		return ADC_OVR_IRQ;
	default:
		return INVALID_IRQ;
	}
}

void ADC::enableIRQ(irq_source_inten_t irq) {
	if (this->getNVIC_IRQ(irq) != INVALID_IRQ) {
		ADC0->INTEN |= (1 << irq);
		this->EnableNVIC_IRQ(this->getNVIC_IRQ(irq));
	}
}

void ADC::disableIRQ(irq_source_inten_t irq) {
	if (this->getNVIC_IRQ(irq) != INVALID_IRQ) {
		ADC0->INTEN &= ~(1 << irq);
        this->DisableNVIC_IRQ(this->getNVIC_IRQ(irq));
	}
}

void ADC::initADC(void) {
    this->enablePower();
    this->enableClock();
    this->calibrateAdc();
    this->configVoltage(HIGH_VOLTAGE);
    this->config();
}

void ADC::configSEQA(void) {
	this->setModeSEQA(SEQUENCE_INTERRUPT);
    this->enableIRQ(ADC_SEQA_IRQ_INTEN);
	this->bindChannel();
	// The SEQA sequence has configured by default:
	// * - Without triggers by hardware.
	// * - Negative polarity.
	// * - Synchronous.
	// * - Without start condition and burst yet.
	// * - It interrupts by completed sequence, it's not by SINGLESTEP.
	// * - Lower priority.
	// * - Disabled.
}

void ADC::configSEQB(void) {
	this->setModeSEQB(SEQUENCE_INTERRUPT);
    this->enableIRQ(ADC_SEQB_IRQ_INTEN);
	this->bindChannel();
	// The SEQB sequence has configured by default:
	// * - Without triggers by hardware.
	// * - Negative polarity.
	// * - Synchronous.
	// * - Without start condition and burst yet.
	// * - It interrupts by completed sequence, it's not by SINGLESTEP.
	// * - Lower priority.
	// * - Disabled.
}

void ADC::bindChannel(void) {
    this->enableSwm();
	if (this->m_sequence == ADC_SEQA) this->bindSEQA();
	else if (this->m_sequence == ADC_SEQB) this->bindSEQB();
}

void ADC::unbindChannel(void) {
    this->disableSwm();
	if (this->m_sequence == ADC_SEQA) this->unbindSEQA();
	else if (this->m_sequence == ADC_SEQB) this->unbindSEQB();
}

void ADC::triggerSEQ(void) {
	if (this->m_sequence == ADC_SEQA) ADC0->SEQ_CTRL[0] |= (1 << ADC_SEQ_CTRL_START_SHIFT);
	else if (this->m_sequence == ADC_SEQB) ADC0->SEQ_CTRL[1] |= (1 << ADC_SEQ_CTRL_START_SHIFT);
}

uint16_t ADC::getResult(void) {
	return ((ADC0->DAT[this->m_channel] & ADC_DAT_RESULT_MASK) >> ADC_DAT_RESULT_SHIFT);
}

bool ADC::checkResult(void) {
	return this->m_acquisition_ready;
}

uint16_t ADC::analogRead(void) {
	this->triggerSEQ();
	if (this->checkResult()) this->m_acquisition_ready = false;
	return this->m_result;
}

void ADC::handlerSEQA(void) {
	this->m_result = this->getResult();
	this->m_acquisition_ready = true;

	ADC0->FLAGS |= (1 << ADC_FLAGS_SEQA_INT_SHIFT); // Cleans ADC interrupt flag.
}

void ADC::handlerSEQB(void) {
	this->m_result = this->getResult();
	this->m_acquisition_ready = true;

	ADC0->FLAGS |= (1 << ADC_FLAGS_SEQB_INT_SHIFT); // Cleans ADC interrupt flag.
}

void ADC::handler(adc_isr_t isr) {
	switch(isr) {
	case SEQA_ISR:
        this->handlerSEQA();
		break;
	case SEQB_ISR:
        this->handlerSEQB();
		break;
	case THCMP_ISR:
		// Not implemented yet
		break;
	case OVR_ISR:
		// Not implemented yet
		break;
	default:
		// INVALID VALUE
		break;
	}
}

ADC::~ADC() { }

void ADC_SEQA_IRQHandler(void) {
	for (uint8_t index = 0; index < MAX_ADC_CHANNELS; index++) {
		if (g_adc[index] != nullptr) g_adc[index]->handler(ADC::SEQA_ISR);
	}
}

void ADC_SEQB_IRQHandler(void) {
	for (uint8_t index = 0; index < MAX_ADC_CHANNELS; index++) {
		if (g_adc[index] != nullptr) g_adc[index]->handler(ADC::SEQB_ISR);
	}
}

void ADC_THCMP_IRQHandler(void) {
	for (uint8_t index = 0; index < MAX_ADC_CHANNELS; index++) {
		if (g_adc[index] != nullptr) g_adc[index]->handler(ADC::THCMP_ISR);
	}
}

void ADC_OVR_IRQHandler(void) {
	for (uint8_t index = 0; index < MAX_ADC_CHANNELS; index++) {
		if (g_adc[index] != nullptr) g_adc[index]->handler(ADC::OVR_ISR);
	}
}

/////////////////////////////
/// Preset initialization ///
/////////////////////////////

void initPreset(void) {
	#ifdef ANALOG_FST_CHANNEL_ENABLED

	static ADC preset(FREQ_CLOCK_MCU, ADC::ADC_0, ADC_SLOW_SAMPLE_RATE);

	g_preset = &preset;

	#endif // ANALOG_FST_CHANNEL_ENABLED
}

void initADC(void) {
	#ifdef ANALOG_SND_CHANNEL_ENABLED

	static ADC adc(FREQ_CLOCK_MCU, ADC::ADC_1, ADC_SLOW_SAMPLE_RATE);

	g_adcExternal = &adc;

	#endif // ANALOG_SND_CHANNEL_ENABLED
}
