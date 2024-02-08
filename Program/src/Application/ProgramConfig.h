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

//////////////////////////////////////////////
/// Hardware definitions (Infotronic 2023) ///
//////////////////////////////////////////////

#define CN5_PINS	// SENSOR_IN {Sensor}
#define CN6_PINS	// DIG_OUT0 {O0} - DIG_OUT1 {O1} - DIG_OUT2 {O2} - DIG_OUT3 {A0} - DIG_OUT4 {A1}
//#define CN7_PINS	// ANALOG_OUT ~ DAC1_PIN {AOUT}
#define CN8_PINS	// ANALOG_IN ~ ANALOG1_PIN {AIN}
//#define CN9_PINS	// SWDIO [PIN 2] - SWCLK [PIN 3] - DIG_IN [PIN 4] - KEY_RESET [PIN 5]
#define CN10_PINS	// INT0_IN {IN0} - INT1_IN {IN1}
//#define CN12_PINS	// BCDA {BCDA} - BCDB {BCDB} - BCDC {BCDC} - BCDD {BCDD} - BCD_RST {RST} - BCD_CLK {CK}
#define CN13_PINS	// RX0_IN {RX} - TX0_OUT {TX} - EN_OUT {EN}
//#define CN15_PINS	// LCD_D7 {D7} - LCD_D6 {D6} - LCD_D5 {D5} - LCD_D4 {D4} - LCD_RS {RS} - LCD_EN {E}
//#define CN16_PINS	// ROW0_IN {F0} - ROW1_IN {F1} - COL0_IN {C0} - COL1_IN {C1} - COL2_IN {C2}
//#define CN19_PINS	// ~DIG_OUT0 {O0} - ~DIG_OUT1 {O1} - ~DIG_OUT2 {O2}

#define KEYBOARD_DEBUG_PINS		// ROW0_IN {P0.28} - ROW1_IN {P0.27} - COL0_IN {P0.08}

#define LED_TRIP_PIN	// LED_TRIP_PIN {P0.29}

#define MG90S_SERVO_PIN	// MG90S_SERVO_PIN {P0.23}

#define USB0_PINS		// RX1_IN {RX} - TX1_OUT {TX}

#define LCD_DEBUG_PINS	// LCD_D7 {P0.13} - LCD_D6 {P0.15} - LCD_D5 {P0.26} - LCD_D4 {P0.09} - LCD_RS {P0.01} - LCD_EN {P0.14}

#define I2C0_PINS	// I2C0_SCL - I2C0_SDA
//#define I2C1_PINS	// I2C1_SCL - I2C1_SDA
//#define I2C2_PINS	// I2C2_SCL - I2C2_SDA
//#define I2C3_PINS	// I2C3_SCL - I2C3_SDA

//#define SPI0_PINS	// SPI0_SCK - SPI0_MOSI - SPI0_MISO - SPI0_SSEL0
//#define SPI1_PINS	// SPI1_SCK - SPI1_MOSI - SPI1_MISO - SPI1_SSEL0

//#define SPI0_DEBUG_PINS	// SPI0_DEBUG_SCK - SPI0_DEBUG_MOSI - SPI0_DEBUG_MISO - SPI0_DEBUG_SSEL0 - SPI0_DEBUG_SSEL1
#define SPI1_DEBUG_PINS	// SPI1_DEBUG_SCK - SPI1_DEBUG_MOSI - SPI1_DEBUG_MISO - SPI1_DEBUG_SSEL0 - SPI1_DEBUG_SSEL1

#define ANALOG_FST_CHANNEL_ENABLED	// POTENCIOMETER - ANALOG1_PIN {P0.07}
#define ANALOG_SND_CHANNEL_ENABLED	// ANALOG1_PIN {P0.06}
#define ANALOG_TRD_CHANNEL_ENABLED	// ANALOG2_PIN {P0.14}
//#define ANALOG_FOU_CHANNEL_ENABLED	// ANALOG3_PIN {P0.23}
//#define ANALOG_FIF_CHANNEL_ENABLED	// ANALOG4_PIN {P0.22}
//#define ANALOG_SIX_CHANNEL_ENABLED	// ANALOG5_PIN {P0.21}
//#define ANALOG_SEV_CHANNEL_ENABLED	// ANALOG6_PIN {P0.20}
//#define ANALOG_EGH_CHANNEL_ENABLED	// ANALOG7_PIN {P0.19}
//#define ANALOG_NTH_CHANNEL_ENABLED	// ANALOG8_PIN {P0.18}
//#define ANALOG_TTH_CHANNEL_ENABLED	// ANALOG9_PIN {P0.17}
//#define ANALOG_ELE_CHANNEL_ENABLED	// ANALOG10_PIN {P0.13}
//#define ANALOG_TWE_CHANNEL_ENABLED	// ANALOG11_PIN {P0.04}

//#define DAC_FST_CHANNEL_ENABLED 	// DAC0_PIN {P0.17}
//#define DAC_SND_CHANNEL_ENABLED	// DAC1_PIN {P0.29}

/////////////////////////////////////
/// Errors during the compilation ///
/////////////////////////////////////

#if defined(CN6_PINS) && defined(CN19_PINS)
#error "Macros CN6_PINS and CN19_PINS cannot be defined simultaneously"
#endif

#if defined(CN9_PINS) && defined(I2C3_PINS)
#error "Macros CN9_PINS and I2C3_PINS cannot be defined simultaneously"
#endif

#if defined(CN9_PINS) && defined(I2C2_PINS)
#error "Macros CN9_PINS and I2C2_PINS cannot be defined simultaneously"
#endif

#if defined(CN5_PINS) && defined(I2C2_PINS)
#error "Macros CN5_PINS and I2C2_PINS cannot be defined simultaneously"
#endif

#if defined(CN9_PINS) && defined(CN5_PINS)
#error "Macros CN9_PINS and CN5_PINS cannot be defined simultaneously"
#endif

#if defined(CN15_PINS) && defined(I2C0_PINS)
#error "Macros CN15_PINS and I2C0_PINS cannot be defined simultaneously"
#endif

#if defined(CN15_PINS) && defined(LCD_DEBUG_PINS)
#error "Macros CN15_PINS and LCD_DEBUG_PINS cannot be defined simultaneously"
#endif

#if defined(CN16_PINS) && defined(KEYBOARD_DEBUG_PINS)
#error "Macros CN16_PINS and KEYBOARD_DEBUG_PINS cannot be defined simultaneously"
#endif

#if defined(CN16_PINS) && defined(LCD_DEBUG_PINS)
#error "Macros CN16_PINS and LCD_DEBUG_PINS cannot be defined simultaneously"
#endif

#if defined(SPI0_DEBUG_PINS) && defined(I2C0_PINS)
#error "Macros SPI0_DEBUG_PINS and I2C0_PINS cannot be defined simultaneously"
#endif

#if defined(CN13_PINS) && defined(I2C1_PINS)
#error "Macros CN13_PINS and I2C1_PINS cannot be defined simultaneously"
#endif

#if defined(CN13_PINS) && defined(DAC_FST_CHANNEL_ENABLED)
#error "Macros CN13_PINS and DAC_FST_CHANNEL_ENABLED cannot be defined simultaneously"
#endif

#if defined(I2C1_PINS) && defined(DAC_FST_CHANNEL_ENABLED)
#error "Macros I2C1_PINS and DAC_FST_CHANNEL_ENABLED cannot be defined simultaneously"
#endif

#if defined(LED_TRIP_PIN) && (defined(DAC_SND_CHANNEL_ENABLED) || defined(CN7_PINS))
#error "Macros LED_TRIP_PIN and DAC_SND_CHANNEL_ENABLED or CN7_PINS cannot be defined simultaneously"
#endif

#if defined(CN12_PINS) && defined(SPI0_PINS)
#error "Macros CN12_PINS and SPI0_PINS cannot be defined simultaneously"
#endif

#if defined(USB0_PINS) && defined(SPI0_PINS)
#error "Macros USB0_PINS and SPI0_PINS cannot be defined simultaneously"
#endif

#if defined(MG90S_SERVO_PIN) && defined(CN12_PINS)
#error "Macros MG90S_SERVO_PIN and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(SPI1_DEBUG_PINS) && defined(CN12_PINS)
#error "Macros SPI1_DEBUG_PINS and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_FST_CHANNEL_ENABLED) && defined(SPI1_PINS)
#error "Macros ANALOG_FST_CHANNEL_ENABLED and SPI1_PINS cannot be defined simultaneously"
#endif

#if (defined(CN8_PINS) && defined(ANALOG_SND_CHANNEL_ENABLED)) && defined(SPI1_PINS)
#error "Macros CN8_PINS, ANALOG_SND_CHANNEL_ENABLED and SPI1_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_FOU_CHANNEL_ENABLED) && defined(CN12_PINS)
#error "Macros ANALOG_FOU_CHANNEL_ENABLED and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_FIF_CHANNEL_ENABLED) && defined(CN12_PINS)
#error "Macros ANALOG_FIF_CHANNEL_ENABLED and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_SIX_CHANNEL_ENABLED) && defined(CN12_PINS)
#error "Macros ANALOG_SIX_CHANNEL_ENABLED and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_SEV_CHANNEL_ENABLED) && defined(CN12_PINS)
#error "Macros ANALOG_SEV_CHANNEL_ENABLED and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_EGH_CHANNEL_ENABLED) && defined(CN12_PINS)
#error "Macros ANALOG_EGH_CHANNEL_ENABLED and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_NTH_CHANNEL_ENABLED) && defined(CN12_PINS)
#error "Macros ANALOG_NTH_CHANNEL_ENABLED and CN12_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_TTH_CHANNEL_ENABLED) && defined(CN13_PINS)
#error "Macros ANALOG_TTH_CHANNEL_ENABLED and CN13_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_ELE_CHANNEL_ENABLED) && defined(CN15_PINS)
#error "Macros ANALOG_ELE_CHANNEL_ENABLED and CN15_PINS cannot be defined simultaneously"
#endif

#if defined(SPI0_DEBUG_PINS) && defined(CN15_PINS)
#error "Macros SPI0_DEBUG_PINS and CN15_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_ELE_CHANNEL_ENABLED) && defined(SPI0_DEBUG_PINS)
#error "Macros ANALOG_ELE_CHANNEL_ENABLED and SPI0_DEBUG_PINS cannot be defined simultaneously"
#endif

#if defined(ANALOG_TWE_CHANNEL_ENABLED) && defined(CN10_PINS)
#error "Macros ANALOG_TWE_CHANNEL_ENABLED and CN10_PINS cannot be defined simultaneously"
#endif

///////////////////////////////////////
/// Warnings during the compilation ///
///////////////////////////////////////

#if !defined(CN12_PINS) && !defined(MG90S_SERVO_PIN) && !defined(SPI1_DEBUG_PINS)
#warning "Macro CN12_PINS is not defined and Seven Segment Display module initialization will have no effect"
#endif

#if !defined(MG90S_SERVO_PIN) && !defined(CN12_PINS) && !defined(SPI1_DEBUG_PINS)
#warning "Macro MG90S_SERVO_PIN is not defined and Servo initialization will have no effect"
#endif

#if !defined(SPI1_DEBUG_PINS) && !defined(MG90S_SERVO_PIN) && !defined(CN12_PINS)
#warning "Macro SPI1_DEBUG_PINS is not defined and the initialization of the SPI modules will have no effect"
#endif

#if !defined(CN13_PINS) && !defined(DAC_FST_CHANNEL_ENABLED)
#warning "Macro CN13_PINS is not defined and USB initialization will have no effect"
#endif

#if !defined(DAC_FST_CHANNEL_ENABLED) && !defined(CN13_PINS)
#warning "Macro DAC_FST_CHANNEL_ENABLED is not defined and DAC pin will have no effect"
#endif

#if !defined(CN16_PINS) && !defined(KEYBOARD_DEBUG_PINS) && !defined(LCD_DEBUG_PINS)
#warning "Macro CN16_PINS or KEYBOARD_DEBUG_PINS are not defined and Matrix Keyboard module initialization will have no effect"
#endif

#if !(defined(CN8_PINS) && defined(ANALOG_SND_CHANNEL_ENABLED))
#warning "Macros CN8_PINS and ANALOG_SND_CHANNEL_ENABLED are not defined and external ADC initialization will have no effect"
#endif

#if !defined(ANALOG_FST_CHANNEL_ENABLED)
#warning "Macro ANALOG_FST_CHANNEL_ENABLED is not defined and internal preset initialization will have no effect"
#endif

#if !defined(LED_TRIP_PIN) && !(defined(CN7_PINS) && defined(DAC_SND_CHANNEL_ENABLED))
#warning "Macros LED_TRIP_PIN and CN7_PINS or DAC_SND_CHANNEL_ENABLED are not defined and external RGB LEDs initialization will have no effect"
#endif

#if !(defined(CN7_PINS) && defined(DAC_SND_CHANNEL_ENABLED)) && !defined(LED_TRIP_PIN)
#warning "Macros CN7_PINS or DAC_SND_CHANNEL_ENABLED or LED_TRIP_PIN are not defined and external DAC initialization will have no effect"
#endif

#if !defined(I2C3_PINS) && !defined(CN13_PINS) && !defined(CN9_PINS)
#warning "Macro I2C3_PINS is not defined and the initialization of the TWI modules will have no effect"
#endif

#if !defined(I2C2_PINS) && !defined(CN5_PINS) && !defined(CN9_PINS)
#warning "Macro I2C2_PINS is not defined and the initialization of the TWI modules will have no effect"
#endif

#if !defined(CN15_PINS) && !defined(I2C0_PINS) && !defined(SPI0_DEBUG_PINS)
#warning "Macro CN15_PINS is not defined and LCD1602 or LCD2004 module initialization will have no effect"
#endif

#if !defined(I2C0_PINS) && !defined(CN15_PINS) && !defined(SPI0_DEBUG_PINS)
#warning "Macro I2C0_PINS is not defined and the initialization of the TWI modules will have no effect"
#endif

#if !defined(SPI0_DEBUG_PINS) && !defined(I2C0_PINS) && !defined(CN15_PINS)
#warning "Macro SPI0_DEBUG_PINS is not defined and the initialization of the SPI modules will have no effect"
#endif

//////////////////////////////////////////////////
/// Hardware initializations (Infotronic 2023) ///
//////////////////////////////////////////////////

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

#ifdef KEYBOARD_DEBUG_PINS
extern Gpio ROW0_DEBUG_OUT;
extern Gpio ROW1_DEBUG_OUT;
extern Gpio COL0_DEBUG_IN;
#endif // KEYBOARD_DEBUG_PINS

#ifdef LED_TRIP_PIN
extern Gpio LED_TRIP;
#endif // LED_TRIP_PIN

#ifdef MG90S_SERVO_PIN
extern Gpio MG90S_SERVO;
#endif // MG90S_SERVO_PIN

#ifdef USB0_PINS
extern Gpio RX1_IN;
extern Gpio TX1_OUT;
#endif // USB0_PINS

#ifdef LCD_DEBUG_PINS
extern Gpio LCD_DEBUG_D7;
extern Gpio LCD_DEBUG_D6;
extern Gpio LCD_DEBUG_D5;
extern Gpio LCD_DEBUG_D4;
extern Gpio LCD_DEBUG_RS;
extern Gpio LCD_DEBUG_EN;
#endif // LCD_DEBUG_PINS

#ifdef I2C0_PINS
extern Gpio I2C0_SCL;
extern Gpio I2C0_SDA;
#endif // I2C0_PINS

#ifdef I2C1_PINS
extern Gpio I2C1_SCL;
extern Gpio I2C1_SDA;
#endif // I2C1_PINS

#ifdef I2C2_PINS
extern Gpio I2C2_SCL;
extern Gpio I2C2_SDA;
#endif // I2C2_PINS

#ifdef I2C3_PINS
extern Gpio I2C3_SCL;
extern Gpio I2C3_SDA;
#endif // I2C3_PINS

#ifdef SPI0_PINS
extern Gpio SPI0_SCK;
extern Gpio SPI0_MOSI;
extern Gpio SPI0_MISO;
extern Gpio SPI0_SSEL0;
#endif // SPI0_PINS

#ifdef SPI1_PINS
extern Gpio SPI1_SCK;
extern Gpio SPI1_MOSI;
extern Gpio SPI1_MISO;
extern Gpio SPI1_SSEL0;
#endif // SPI1_PINS

#ifdef SPI0_DEBUG_PINS
extern Gpio SPI0_DEBUG_SCK;
extern Gpio SPI0_DEBUG_MOSI;
extern Gpio SPI0_DEBUG_MISO;
extern Gpio SPI0_DEBUG_SSEL0;
extern Gpio SPI0_DEBUG_SSEL1;
#endif // SPI0_DEBUG_PINS

#ifdef SPI1_DEBUG_PINS
extern Gpio SPI1_DEBUG_SCK;
extern Gpio SPI1_DEBUG_MOSI;
extern Gpio SPI1_DEBUG_MISO;
extern Gpio SPI1_DEBUG_SSEL0;
extern Gpio SPI1_DEBUG_SSEL1;
#endif // SPI1_DEBUG_PINS

#endif /* PROGRAM_CONFIG_H_ */

