/*!
 * @file Sweep.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief Sweep Abstract Class API (Application Programming Interface).
 */

#ifndef SWEEP_H_
#define SWEEP_H_

class Sweep {
public:
	virtual void SetDigit(void) = 0;
	virtual void InitSweep(void) = 0;
	virtual ~Sweep() = default;
};

#endif /* SWEEP_H_ */
