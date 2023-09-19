/*!
 * @file Segments.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief Segments Abstract Class API (Application Programming Interface).
 */

#ifndef SEGMENTS_H_
#define SEGMENTS_H_

#include "utils.h"

class Segments {
public:
	virtual void SetSegments(uint16_t value) = 0;
	virtual ~Segments() = default;
};

#endif /* SEGMENTS_H_ */
