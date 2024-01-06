/*/*!
 * @file S5050DJ.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 06/01/2024 12:11:36
 */

#include "S5050DJ.h"

S5050DJ::S5050DJ(const Gpio &output) : Gpio(output), CTimer() {
	if (this->m_direction != Gpio::OUTPUT) this->toggleDir();

}

S5050DJ::~S5050DJ() {

}

