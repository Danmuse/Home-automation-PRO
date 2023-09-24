/*
 * TIMER_DRIVER.c
 *
 *  Created on: Jul 25, 2019
 *      Author: GJF-Trabajo
 */

#include "dr_systick.h"
#include "core_cm0plus.h"
#include "io.h"
#include "teclado.h"
#include "timers.h"


#define MAX_TICKS 	0xffffff

volatile uint16_t segundos = SEGUNDOS;
extern uint32_t buffer_seno[];
extern uint8_t demora_i2c;

void SysTick_Handler(void)
{
	static uint8_t flag = 0;

/*	if( flag )
			GPIO_Set( PIN_O2, 1 );
		else
			GPIO_Set( PIN_O2, 0 );
	flag++;
	flag %=2;
*/
	//TECLADO_Barrido( );
	AnalizarTimers( );
	//Debounce( );
	segundos--;

	if( demora_i2c )
		demora_i2c --;

	if(!segundos)
	{
		segundos = SEGUNDOS;



	}


}

uint32_t Inicializar_SysTick( uint32_t ticks )
{
	if (ticks > MAX_TICKS)
		return 1 ;          			//* Reload value impossible

	SysTick->LOAD = ticks - 1;   		//* set reload register


	//SysTick->LOAD = (SysTick->CALIB)/10 -1;   		//* set reload register

	SysTick->VAL = 0;                   //* Load the SysTick Counter Value

	SysTick->CTRL = 7;					//* Enable SysTick IRQ and SysTick Timer and system clock clock source

	return 0;
}

