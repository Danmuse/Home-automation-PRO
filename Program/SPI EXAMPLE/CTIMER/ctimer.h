/*
 * ctimer.h
 *
 *  Created on: 7 may. 2021
 *      Author: mariano
 */

#ifndef CTIMER_H_
#define CTIMER_H_

#include <misc.h>
#include "gpio.h"
#include "io.h"
#include "tipos.h"


//#define MATCH0		24		//IRQ=7,5us
//#define MATCH0		852	//IRQ=35,511us -> 64 samples 2,7272ms - 440Hz
#define MATCH0		480		//20us
#define MATCH1		270	//~10us

#define	ON			1
#define	OFF			0

#define	CAP0RE	0
#define	CAP0FE	1
#define	CAP0I	2


void Inicializar_CTimer0( void );
void ConfigMatch( void );
uint8_t GetFlagProcesarPulso( void );
void SetFlagProcesarPulso( uint8_t );
uint32_t GetTiempoPulso( void );

#endif /* CTIMER_H_ */
