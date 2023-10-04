//************************************************************************************************
// Archivo:				OS_GPIO_DRIVER.c
//
// Descripcion:
//
// Realizado por:		Ing. Fresno Gustavo (13/09/11)
//
//************************************************************************************************

#include "gpio.h"

#include <LPC845.h>

void Inicializar_GPIO(void)
{
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);								// 7 = SWM
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 6) | (1 << 20) | (1 << 18);		// 6 = GPIO0	20 = GPIO1	18 = IOCON

	GPIO_Dir( PIN_AZUL, 1 ); // Rojo
	GPIO_Set( PIN_AZUL, LED_OFF );

	GPIO_Dir( PIN_VERDE, 1 ); // verde
	GPIO_Set( PIN_VERDE, LED_OFF );

	GPIO_Dir( PIN_ROJO, 1 );
	GPIO_Set( PIN_ROJO, LED_OFF );


}

// Funcion para la escritura de un puerto
void GPIO_Set(uint32_t Port, uint32_t Pin, uint32_t Valor)
{
	if(Valor)
		GPIO->SET[Port] |= 1<<Pin;
	else
		GPIO->CLR[Port] |= 1<<Pin;

}

// Funcion para configurar un puerto como entrada o salida
void GPIO_Dir(uint32_t Port, uint32_t Pin, uint32_t Valor)
{
	if(Valor)
		GPIO->DIRSET[Port] |= 1<<Pin;
	else
		GPIO->DIRCLR[Port] |= 1<<Pin;
}

// Funcion para la lectura de un puerto
uint32_t GPIO_Read(uint32_t Port, uint32_t Pin)
{
	uint32_t	Lectura;

	Lectura = GPIO->PIN[Port];

	Lectura = (Lectura >> Pin ) & 1 ;

	return Lectura;
}

int32_t IOCON_config_io(uint32_t Indice_PortPin, IOCON_config_t *pin_config)
{
	IOCON_PIO_reg_t *pio_reg;
	IOCON_PIO_reg_t pio_reg_to_write;

	// Mantengo configuraciones de DAC/IIC de haberlas
	pio_reg = (IOCON_PIO_reg_t *)&IOCON->PIO[Indice_PortPin];

	pio_reg_to_write = *pio_reg;

	if(pin_config->iic_mode > 0)
		pio_reg_to_write.I2CMODE = pin_config->iic_mode;

	if(pin_config->dac_mode > 0)
		pio_reg_to_write.DACMODE = pin_config->dac_mode;

	if(pin_config->mode != 0xff)
		pio_reg_to_write.MODE = pin_config->mode & 0x03;

	pio_reg_to_write.HYS = pin_config->hysteresis;
	pio_reg_to_write.INV = pin_config->invert_input;
	pio_reg_to_write.OD = pin_config->open_drain;
	pio_reg_to_write.S_MODE = pin_config->sample_mode;
	pio_reg_to_write.CLK_DIV = pin_config->clk_sel;

	*pio_reg = pio_reg_to_write;

	return IOCON_CONFIG_SUCCESS;
}



