/*!
 * @file ProgramConfig.h
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/08/2023 23:40:36
 * @brief Program configurations API (Application Programming Interface).
 */

#ifndef PROGRAM_CONFIG_H_
#define PROGRAM_CONFIG_H_

#include "GPIO.h"

#define DEBUG_MODE 1

//#define CN5_PINS	// SENSOR_IN {Sensor}
#define CN6_PINS	// DIG_OUT0 {O0} - DIG_OUT1 {O1} - DIG_OUT2 {O2} - DIG_OUT3 {A0} - DIG_OUT4 {A1}
//#define CN7_PINS	// ANALOG_OUT {AOUT}
//#define CN8_PINS	// ANALOG_IN {AIN}
//#define CN9_PINS	// SWDIO [PIN 2] - SWCLK [PIN 3] - DIG_IN [PIN 4] - KEY_RESET [PIN 5]
#define CN10_PINS	// INT0_IN {IN0} - INT1_IN {IN1}
#define CN12_PINS	// BCDA {BCDA} - BCDB {BCDB} - BCDC {BCDC} - BCDD {BCDD} - BCD_RST {RST} - BCD_CLK {CK}
#define CN13_PINS	// RX0_IN {RX} - TX0_OUT {TX} - EN_OUT {EN}
//#define CN15_PINS	// LCD_D7 {D7} - LCD_D6 {D6} - LCD_D5 {D5} - LCD_D4 {D4} - LCD_RS {RS} - LCD_EN {E}
#define CN16_PINS	// ROW0_IN {F0} - ROW1_IN {F1} - COL0_IN {C0} - COL1_IN {C1} - COL2_IN {C2}
//#define CN19_PINS	// ~DIG_OUT0 {O0} - ~DIG_OUT1 {O1} - ~DIG_OUT2 {O2}

#define ANALOG_PIN	// ANALOG_POT {INTERNAL}

#define USB0_PINS	// RX1_IN {RX} - TX1_OUT {TX}

#define I2C0_PINS	// I2C0_SCL - I2C0_SDA
//#define I2C1_PINS	// I2C1_SCL - I2C1_SDA

//#define SPI0_PINS	// SPI0_SCK - SPI0_MISO - SPI0_MOSI - SPI0_SSEL0
//#define SPI1_PINS	// SPI1_SCK - SPI1_MISO - SPI1_MOSI - SPI1_SSEL0

#if defined(CN6_PINS) && defined(CN19_PINS)
#error "Macros CN6_PINS and CN19_PINS cannot be defined simultaneously"
#endif

#if defined(CN15_PINS) && defined(I2C0_PINS)
#error "Macros CN15_PINS and I2C0_PINS cannot be defined simultaneously"
#endif

#if defined(CN13_PINS) && defined(I2C1_PINS)
#error "Macros CN13_PINS and I2C1_PINS cannot be defined simultaneously"
#endif

#if defined(CN12_PINS) && defined(SPI0_PINS)
#error "Macros CN12_PINS and SPI0_PINS cannot be defined simultaneously"
#endif

#if defined(USB0_PINS) && defined(SPI0_PINS)
#error "Macros USB0_PINS and SPI0_PINS cannot be defined simultaneously"
#endif

#if defined(CN8_PINS) && defined(SPI1_PINS)
#error "Macros CN8_PINS and SPI1_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_PIN) && defined(SPI1_PINS)
#error "Macros ANALOG_PIN and SPI1_PINS cannot be defined simultaneously"
#endif

#if !defined(CN12_PINS)
#warning "Macro CN12_PINS is not defined and Seven Segment Display module initialization will have no effect"
#endif

#if !defined(CN13_PINS)
#warning "Macro CN13_PINS is not defined and USB initialization will have no effect"
#endif

#if !defined(CN16_PINS)
#warning "Macro CN16_PINS is not defined and Matrix Keyboard module initialization will have no effect"
#endif

#if !defined(CN15_PINS) && !defined(I2C0_PINS)
#warning "Macro CN15_PINS is not defined and LCD1602 module initialization will have no effect"
#endif

#if !defined(I2C0_PINS) && !defined(CN15_PINS)
#warning "Macro I2C0_PINS is not defined and the initialization of the TWI modules will have no effect"
#endif

#ifdef CN5_PINS
extern Gpio SENSOR_IN;
#endif // CN5_PINS

#ifdef CN6_PINS
extern Gpio DIG_OUT0;
extern Gpio DIG_OUT1;
extern Gpio DIG_OUT2;
extern Gpio DIG_OUT3;
extern Gpio DIG_OUT4;

extern Gpio LED_RED;
extern Gpio LED_GREEN;
extern Gpio LED_BLUE;
#endif // CN6_PINS

#ifdef CN7_PINS
extern Gpio ANALOG_OUT;
#endif // CN7_PINS

#ifdef CN8_PINS
extern Gpio ANALOG_IN;
#endif // CN8_PINS

#ifdef CN9_PINS
#ifndef CN10_PINS
#define CN10_PINS
#endif // CN10_PINS
extern Gpio SWDIO;
extern Gpio SWCLK;
extern Gpio DIG_IN;
extern Gpio KEY_RESET;
#endif // CN9_PINS

#ifdef CN10_PINS
extern Gpio INT0_IN;
extern Gpio INT1_IN;

extern Gpio UserSwitch;
#endif // CN10_PINS

#ifdef CN12_PINS
extern Gpio BCDA;
extern Gpio BCDB;
extern Gpio BCDC;
extern Gpio BCDD;
extern Gpio BCD_RST;
extern Gpio BCD_CLK;
#endif // CN12_PINS

#ifdef CN13_PINS
extern Gpio RX0_IN;
extern Gpio TX0_OUT;
extern Gpio EN_OUT;
#endif // CN13_PINS

#ifdef CN15_PINS
extern Gpio LCD_D7;
extern Gpio LCD_D6;
extern Gpio LCD_D5;
extern Gpio LCD_RS;
extern Gpio LCD_D4;
extern Gpio LCD_EN;
#endif // CN15_PINS

#ifdef CN16_PINS
extern Gpio ROW0_OUT;
extern Gpio ROW1_OUT;
extern Gpio COL0_IN;
extern Gpio COL1_IN;
extern Gpio COL2_IN;
#endif // CN16_PINS

#ifdef CN19_PINS
extern Gpio DIG_OUT0;
extern Gpio DIG_OUT1;
extern Gpio DIG_OUT2;
#endif // CN19_PINS

#ifdef ANALOG_PIN
extern Gpio ANALOG_POT;
#endif // ANALOG_PIN

#ifdef USB0_PINS
extern Gpio RX1_IN;
extern Gpio TX1_OUT;
#endif // USB0_PINS

#ifdef I2C0_PINS
extern Gpio I2C0_SCL;
extern Gpio I2C0_SDA;
#endif // I2C0_PINS

#ifdef I2C1_PINS
extern Gpio I2C1_SCL;
extern Gpio I2C1_SDA;
#endif // I2C1_PINS

#ifdef SPI0_PINS
extern Gpio SPI0_SCK;
extern Gpio SPI0_MISO;
extern Gpio SPI0_MOSI;
extern Gpio SPI0_SSEL0;
#endif // SPI0_PINS

#ifdef SPI1_PINS
extern Gpio SPI1_SCK;
extern Gpio SPI1_MISO;
extern Gpio SPI1_MOSI;
extern Gpio SPI1_SSEL0;
#endif // SPI1_PINS

#endif /* PROGRAM_CONFIG_H_ */

