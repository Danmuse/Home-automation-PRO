/*
 * DRIVER_LCD16X2.c
 *
 *  Created on: Jan 6, 2020
 *      Author: GJF-Trabajo
 */
#include "lcd.h"

#include "gpio.h"

void LCD_Escribir(uint8_t data , uint8_t control)
{
	int32_t q , i = 1;

	do
	{
		GPIO_Set(PIN_LCD16X2_D7, ( data >> ( 3 + i * 4 ) ) & 0x01);
		GPIO_Set(PIN_LCD16X2_D6, ( data >> ( 2 + i * 4 ) ) & 0x01) ;
		GPIO_Set(PIN_LCD16X2_D5, ( data >> ( 1 + i * 4 ) ) & 0x01) ;
		GPIO_Set(PIN_LCD16X2_D4, ( data >> ( 0 + i * 4 ) ) & 0x01) ;

		if( control )
			GPIO_Set(PIN_LCD16X2_RS, 0);
		else
			GPIO_Set(PIN_LCD16X2_RS, 1);

		GPIO_Set(PIN_LCD16X2_E, 1);

		for( q = 0 ; q < 400 ; q++ );

		GPIO_Set(PIN_LCD16X2_E, 0);

		for( q = 0 ; q < 14000 ; q++ );
	}
	while ( i-- );
}

void Display_LCD( uint8_t *msg , uint8_t r , uint8_t p )
{
	unsigned char i ;

	if( r )
        p = p + 0xc0 ;
	else
		p = p + 0x80 ;

	LCD_Escribir( p , LCD16x2_CONTROL );
	for( i = 0 ; msg[ i ] != '\0' ; i++ )
		LCD_Escribir( msg[ i ] , LCD16x2_DATA );
}


void Inicializar_LCD( void )
{
	IOCON_config_t	Temporal;
	uint32_t i, j;

	Temporal.open_drain = 1;
	Temporal.mode = PULL_NONE;
	Temporal.clk_sel = IOCON_CLK_DIV_0;
	Temporal.iic_mode = 0;
	Temporal.dac_mode = 0;
	Temporal.sample_mode = 0;

	IOCON_config_io(IOCON_INDEX_PIO0_14, &Temporal);
	GPIO_Dir(PIN_LCD16X2_E, 1);
	GPIO_Set(PIN_LCD16X2_E, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_15, &Temporal);
	GPIO_Dir(PIN_LCD16X2_RS, 1);
	GPIO_Set(PIN_LCD16X2_RS, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_13, &Temporal);
	GPIO_Dir(PIN_LCD16X2_D7, 1);
	GPIO_Set(PIN_LCD16X2_D7, 0);

	// Configuro los pines del micro con pullup
	IOCON_config_io(IOCON_INDEX_PIO0_11, &Temporal);
	GPIO_Dir(PIN_LCD16X2_D6, 1);
	GPIO_Set(PIN_LCD16X2_D6, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_10, &Temporal);
	GPIO_Dir(PIN_LCD16X2_D5, 1);
	GPIO_Set(PIN_LCD16X2_D5, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_9, &Temporal);
	GPIO_Dir(PIN_LCD16X2_D4, 1);
	GPIO_Set(PIN_LCD16X2_D4, 0);

	for( i = 0 ; i < 3 ; i++ )
	{
		GPIO_Set(PIN_LCD16X2_E, 0);
		LCD_Delay( );
		GPIO_Set(PIN_LCD16X2_D7, 0);
		GPIO_Set(PIN_LCD16X2_D6, 0) ;
		GPIO_Set(PIN_LCD16X2_D5, 1) ;
		GPIO_Set(PIN_LCD16X2_D4, 1) ;

		GPIO_Set(PIN_LCD16X2_RS, 0);
		GPIO_Set(PIN_LCD16X2_E, 1);
		for( j = 0 ; j < 10000 ; j++ );//4500
		GPIO_Set(PIN_LCD16X2_E, 0);
	}

	GPIO_Set(PIN_LCD16X2_E, 0);
	LCD_Delay( );

	GPIO_Set(PIN_LCD16X2_D7, 0);
	GPIO_Set(PIN_LCD16X2_D6, 0) ;
	GPIO_Set(PIN_LCD16X2_D5, 1) ;
	GPIO_Set(PIN_LCD16X2_D4, 0) ;

	GPIO_Set(PIN_LCD16X2_RS, 0);
	GPIO_Set(PIN_LCD16X2_E, 1);
	for( j = 0 ; j < 10000 ; j++ );//4500
	GPIO_Set(PIN_LCD16X2_E, 0);

	// A partir de aca pasa a 4 bits
	LCD_Escribir( 0x28 , LCD16x2_CONTROL);
	LCD_Escribir( 0x08 , LCD16x2_CONTROL);	// display OFF
	LCD_Escribir( 0x01 , LCD16x2_CONTROL);	// clear display
	LCD_Escribir( 0x06 , LCD16x2_CONTROL);	// programo el LCD para mis necesidades
	LCD_Escribir( 0x0C , LCD16x2_CONTROL);	// display ON,sin cursor y blinking OFF del cursor
}

void LCD_Delay( void )
{
  uint32_t 	i;

  for ( i = 0 ; i < 500000 ; i++ ); // 300000
}

