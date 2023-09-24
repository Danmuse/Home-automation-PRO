// =========================================================================================
//  Modulo: i2c_driver.C
//  Proposito: Funciones de lectura y grabacion de memorias IIC
//  Fecha: 26-07-2011
//  Autor: Ing. Gustavo Fresno
//=========================================================================================

#include "i2c.h"
uint8_t demora_i2c;

//-------------------------------------------
void IIC_Inicializacion(void)
{
//	PINSEL_CFG_Type PinCfg;

//	// Configuro los pines del micro con pullup
//	GPIO_Config_Pin(SDA, 0, PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
//	GPIO_Config_Pin(SCL, 0, PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
//
//	// Configuro los pines del micro como salida
//	GPIO_Dir(SDA, 1);
//	GPIO_Dir(SCL, 1);
//
//	// Seteo los pines
//	GPIOSetValue(SDA, 1);
//	GPIOSetValue(SCL, 1);

	// Configuro los pines del micro como salida
	//GPIO_Dir( SDA, 1 );
	//LPC_IOCON->R_PIO1_1 = 1 | (1<<7);
	//GPIO_Dir( SCL, 1 );
	//LPC_IOCON->R_PIO1_0 = 1 | (1<<7);

	// Configuro los pines del micro como salida
	GPIO_Dir(SDA, 1);
	GPIO_Dir(SCL, 1);
	// Seteo los pines
	GPIO_Set( SDA, 1 );
	GPIO_Set( SCL, 1 );

	// Envio señales de Start
	IIC_Demora(TIEMPO_IIC);

	GPIO_Set(SDA, 0);

	IIC_Demora(TIEMPO_IIC);

	GPIO_Set(SCL, 0);
}

void IIC_Start(void)
{
	// Configuro los pines del micro como salida
//	GPIO_Dir(SDA, 1);
//	GPIO_Dir(SCL, 1);

	// Seteo los pines
	GPIO_Set(SDA, 1);
	GPIO_Set(SCL, 1);

	// Envio señales de Start
	IIC_Demora(TIEMPO_IIC);

	GPIO_Set(SDA, 0);

	IIC_Demora(TIEMPO_IIC);

	GPIO_Set(SCL, 0);

	IIC_Demora(TIEMPO_IIC);
}
//--------------------------------------------------------------
void IIC_Stop(void)
{
//	uint32_t	i;

	GPIO_Set(SDA, 0);
	GPIO_Set(SCL, 1);

	IIC_Demora(TIEMPO_IIC);

	GPIO_Set(SDA, 1);

	IIC_Demora(TIEMPO_IIC);
}

//--------------------------------------------------------------
// Funcion donde se envia el clock y se realiza la lectura del pin SDA
//--------------------------------------------------------------
uint8_t IIC_Read_Clock(void)
{
	uint8_t Level;         	/* linea de SDA */
	
	IIC_Demora(TIEMPO_IIC_1);

	// Seteo el pin CLK
	GPIO_Set(SCL, 1);

	// Se configura como entrada, para realizar la lectura
	GPIO_Dir(SDA, 0);

	IIC_Demora(TIEMPO_IIC_1);



	// Realizo la lectura del SDA
	Level = GPIO_Read(SDA);

	//Demora_FOR(TIEMPO_IIC_1);
	//GPIOSetValue(SCL, 1);

	// Se configura SDA como salida nuevamente
	GPIO_Dir(SDA, 1);
	GPIO_Set(SDA, 1);


	GPIO_Set(SCL, 0);

	IIC_Demora(TIEMPO_IIC_1);

	return(Level);
}

//--------------------------------------------------------------
// Funcion donde se envia el clock
//--------------------------------------------------------------
void IIC_Clock(void)
{
	IIC_Demora(TIEMPO_IIC_1);

	GPIO_Set(SCL, 1);

	IIC_Demora(TIEMPO_IIC_1);

	GPIO_Set(SCL, 0);


}

//--------------------------------------------------------------
// Funcion donde se envia un byte a la memoria
//--------------------------------------------------------------
uint8_t IIC_Write(uint8_t byte)
{
	uint8_t 	mask = 0x80;
	
	while(mask)
	{
		if (byte & mask)
			GPIO_Set(SDA, 1);
		else
			GPIO_Set(SDA, 0);

		IIC_Clock();
		mask >>= 1;
	}
	//GPIO_Set(SDA, 1);
	return(IIC_Read_Clock());
}
//--------------------------------------------------------------
// Funcion donde se lee un byte de la memoria
//--------------------------------------------------------------
uint8_t IIC_Read(uint8_t acknowledgment)
{
	uint8_t mask = 0x80,byte = 0x00;
	
	while(mask)
	{
		if (IIC_Read_Clock())
			byte |= mask;
		mask >>= 1;
	}

	if(acknowledgment)
	{
		// Esto es para lectura consecutivas
		GPIO_Set(SDA, 0);
		IIC_Clock();
		GPIO_Set(SDA, 1);
	}
	else
	{
		GPIO_Set(SDA, 1);
		IIC_Clock();
	}
	return(byte);
}

uint8_t GetDemora_IIC( void )
{
	return demora_i2c;
}
void SetDemora_IIC( uint8_t value )
{
	demora_i2c = value;
}



void IIC_Write_Driver_32b(uint32_t Direccion, uint8_t Memoria, uint32_t Dato)
{
	/*STRUCT_INT32b	Dato_Temporal;

	Dato_Temporal.Uint32b = Dato;

	IIC_Write_Driver(Direccion, Memoria, Dato_Temporal.Bytes.Byte[0]);
	IIC_Write_Driver(Direccion + 1, Memoria, Dato_Temporal.Bytes.Byte[1]);
	IIC_Write_Driver(Direccion + 2, Memoria, Dato_Temporal.Bytes.Byte[2]);
	IIC_Write_Driver(Direccion + 3, Memoria, Dato_Temporal.Bytes.Byte[3]);*/
}

uint32_t IIC_Read_Driver_32b(uint32_t Direccion, uint8_t Memoria)
{
	/*STRUCT_INT32b	Dato_Temporal;

	Dato_Temporal.Bytes.Byte[3] = (uint32_t)IIC_Read_Driver(Direccion + 3, Memoria);
	Dato_Temporal.Bytes.Byte[2] = (uint32_t)IIC_Read_Driver(Direccion + 2, Memoria);
	Dato_Temporal.Bytes.Byte[1] = (uint32_t)IIC_Read_Driver(Direccion + 1, Memoria);
	Dato_Temporal.Bytes.Byte[0] = (uint32_t)IIC_Read_Driver(Direccion, Memoria);

	return Dato_Temporal.Uint32b;*/
	return 0;
}

uint16_t IIC_Read_Driver_16b(uint32_t Direccion, uint8_t Memoria)
{
	uint16_t	Dato_Temporal;

	Dato_Temporal = (uint8_t)IIC_Read_Driver(Direccion, Memoria);
	Dato_Temporal *= 256;
	Dato_Temporal += (uint8_t)IIC_Read_Driver(Direccion + 1, Memoria);

	return Dato_Temporal;
}

void IIC_Write_Driver_16b(uint32_t Direccion, uint8_t Memoria, uint16_t Dato)
{
	IIC_Write_Driver(Direccion, Memoria, (uint8_t)(Dato / 256));
	IIC_Write_Driver(Direccion + 1, Memoria, (uint8_t)(Dato % 256));
}

/*
//--------------------------------------------------------------
// Funcion para escribir una posicion de memoria
// Parametros:
//	Direccion: es una direccion de la memoria desde 0 a TAMANIO_MEMORIA
//	Memoria: es la direccion fisica de la memoria
// 	byte: dato a grabar en la memoria
//--------------------------------------------------------------*/
//uint8_t 	status;
uint8_t IIC_Write_Driver(uint32_t Direccion, uint8_t Memoria, uint8_t byte)
{
	/*uint8_t Reintento = 0;
	
	//xSemaphoreTake( xMutexIICDriver, portMAX_DELAY );

	//status = 0;                falla por default

	do
	{
		NVIC->ICER[0] = (1 << 16);

		IIC_Start();
		if (!IIC_Write(Memoria))
		{
			//status = 1;
			if (!IIC_Write((unsigned char)(Direccion >> 8)))
			{
				//status = 2;
				if (!IIC_Write((unsigned char)(Direccion)))
				{
					//status = 3;
					if (!IIC_Write(byte))
					{
						//status = 4;
						IIC_Stop();
						NVIC->ISER[0] = (1 << 16);
						return 1;           success
					}
				}
			}
		}
		IIC_Stop();

		NVIC->ISER[0] = (1 << 16);
		Reintento++;

	}while(Reintento < CANTIDAD_REINTENTOS_I2C);
	//xSemaphoreGive( xMutexIICDriver );*/

	return(0);
}
//--------------------------------------------------------------
// Funcion para leer una posicion de memoria
// Parametros:
//	Direccion: es una direccion de la memoria desde 0 a TAMANIO_MEMORIA
//	Memoria: es la direccion fisica de la memoria
// Devuelve: uchar dato leido
//--------------------------------------------------------------
uint8_t IIC_Read_Driver(uint32_t Direccion, uint8_t Memoria)
{
	/*uint8_t	Dato_Leido = 0xff, Reintento = 0;
	
	//xSemaphoreTake( xMutexIICDriver, portMAX_DELAY );
	do
	{
		NVIC->ICER[0] = (1 << 16);
		IIC_Start();
		if (!IIC_Write(Memoria))
		{
			if (!IIC_Write((unsigned char)(Direccion >> 8)))
			{
				if (!IIC_Write((unsigned char)Direccion))
				{
					IIC_Start();
					if (!IIC_Write(Memoria | 0x01))
					{
						Dato_Leido = IIC_Read(NO_ACK);
						IIC_Stop();
						NVIC->ISER[0] = (1 << 16);
						return(Dato_Leido);
					}
				}
			}
		}
		IIC_Stop();
		NVIC->ISER[0] = (1 << 16);
		Reintento++;

	}while(Reintento < CANTIDAD_REINTENTOS_I2C);

	return(Dato_Leido);
*/
	return 0;
}

void IIC_Leer_Bloque(uint32_t Direccion, uint8_t* Datos, uint32_t Tamanio)
{
/*	uint32_t	i;

	for(i = 0 ; i < Tamanio ; i++)
	{
		Datos[i] = (uint8_t)IIC_Read_Driver(Direccion + i, MEMORIA_1);
	}*/
}

void IIC_Guardar_Bloque(uint32_t Direccion, uint8_t* Datos, uint32_t Tamanio)
{
/*	uint32_t	i;

	for(i = 0 ; i < Tamanio ; i++)
	{
		IIC_Write_Driver(Direccion + i, MEMORIA_1, Datos[i]);
	}*/
}

//--------------------------------------------------------------
// Funcion para tener una demora
//--------------------------------------------------------------
void IIC_Demora(uint32_t demora)
{
	uint32_t	tiempo;

	for(tiempo = 0; tiempo < demora ; tiempo++);
//	for(tiempo = 0; tiempo < demora ; tiempo++);
//	for(tiempo = 0; tiempo < demora ; tiempo++);
//  	for(tiempo = 0; tiempo < demora ; tiempo++);
}					

//--------------------------------------------------------------
// Funcion para borrar toda las memorias del log
//--------------------------------------------------------------
void Borrar_Memoria(uint8_t Memoria)
{
	uint32_t	i;

	for( i = 0 ; i < TAMANIO_MEMORIA ; i++)
	{
		IIC_Escribir(i , Memoria, 0 );
	}
}

/*****************************************************************************
** Function name:		IIC_Open_driver
**
** Descriptions:		Funcion que se encarga de chequear que ningun otra tarea este utilizando la memoria
**
** parameters:			void
** Returned value:		OK or ERROR
**
*****************************************************************************/
//int32_t IIC_Open_driver(void)
//{
//	if( pdTRUE == xSemaphoreTake( xMutexIICDriver, portMAX_DELAY ) )
//	{
//		return OK;
//	}
//
//	return ERROR;
//}

/*****************************************************************************
** Function name:		IIC_Close_driver
**
** Descriptions:		Cierro la lectura o grabacion de la memoria
**
** parameters:			void
** Returned value:		void
**
*****************************************************************************/
//void IIC_Close_driver(void)
//{
//	xSemaphoreGive( xMutexIICDriver );
//
//	return;
//}

/*****************************************************************************
** Function name:		IIC_Init_driver
**
** Descriptions:		Funcion que se encarga de inicializar el driver de la memoria
**
** parameters:			void
** Returned value:		OK or ERROR
**
*****************************************************************************/
//int32_t IIC_Init_driver(void)
//{
//    // Creo el semaforo
//	if( NULL == ( xMutexIICDriver = xSemaphoreCreateMutex() ) )
//    	return ERROR;
//
// 	vQueueAddToRegistry( xMutexIICDriver, ( signed portCHAR * ) "xMutexIICDriver" );
//
//	return OK;
//}

