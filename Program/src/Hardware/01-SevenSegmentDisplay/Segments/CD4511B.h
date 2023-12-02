/*!
 * @file CD4511B.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief CD4511B Class API (Application Programming Interface).
 */

#ifndef CD4511B_H_
#define CD4511B_H_

#include <vector>
#include "Segments.h"
#include "GPIO.h"

#define BCDA_INDEX 0
#define BCDB_INDEX 1
#define BCDC_INDEX 2
#define BCDD_INDEX 3

class CD4511B : public Segments {
private:
	const std::vector<Gpio*> &m_BCD;
public:
	CD4511B() = delete;
	CD4511B(const std::vector<Gpio*> &BCD);
	void setSegments(uint16_t value) override;
	virtual ~CD4511B();
};

#endif /* CD4511B_H_ */
