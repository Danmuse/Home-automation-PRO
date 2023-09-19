/*!
 * @file CD4017.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief CD4017 Class API (Application Programming Interface).
 */

#ifndef CD4017_H_
#define CD4017_H_

#include <vector>
#include "Sweep.h"
#include "GPIO.h"

#define RST_INDEX 0
#define CLK_INDEX 1

class CD4017 : public Sweep {
private:
	uint8_t m_index;
	const std::vector<Gpio*> &m_pins4017;
	const uint8_t m_maxOutputs;
public:
	CD4017() = delete;
	CD4017(const std::vector<Gpio*> &pins4017, uint8_t maxOutputs);
	void SetDigit(void) override;
	void SetReset(void);
	void SetClock(void);
	virtual ~CD4017();
};

#endif /* CD4017_H_ */
