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
typedef	short int 			int16_t;
typedef	unsigned int 		uint32_t;
typedef	int 				int32_t;

#define	INT32_MAX			(2147483647) 	//!< Maximum value of the INT32 variable type.
#define	INT32_MIN			(-2147483648) 	//!< Minimum value of the INT32 variable type.
#define	UINT32_MAX			(4294967295) 	//!< Maximum value of the UINT32 variable type.

#define	INT16_MAX			(32767) 		//!< Maximum value of the INT16 variable type.
#define	INT16_MIN			(-32768) 		//!< Minimum value of the INT16 variable type.
#define	UINT16_MAX			(65535) 		//!< Maximum value of the UINT16 variable type.

#define	INT8_MAX			(127)			//!< Maximum value of the INT8 variable type.
#define	INT8_MIN			(-128)			//!< Minimum value of the INT8 variable type.
#define	UINT8_MAX			(255)			//!< Maximum value of the UINT8 variable type.

#define BYTE_SIZE			8
#define REG_BYTES_SIZE		32 // BYTE_SIZE * 4

#endif /* UTILS_H_ */
