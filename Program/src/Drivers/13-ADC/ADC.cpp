/*/*!
 * @file ADC.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "ADC.h"

ADC_Group* ADC::m_global_adc = nullptr;

uint8_t pin_index[MAX_ADC_CHANNELS] = { 7, 6, 14, 23, 22, 21, 20, 19, 18, 17, 13, 4 };

ADC::ADC(uint8_t channel, uint32_t clk_freq, uint32_t sample_rate) : Gpio(Gpio::PORT0, pin_index[channel], Gpio::PULLDOWN, Gpio::INPUT, Gpio::LOW) {
	if (this->m_global_adc == nullptr) {
		if (clk_freq != 0 && sample_rate != 0)
			this->m_global_adc = new ADC_Group(clk_freq, sample_rate, false);
		this->m_global_adc->Inicializar();
	}
	this->m_channel = channel;
}

void ADC::Inicializar(void) {
	this->m_global_adc->InitADCChanel(this->m_channel);
}

int32_t ADC::Get(void) {
	return (this->m_global_adc->GetValue(this->m_channel));
}

void ADC::Trigger(void) {
	this->m_global_adc->TriggerStartSeqA();
}

bool ADC::IsResultReady(void) {
	return (this->m_global_adc->IsResultReady(this->m_channel));
}

ADC::~ADC() {
	delete[] this->m_global_adc;
}
