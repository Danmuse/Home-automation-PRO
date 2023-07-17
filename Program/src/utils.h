/*!
 * @file utils.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 14/06/2023 20:36:24
 * @brief Utils API (Application Programming Interface).
 */

#ifndef UTILS_H_
#define UTILS_H_

#define __I    				volatile const  //!< Defines 'read only' permission
#define __O    				volatile        //!< Defines 'write only' permission
#define __IO   				volatile        //!< Defines 'read / write' permission

typedef	unsigned char 		uint8_t;
typedef	char 				int8_t;
typedef	short unsigned int 	uint16_t;
typedef	short  int 			int16_t;
typedef	unsigned int 		uint32_t;
typedef	int 				int32_t;

#define	INT32_MAX			(2147483647) 	//!< Valor máximo del INT32
#define	INT32_MIN			(-2147483648) 	//!< Valor mínimo del INT32
#define	UINT32_MAX			(4294967295) 	//!< Valor máximo del UINT32

#define	INT8_MAX			(127)			//!< Valor máximo del INT8
#define	INT8_MIN			(-128)			//!< Valor mínimo del INT8
#define	UINT8_MAX			(255)			//!< Valor máximo del UINT8

#define BYTE_SIZE			32

#endif /* UTILS_H_ */
