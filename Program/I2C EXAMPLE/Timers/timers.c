/*******************************************************************************************************************************//**
 *
 * @file		DR_Timers.c
 * @brief		Descripcion del modulo
 * @date		9 de jun. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "timers.h"
#include "gpio.h"
#include "io.h"

#include "tipos.h"
volatile 	uint32_t Tmr_Run[ N_TIMERS ];
volatile 	uint8_t  TMR_Events[ N_TIMERS ];
void 	 	(* TMR_handlers [N_TIMERS]) (void);
volatile 	uint8_t  TMR_StandBy[ N_TIMERS ];
volatile 	uint8_t  Tmr_Base[ N_TIMERS ];
volatile 	uint16_t tiempoDAC = 0;
uint8_t 	eeprom_ready = 0;
/**
	\fn void Timer_Check(void)
	\brief Decremento periodico de los contadores
 	\details Decrementa los contadores de los timers en ejecucion. Debe ser llamada periodicamente con la base de tiempos
	\return void
*/
void AnalizarTimers(void)
{
	uint32_t i;
	for(i=0; i< N_TIMERS ; i++)
	{
		if(Tmr_Run[ i ])
		{
			if ( !TMR_StandBy[ i ] )
			{
				Tmr_Run[ i ]--;
				if(!Tmr_Run[ i ])
					TMR_Events[ i ] = 1;
			}
		}
	}
}

/**
	\fn void Timer_Event(void)
	\brief Chequeo de timers vencidos
 	\details Llama a los callbacks de los timers vencidos. Debe llamarse desde el lazo principal del programa
	\return void
*/
void TimerEvent(void)
{
	uint8_t i;
	for( i=0 ; i < N_TIMERS ; i++ )
	{
		if(TMR_Events[ i ])
		{
			TMR_handlers[ i ]( );
			TMR_Events[ i ] = 0;
		}
	}
}

/**
	\fn void TimerStart(uint8_t event, timer_t t, void (*handler)(void))
	\brief Inicia un timer
 	\details Inicia el timer \a e al transcurrir el tiempo especificado por \a t se llama a la funcion apuntada por \a handler
 	\param [in] event Numero de evento entre 0 y 31
 	\param [in] t Tiempo del evento. Dependiente de la base de tiempos
 	\param [in] handler Callback del evento
	\return void
*/
void TimerStart(uint8_t event, uint32_t time, Timer_Handler handler , uint8_t base )
{
	switch ( base )
	{
		case DEC:
			time *= DECIMAS;
			break;
		case SEG:
			time *= ( SEGUNDOS * DECIMAS );
			break;
		case MIN:
			time *= ( MINUTOS * SEGUNDOS * DECIMAS );
			break;
	}

	Tmr_Base[event] = base;

	if(time != 0)	//el tiempo no es 0, lo cargo
	{
		Tmr_Run[event] = time;
		TMR_Events[event] = 0;
	}
	else	//el tiempo es cero, el timer vence automáticamente
	{
		Tmr_Run[event] = 0;
		TMR_Events[event] = 1;
	}
	TMR_handlers[event] = handler;
}

/**
	\fn void SetTimer( uint8_t event , timer_t t )
	\brief Inicia un timer
 	\details Reinicia el timer con el valor t (no lo resetea)
 	\param [in] event Numero de evento entre 0 y 31
 	\param [in] t Tiempo del evento. Dependiente de la base de tiempos
 	\return void
*/
void SetTimer( uint8_t event, uint32_t time )
{
	switch ( Tmr_Base[event] )
	{
		case DEC:
			time *= DECIMAS;
			break;
		case SEG:
			time *= ( SEGUNDOS * DECIMAS );
			break;
		case MIN:
			time *= ( MINUTOS * SEGUNDOS * DECIMAS );
			break;
	}
	Tmr_Run[event] = time;
}

/**
	\fn  GetTimer( uint8_t event )
	\brief Toma el valor al vuelo del timer en cuestion
 	\details Lee el timer
 	\param [in] event Numero de evento entre 0 y 31
 	\return valor del timer
*/
uint32_t GetTimer( uint8_t event )
{
	uint32_t time = Tmr_Run[event];

	switch ( Tmr_Base[event] )
	{
		case DEC:
			time /= DECIMAS;
			break;
		case SEG:
			time /= ( SEGUNDOS * DECIMAS );
			break;
		case MIN:
			time /= ( MINUTOS * SEGUNDOS * DECIMAS );
			break;
	}
	return time;
}

/**
	\fn  StandByTimer( uint8_t event , uint8_t accion)
	\brief Detiene/Arranca el timer, NO lo resetea
 	\details lo pone o lo saca de stand by
 	\param [in] event Numero de evento entre 0 y 31
 	\param [in] accion RUN lo arranca, PAUSE lo pone en stand by
 	\return void
*/
void StandByTimer( uint8_t event , uint8_t accion)
{
	TMR_StandBy[ event ] = accion;
}

/**
	\fn void Timer_Stop(Eventos e)
	\brief Detiene un timer
 	\details Detiene el timer \a e
 	\param [in] e Numero de evento entre 0 y 31
	\return void
*/
void TimerStop(uint8_t event)
{
	Tmr_Run[ event ] = 0;
	TMR_Events[ event ] = 0;
	TMR_handlers[ event ] = NULL;
	Tmr_Base[ event ] = 0;
	TMR_StandBy[ event ] = RUN;
}

/**
	\fn void Timer_Close(void)
	\brief Detiene los timers
 	\details Detiene todos los timers
	\return void
*/
void TimerClose(void)
{
	uint32_t i;

	for( i=0 ; i < N_TIMERS ; i++ )
		TimerStop( i );
}

/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Ing. Marcelo Trujillo
 	\date 20 feb. 2020
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
void eeprom_action (void)
{
	eeprom_ready = 1;
	TimerStart(0, 10, eeprom_action , DEC );

}

uint8_t GetFlagEeprom( void )
{
	return eeprom_ready;
}

void SetFlagEeprom( uint8_t value  )
{
	eeprom_ready = value;
}

void EV_DAC( void )
{
	tiempoDAC++;
	tiempoDAC%=1023;
	TimerStart( 1 , 1 , EV_DAC, DEC );
}
uint32_t GetTiempoDAC( void )
{
	return tiempoDAC;
}


