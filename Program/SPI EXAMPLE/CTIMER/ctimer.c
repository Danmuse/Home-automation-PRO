/*
 * ctimer.c
 *
 *  Created on: 7 may. 2021
 *      Author: mariano
 */
#include "ctimer.h"

static uint8_t flagProcesarPulso = 0;
static uint32_t pulsos[2];

void CTIMER0_IRQHandler(void)
{
	static uint8_t  i=0;
	uint32_t valor;



	if( CTIMER0->IR & 0x01 )						//Interrupcion por MR0
	{
		CTIMER0->IR |= (1 << 0);					// Reset flag MROint
		CTIMER0->MCR &= ~(1 << 0);					// Disable Interrupt on MATCH0
		CTIMER0->MR[1] = MATCH1;					// MR1
		CTIMER0->MCR |= (1 << 3);					//Interrupt on MATCH1
		//CTIMER0->MCR |= (1 << 4);					//Reset timer on interrupt MATCH1
		GPIO_Set( TRIGGER, 1 );

	}

	if( CTIMER0->IR & 0x02 )						//Interrupcion por MR1
	{
		CTIMER0->IR |= (1 << 1);					// Reset flag MR1int

		CTIMER0->TCR = 0;							// Disable ctimer
		CTIMER0->MCR &= ~(1 << 0);					// Disable Interrupt on MATCH0
		CTIMER0->MCR &= ~(1 << 3);					// Disable Interrupt on MATCH1
		//CTIMER0->MR[0] = MATCH0;					// MR0
		//CTIMER0->MCR |= (1 << 0);					// Interrupt on MATCH0
		//CTIMER0->MCR |= (1 << 1);					// Reset timer on interrupt MATCH0
		GPIO_Set( TRIGGER, 0 );


		//CTIMER0->CCR |= (1 << CAP0RE) | (1 << CAP0FE);	//Habilitar captura por falling edge y rising edge
		CTIMER0->CCR |= 1 << CAP0RE;				//Habilitar captura por falling edge
		//CTIMER0->CCR |= 1 << CAP0FE;				//Habilitar captura por falling edge
		valor = (~(0x000000E2) << 8);
		valor |= 0xFF;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);			// Enable 7 = SWM
		SWM0->PINASSIGN_DATA[14] &= valor;			// T0_CAP0 Input Timer Capture channel 0 PINASSIGN14 P029 0x1D es la captura
		SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);		// Disable 7 = SWM
		CTIMER0->CCR |= (1 << CAP0I);				// Interrupt on CAPT0
		CTIMER0->TC = 0;							// Reset la cuenta
		CTIMER0->TCR = 1;							// Enable ctimer

	}

	if( CTIMER0->IR & 0x10 )						//Interrupcion por CR0INT captura
	{
		CTIMER0->IR |= (1 << 4);					// Reset flag CR0INT
		pulsos[i] = CTIMER0->CR[0];
		i ++;


		if( i ==  1 )
		{
			CTIMER0->CCR &= ~(1 << CAP0RE);					//Deshabilitar captura por rising edge
			CTIMER0->CCR |= 1 << CAP0FE;					//Habilitar captura por falling edge
		}

		if( i == 2 )
		{
			i = 0;
			CTIMER0->TCR = 0;
			CTIMER0->CCR &= ~(1 << CAP0FE);					//Deshabilitar captura por falling edge
			CTIMER0->CCR &= ~(1 << CAP0RE);					//Deshabilitar captura por rising edge

			valor |= 0xFFFFFFFF;
			SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);			// Enable 7 = SWM
			SWM0->PINASSIGN_DATA[14] |= valor;			// Reset T0_CAP0 Input Timer Capture channel 0 PINASSIGN14 P029 0x1D es la captura
			SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);		// Disable 7 = SWM
			CTIMER0->CCR &= ~(1 << CAP0I);				// No Interrupt on CAPT0
			CTIMER0->TCR = 1;							// Enable ctimer
			flagProcesarPulso = 1;

		}
		//CTIMER0->TC = 0;							//Reset la cuenta

	}


}

void Inicializar_CTimer0( void )
{
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 25);		// 25= CTIMER0
	SYSCON->PRESETCTRL0 |= (1 << 25);			// 25= CTIMER0 clear the timer reset
	CTIMER0->PR = 1;							// Prescaler --> clock dividido 2
	NVIC->ISER[0] = (1 << 23); 					// enable interrupt ISE_CT32B0_IRQ

}

void ConfigMatch( void )
{
	CTIMER0->MR[0] = MATCH0;					// MR0
	CTIMER0->MCR |= (1 << 0);					//Interrupt on MATCH0
	CTIMER0->MCR |= (1 << 1);					//Reset timer on interrupt MATCH0
	CTIMER0->TCR = (1 << 1);					// Reset timer
	CTIMER0->IR = 1;
	CTIMER0->TCR = 1;

}

uint8_t GetFlagProcesarPulso( void )
{
	return flagProcesarPulso;
}

void SetFlagProcesarPulso( uint8_t valor )
{
	flagProcesarPulso = valor;
}

uint32_t GetTiempoPulso( void )
{

	if( pulsos[1] > pulsos[0])
		return( pulsos[1] - pulsos[0] );
	else
		return( ~pulsos[1] + pulsos[0] );
}
