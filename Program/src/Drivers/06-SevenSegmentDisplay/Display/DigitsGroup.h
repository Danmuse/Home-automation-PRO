/*!
 * @file DigitsGroup.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/07/2023 12:45:36
 * @brief Digits Group API (Application Programming Interface).
 */

#ifndef DIGITS_GROUP_H_
#define DIGITS_GROUP_H_

#include "utils.h"
#include "Digit.h"

struct DigitsGroup {
	const uint8_t m_begin;
	const uint8_t m_quantity;
public:
	DigitsGroup() = delete;
	DigitsGroup(uint8_t	begin, uint8_t quantity) : m_begin(begin), m_quantity(quantity) { }
	virtual ~DigitsGroup() { }
};

#endif /* DIGITS_GROUP_H_ */
