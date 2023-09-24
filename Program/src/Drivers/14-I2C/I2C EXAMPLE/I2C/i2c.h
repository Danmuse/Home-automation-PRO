// =========================================================================================
//  Modulo: i2c_driver.C
//  Proposito: Funciones de lectura y grabacion de memorias IIC
//  Fecha: 26-07-2011
//  Autor: Ing. Gustavo Fresno
//=========================================================================================

#ifndef I2c_Driver_
#define I2c_Driver_

#include "tipos.h"
#include "gpio.h"
#define NO_ACK  					0       /* lack of acknowledgment          */

#define	TIEMPO_IIC					30	// Tiempos entre grabacion
#define	TIEMPO_IIC_1				20		// Tiempo para la lectura de un cambio de estado en el puerto

#define	CANTIDAD_REINTENTOS_I2C		5
#define EEPROM_24LC256_address		0xA0

/* Pinout LCP845-BRK

		P0.16	o		o	VDD 	3.3V
		P0.17	o		o	P0.00
		P0.18	o		o	P0.01
		P0.19	o		o	P0.02
		P0.20	o		o	P0.02
		P0.21	o		o	P0.04
		P0.22	o		o	P0.05
		P0.23	o		o	P0.06
		P0.05	o		o	P0.07
		P0.24	o		o	P1.0
		P0.25	o		o	P1.1
		P0.26	o		o	P1.2
		P0.27	o		o	P0.15
		P0.28	o		o	P0.14
		P0.29	o		o	P0.13
		P0.30	o		o	P0.12
		P0.31	o		o	P0.11	SDA
		P0.09	o		o	P0.10	SCL
		P0.08	o		o	P0.09
		GND		o		o	P0.08

 */

#define	SDA							0,11
#define	SCL							0,10

#define	TAMANIO_MEMORIA				(1024 * 8)


void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Read_Clock(void);
void IIC_Clock(void);
uint8_t IIC_Write(uint8_t );
uint8_t IIC_Read(uint8_t );
uint8_t IIC_Escribir(uint32_t , uint8_t , uint8_t );
uint8_t IIC_Leer(uint32_t , uint8_t );
void IIC_Demora(uint32_t demora);
void Borrar_Memoria(uint8_t);
void IIC_Inicializacion(void);
uint8_t GetDemora_IIC( void );
void SetDemora_IIC( uint8_t value );



uint8_t IIC_Write_Driver(uint32_t Direccion, uint8_t Memoria, uint8_t byte);
uint8_t IIC_Read_Driver(uint32_t Direccion, uint8_t Memoria);

void IIC_Write_Driver_32b(uint32_t Direccion, uint8_t Memoria, uint32_t Dato);
uint32_t IIC_Read_Driver_32b(uint32_t Direccion, uint8_t Memoria);

uint16_t IIC_Read_Driver_16b(uint32_t Direccion, uint8_t Memoria);
void IIC_Write_Driver_16b(uint32_t Direccion, uint8_t Memoria, uint16_t Dato);

void IIC_Leer_Bloque(uint32_t Direccion, uint8_t* Datos, uint32_t Tamanio);
void IIC_Guardar_Bloque(uint32_t Direccion, uint8_t* Datos, uint32_t Tamanio);

#endif /* PROYECTO_LPC1768_H_ */
