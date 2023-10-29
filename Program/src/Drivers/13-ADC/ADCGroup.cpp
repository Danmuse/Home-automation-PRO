/*/*!
 * @file ADCGroup.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "ADCGroup.h"

ADC_Group *g_adc = nullptr;

ADC_Group::ADC_Group(uint32_t clk_freq, uint32_t sample_rate, bool init_channel0) :
m_sample_rate{sample_rate},
m_clk_freq{clk_freq},
m_enabled_channels{0} {
	if (init_channel0) this->bindChannel(false);
	g_adc = this;
}

void ADC_Group::initialize(void) {
	this->initADC();
	this->ConfigSEQA();
}

void ADC_Group::EnableSWM(uint8_t channel, bool enable) {
	uint8_t bit_index = channel + 14;
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (enable) SWM->PINENABLE0 &= ~(1 << bit_index); // Bits 14 to 21 for each channel.
	else SWM->PINENABLE0 |= (1 << bit_index); // Bits 14 to 21 for each channel.
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

uint32_t ADC_Group::CalculateDivisor(uint32_t sample_rate) {
	return (uint32_t)(((this->m_clk_freq / sample_rate) - 1) / ADC_CLOCKS_PER_SAMPLE);
}

void ADC_Group::ConfigVoltage(trm_voltage_config_t config) {
	if (config) ADC0->TRM |= (1 << 5);
	else ADC0->TRM &= ~(1 << 5);
}

void ADC_Group::EnablePower(void) {
	// Turn on the Analog-Digital Converter (<tt>ADC</tt>).
	// To turn off tied it on low consumption.
	SYSCON->PDRUNCFG &= ~(1 << 4);
}

void ADC_Group::EnableClock(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);
	SYSCON->ADCCLKDIV &= ~(0xFFUL);
	SYSCON->ADCCLKDIV |= 1; // ADCCLK = FRO
//	SYSCON->ADCCLKSEL // It could be changed if it's necessary.
//	SYSCON->ADCCLKDIV // It could be changed if it's necessary.
}

void ADC_Group::EnableNVIC_IRQ(irq_source_nvic_t source) {
	NVIC->ISER[0] |= (1 << source);
}

void ADC_Group::DisableNVIC_IRQ(irq_source_nvic_t source) {
	NVIC->ICER[0] |= (1 << source);
}

void ADC_Group::CalibrateADC(void) {
	uint32_t original_ctrl = ADC0->CTRL;
	uint32_t new_ctrl = (original_ctrl) & ~(0xFFUL | (1 << 10) | (1 << 30)); // CLKDIV = 0, LP = 0, CALMODE = 0
	new_ctrl |= (this->CalculateDivisor(ADC_STD_FREQUENCY) | (1 << 30)); // CALMODE = 1, CLKDIV para 500KHz
	ADC0->CTRL = new_ctrl; // The register must be wrote in once instruction.

	while ((ADC0->CTRL & (1 << 30))); // Await for CALMODE = 0
	// Other way is generate 300us and ask for it, then return an error if it's not tied to zero.
	ADC0->CTRL = original_ctrl;
}

void ADC_Group::setLowPowerMode(bool low_power) {
	if (low_power) ADC0->CTRL |= (1 << 10);
	else ADC0->CTRL &= ~(1 << 10);
}

void ADC_Group::setSampleRate(void) {
	uint32_t clkdiv = this->CalculateDivisor(this->m_sample_rate);
	uint32_t temp = ADC0->CTRL & ~(0xFFUL);
	ADC0->CTRL = temp | clkdiv;
	// this->m_sample_rate = sample_rate; // UNUSED INSTRUCTION
}

void ADC_Group::Config(void) {
	ADC0->INTEN = 0;
	this->setLowPowerMode(false);
	this->setSampleRate();
}

void ADC_Group::bindSEQA(uint8_t channel) {
	ADC0->SEQ_CTRL[0] |= (1 << channel);
}

void ADC_Group::bindSEQB(uint8_t channel) {
	ADC0->SEQ_CTRL[1] |= (1 << channel);
}

void ADC_Group::unbindSEQA(uint8_t channel) {
	ADC0->SEQ_CTRL[0] &= ~(1 << channel);
}

void ADC_Group::unbindSEQB(uint8_t channel) {
	ADC0->SEQ_CTRL[1] &= ~(1 << channel);
}

void ADC_Group::setModeSEQA(conversion_mode_t mode) {
	if (mode) ADC0->SEQ_CTRL[0] |= (1 << 30);
	else ADC0->SEQ_CTRL[0] &= ~(1 << 30);
}

void ADC_Group::setModeSEQB(conversion_mode_t mode) {
	if (mode) ADC0->SEQ_CTRL[1] |= (1 << 30);
	else ADC0->SEQ_CTRL[1] &= ~(1 << 30);
}

void ADC_Group::EnableSEQA(void) {
	ADC0->SEQ_CTRL[0] |= (1 << 31);
}

void ADC_Group::EnableSEQB(void) {
	ADC0->SEQ_CTRL[1] |= (1 << 31);
}

void ADC_Group::DisableSEQA(void) {
	ADC0->SEQ_CTRL[0] &= ~(1 << 31);
}

void ADC_Group::DisableSEQB(void) {
	ADC0->SEQ_CTRL[1] &= ~(1 << 31);
}

ADC_Group::irq_source_nvic_t ADC_Group::getNVIC_IRQ(irq_source_inten_t irq) {
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

void ADC_Group::EnableIRQ(irq_source_inten_t irq) {
	irq_source_nvic_t irq_nvic = this->getNVIC_IRQ(irq);
	if (irq_nvic != INVALID_IRQ) {
		ADC0->INTEN |= (1 << irq);
		this->EnableNVIC_IRQ(irq_nvic);
	}
}

void ADC_Group::DisableIRQ(irq_source_inten_t irq) {
	irq_source_nvic_t irq_nvic = this->getNVIC_IRQ(irq);
	if (irq_nvic != INVALID_IRQ) {
		ADC0->INTEN &= ~(1 << irq);
		this->DisableNVIC_IRQ(irq_nvic);
	}
}

void ADC_Group::initADC(void) {
	this->EnablePower();
	this->EnableClock();
	this->CalibrateADC();
	this->ConfigVoltage(HIGH_VOLTAGE);
	this->Config();
}

void ADC_Group::ConfigSEQA(void) {
	this->setModeSEQA(SEQUENCE_INTERRUPT);
	/* The SEQA sequence has configured by default:
	 * - Without triggers by hardware.
	 * - Negative polarity.
	 * - Synchronous.
	 * - Without start condition and burst yet.
	 * - It interrupts by completed sequence, it's not by SINGLESTEP.
	 * - Lower priority.
	 * - Disabled.
	 */
	this->EnableSEQA();
	this->EnableIRQ(ADC_SEQA_IRQ_INTEN);
}

ADC_Group::error_t ADC_Group::bindChannel(uint8_t channel) {
	if(channel >= MAX_ADC_CHANNELS) return ERROR;
	this->EnableSWM(channel, true);
	this->bindSEQA(channel);
	this->m_enabled_channels |= (1 << channel);
	return OK;
}

ADC_Group::error_t ADC_Group::unbindChannel(uint8_t channel) {
	if (channel >= MAX_ADC_CHANNELS) return ERROR;
	this->EnableSWM(channel, false);
	this->unbindSEQA(channel);
	this->m_enabled_channels &= ~(1 << channel);
	return OK;
}

void ADC_Group::triggerSEQA(void) {
	ADC0->SEQ_CTRL[0] |= (1 << 26);
}

uint16_t ADC_Group::getResult(uint8_t channel) {
	return ((ADC0->DAT[channel] >> 4) & 0xFFF);
}

int32_t ADC_Group::getValue(uint8_t channel) {
	if (channel >= MAX_ADC_CHANNELS) return ERROR;
	if (this->m_result_ready[channel]) {
		this->m_result_ready[channel] = false;
		return (int32_t)(this->m_result[channel]);
	}
	return -2; // There's no value
}

bool ADC_Group::checkStatus(uint8_t channel) const {
	if (channel >= MAX_ADC_CHANNELS) return false;
	return m_result_ready[channel];
}

void ADC_Group::HandlerSEQA(void) {
	uint8_t index = 0;
	for (index = 0; index < MAX_ADC_CHANNELS; index++) {
		if ((this->m_enabled_channels >> index) & 1) {
			this->m_result[index] = this->getResult(index);
			this->m_result_ready[index] = true;
		}
	}
	ADC0->FLAGS |= (1 << 28); // Cleans ADC interrupt flag.
}

void ADC_Group::Handler(adc_isr_t isr) {
	switch(isr) {
	case SEQA_ISR:
		this->HandlerSEQA();
		break;
	case SEQB_ISR:
		// Not implemented
		break;
	case THCMP_ISR:
		// Not implemented
		break;
	case OVR_ISR:
		// Not implemented
		break;
	default:
		// INVALID VALUE
		break;
	}
}

ADC_Group::~ADC_Group() { }

void ADC_SEQA_IRQHandler(void) {
	g_adc->Handler(ADC_Group::SEQA_ISR);
}

void ADC_SEQB_IRQHandler(void) {
	g_adc->Handler(ADC_Group::SEQB_ISR);
}

void ADC_THCMP_IRQHandler(void) {
	g_adc->Handler(ADC_Group::THCMP_ISR);
}

void ADC_OVR_IRQHandler(void) {
	g_adc->Handler(ADC_Group::OVR_ISR);
}
