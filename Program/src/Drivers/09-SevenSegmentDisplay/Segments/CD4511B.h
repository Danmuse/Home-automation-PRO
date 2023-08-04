/*!
 * @file CD4511B.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief CD4511B Class API (Application Programming Interface).
 */

#ifndef CD4511B_H_
#define CD4511B_H_

#include <list>
#include "Segments.h"
#include "GPIO.h"

#define MAX_BCD_PINS 4 // pinBCDA - pinBCDB - pinBCDC - pinBCDD

class CD4511B : public Segments {
private:
	const std::list<Gpio*> &m_BCD;
public:
	CD4511B() = delete;
	CD4511B(const std::list<Gpio*> &BCD);
	void SetSegments(uint16_t value) override;
	void InitSegments(void) override;
	virtual ~CD4511B();
};

#endif /* CD4511B_H_ */
