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

#define		UART1_TAMANIO_COLA_RX					20
#define		UART1_TAMANIO_COLA_TX					20
#define		PIN_ENABLE485							0,0


typedef struct
{
	uint8_t						Datos_Validos;
	uint16_t					Tamanio;
	uint8_t						Buffer[UART1_TAMANIO_COLA_RX];
	uint16_t					Indice_in, Indice_out;
}STRUCT_RX1;

typedef struct
{
	uint8_t						Datos_Validos;
	uint16_t					Tamanio;
	uint8_t						Buffer[UART1_TAMANIO_COLA_TX];
	uint16_t					Indice_in, Indice_out;
}STRUCT_TX1;

typedef struct
{
	STRUCT_RX1					RX;
	STRUCT_TX1					TX;
}UART1_Struct;

void Inicializar_UART1(uint32_t baudrate);
void UART1_Send(uint8_t *Datos, uint32_t Tamanio);
void UART1_PushTx(uint8_t Dato);
int32_t UART1_PopTx( void );
void UART1_PushRx(uint8_t Dato);
int32_t UART1_PopRx( void );
uint32_t MyPow( uint8_t, uint8_t);
uint8_t LeerBufferRX( void );
int8_t AnalizarTramaUart1(void);


#endif /* DRIVER_UART_UART1_DRIVER_H_ */
