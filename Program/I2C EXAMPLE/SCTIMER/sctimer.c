/*
 * sctimer.c
 *
 *  Created on: 1 sep. 2021
 *      Author: mariano
 */

#include "sctimer.h"
#include "gpio.h"

static uint8_t sampleDAC = 0;
static uint32_t match0DAC = DACMATCH0;

void Inicializar_SCTimer( void )
{
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 8);			// 8= SCTIMER
	SYSCON->PRESETCTRL0 |= (1 << 8);			// 8= SCTIMER clear the timer reset

	SCT0->CONFIG = (1 << 0 ) | (1 << 17 );		// bit0 UNIFY = 1 one timer 32 bits - bits 17 AUTOLIMIT L
	SCT0->CONFIG |= (1 << 0 );					// bit0 UNIFY = 1 one timer 32 bits - bits 17 AUTOLIMIT L
	SCT0->CONFIG &= ~(3 << 1 );					// bits 0 1 CLKMODE 00 -> systemclock mode
	SCT0->SCTMATCH[0] = match0DAC;
	SCT0->SCTMATCHREL[0] = match0DAC;				// Sino interrumpe indefinidamente

	SCT0->EVENT[0].STATE |= 1;					// To perform a basic timer
	SCT0->EVENT[0].CTRL = (1<<12);				// Combmode 1 MATCH
	SCT0->EVEN = (1 << 0);						// Interrupt event 0

	NVIC->ISER[0] = (1 << 9); 					// enable interrupt ISE_SCT_IRQ EVFLAG SCT event
	SCT0->CTRL &= ~(1<<2);						// Clear bit 2 halt to run
}



void SCT_IRQHandler(void)
{
	uint32_t flagEvent = SCT0->EVFLAG;
//	static uint8_t encender = 0;

	if( flagEvent & 1 )
		SCT0->EVFLAG |= ( 1 << 0);							// Reset interrupt event0 flag
/*
	encender ++;
	encender %= 2;
	if( encender )
		GPIO_Set( PIN_VERDE, LED_ON );
	else
		GPIO_Set( PIN_VERDE, LED_OFF );
*/
	sampleDAC = 1;

}

uint8_t DAC_GetFlagSampleReady( void )
{
	return sampleDAC;
}

void DAC_SetFlagSample( uint8_t valor )
{
	sampleDAC = valor;
}

uint8_t DAC_GetMatch0Value( void )
{
	return match0DAC;
}

void DAC_SetMatch0Value( uint32_t valor )
{
	match0DAC = valor;
	ReloadSctimer();
}

void ReloadSctimer( void )
{
	SCT0->CTRL |= (1<<2);						// Set bit 2 halt to halt timer
	Inicializar_SCTimer( );

}
