/*
 * i2c.h
 *
 *  Created on: 27 oct. 2022
 *      Author: mariano
 */

#ifndef I2C_H_
#define I2C_H_

#include "tipos.h"
#include "gpio.h"

#define 	IIC_ESPERA_DATOS	100
#define 	IIC_DELAY			100

//#define		PIN_I2C_SCL		0,0		//Pin39 del stick
//#define		PIN_I2C_SDA		0,1		//Pin38 del stick

//#define		PIN_I2C_SCL		0,9		//Pin18 del stick
//#define		PIN_I2C_SDA		0,8		//Pin19 del stick

#define		PIN_I2C_SCL		0,10		//Pin del stick
#define		PIN_I2C_SDA		0,11		//Pin del stick


void IIC_init(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_writex( uint8_t );
uint8_t IIC_readx(void);
void IIC_ack(void);
int32_t IIC_readword(uint8_t , uint8_t );
uint8_t IIC_read( uint8_t , uint8_t );
uint8_t IIC_readDataHMC5838L( uint8_t* );
void IIC_write(uint8_t ,uint8_t , uint8_t );
uint8_t GetDemora_IIC( void );
void SetDemora_IIC( uint8_t );
void IIC_Demora(uint32_t );

#endif /* I2C_H_ */
