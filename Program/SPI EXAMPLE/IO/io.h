/*
 * dr_io.h
 *
 *  Created on: 26 abr. 2021
 *      Author: mariano
 */

#ifndef DR_IO_H_
#define DR_IO_H_
#include <LPC845.h>

#include "gpio.h"
#include "rc522.h"

#define	ENTRADAS			4
#define ACEPTAReSTADO		3
#define PIN_MICROSWITCH		0,4	// Port for us
#define PIN_SODO			0,26	// Port for so-do
#define PIN_O0				1,0	// Port for O0
#define PIN_O1				1,1	// Port for O1
#define PIN_O2				1,2	// Port for O2
#define PIN_O3				0,1	// Port for O3



#define	TRIGGER				PIN_O3

void Inicializar_IO(void);
uint8_t Entradas( void );
void Debounce(void);
#endif /* DR_IO_H_ */
