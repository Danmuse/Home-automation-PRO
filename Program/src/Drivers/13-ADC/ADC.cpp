/*/*!
 * @file ADC.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 15/10/2023 16:40:42
 */

#include "ADC.h"

ADC_Group* ADC::m_global_adc = nullptr;

uint8_t pin_index[MAX_ADC_CHANNELS] = { 7, 6, 14, 23, 22, 21, 20, 19, 18, 17, 13, 4 };

ADC *g_preset = nullptr;

ADC::ADC(const Gpio& ADC, uint32_t clk_freq, uint32_t sample_rate) : Gpio(ADC) {
	uint8_t index = 0;
	if (this->m_direction != Gpio::INPUT) this->ToggleDir();
	if (this->m_global_adc == nullptr) {
		if (clk_freq != 0 && sample_rate != 0)
			this->m_global_adc = new ADC_Group(clk_freq, sample_rate, false);
		this->m_global_adc->initialize();
	}
	for (index = 0; index < MAX_ADC_CHANNELS; index++)
		if (this->GetBit() == pin_index[index]) break;
	this->m_channel = index;
}

void ADC::initialize(void) {
	this->m_global_adc->bindChannel(this->m_channel);
}

int32_t ADC::get(void) {
	return (this->m_global_adc->getValue(this->m_channel));
}

void ADC::trigger(void) {
	this->m_global_adc->triggerSEQA();
}

bool ADC::getStatus(void) {
	return (this->m_global_adc->checkStatus(this->m_channel));
}

ADC::~ADC() {
	delete[] this->m_global_adc;
}

/////////////////////////////
/// Preset initialization ///
/////////////////////////////

void initPreset(void) {
	#ifdef ANALOG_PIN

	static ADC preset(ANALOG_POT);
	preset.initialize();

	g_preset = &preset;

	#endif // ANALOG_PIN
}
