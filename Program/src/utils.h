/*!
 * @file utils.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 * @brief Utils API (Application Programming Interface).
 */

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef UTILS_H_
#define UTILS_H_

#define  	__I    				volatile const  //!< Defines 'read only' permission
#define  	__O    				volatile        //!< Defines 'write only' permission
#define  	__IO   				volatile        //!< Defines 'read / write' permission

typedef		unsigned char 		uint8_t;
typedef		char 				int8_t;
typedef		short unsigned int 	uint16_t;
typedef		short  int 			int16_t;
typedef		unsigned int 		uint32_t;
typedef		int 				int32_t;

#endif /* UTILS_H_ */
