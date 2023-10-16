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
	if (init_channel0) this->InitADCChanel(false);
	g_adc = this;
}

void ADC_Group::Inicializar(void) {
	this->InitADC();
	this->SetUpSeqA();
}

void ADC_Group::ConfigSWM(uint8_t channel, bool enable) {
	uint8_t bit_index = channel + 14;
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (enable) SWM->PINENABLE0 &= ~(1 << bit_index); //Bits 14 a 21 para cada canal
	else SWM->PINENABLE0 |= (1 << bit_index); //Bits 14 a 21 para cada canal
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void ADC_Group::EnableADCPower(void) {
	SYSCON->PDRUNCFG &= ~(1 << 4); // Prender el ADC. Para apagar se puede poner en bajo consumo
}

void ADC_Group::EnableADCClock(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 24); //24 es para ADC0 (que es el único ADC)
	SYSCON->ADCCLKDIV &= ~(0xFFUL);
	SYSCON->ADCCLKDIV |= 1; // ADCCLK = FRO
	//SYSCON->ADCCLKSEL podría ser modificado en caso de ser necesario
	//SYSCON->ADCCLKDIV podría ser modificado en caso de ser necesario
}

void ADC_Group::EnableNvicADCInterrupt(irq_source_nvic_t source) {
	NVIC->ISER[0] |= (1 << source);
}

void ADC_Group::DisableNvicADCInterrupt(irq_source_nvic_t source) {
	NVIC->ICER[0] |= (1 << source);
}

uint32_t ADC_Group::CalculateDivisor(uint32_t sample_rate) {
	return (uint32_t)(((this->m_clk_freq / sample_rate) - 1) / CLOCKS_PER_SAMPLE);
}

void ADC_Group::SetADCVoltage(trm_voltage_config_t config) {
	if (config) ADC0->TRM |= (1 << 5);
	else ADC0->TRM &= ~(1 << 5);
}

void ADC_Group::CalibrateADC(void) {
	uint32_t original_ctrl = ADC0->CTRL;
	uint32_t new_ctrl = (original_ctrl) & ~(0xFFUL | (1 << 10) | (1 << 30)); //clkdiv = 0, LP = 0, CALMODE = 0
	new_ctrl |= (this->CalculateDivisor(CLK_500KHz) | (1 << 30)); // CALMODE = 1, CLKDIV para 500KHz
	ADC0->CTRL = new_ctrl; // El registro debe ser escrito en una única instruccion

	while ((ADC0->CTRL & (1 << 30))); // Esperar CALMODE = 0
	// Otra forma sería esperar 300us y preguntar, devolver error si no está en 0.
	ADC0->CTRL = original_ctrl;
}

void ADC_Group::SetLowPowerMode(bool low_power) {
	if (low_power) ADC0->CTRL |= (1 << 10);
	else ADC0->CTRL &= ~(1 << 10);
}

void ADC_Group::SetSampleRate(void) {
	uint32_t clkdiv = this->CalculateDivisor(this->m_sample_rate);
	uint32_t temp = ADC0->CTRL & ~(0xFFUL);
	ADC0->CTRL = temp | clkdiv;
	// this->m_sample_rate = sample_rate; // Por si se decide cambiar a posteriori
}

void ADC_Group::ADCConfig(void) {
	ADC0->INTEN = 0; // Deshabilitar interrupciones mientras inicializa
	this->SetLowPowerMode(false);
	this->SetSampleRate();
}

void ADC_Group::AddChannel_to_SequenceA(uint8_t channel) {
	ADC0->SEQ_CTRL[0] |= (1 << channel);
}

void ADC_Group::AddChannel_to_SequenceB(uint8_t channel) {
	ADC0->SEQ_CTRL[1] |= (1 << channel);
}

void ADC_Group::RemoveChannelOfSequenceA(uint8_t channel) {
	ADC0->SEQ_CTRL[0] &= ~(1 << channel);
}

void ADC_Group::RemoveChannelOfSequenceB(uint8_t channel) {
	ADC0->SEQ_CTRL[1] &= ~(1 << channel);
}

void ADC_Group::SetSeqAMode(conversion_mode_t mode) {
	if (mode) ADC0->SEQ_CTRL[0] |= (1 << 30);
	else ADC0->SEQ_CTRL[0] &= ~(1 << 30);
}

void ADC_Group::SetSeqBMode(conversion_mode_t mode) {
	if (mode) ADC0->SEQ_CTRL[1] |= (1 << 30);
	else ADC0->SEQ_CTRL[1] &= ~(1 << 30);
}

void ADC_Group::EnableSeqA(void) {
	ADC0->SEQ_CTRL[0] |= (1 << 31);
}

void ADC_Group::EnableSeqB(void) {
	ADC0->SEQ_CTRL[1] |= (1 << 31);
}

void ADC_Group::DisableSeqA(void) {
	ADC0->SEQ_CTRL[0] &= ~(1 << 31);
}

void ADC_Group::DisableSeqB(void) {
	ADC0->SEQ_CTRL[1] &= ~(1 << 31);
}

ADC_Group::irq_source_nvic_t ADC_Group::GetNvicIrq(irq_source_inten_t irq) {
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

void ADC_Group::EnableIrq(irq_source_inten_t irq) {
	irq_source_nvic_t irq_nvic = this->GetNvicIrq(irq);
	if (irq_nvic != INVALID_IRQ) {
		ADC0->INTEN |= (1 << irq);
		this->EnableNvicADCInterrupt(irq_nvic);
	}
}

void ADC_Group::DisableIrq(irq_source_inten_t irq) {
	irq_source_nvic_t irq_nvic = this->GetNvicIrq(irq);
	if (irq_nvic != INVALID_IRQ) {
		ADC0->INTEN &= ~(1 << irq);
		this->DisableNvicADCInterrupt(irq_nvic);
	}
}

void ADC_Group::InitADC(void) {
	this->EnableADCPower();
	this->EnableADCClock();
	this->CalibrateADC();
	this->SetADCVoltage(HIGH_VOLTAGE);
	this->ADCConfig();
}

void ADC_Group::SetUpSeqA(void) {
	this->SetSeqAMode(SEQUENCE_INTERRUPT);
	/* El resto de la secuencia A queda configurado por defecto:
	 * Sin triggers por hardware
	 * Polaridad negativa
	 * Sincrónico
	 * Ni Start, ni Burst todavía
	 * Interrumpe por secuencia completa, no por SINGLESTEP
	 * Prioridad baja
	 * Deshabilitado
	 */
	this->EnableSeqA();
	this->EnableIrq(ADC_SEQA_IRQ_INTEN);
}

ADC_Group::error_t ADC_Group::InitADCChanel(uint8_t channel) {
	if(channel >= MAX_ADC_CHANNELS) return ERROR;
	this->ConfigSWM(channel, true);
	this->AddChannel_to_SequenceA(channel);
	this->m_enabled_channels |= (1 << channel);
	return OK;
}

ADC_Group::error_t ADC_Group::RemoveADCChanel(uint8_t channel) {
	if (channel >= MAX_ADC_CHANNELS) return ERROR;
	this->ConfigSWM(channel, false);
	this->RemoveChannelOfSequenceA(channel);
	this->m_enabled_channels &= ~(1 << channel);
	return OK;
}

void ADC_Group::TriggerStartSeqA(void) {
	ADC0->SEQ_CTRL[0] |= (1 << 26);
}

uint16_t ADC_Group::GetResult(uint8_t channel) {
	return ((ADC0->DAT[channel] >> 4) & 0xFFF);
}

int32_t ADC_Group::GetValue(uint8_t channel) {
	if (channel >= MAX_ADC_CHANNELS) return ERROR;
	if (this->m_result_ready[channel]) {
		this->m_result_ready[channel] = false;
		return (int32_t)(this->m_result[channel]);
	}
	return -2; // There's no value
}

bool ADC_Group::IsResultReady(uint8_t channel) const {
	if (channel >= MAX_ADC_CHANNELS) return false;
	return m_result_ready[channel];
}

void ADC_Group::handlerSeqA(void) {
	uint8_t index = 0;
	for (index = 0; index < MAX_ADC_CHANNELS; index++) {
		if ((this->m_enabled_channels >> index) & 1) {
			this->m_result[index] = this->GetResult(index);
			this->m_result_ready[index] = true;
		}
	}
	ADC0->FLAGS |= (1 << 28); // Cleans ADC interrupt flag.
}

void ADC_Group::Handler(adc_isr_t isr) {
	switch(isr) {
	case SEQA_ISR:
		this->handlerSeqA();
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
