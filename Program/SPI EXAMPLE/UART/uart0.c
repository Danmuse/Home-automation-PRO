/*

 * UART_Driver.c
 *
 *  Created on: May 17, 2019
 *      Author: GJF-Trabajo
 */

#include <LPC845.h>
#include <uart0.h>
#include "tipos.h"
#include "gpio.h"
#include "timers.h"

UART0_Struct		UART0;
uint8_t tramaRecibida[16];
uint8_t ipServer[16];
uint8_t flagPrimerOctetoOK = 0;
uint8_t flagSegundoOctetoOK = 0;
uint8_t flagTercerOctetoOK = 0;

/*****************************************************************************
** Function name:		UART0_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART0_IRQHandler (void)
{
	int32_t Temporal;

	uint32_t	Int = USART0->STAT;

	if(Int & (1 << 0))
	{
		//RX
		Temporal = (int32_t)USART0->RXDAT;

		UART0_PushRx((uint8_t)Temporal);

	}

	if(Int & (1 << 2))
	{
		//TX
		Temporal = UART0_PopTx();
		if(Temporal >= 0)
			USART0->TXDAT = (uint8_t)Temporal;
		else
		{
			USART0->INTENCLR = (1 << 2); //disable int TX
			UART0.TX.Datos_Validos = 0;
		}

	}


}


void Inicializar_UART0(uint32_t baudrate)
{
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 14);							// 14 = UART0

	SWM0->PINASSIGN.PINASSIGN0 = (0x19 << 0) | (0x18 << 8);			// TXD -> RX0 = P0.25 -- RXD -> TX0 = P0.24
	USART0->CFG = 	(0)				// 0=DISABLE 1=ENABLE
					| (1 << 2)		// 0=7BITS 1=8BITS 2=9BITS
					| (0 << 4)		// 0=NOPARITY 2=PAR 3=IMPAR
					| (0 << 6)		// 0=1BITSTOP 1=2BITSTOP
					| (0 << 9)		// 0=NOFLOWCONTROL 1=FLOWCONTROL
					| (0 << 11)		// 0=ASINCRONICA 1=SINCRONICA
					| (0 << 14);	// 0=SLAVE 1=MASTER


	USART0->CTL = 0;

	//USART0->INTENSET = (1 << 0);	//RX Y TX INTERRUPT

	UART0CLKSEL = 1;

	USART0->BRG = (FREQ_PRINCIPAL / baudrate) / 8;

	//NVIC->ISER[0] = (1 << 3); 		/* enable interrupt UART0_IRQ */

	USART0->CFG |= 	(1);			// ENABLE USART0

	return;
}

void UART0_Send(uint8_t *Datos, uint32_t Tamanio)
{
	uint32_t i;

	if(0 == Tamanio)
		Tamanio = strlen((char *)Datos);

	for(i = 0 ; i < Tamanio ; i++)
		UART0_PushTx(Datos[i]);

	return;
}


void UART0_PushTx(uint8_t dato)
{
	UART0.TX.Buffer[UART0.TX.Indice_in] = dato;

	UART0.TX.Indice_in ++;
	UART0.TX.Indice_in %= UART0_TAMANIO_COLA_TX;

	if ( UART0.TX.Datos_Validos == 0 )
	{
		UART0.TX.Datos_Validos = 1;

		USART0->TXDAT = (uint8_t)UART0_PopTx();

		USART0->INTENSET = (1 << 2); // int tx
	}
}

int32_t UART0_PopTx( void )
{
	int32_t dato = -1;

	if ( UART0.TX.Indice_in != UART0.TX.Indice_out )
	{
		dato = (int32_t) UART0.TX.Buffer[UART0.TX.Indice_out];
		UART0.TX.Indice_out ++;
		UART0.TX.Indice_out %= UART0_TAMANIO_COLA_TX;
	}
	return dato;
}

void UART0_PushRx(uint8_t dato)
{
	UART0.RX.Buffer[UART0.RX.Indice_in] = dato;

	UART0.RX.Indice_in ++;
	UART0.RX.Indice_in %= UART0_TAMANIO_COLA_RX;
}

int32_t UART0_PopRx( void )
{
	int32_t dato = -1;

	if ( UART0.RX.Indice_in != UART0.RX.Indice_out )
	{
		dato = (int32_t) UART0.RX.Buffer[UART0.RX.Indice_out];
		UART0.RX.Indice_out ++;
		UART0.RX.Indice_out %= UART0_TAMANIO_COLA_RX;
	}
	return dato;
}



int8_t AnalizarTramaUart0( int8_t caracter )
{
		return 0;
}

uint8_t EsNumero( uint8_t caracter )
{
	caracter -= '0';
	if( (caracter >= 0 ) && (caracter <= 9) )
		return 1;
	else
		return 0;
}

uint8_t LeerBufferRXUART0( void )
{
	uint8_t caracter;

	caracter = UART0_PopRx();

	return caracter;
}

void RestartIndexesTxUART0( void )
{
	UART0.TX.Indice_in = 0;
	UART0.TX.Indice_out = 0;
}
void RestartIndexesRxUART0(void )
{
	UART0.RX.Indice_in = 0;
	UART0.RX.Indice_out = 0;
}



