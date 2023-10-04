/*
 * sctimer.h
 *
 *  Created on: 1 sep. 2021
 *      Author: mariano
 */

#ifndef SCTIMER_H_
#define SCTIMER_H_

#include <LPC845.h>

//#define DACMATCH0				48000		//1ms -> clk system 48MHz
#define DACMATCH0				6000		//125us --> Fs = 8KHz
#define DACMATCH0mONSTER		8000		// Fs = 15,384KHz
#define DACMATCH0rATT			4000		// Fs =

void Inicializar_SCTimer( void );
uint8_t DAC_GetFlagSampleReady( void );
void DAC_SetFlagSample( uint8_t );
uint8_t DAC_GetMatch0Value( void );
void DAC_SetMatch0Value( uint32_t  );
void ReloadSctimer( void );

#endif /* SCTIMER_H_ */
