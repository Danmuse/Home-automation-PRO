/*
 * UART1_Driver.h
 *
 *  Created on: Jul 26, 2019
 *      Author: GJF-Trabajo
 */

#ifndef DRIVER_UART_UART1_DRIVER_H_
#define DRIVER_UART_UART1_DRIVER_H_

#include "tipos.h"
#include "teclado.h"
#include <string.h>

#define		UART0_TAMANIO_COLA_RX					20
#define		UART0_TAMANIO_COLA_TX					20

#define		PRIMERoCTETO							0
#define		SEGUNDOoCTETO							1
#define		TERCERoCTETO							2
#define		CUARTOoCTETO							3

#define		LIMITEcARCTERESpRIMERoCTETO				4
#define		LIMITEcARCTERESsEGUNDOoCTETO			8
#define		LIMITEcARCTEREStERCERoCTETO				12
#define		LIMITEcARCTEREScuARTOoCTETO				16




typedef struct
{
	uint8_t						Datos_Validos;
	uint16_t					Tamanio;
	uint8_t						Buffer[UART0_TAMANIO_COLA_RX];
	uint16_t					Indice_in, Indice_out;
}STRUCT_RX0;

typedef struct
{
	uint8_t						Datos_Validos;
	uint16_t					Tamanio;
	uint8_t						Buffer[UART0_TAMANIO_COLA_TX];
	uint16_t					Indice_in, Indice_out;
}STRUCT_TX0;

typedef struct
{
	STRUCT_RX0					RX;
	STRUCT_TX0					TX;
}UART0_Struct;

void Inicializar_UART0(uint32_t);
void UART0_Send(uint8_t *, uint32_t );
void UART0_PushTx(uint8_t );
int32_t UART0_PopTx( void );
void UART0_PushRx(uint8_t Dato);
int32_t UART0_PopRx( void );
void RestartIndexesTxUART0( void );
void RestartIndexesRxUART0( void );

uint8_t LeerBufferRXUART0( void );
int8_t AnalizarTramaUart0(int8_t);
uint8_t EsNumero( uint8_t );
void ActualizarIpServidor( void );
void DefaultIpServidor( void );
void restartFlagsOctetos(void);
uint8_t CheckFlagsOctetos( void );


#endif /* DRIVER_UART_UART1_DRIVER_H_ */
