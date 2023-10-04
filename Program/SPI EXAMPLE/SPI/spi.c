// =========================================================================================
//  Modulo: i2c_driver.C
//  Proposito: Funciones de lectura y grabacion de memorias IIC
//  Fecha: 26-07-2011
//  Autor: Ing. Gustavo Fresno
//=========================================================================================

//#include "OS_Start.h"
#include "spi.h"

// Declaro el semaforo que bloquea que no haya mas de una tarea utilizando la memoria
uint8_t 	Manufacturer_Id;
uint8_t 	Device_Type;
uint8_t 	Device_Id;


//					SPI_Jedec_ID_Leer();

//					SPI_Borrar_Sector(4096);
//					SPI_Guardar(4096, "Hola Gustavo", 13);
//					SPI_Leer(4096, Buffer_SPI, 13);

//					Buffer_SPI[20] = SPI_Leer_Configuracion(COMANDO_READ_STATUS);
//					Buffer_SPI[21] = SPI_Leer_Configuracion(COMANDO_READ_CONF);

//-------------------------------------------
void SPI_MEMORIA_Inicializacion(void)
{
	uint8_t	Temporal[2];
/*
	// Configuro los pines del micro con pullup
	GPIO_Config_Pin(MISO_MEMORIA, 0, PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_OPENDRAIN);
	GPIO_Config_Pin(MOSI_MEMORIA, 0, PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_OPENDRAIN);
	GPIO_Config_Pin(SCK_MEMORIA, 0, PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
	GPIO_Config_Pin(SEL_MEMORIA, 0, PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);

	// Configuro los pines del micro como salida
	GPIO_Dir(MISO_MEMORIA, 0);
	GPIO_Dir(MOSI_MEMORIA, 1);
	GPIO_Dir(SCK_MEMORIA, 1);
	GPIO_Dir(SEL_MEMORIA, 1);

	// Seteo los pines
	GPIO_Value(MISO_MEMORIA, 0);
	GPIO_Value(MOSI_MEMORIA, 0);
	GPIO_Value(SCK_MEMORIA, 0);
	GPIO_Value(SEL_MEMORIA, 1);
*/
	SPI_Reset_SW();

	SPI_HW_Write(COMANDO_DISABLE_QIO);

	SPI_WRITE_Enable();
	SPI_HW_Write(COMANDO_WRITE_UNLOCK_PROTECTION);

	SPI_WRITE_Enable();
	Temporal[0] = SPI_Leer_Configuracion(COMANDO_READ_STATUS);
	Temporal[1] = 0x00;
	SPI_Guardar_Configuracion(COMANDO_WRITE_STATUS, Temporal, 2);
	SPI_Wait_Busy();

	SPI_Jedec_ID_Leer();

	Temporal[0] = SPI_Leer_Configuracion(COMANDO_READ_CONF);

	SPI_WriteBlock_Protection();
}

void SPI_HW_Enable(void)
{
	GPIO_Set(nss, OFF);
	//nss = OFF;
}

void SPI_HW_Disable(void)
{
	GPIO_Set(nss, ON);
	//nss = ON;
}

void SPI_WRITE_Enable(void)
{
	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_ENABLE);

	SPI_HW_Disable();
}

void SPI_WRITE_Disable(void)
{
	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_DISABLE);

	SPI_HW_Disable();
}
//;********************************************************************
//;*   Escritura de un byte.
//;********************************************************************
void SPI_HW_Write(uint8_t Data)
{
	uint8_t 	i;
	
	for( i = 0; i < 8 ; i++ )
	{
		if( Data & ((uint8_t)(0x80) >> i))
			GPIO_Set(mosi, ON);
		else
			GPIO_Set(mosi, OFF);

		GPIO_Set(sck, ON);

		SPI_Demora(SPI_TIEMPO);

		GPIO_Set(sck, OFF);

		SPI_Demora(SPI_TIEMPO);
	}
}

//;********************************************************************
//;*   Lectura de un byte.
//;********************************************************************
uint8_t SPI_HW_Read(void)
{
	uint32_t 	i;
	uint8_t		Data = 0;
	
	for( i = 0; i < 8 ; i++ )
	{
		GPIO_Set(sck, ON);

		SPI_Demora(SPI_TIEMPO);

		if(GPIO_Read(miso))
			Data |= ((uint8_t)(0x80) >> i);

		GPIO_Set(sck, OFF);

		SPI_Demora(SPI_TIEMPO);
	}

	return Data;
}

//--------------------------------------------------------------
// Funcion para tener una demora
//--------------------------------------------------------------
void SPI_Demora(uint32_t demora)
{
	uint32_t	tiempo;

	for(tiempo = 0; tiempo < demora ; tiempo++);
}

void SPI_Borrar_Sector(uint32_t Direccion)
{
	SPI_WRITE_Enable();

	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_ERASE_4K);

	SPI_HW_Write((uint8_t)((Direccion >> 16) & 0xff));
	SPI_HW_Write((uint8_t)((Direccion >> 8) & 0xff));
	SPI_HW_Write((uint8_t)(Direccion & 0xff));

	SPI_HW_Disable();

	SPI_Wait_Busy();
}

void SPI_Borrar_Todo(void)
{
	SPI_WRITE_Enable();

	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_ERASE_TODO);

	SPI_HW_Disable();

	SPI_Wait_Busy();
}

void SPI_Borrar_Bloque(uint32_t Direccion)
{
	SPI_WRITE_Enable();

	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_ERASE_XK);

	SPI_HW_Write((uint8_t)((Direccion >> 16) & 0xff));
	SPI_HW_Write((uint8_t)((Direccion >> 8) & 0xff));
	SPI_HW_Write((uint8_t)(Direccion & 0xff));

	SPI_HW_Disable();

	SPI_Wait_Busy();
}

void SPI_Reset_SW(void)
{
	SPI_HW_Enable();

	SPI_HW_Write(0x66);

	SPI_HW_Disable();

	SPI_Demora(100000);

	SPI_HW_Enable();

	SPI_HW_Write(0x99);

	SPI_HW_Disable();
}

void SPI_Guardar(uint32_t Direccion, uint8_t* Dato, uint32_t Tamanio)
{
	uint32_t 	i;

	for( i = 0 ; i < Tamanio ; i++ )
	{
		SPI_Guardar_Byte(Direccion + i, Dato[i]);
	}
}

void SPI_Guardar_Byte(uint32_t Direccion, uint8_t Dato)
{
	//uint32_t 	i;

#ifdef AVL
	taskENTER_CRITICAL();
#endif

	SPI_WRITE_Enable();

	SPI_Demora(100000);
	SPI_HW_Enable();

	SPI_Demora(100000);
	SPI_HW_Write(COMANDO_WRITE);

	SPI_HW_Write((uint8_t)((Direccion >> 16) & 0xff));
	SPI_HW_Write((uint8_t)((Direccion >> 8) & 0xff));
	SPI_HW_Write((uint8_t)(Direccion & 0xff));

	SPI_HW_Write(Dato);

	SPI_HW_Disable();

	SPI_Wait_Busy();

#ifdef AVL
	taskEXIT_CRITICAL();
#endif
}

void SPI_Leer(uint32_t Direccion, uint8_t* Dato, uint32_t Tamanio)
{
	uint32_t 	i;

#ifdef AVL
	taskENTER_CRITICAL();
#endif

	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_READ);

	SPI_HW_Write((uint8_t)((Direccion >> 16) & 0xff));
	SPI_HW_Write((uint8_t)((Direccion >> 8) & 0xff));
	SPI_HW_Write((uint8_t)(Direccion & 0xff));

	for( i = 0 ; i < Tamanio ; i++ )
	{
		Dato[i] = SPI_HW_Read();
	}

	SPI_HW_Disable();

	SPI_Wait_Busy();

#ifdef AVL
	taskEXIT_CRITICAL();
#endif
}

uint8_t	SPI_Leer_Configuracion(uint8_t Comando)
{
	uint8_t		Temporal;

	SPI_HW_Enable();

	SPI_HW_Write(Comando);

	Temporal = SPI_HW_Read();

	SPI_HW_Disable();

	return Temporal;
}

void SPI_Guardar_Configuracion(uint8_t Comando, uint8_t *Dato, uint32_t Tamanio)
{
	uint8_t		i;

	SPI_HW_Enable();

	SPI_HW_Write(Comando);

	for( i = 0 ; i < Tamanio ; i++ )
	{
		SPI_HW_Write(Dato[i]);
	}

	SPI_HW_Disable();

	return;
}

void SPI_Jedec_ID_Leer(void)
{
	SPI_HW_Enable();

	SPI_HW_Write(0x9F);

	Manufacturer_Id = SPI_HW_Read();       /* receive byte */
  	Device_Type = SPI_HW_Read();           /* receive byte */
  	Device_Id = SPI_HW_Read();             /* receive byte */

	SPI_HW_Disable();
}

void SPI_WriteBlock_Protection(void)
{
	uint32_t 	i;

	SPI_WRITE_Enable();

	SPI_HW_Enable();

	SPI_HW_Write(COMANDO_WRITE_BLOCK_PROTECTION);

	for( i = 0 ; i < 10 ; i++ )
	{
		SPI_HW_Write(0);
	}

	SPI_HW_Disable();

	SPI_Wait_Busy();
}

void SPI_Wait_Busy(void)
{
	while ((SPI_Leer_Configuracion(COMANDO_READ_STATUS)& 0x80) == 0x80);	// waste time until not busy
}

