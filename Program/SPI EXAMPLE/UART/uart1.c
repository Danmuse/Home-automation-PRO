/*

 * UART_Driver.c
 *
 *  Created on: May 17, 2019
 *      Author: GJF-Trabajo
 */

#include "uart1.h"

#include <LPC845.h>
#include "tipos.h"
#include "gpio.h"

UART1_Struct		UART1;

/*****************************************************************************
** Function name:		UART1_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART1_IRQHandler (void)
{
	int32_t Temporal;

	uint32_t	Int = USART1->STAT;

	if(Int & (1 << 0))
	{
		//RX
		Temporal = (int32_t)USART1->RXDAT;

		UART1_PushRx((uint8_t)Temporal);

	}

	if(Int & (1 << 2))
	{
		//TX
		Temporal = UART1_PopTx();
		if(Temporal >= 0)
			USART1->TXDAT = (uint8_t)Temporal;
		else
		{
			USART1->INTENCLR = (1 << 2); //disable int TX
			UART1.TX.Datos_Validos = 0;
		}

	}


}


void Inicializar_UART1(uint32_t baudrate)
{
	IOCON_config_t	Temporal;

	Temporal.open_drain = 0;
	Temporal.mode = PULL_UP;
	Temporal.clk_sel = IOCON_CLK_DIV_0;
	Temporal.iic_mode = 0;
	Temporal.dac_mode = 0;
	Temporal.sample_mode = 0;

	IOCON_config_io(IOCON_INDEX_PIO0_0, &Temporal);
	GPIO_Dir( PIN_ENABLE485, 1 );
	GPIO_Set( PIN_ENABLE485, 0 );							//ENABLE 485

	SYSCON->SYSAHBCLKCTRL0 |= (1 << 15);							// 15 = UART1

	//SWM0->PINASSIGN.PINASSIGN1 = (0x24 << 8) | (23 << 16);			// TX1 = P1.4	RX1 = P0.23
	SWM0->PINASSIGN.PINASSIGN1 = (0x10 << 8) | (0x11 << 16);			// TX1 = P0.16	RX1 = P0.17
	USART1->CFG = 	(0)				// 0=DISABLE 1=ENABLE
					| (1 << 2)		// 0=7BITS 1=8BITS 2=9BITS
					| (0 << 4)		// 0=NOPARITY 2=PAR 3=IMPAR
					| (0 << 6)		// 0=1BITSTOP 1=2BITSTOP
					| (0 << 9)		// 0=NOFLOWCONTROL 1=FLOWCONTROL
					| (0 << 11)		// 0=ASINCRONICA 1=SINCRONICA
					| (0 << 14);	// 0=SLAVE 1=MASTER


	USART1->CTL = 0;

	USART1->INTENSET = (1 << 0);	//RX Y TX INTERRUPT

	UART1CLKSEL = 1;

	USART1->BRG = (FREQ_PRINCIPAL / baudrate) / 8;

	NVIC->ISER[0] = (1 << 4); /* enable interrupt */

	USART1->CFG |= 	(1);			// ENABLE USART1

	return;
}

void UART1_Send(uint8_t *Datos, uint32_t Tamanio)
{
	uint32_t i;

	/*if(0 == Tamanio)
		Tamanio = CADENAS_Strlen(Datos);
	*/
	for(i = 0 ; i < Tamanio ; i++)
		UART1_PushTx(Datos[i]);

	return;
}


void UART1_PushTx(uint8_t dato)
{
	UART1.TX.Buffer[UART1.TX.Indice_in] = dato;

	UART1.TX.Indice_in ++;
	UART1.TX.Indice_in %= UART1_TAMANIO_COLA_TX;

	if ( UART1.TX.Datos_Validos == 0 )
	{
		UART1.TX.Datos_Validos = 1;

		USART1->TXDAT = (uint8_t)UART1_PopTx();

		USART1->INTENSET = (1 << 2); // int tx
	}
}

int32_t UART1_PopTx( void )
{
	int32_t dato = -1;

	if ( UART1.TX.Indice_in != UART1.TX.Indice_out )
	{
		dato = (int32_t) UART1.TX.Buffer[UART1.TX.Indice_out];
		UART1.TX.Indice_out ++;
		UART1.TX.Indice_out %= UART1_TAMANIO_COLA_TX;
	}
	return dato;
}

void UART1_PushRx(uint8_t dato)
{
	UART1.RX.Buffer[UART1.RX.Indice_in] = dato;

	UART1.RX.Indice_in ++;
	UART1.RX.Indice_in %= UART1_TAMANIO_COLA_RX;
}

int32_t UART1_PopRx( void )
{
	int32_t dato = -1;

	if ( UART1.RX.Indice_in != UART1.RX.Indice_out )
	{
		dato = (int32_t) UART1.RX.Buffer[UART1.RX.Indice_out];
		UART1.RX.Indice_out ++;
		UART1.RX.Indice_out %= UART1_TAMANIO_COLA_RX;
	}
	return dato;
}

uint32_t MyPow(uint8_t base, uint8_t exponente)
{
	unsigned long retorno = 1;
	if( exponente == 0 )
		return retorno;

	while(exponente)
	{
		retorno *= base;
		exponente --;
	}

	return retorno;
}


int8_t AnalizarTramaUart1(void)
{
	uint8_t caracter, j, signoMenos = 0;
	static uint8_t i = 0, checksum = 0, estado = 0;
	static char texto[40];
	int8_t grados = -91, aux = 0;


		caracter = LeerBufferRX();

		if( caracter != NO_KEY)
		{
			switch(estado)

			{
			case 0:
				if( caracter == '<' )
				{
					estado = 1;
					texto[i] = caracter;
					checksum += caracter;
					i++;
				}
				else
				{
					i = 0;
					estado = 0;
					texto[0] = '\0';
					checksum = 0;
				}

				break;

			case 1:
				if( caracter == '-' )
					{
					estado = 2;
					texto[i] = caracter;
					checksum += caracter;
					i++;
					}
				else
					{
					if( (caracter - '0') >= 0 && (caracter - '0') <= 9 ) //Vino un numero positivo
						{
						texto[i] = caracter;
						estado = 3;
						checksum += caracter;
						i++;
						}
					else
						{
						i = 0;
						estado = 0;
						texto[0] = '\0';
						checksum = 0;
						}


					}

				break;

			case 2:
				if( (caracter - '0') >= 0 && (caracter - '0') <= 9 )
					{
					texto[i] = caracter;
					estado = 3;
					checksum += caracter;
					i++;
					}
				else
					{
					i = 0;
					estado = 0;
					checksum = 0;
					texto[0] = '\0';
					}

					break;
			case 3:	//Segundo dígito
				if( (caracter - '0') >= 0 && (caracter - '0') <= 9 )
					{
					texto[i] = caracter;
					checksum += caracter;
					estado = 4;
					i++;
					}
				else		//Es el checksum de un digito positivo?
					{
						texto[i] = caracter;
						if( texto[i] == checksum )	//Checksum OK
						{
						estado = 5;
						i++;
						}
						else
						{
						i = 0;
						estado = 0;
						checksum = 0;
						texto[0] = '\0';
						}
					}

					break;
			case 4:		//Chequear cheksum con segundo digito
				if( caracter == checksum )	//Checksum OK
				{
				texto[i] = caracter;
				estado = 5;
				i++;
				}
				else
				{
				i = 0;
				estado = 0;
				checksum = 0;
				texto[0] = '\0';
				}

				break;
			case 5:		//Esperar fin de trama
				if( caracter == '>' )	//Fin de trama OK OK
				{
				texto[i] = caracter;
				estado = 6;
				//i++;
				}
				else
				{
				i = 0;
				estado = 0;
				checksum = 0;
				texto[0] = '\0';
				}

					break;
			case 6:		//Parseo de valores
				for( j = 1; j <= i; j++)		//Descarto el <
				{
					if( texto[j] == '-')
						signoMenos = 1;
					else
					{
						if(signoMenos)
							{
							if( i > 4)		//Dos digitos negativos
								aux += (texto[j] - '0')*MyPow(10,(3-j));
							else			//Un digito negativo
								{
								aux += (texto[j]-'0')*MyPow(10,(2-j));
								break;
								}
							}

						else
							{
							if( i > 3)		//Dos digitos positivos
								aux += (texto[j]-'0')*MyPow(10,2-j);
							else			//Un digito positivo
								{
								aux += (texto[j]-'0')*MyPow(10,1-j);
								break;
								}

							}
					}
				}
				if( signoMenos )
					aux *= -1;

				grados = aux;		//Resultado final ok
				//sprintf(texto, "<<Grados: %d >>", aux);
				//Display_lcd( texto , 1 , 0 ) ;
				UART1_Send((uint8_t *)texto, strlen(texto) -1 );
				//Dejar variables preparadas para otra trama
				i = 0;
				estado = 0;
				checksum = 0;
				texto[0] = '\0';

					break;
			default:
				i = 0;
					estado = NO_KEY;
					checksum = 0;
					texto[0] = '\0';
					break;
			}
		}


			return grados;
}


uint8_t LeerBufferRX( void )
{
	uint8_t caracter;

	caracter = UART1_PopRx();

		if ( caracter == NO_KEY )
			return NO_KEY;

	return caracter;
}
