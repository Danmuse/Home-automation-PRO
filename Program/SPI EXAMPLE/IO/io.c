/*
 * dr_io.c
 *
 *  Created on: 26 abr. 2021
 *      Author: mariano
 */
#include "io.h"

uint8_t BufferEntradas;				//!< Buffer de Entradas Digitales

/**
	\fn void Debounce(void)
	\brief Filtrado de entradas digitales
 	\param void
	\return void
*/
uint8_t Entradas( void )
{
	return BufferEntradas;
}
void Inicializar_IO(void)
{
	IOCON_config_t	Temporal;

	Temporal.open_drain = 0;
	Temporal.mode = PULL_UP;
	Temporal.clk_sel = IOCON_CLK_DIV_0;
	Temporal.iic_mode = 0;
	Temporal.dac_mode = 0;
	Temporal.sample_mode = 0;

	IOCON_config_io(IOCON_INDEX_PIO0_4, &Temporal);
	GPIO_Dir( PIN_MICROSWITCH, 0 );
	GPIO_Set( PIN_MICROSWITCH, 0 );

	IOCON_config_io(IOCON_INDEX_PIO0_26, &Temporal);
	GPIO_Dir( PIN_SODO, 0 );
	GPIO_Set( PIN_SODO, 0 );

	IOCON_config_io(IOCON_INDEX_PIO1_0, &Temporal);
	GPIO_Dir( PIN_O0, 1 );
	GPIO_Set( PIN_O0, 0 );

	IOCON_config_io(IOCON_INDEX_PIO1_1, &Temporal);
	GPIO_Dir( PIN_O1, 1 );
	GPIO_Set( PIN_O1, 0 );

	IOCON_config_io(IOCON_INDEX_PIO1_2, &Temporal);
	GPIO_Dir( PIN_O2, 1 );
	GPIO_Set( PIN_O2, 0 );



	IOCON_config_io(IOCON_INDEX_PIO0_9, &Temporal);
	GPIO_Dir( nss, 1 );
	GPIO_Set( nss, 0 );

	IOCON_config_io(IOCON_INDEX_PIO0_8, &Temporal);
	GPIO_Dir( nrst, 1 );
	GPIO_Set( nrst, 0 );

	IOCON_config_io(IOCON_INDEX_PIO0_31, &Temporal);
	GPIO_Dir( miso, 0 );		//MISO es entrada
	GPIO_Set( miso, 0 );

	IOCON_config_io(IOCON_INDEX_PIO0_30, &Temporal);
	GPIO_Dir( mosi, 1 );
	GPIO_Set( mosi, 0 );

	IOCON_config_io(IOCON_INDEX_PIO0_29, &Temporal);
	GPIO_Dir( sck, 1 );
	GPIO_Set( sck, 0 );

}


void Debounce(void)
{
	unsigned char cambios,j = 0;

	static char ContadorEstados[ 4 ];

	if( !GPIO_Read(PIN_MICROSWITCH) )
		j = 1;
	if( !GPIO_Read(PIN_SODO) )
		j |= 0x02;

	cambios = ( BufferEntradas ^ j );

	if( cambios )
	{
		for( j = 0 ; j < ENTRADAS ; j++ )
		{
			if( cambios & (0x01 << j) )
			{
				ContadorEstados[ j ] ++;

				if(ContadorEstados[ j ] == ACEPTAReSTADO)
				{
					ContadorEstados[ j ] = 0;
					BufferEntradas = BufferEntradas ^ (0x01 << j);
				}
			}
			else
				ContadorEstados[ j ] = 0;
		}
	}
	else
	{
		for( j = 0 ; j < ENTRADAS ; j++ )
			ContadorEstados[ j ] = 0;
	}
}

