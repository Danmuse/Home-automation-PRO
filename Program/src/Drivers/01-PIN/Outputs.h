/*!
 * @file Outputs.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 * @brief Outputs Abstract Class API (Application Programming Interface).
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

#include "utils.h"

class Outputs {
public:
	virtual void setPin(void) = 0;
	virtual void clearPin(void) = 0;
	virtual void togglePin(void) = 0;
	virtual void setDirOutputs(void) = 0;
	virtual void setPinResistor(void) = 0;
	virtual ~Outputs() = default;
};

#endif /* OUTPUTS_H_ */
