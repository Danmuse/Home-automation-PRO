/*******************************************************************************************************************************//**
 *
 * @file		pr_teclado.c
 * @brief		Driver de teclado
 * @date		5 feb. 2020
 * @author		Ing. Gustavo Fresno
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../Teclado/teclado.h"

#include "gpio.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define		TECLADO_FILA_1			0,27
#ifdef KIT_DAC_MAL
	#define		TECLADO_FILA_2			0,29
#endif
#ifdef KIT_DAC_BIEN
	#define		TECLADO_FILA_2			0,28
#endif

#define		TECLADO_COLUMNA_1		0,30
#define		TECLADO_COLUMNA_2		0,31
#define		TECLADO_COLUMNA_3		0,8

#define 	PULSADOR_FRENTE_1		0,4


#define		CANTIDAD_DE_REBOTES		10

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/
uint8_t	BufferTeclado = NO_KEY;

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  void Inicializar_Display7Seg(void)
	\brief Inicializa el Display de 7 Segmentos
 	\author Ing. Gustavo Fresno
 	\date 5 feb. 2020
 	\param [in] void
 	\param [out] void
	\return void
*/
void Inicializar_Teclado( void )
{
	IOCON_config_t	Temporal;

	Temporal.open_drain = 0;
	Temporal.mode = PULL_UP;
	Temporal.clk_sel = IOCON_CLK_DIV_0;
	Temporal.iic_mode = 0;
	Temporal.dac_mode = 0;
	Temporal.sample_mode = 0;

	IOCON_config_io(IOCON_INDEX_PIO0_27, &Temporal);
	GPIO_Dir(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 0);

	#ifdef KIT_DAC_MAL
		IOCON_config_io(IOCON_INDEX_PIO0_29, &Temporal);
	#endif
	#ifdef KIT_DAC_BIEN
		IOCON_config_io(IOCON_INDEX_PIO0_28, &Temporal);
	#endif
	GPIO_Dir(TECLADO_FILA_2, 1);
	GPIO_Set(TECLADO_FILA_2, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_30, &Temporal);
	GPIO_Dir(TECLADO_COLUMNA_1, 0);
	GPIO_Set(TECLADO_COLUMNA_1, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_31, &Temporal);
	GPIO_Dir(TECLADO_COLUMNA_2, 0);
	GPIO_Set(TECLADO_COLUMNA_2, 0);

	IOCON_config_io(IOCON_INDEX_PIO0_8, &Temporal);
	GPIO_Dir(TECLADO_COLUMNA_3, 0);
	GPIO_Set(TECLADO_COLUMNA_3, 0);
}

//==============================================================================================
//		-		Proposito: Analisis del hardware del teclado. Se cuelga de una interruocion de timer
//		-		Parametros:   void
//		-		Retorna: void
//==============================================================================================
void TECLADO_Barrido(void)
{
	uint8_t CodigoActual ;

	CodigoActual = TECLADO_Barrido_HW();						// Se Compromete con el HW
	TECLADO_Barrido_SW(CodigoActual);							//  No se compromete con el HW
}

//==============================================================================================
//		-		Proposito: Analiza el rebote del teclado
//		-		Parametros:
//		-				CodigoActual : Ultima lectura
//		-		Retorna: void
//==============================================================================================
uint8_t CodigoAnterior = NO_KEY;
uint16_t EstadosEstables = 0;
void TECLADO_Barrido_SW( uint8_t CodigoActual )
{
	if( CodigoActual == NO_KEY )								// No hay tecla presionada o hubo un rebote
	{
		CodigoAnterior = NO_KEY;

		EstadosEstables = 0;
		return;
	}

	if( EstadosEstables == 0  )										// Se detecto una tecla por primera vez
	{
		CodigoAnterior = CodigoActual;
		EstadosEstables = 1;
		return;
	}

	if( CodigoActual != CodigoAnterior )						// Pregunto si la tecla sigue pulsada
	{
		CodigoAnterior = NO_KEY;
		EstadosEstables = 0;
		return;
	}

	if( EstadosEstables == CANTIDAD_DE_REBOTES )
	{
		BufferTeclado = (uint8_t)CodigoActual;

		EstadosEstables++;

		return;
	}

	if( EstadosEstables >= CANTIDAD_DE_REBOTES + 1)
	{
		return;
	}

	EstadosEstables ++;

	return;
}

//==============================================================================================
//		-		Proposito: Lectura de las entradas para ver el estado de las teclas
//		-		Parametros: void
//	Retorna
//		-		Codigo de tecla si hubo tecla
//		-		NO_KEY si no hubo tecla
//==============================================================================================
uint8_t TECLADO_Barrido_HW(void)
{
	// Funcion de barrido de teclado de 3x2
	// Barro fila 1
	GPIO_Set(TECLADO_FILA_1, 0);
	GPIO_Set(TECLADO_FILA_2, 1);

	if(!GPIO_Read(TECLADO_COLUMNA_1))
		return CODIGO_TECLA_0;
	if(!GPIO_Read(TECLADO_COLUMNA_2))
		return CODIGO_TECLA_1;
	if(!GPIO_Read(TECLADO_COLUMNA_3))
		return CODIGO_TECLA_2;

	// Barro fila 2
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_1, 1);
	GPIO_Set(TECLADO_FILA_2, 0);
	GPIO_Set(TECLADO_FILA_2, 0);
	GPIO_Set(TECLADO_FILA_2, 0);
	GPIO_Set(TECLADO_FILA_2, 0);
	GPIO_Set(TECLADO_FILA_2, 0);
	GPIO_Set(TECLADO_FILA_2, 0);

	if(!GPIO_Read(TECLADO_COLUMNA_1))
		return CODIGO_TECLA_3;
	if(!GPIO_Read(TECLADO_COLUMNA_2))
		return CODIGO_TECLA_4;
	if(!GPIO_Read(TECLADO_COLUMNA_3))
		return CODIGO_TECLA_5;

  	return NO_KEY;
}

/***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/
/**
	\fn  uint8_t GetKey( void )
	\brief Lectura de teclado
 	\author Ing. Marcelo Trujillo
 	\date 13 mar. 2020
 	\param [in] void
	\return uint8_t: codigo de tecla pulsada o NO_KEY
*/

uint8_t GetKey( void )
{
	uint8_t Data = BufferTeclado;

	BufferTeclado = NO_KEY;

	return Data;
}


