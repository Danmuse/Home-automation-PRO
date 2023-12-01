/*/*!
 * @file ProgramConfig.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 23/08/2023 23:40:36
 */

#include "ProgramConfig.h"

#ifdef CN5_PINS
Gpio SENSOR_IN(Gpio::PORT0, 3, Gpio::PULLDOWN, Gpio::INPUT, Gpio::LOW);
#endif // CN5_PINS

#ifdef CN6_PINS
Gpio DIG_OUT0(Gpio::PORT1, 0, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio DIG_OUT1(Gpio::PORT1, 1, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio DIG_OUT2(Gpio::PORT1, 2, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio DIG_OUT3(Gpio::PORT0, 30, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio DIG_OUT4(Gpio::PORT0, 31, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);

Gpio LED_RED(DIG_OUT2);
Gpio LED_GREEN(DIG_OUT0);
Gpio LED_BLUE(DIG_OUT1);
#endif // CN6_PINS

#ifdef CN9_PINS
Gpio SWDIO(Gpio::PORT0, 2, Gpio::PULLUP, Gpio::INPUT, Gpio::LOW);
Gpio SWCLK(Gpio::PORT0, 3, Gpio::PULLUP, Gpio::INPUT, Gpio::LOW);
Gpio DIG_IN(INT0_IN);
Gpio KEY_RESET(Gpio::PORT0, 5, Gpio::INACTIVE, Gpio::INPUT, Gpio::LOW);
#endif // CN9_PINS

#ifdef CN10_PINS
Gpio INT0_IN(Gpio::PORT0, 4, Gpio::REPEATER, Gpio::INPUT, Gpio::LOW);
Gpio INT1_IN(Gpio::PORT0, 3, Gpio::REPEATER, Gpio::INPUT, Gpio::LOW);

Gpio UserSwitch(INT0_IN);
#endif // CN10_PINS

#ifdef CN12_PINS
Gpio BCDA(Gpio::PORT0, 23, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio BCDB(Gpio::PORT0, 22, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio BCDC(Gpio::PORT0, 21, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio BCDD(Gpio::PORT0, 20, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio BCD_RST(Gpio::PORT0, 18, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio BCD_CLK(Gpio::PORT0, 19, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
#endif // CN12_PINS

#ifdef CN13_PINS
Gpio RX0_IN(Gpio::PORT0, 17, Gpio::INACTIVE, Gpio::INPUT, Gpio::LOW);
Gpio TX0_OUT(Gpio::PORT0, 16, Gpio::INACTIVE, Gpio::INPUT, Gpio::LOW);
Gpio EN_OUT(Gpio::PORT0, 0, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
#endif // CN13_PINS

#ifdef CN15_PINS
Gpio LCD_D7(Gpio::PORT0, 13, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio LCD_D6(Gpio::PORT0, 11, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio LCD_D5(Gpio::PORT0, 10, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio LCD_D4(Gpio::PORT0, 9, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio LCD_RS(Gpio::PORT0, 1, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio LCD_EN(Gpio::PORT0, 14, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::HIGH);
#endif // CN15_PINS

#ifdef CN16_PINS
Gpio ROW0_OUT(Gpio::PORT0, 28, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio ROW1_OUT(Gpio::PORT0, 27, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio COL0_IN(Gpio::PORT0, 8, Gpio::PULLUP, Gpio::INPUT, Gpio::LOW);
Gpio COL1_IN(Gpio::PORT0, 15, Gpio::PULLUP, Gpio::INPUT, Gpio::LOW);
Gpio COL2_IN(Gpio::PORT0, 26, Gpio::PULLUP, Gpio::INPUT, Gpio::LOW);
#endif // CN16_PINS

#ifdef CN19_PINS
Gpio DIG_OUT0(Gpio::PORT1, 0, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio DIG_OUT1(Gpio::PORT1, 1, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
Gpio DIG_OUT2(Gpio::PORT1, 2, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::HIGH);
#endif // CN19_PINS

#ifdef USB0_PINS
Gpio RX1_IN(Gpio::PORT0, 24, Gpio::INACTIVE, Gpio::INPUT, Gpio::LOW);
Gpio TX1_OUT(Gpio::PORT0, 25, Gpio::INACTIVE, Gpio::INPUT, Gpio::LOW);
#endif // USB0_PINS

#ifdef I2C0_PINS
Gpio I2C0_SDA(Gpio::PORT0, 11, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::LOW);
Gpio I2C0_SCL(Gpio::PORT0, 10, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::LOW);
#endif // I2C0_PINS

#ifdef I2C1_PINS
Gpio I2C1_SDA(Gpio::PORT0, 17, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::LOW);
Gpio I2C1_SCL(Gpio::PORT0, 16, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::LOW);
#endif // I2C1_PINS

#ifdef SPI0_PINS
Gpio SPI0_SCK(Gpio::PORT0, 20, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio SPI0_MISO(Gpio::PORT0, 21, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio SPI0_MOSI(Gpio::PORT0, 25, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio SPI0_SSEL0(Gpio::PORT0, 24, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
#endif // SPI0_PINS

#ifdef SPI1_PINS
Gpio SPI1_SCK(Gpio::PORT0, 6, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio SPI1_MISO(Gpio::PORT0, 7, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio SPI1_MOSI(Gpio::PORT1, 19, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
Gpio SPI1_SSEL0(Gpio::PORT1, 18, Gpio::PUSHPULL, Gpio::OUTPUT, Gpio::LOW);
#endif // SPI1_PINS
