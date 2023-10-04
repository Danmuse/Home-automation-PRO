/*
 * ServoSG90.h
 *
 *  Created on: 15 ago. 2021
 *      Author: mariano
 */

#ifndef MISC_H_
#define MISC_H_

#include <stdio.h>
#include <cr_section_macros.h>
#include "tipos.h"
#include "dr_pll.h"
#include "dr_systick.h"
#include "gpio.h"
#include "io.h"
#include "lcd.h"
#include "ctimer.h"
#include "uart1.h"
#include "teclado.h"
#include "timers.h"
#include "sctimer.h"
#include "i2c.h"




#define		PERIODOpWM				480000		//20000 * 80ns = 1,6 ms
#define 	NOVENTAgRADOS			57600	//54000	//48000		//2000  * 80ns = 160 us
#define 	CEROgRADOS				(MENOSnOVENTAgRADOS + (NOVENTAgRADOS-MENOSnOVENTAgRADOS)/2)
#define 	MENOSnOVENTAgRADOS		10000//12000	//20000	//24000
#define		SPAN					(NOVENTAgRADOS - MENOSnOVENTAgRADOS)	//12500 (1 ms)
#define		GRADO					SPAN/180	//69,44 (5,55 us por grado)
#define 	REPETICIONgRADOS		9

void Inicializar( void );
void PulsoTrigger( void );

#endif /* MISC_H_ */
