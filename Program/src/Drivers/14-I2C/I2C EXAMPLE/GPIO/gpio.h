/*
 * OS_GPIO_DRIVER.h
 *
 *  Created on: 13/09/2011
 *      Author: GustavoFresno
 */

#ifndef OS_GPIO_DRIVER_H_
#define OS_GPIO_DRIVER_H_

#include "tipos.h"
#define		ENCENDER			1
#define		APAGAR				0

#define		SALIDA_SIN_ACCION	0xff

#define PIN_ROJO		1,2	// Port for led
#define PIN_AZUL		1,1	// Port for led
#define PIN_VERDE		1,0	// Port for led

#define LED_ON			0
#define LED_OFF			1



//#define		ESTADO_ENTRADA_2		(1 << 2)
//#define		ESTADO_ENTRADA_3		(1 << 3)
//#define		ESTADO_ENTRADA_4		(1 << 4)
//#define		ESTADO_ENTRADA_5		(1 << 5)
//#define		ESTADO_ENTRADA_6		(1 << 6)
//#define		ESTADO_ENTRADA_CONTACTO	(1 << 7)
//
//#define		CANTIDAD_DE_ENTRADAS_DIGITALES	8
//#define		CANTIDAD_DE_SALIDAS_DIGITALES	8
//
//#define		TIEMPO_ACTIVACION_ESTADO_NEGATIVO		500
//#define		TIEMPO_ACTIVACION_ESTADO_POSITIVO		500
//
//#define		ESTADO_INICIAL_ENTRADAS			1

#define		ESTADO_1			1
#define		ESTADO_0			0
#define		ESTADO_AMBOS		2
#define		ESTADO_DESACTIVADO	0xFF

#define		IOCON_CONFIG_SUCCESS					0
#define		IOCON_CONFIG_IOCON_NOT_CLOCKED			-1
#define		IOCON_CONFIG_INVALID_PORT				-2
#define		IOCON_CONFIG_INVALID_PIN				-3
#define		IOCON_CONFIG_INVALID_PORTPIN			-4

typedef enum
{
	PULL_NONE = 0,
	PULL_DOWN,
	PULL_UP,
	PULL_REPEATER
}IOCON_pull_mode_en;

typedef enum
{
	SAMPLE_MODE_BYPASS = 0,
	SAMPLE_MODE_1_CLOCK,
	SAMPLE_MODE_2_CLOCK,
	SAMPLE_MODE_3_CLOCK
}IOCON_sample_mode_en;

typedef enum
{
	IOCON_CLK_DIV_0 = 0,
	IOCON_CLK_DIV_1,
	IOCON_CLK_DIV_2,
	IOCON_CLK_DIV_3,
	IOCON_CLK_DIV_4,
	IOCON_CLK_DIV_5,
	IOCON_CLK_DIV_6
}IOCON_clk_sel_en;

typedef enum
{
	IIC_MODE_STANDARD = 0,
	IIC_MODE_GPIO,
	IIC_MODE_FAST_MODE
}IOCON_iic_mode_en;

typedef struct
{
	IOCON_pull_mode_en mode;
	uint8_t hysteresis;
	uint8_t invert_input;
	uint8_t open_drain;
	IOCON_sample_mode_en sample_mode;
	IOCON_clk_sel_en clk_sel;
	uint8_t dac_mode;
	IOCON_iic_mode_en iic_mode;
}IOCON_config_t;

typedef struct
{
	uint32_t RESERVED_1 : 3;
	uint32_t MODE : 2;
	uint32_t HYS : 1;
	uint32_t INV : 1;
	uint32_t I2CMODE : 2;
	uint32_t RESERVED_2 : 1;
	uint32_t OD : 1;
	uint32_t S_MODE : 2;
	uint32_t CLK_DIV : 3;
	uint32_t DACMODE : 1;
	uint32_t RESERVED_3 : 15;
}IOCON_PIO_reg_t;

typedef struct
{
//	STRUCT_Conf_GPIO	Logica_Entradas[CANTIDAD_DE_ENTRADAS_DIGITALES];
//
//	uint8_t				Estados_Salidas_Digitales;
//
//	uint8_t				Tiempo_Salidas[CANTIDAD_DE_SALIDAS_DIGITALES];
//	uint8_t				Modo_Salidas[CANTIDAD_DE_SALIDAS_DIGITALES];
//
//	uint8_t				Estados_Entradas_Digitales;
//
//	// Variables internas
//	uint16_t			Cantidad_Aceptar_Estado_P[CANTIDAD_DE_ENTRADAS_DIGITALES];		//Cantidad de veces de antirebote para aceptar el estado de negativo a positivo
//	uint16_t			Cantidad_Aceptar_Estado_N[CANTIDAD_DE_ENTRADAS_DIGITALES];		//Cantidad de veces de antirebote para aceptar el estado de positivo a negativo
//
//	// Variables para Antirebote
////	uint16_t			Aceptar_Estado;
//	uint16_t 			ContadorEstados[CANTIDAD_DE_ENTRADAS_DIGITALES];
}STRUCT_GPIO;

void Inicializar_GPIO(void);
void GPIO_Set(uint32_t Port, uint32_t Pin, uint32_t Valor);
void GPIO_Dir(uint32_t Port, uint32_t Pin, uint32_t Valor);
uint32_t GPIO_Read(uint32_t Port, uint32_t Pin);
void GPIO_Config_Pin(int32_t Port, uint32_t Pin, uint32_t Func, uint32_t Pinmode, uint32_t OpenDrain);
void GPIO_OpenDrain(uint8_t Puerto, uint8_t Pin, uint8_t OpenDrain);
void GPIO_Seteo_Salidas_Digitales(uint32_t Numero, uint32_t Estado);
uint8_t GPIO_Estado_Entradas_Digitales(void);
void GPIO_Barrido_Salidas(void);
void GPIO_Barrido_Entradas(void);
void GPIO_Mode(uint8_t Puerto, uint8_t Pin, uint8_t Modo);
void GPIO_Funcion(uint8_t Puerto, uint8_t Pin, uint8_t Funcion);
int32_t IOCON_config_io(uint32_t Indice_PortPin, IOCON_config_t *pin_config);

uint32_t ENTRADA_Leer(uint32_t	Entrada);
uint32_t CONTACTO_Get(void);
uint32_t ENTRADA_Leer_Todas(void);
uint32_t ENTRADA_Analizar(void);

void SALIDAS_Inicializar(void);
void SALIDAS_Inic_Timer(unsigned char Salida, unsigned char Modo, unsigned char Tiempo);
void SALIDAS_Analizar_Modo_y_Tiempo(void);

void LOG_GPIO_Inicializar(void);
void LOG_GPIO_Configurar(uint8_t Nro_Entrada, uint8_t Modo, uint8_t Nro_Salida, uint16_t Accion_1, uint16_t Accion_2, uint16_t Mensaje, uint16_t Evento);
void LOG_GPIO_Analisis(uint8_t Nro_Entrada);


#endif /* OS_GPIO_DRIVER_H_ */
