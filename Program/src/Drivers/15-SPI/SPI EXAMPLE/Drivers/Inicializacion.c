/*
 * Inicializacion.c
 *
 *  Created on: 16 ago. 2021
 *      Author: mariano
 */
#include <misc.h>

void Inicializar( void )
{

	Inicializar_PLL( );
	//Inicializar_SysTick( FREQ_PRINCIPAL / FREQ_SYSTICK * 2 );			//48000
	//Inicializar_SysTick( FREQ_PRINCIPAL / FREQ_SYSTICK );				//24000
    Inicializar_GPIO( );
    //Inicializar_UART0(9600);
    //Inicializar_CTimer0();
    //Inicializar_SCTimer( );
    Inicializar_IO( );
    //TimerStart(0, 1, HMC5883L_Leer , DEC );
    InitRC522( );


}
