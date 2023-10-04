/*
 * i2c.c
 *
 *  Created on: 27 oct. 2022
 *      Author: mariano
 */

#include "i2c.h"
uint8_t demora_i2c;

void IIC_init(void)
{
	GPIO_Dir(PIN_I2C_SDA, 1);
	GPIO_Dir(PIN_I2C_SCL, 1);
	GPIO_Set(PIN_I2C_SCL, 1);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SDA, 1);
	IIC_Demora(IIC_DELAY);
}
void IIC_start(void)
{
	GPIO_Set(PIN_I2C_SCL, 1);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SDA, 1);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SDA, 0);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SCL, 0);
	IIC_Demora(IIC_DELAY);
}
void IIC_stop(void)
{
	GPIO_Set(PIN_I2C_SCL, 0);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SDA, 0);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SCL, 1);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SDA, 1);
	IIC_Demora(IIC_DELAY);
}
void IIC_writex( uint8_t j)
{
	uint8_t i,temp;
	temp = j;
	for(i=0; i<8; i++)
	{
		GPIO_Set(PIN_I2C_SCL, 0);
		IIC_Demora(IIC_DELAY);
		if(temp & 0x80)
			GPIO_Set(PIN_I2C_SDA, 1);
		else
			GPIO_Set(PIN_I2C_SDA, 0);
		IIC_Demora(IIC_DELAY);
		GPIO_Set(PIN_I2C_SCL, 1);
		IIC_Demora(IIC_DELAY);
		temp = temp << 1;
	}
	GPIO_Set(PIN_I2C_SCL, 0);
	IIC_Demora(IIC_DELAY);
}
uint8_t IIC_readx(void)
{
	uint8_t i, j, k = 0;
	for(i=0; i<8; i++)
	{
		GPIO_Set(PIN_I2C_SCL, 0);
		IIC_Demora(IIC_DELAY);
        GPIO_Dir(PIN_I2C_SDA, 0);
		if(GPIO_Read(PIN_I2C_SDA ) )
		{
			j = 1;
		}
		else
		{
			j = 0;
		}
        GPIO_Dir(PIN_I2C_SDA, 1);
		k = (k << 1) | j;
		GPIO_Set(PIN_I2C_SCL, 1);
		IIC_Demora(IIC_DELAY);
	}
	return(k);
}
void IIC_ack(void)
{
	uint8_t i = 0;
	GPIO_Set(PIN_I2C_SCL, 1);
	IIC_Demora(IIC_DELAY);
	GPIO_Dir(PIN_I2C_SDA, 0);
	while( (GPIO_Read(PIN_I2C_SDA )) && (i < 255))
	{
		i++;
	}
	GPIO_Dir(PIN_I2C_SDA, 1);
    GPIO_Set(PIN_I2C_SCL, 0);
	IIC_Demora(IIC_DELAY);
}
int32_t IIC_readword(uint8_t Address, uint8_t address)
{
	union
	{
	  int32_t x;
	  uint8_t i[2];
	}aux;
	IIC_start();
	IIC_writex(Address);
	IIC_ack();
	IIC_writex(address);
	IIC_ack();
	IIC_start();
	IIC_writex(Address+1);
	IIC_ack();
	aux.i[0] = IIC_readx();
	GPIO_Set(PIN_I2C_SDA, 1);
	GPIO_Set(PIN_I2C_SCL, 1);
	IIC_Demora(IIC_DELAY);
	GPIO_Set(PIN_I2C_SCL, 0);
	IIC_Demora(IIC_DELAY);
	aux.i[1] = IIC_readx();
	IIC_stop();
	return(aux.x);
}

uint8_t IIC_read( uint8_t Address, uint8_t address)
{
	uint8_t i;
	IIC_start();
	IIC_writex(Address);
	IIC_ack();
	IIC_writex(address);
	IIC_ack();
	IIC_start();
	IIC_writex(Address+1);
	IIC_ack();
	i = IIC_readx();
	IIC_stop();
	return(i);
}

uint8_t IIC_readDataHMC5838L( uint8_t* data )
{
	uint8_t i;
	IIC_start();
	for( i = 0; i< 6; i++)
		data[i] = IIC_readx();
	IIC_stop();
	return(i);
}

void IIC_write(uint8_t Address,uint8_t address, uint8_t info)
{
	IIC_start();
	IIC_writex(Address);
	IIC_ack();
	IIC_writex(address);
	IIC_ack();
	if( info < 0xFF )
	{
		IIC_writex(info);
		IIC_ack();
	}

	IIC_stop();
}

uint8_t GetDemora_IIC( void )
{
	return demora_i2c;
}
void SetDemora_IIC( uint8_t value )
{
	demora_i2c = value;
}

//--------------------------------------------------------------
// Funcion para tener una demora
//--------------------------------------------------------------
void IIC_Demora(uint32_t demora)
{
	uint32_t	tiempo;

	for(tiempo = 0; tiempo < demora ; tiempo++);
	for(tiempo = 0; tiempo < demora ; tiempo++);
	//for(tiempo = 0; tiempo < demora ; tiempo++);
  	//for(tiempo = 0; tiempo < demora ; tiempo++);
}
