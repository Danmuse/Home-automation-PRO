/*!
 * @file Outputs.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 * @brief Outputs API (Application Programming Interface).
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

#include "utils.h"

class Outputs {
public:
	virtual void SetPin(void) = 0;
	virtual void ClearPin(void) = 0;
	virtual void TogglePin(void) = 0;
	virtual void SetDirOutputs(void) = 0;
	virtual void SetPinResistor(void) = 0;
	virtual ~Outputs() = default;
};

#endif /* OUTPUTS_H_ */
