/*
 * DRIVER_LCD16X2.h
 *
 *  Created on: Jan 6, 2020
 *      Author: GJF-Trabajo
 */

#ifndef DRIVER_LCD16X2_DRIVER_LCD16X2_H_
#define DRIVER_LCD16X2_DRIVER_LCD16X2_H_
#include <LPC845.h>

#define 	LCD16x2_CONTROL		1
#define 	LCD16x2_DATA		0

#define		PIN_LCD16X2_RS		0,15
#define		PIN_LCD16X2_E		0,14
#define		PIN_LCD16X2_D7		0,13
#define		PIN_LCD16X2_D6		0,11
#define		PIN_LCD16X2_D5		0,10
#define		PIN_LCD16X2_D4		0,9

void LCD_Escribir(uint8_t data , uint8_t control);
void Display_LCD( uint8_t *msg , uint8_t r , uint8_t p );
void Inicializar_LCD( void );
void LCD_Delay( void );


#endif /* DRIVER_LCD16X2_DRIVER_LCD16X2_H_ */
