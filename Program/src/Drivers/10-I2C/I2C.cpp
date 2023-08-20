/*/*!
 * @file I2C.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 */

#include "I2C.h"

SyncComm *g_TWI[4];

I2C::I2C() :
m_SCL{Gpio(PORT_SCL, PIN_SCL, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::HIGH)},
m_SDA{Gpio(PORT_SDA, PIN_SDA, Gpio::OPENCOLECTOR, Gpio::OUTPUT, Gpio::HIGH)},
m_TWI{TWI_I2C} {
	//////////////////////////////////////////
	// Check the following fragment of code //
	//////////////////////////////////////////

	IOCON->PIO[7] = 0x80;
	IOCON->PIO[8] = 0x80;
	SYSCON->PRESETCTRL0 &= ~(1 << 5);
	SYSCON->PRESETCTRL0 |= (1 << 5);

	this->EnableClock();
	this->EnableSWM();
	this->Config();
}

void I2C::SendData(uint32_t address, const uint8_t value) {
	//////////////////////////////////////////
	// Check the following fragment of code //
	//////////////////////////////////////////

	while (!(this->m_TWI->STAT & 1));		// Waiting for I2C idle or pending
	this->m_TWI->MSTDAT = (address << 1);	// Send address with __IO bit = 0
	this->m_TWI->MSTCTL = (1 << 1);			// Send START bit
	while (!(this->m_TWI->STAT & 1));		// Waiting for I2C idle or pending
	this->m_TWI->MSTDAT = value;			// Charge the data into I2C buffer
	while (!(this->m_TWI->STAT & 1));		// Waiting for I2C idle or pending
	this->m_TWI->MSTCTL = (1 << 2);			// Send STOP bit

	if (this->m_statusComm == SUCCESS) this->EnableInterupt();
}

void I2C::ReadData(uint32_t address, const uint8_t *value) {
	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	if (this->m_statusComm == SUCCESS) this->EnableInterupt();
}

void I2C::EnableInterupt(void) {
	this->m_TWI->INTENSET = (1 << 25); // Enable SCL time-out interruption.
}

void I2C::DisableInterupt(void) {
	this->m_TWI->INTENCLR = (1 << 25); // Disable SCL time-out interruption.
}

void I2C::EnableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	// I2C0_SCL enabled on pin PIO0_10 and I2C0_SDA enabled on pin PIO0_11
	if (this->m_TWI == I2C0) SWM->PINENABLE0 |= ((1 << 13) | (1 << 12));
	if (this->m_TWI == I2C1) SWM->PINASSIGN.PINASSIGN9 = ((this->m_SDA.m_bit + this->m_SDA.m_port * 0x20) << 16) | ((this->m_SCL.m_bit + this->m_SCL.m_port * 0x20) << 24);
	if (this->m_TWI == I2C2) SWM->PINASSIGN.PINASSIGN10 = ((this->m_SDA.m_bit + this->m_SDA.m_port * 0x20) << 0) | ((this->m_SCL.m_bit + this->m_SCL.m_port * 0x20) << 8);
	if (this->m_TWI == I2C3) SWM->PINASSIGN.PINASSIGN10 = ((this->m_SDA.m_bit + this->m_SDA.m_port * 0x20) << 16) | ((this->m_SCL.m_bit + this->m_SCL.m_port * 0x20) << 24);
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void I2C::Config(void) {
	//////////////////////////////////////////
	// Check the following fragment of code //
	//////////////////////////////////////////

	uint8_t ISER = 0; // Interrupt Set Enable Register

	if (this->m_TWI == I2C0) ISER = 8;
	if (this->m_TWI == I2C1) ISER = 7;
	if (this->m_TWI == I2C2) ISER = 21;
	if (this->m_TWI == I2C3) ISER = 22;

	NVIC->ISER[0] = (1 << ISER); // Enable I2C_IRQ
	this->m_TWI->CFG |= (1 << 0); // I2C0 is enabled as MASTER device
}

void I2C::EnableClock(void) {
	if (this->m_TWI == I2C0) {
		g_TWI[0] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 5);
	}
	if (this->m_TWI == I2C1) {
		g_TWI[1] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 21);
	}
	if (this->m_TWI == I2C2) {
		g_TWI[2] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 22);
	}
	if (this->m_TWI == I2C3) {
		g_TWI[3] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);
	}

	this->m_TWI->MSTTIME = 0;
	this->m_TWI->CLKDIV = 500;
	// Select Clock PPAL.: FCLKSEL
	if (this->m_TWI == I2C0) SYSCON->FCLKSEL[5] = 1;
	if (this->m_TWI == I2C1) SYSCON->FCLKSEL[6] = 1;
	if (this->m_TWI == I2C2) SYSCON->FCLKSEL[7] = 1;
	if (this->m_TWI == I2C3) SYSCON->FCLKSEL[8] = 1;
}

void I2C::I2C_IRQHandler(void) {
//	uint8_t endTransmission, data;
//	uint32_t stat = this->m_TWI->INTSTAT;

	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////
}

I2C::~I2C() { }

void I2C0_IRQHandler(void) {
	g_TWI[0]->I2C_IRQHandler();
}

void I2C1_IRQHandler(void) {
	g_TWI[1]->I2C_IRQHandler();
}

void I2C2_IRQHandler(void) {
	g_TWI[2]->I2C_IRQHandler();
}

void I2C3_IRQHandler(void) {
	g_TWI[3]->I2C_IRQHandler();
}
