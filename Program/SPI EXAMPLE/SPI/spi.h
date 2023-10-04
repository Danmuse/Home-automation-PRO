// =========================================================================================
//  Modulo: i2c_driver.C
//  Proposito: Funciones de lectura y grabacion de memorias IIC
//  Fecha: 26-07-2011
//  Autor: Ing. Gustavo Fresno
//=========================================================================================

#ifndef SPI_H_
#define SPI_H_
#include "gpio.h"
#include "rc522.h"

#define			ON			1
#define			OFF			0


#define NO_ACK  					0       /* lack of acknowledgment          */

#define	MISO_MEMORIA				1,0
#define	MOSI_MEMORIA				1,1
#define SCK_MEMORIA					1,4
#define	SEL_MEMORIA					1,9

//#define	SPI_TIEMPO					400
#define	SPI_TIEMPO					100

#define	COMANDO_READ					0x03
#define	COMANDO_WRITE_STATUS			0x01
#define	COMANDO_READ_STATUS				0x05
#define	COMANDO_READ_CONF				0x35
#define	COMANDO_WRITE					0x02
#define	COMANDO_WRITE_BLOCK_PROTECTION	0x42
#define	COMANDO_WRITE_UNLOCK_PROTECTION	0x98
#define	COMANDO_DISABLE_QIO				0xFF
#define	COMANDO_ENABLE					0x06
#define	COMANDO_DISABLE					0x04
#define	COMANDO_RESET					0x99
#define	COMANDO_ID						0x9F
#define	COMANDO_ERASE_4K				0x20
#define	COMANDO_ERASE_XK				0xD8
#define	COMANDO_ERASE_TODO				0xC7

void SPI_MEMORIA_Inicializacion(void);
void SPI_HW_Write(uint8_t Data);
uint8_t SPI_HW_Read(void);
void SPI_Demora(uint32_t demora);
void SPI_Reset_SW(void);

void SPI_HW_Enable(void);
void SPI_HW_Disable(void);
void SPI_WRITE_Enable(void);
void SPI_WRITE_Disable(void);

void SPI_Guardar(uint32_t Direccion, uint8_t* Dato, uint32_t Tamanio);
void SPI_Guardar_Byte(uint32_t Direccion, uint8_t Dato);
void SPI_Leer(uint32_t Direccion, uint8_t* Dato, uint32_t Tamanio);
void SPI_Borrar_Sector(uint32_t Direccion);
void SPI_Borrar_Bloque(uint32_t Direccion);
void SPI_Borrar_Todo(void);

uint8_t	SPI_Leer_Configuracion(uint8_t Comando);
void SPI_Guardar_Configuracion(uint8_t Comando, uint8_t *Dato, uint32_t Tamanio);
void SPI_Jedec_ID_Leer(void);
void SPI_WriteBlock_Protection(void);
void SPI_Wait_Busy(void);









#endif /* PROYECTO_LPC1768_H_ */
