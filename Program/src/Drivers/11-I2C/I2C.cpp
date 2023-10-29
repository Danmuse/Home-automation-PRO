/*/*!
 * @file I2C.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 19/08/2023 19:27:36
 */

#include "I2C.h"

SyncCommTWI *g_TWI[MAX_I2C_CHANNELS] = { nullptr, nullptr, nullptr, nullptr };

I2C::I2C(const Gpio& SCL, const Gpio& SDA, channel_t channel, frequencyComm_t frequency) : std::vector<Gpio>({SCL, SDA}),
m_TWI{channel < TWI2 ? (I2C_Type *)(I2C0_BASE + channel*I2C_OFFSET_BASE) : (I2C_Type *)(I2C2_BASE + (channel % 2)*I2C_OFFSET_BASE)},
m_frequency{frequency} {
	if (this->m_TWI == I2C0 && g_TWI[0] != nullptr);
	else if (this->m_TWI == I2C1 && g_TWI[1] != nullptr);
	else if (this->m_TWI == I2C2 && g_TWI[2] != nullptr);
	else if (this->m_TWI == I2C3 && g_TWI[3] != nullptr);
	else { // Initialize the corresponding TWI channel.
		this->EnableClock();
		this->EnableSWM();
		this->Config();
	}
}

SyncCommTWI::statusComm_t I2C::awaitACK(void) {
	uint8_t attempts = 0;

	while (!(this->m_TWI->STAT & 1)) if (!(++attempts < MAX_ATTEMPTS_TWI)) break; // Waiting for I2C idle or pending
	if (!(this->m_TWI->CFG & 1) && ((this->m_TWI->STAT & I2C_STAT_SLVNOTSTR_MASK) >> I2C_STAT_SLVNOTSTR_SHIFT)) return STRETCH_FAULT;
	else if ((this->m_TWI->STAT & I2C_STAT_MSTSTATE_NACK_ADDR_MASK) == I2C_STAT_MSTSTATE_NACK_ADDR_MASK) return NACK_ADDR_FAULT;
	else if ((this->m_TWI->STAT & I2C_STAT_MSTSTATE_NACK_DATA_MASK) == I2C_STAT_MSTSTATE_NACK_DATA_MASK) return NACK_DATA_FAULT;
	else if (attempts == MAX_ATTEMPTS_TWI) return TIME_OUT_FAULT;
	else return TWI_SUCCESS;
}

SyncCommTWI::statusComm_t I2C::awaitNACK(void) {
	uint8_t attempts = 0;

	while (!(this->m_TWI->STAT & 1)) if (!(++attempts < MAX_ATTEMPTS_TWI)) break; // Waiting for I2C idle or pending
	if (!(this->m_TWI->CFG & 1) && ((this->m_TWI->STAT & I2C_STAT_SLVNOTSTR_MASK) >> I2C_STAT_SLVNOTSTR_SHIFT)) return STRETCH_FAULT;
	else if ((this->m_TWI->STAT & I2C_STAT_MSTSTATE_NACK_ADDR_MASK) == I2C_STAT_MSTSTATE_NACK_ADDR_MASK) return NACK_ADDR_FAULT;
	else if (attempts == MAX_ATTEMPTS_TWI) return TIME_OUT_FAULT;
	else return TWI_SUCCESS;
}

SyncCommTWI::statusComm_t I2C::prepareConditions(const uint8_t address, const uint8_t regOffset, SyncCommTWI::actionComm_t action) {
	if (action == WRITE) {
		this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
		this->m_TWI->MSTCTL = (1 << 1); // Master Start control. A START bit will be generated on the I2C bus at the next allowed time.
		this->m_TWI->MSTDAT = (address << 1); // Charge the slave address into I2C buffer with WRITE bit command.
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		if (!(this->awaitACK())) {
			this->m_TWI->MSTDAT = regOffset; // Charge the data into I2C buffer.
			this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		} else return TWI_FAILURE;
		return TWI_SUCCESS;
	} else if (action == WRITE_OFFSET_NONE) {
		this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
		this->m_TWI->MSTCTL = (1 << 1); // Master Start control. A START bit will be generated on the I2C bus at the next allowed time.
		this->m_TWI->MSTDAT = (address << 1); // Charge the slave address into I2C buffer with WRITE bit command.
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		return TWI_SUCCESS;
	} else if (action == READ) {
		this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
		this->m_TWI->MSTCTL = (1 << 1); // Master Start control. A START bit will be generated on the I2C bus at the next allowed time.
		this->m_TWI->MSTDAT = (address << 1); // Charge the slave address into I2C buffer with WRITE bit command.
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		if (!(this->awaitACK())) {
			this->m_TWI->MSTDAT = regOffset; // Charge the data into I2C buffer.
			this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		} else return TWI_FAILURE;
		if (!(this->awaitACK())) {
			this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
			this->m_TWI->MSTCTL = (1 << 1); // Master Start control. A START bit will be generated on the I2C bus at the next allowed time.
			this->m_TWI->MSTDAT = ((address << 1) | 1); // Charge the slave address into I2C buffer with READ bit command.
			this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		} else return TWI_FAILURE;
		return TWI_SUCCESS;
	} else if (action == READ_OFFSET_NONE) {
		this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
		this->m_TWI->MSTCTL = (1 << 1); // Master Start control. A START bit will be generated on the I2C bus at the next allowed time.
		this->m_TWI->MSTDAT = (address << 1); // Charge the slave address into I2C buffer with WRITE bit command.
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		if (!(this->awaitACK())) {
			this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
			this->m_TWI->MSTCTL = (1 << 1); // Master Start control. A START bit will be generated on the I2C bus at the next allowed time.
			this->m_TWI->MSTDAT = ((address << 1) | 1); // Charge the slave address into I2C buffer with READ bit command.
			this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
		} else return TWI_FAILURE;
		return TWI_SUCCESS;
	} else return TWI_FAILURE;
}

SyncCommTWI::statusComm_t I2C::transmitStopBit(void) {
	if (!(this->awaitACK())) {
		this->m_TWI->MSTCTL = (1 << 2); // Master Stop control. A STOP will be generated on the I2C bus at the next allowed time.
		this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
	} else return TWI_FAILURE;
	return TWI_SUCCESS;
}

SyncCommTWI::statusComm_t I2C::transmitByte(const uint8_t address, const uint8_t regOffset, uint8_t value, SyncCommTWI::actionComm_t action) {
	prepareConditions(address, regOffset, action);

	if (!(this->awaitACK())) {
		this->m_TWI->MSTDAT = value; // Charge the data into I2C buffer
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
	} else return TWI_FAILURE;

	if (this->transmitStopBit()) return TWI_FAILURE;
	return TWI_SUCCESS;
}

SyncCommTWI::statusComm_t I2C::receiveByte(const uint8_t address, const uint8_t regOffset, uint8_t* value, SyncCommTWI::actionComm_t action) {
	prepareConditions(address, regOffset, action);

	if (!(this->awaitACK())) {
		*value = this->m_TWI->MSTDAT; // Charge the data into the variable
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
	} else return TWI_FAILURE;

	if (!(this->awaitNACK())) {
		this->m_TWI->MSTCTL = (1 << 2); // Master Stop control. A STOP will be generated on the I2C bus at the next allowed time.
		this->m_TWI->STAT &= ~(1 << 0); // MSTPENDING flag bit would be cleared at the same time as setting either the MSTSTOP or MSTSTART control bit.
	} else return TWI_FAILURE;
	return TWI_SUCCESS;
}

SyncCommTWI::statusComm_t I2C::transmitBytes(const uint8_t address, const uint8_t regOffset, uint8_t values[], size_t numBytes, SyncCommTWI::actionComm_t action) {
	prepareConditions(address, regOffset, action);

	/*
	if (!(this->awaitACK())) {
		this->m_TWI->MSTDAT = value; // Charge the data into I2C buffer
		this->m_TWI->MSTCTL = (1 << 0); // Master Continue control. Informs the Master function to continue to the next operation.
	} else return TWI_FAILURE;
	*/

	if (this->transmitStopBit()) return TWI_FAILURE;

	return TWI_SUCCESS;
}

SyncCommTWI::statusComm_t I2C::receiveBytes(const uint8_t address, const uint8_t regOffset, uint8_t* values[], size_t numBytes, SyncCommTWI::actionComm_t action) {
	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	return TWI_SUCCESS;
}

void I2C::EnableInterrupt(void) {
	this->m_TWI->INTENSET = (1 << 0); // Master Pending interrupt Enable.
//	this->m_TWI->INTENSET = (1 << 25); // Enable SCL time-out interruption.
}

void I2C::DisableInterrupt(void) {
	this->m_TWI->INTENCLR = (1 << 0); // Master Pending interrupt Disable.
//	this->m_TWI->INTENCLR = (1 << 25); // Disable SCL time-out interruption.
}

void I2C::EnableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	// I2C0_SCL enabled on pin PIO0_10 and I2C0_SDA enabled on pin PIO0_11
	if (this->m_TWI == I2C0) SWM->PINENABLE0 &= ~((1 << 13) | (1 << 12));
	if (this->m_TWI == I2C1) SWM->PINASSIGN.PINASSIGN9 &= ((((at(SDA_IDX).GetBit() + at(SDA_IDX).GetPort() * 0x20) << 16) | ((at(SCL_IDX).GetBit() + at(SCL_IDX).GetPort() * 0x20) << 24)) | ~(0xFFFF << 16));
	if (this->m_TWI == I2C2) SWM->PINASSIGN.PINASSIGN10 &= ((((at(SDA_IDX).GetBit() + at(SDA_IDX).GetPort() * 0x20) << 0) | ((at(SCL_IDX).GetBit() + at(SCL_IDX).GetPort() * 0x20) << 8)) | ~(0xFFFF << 0));
	if (this->m_TWI == I2C3) SWM->PINASSIGN.PINASSIGN10 &= ((((at(SDA_IDX).GetBit() + at(SDA_IDX).GetPort() * 0x20) << 16) | ((at(SCL_IDX).GetBit() + at(SCL_IDX).GetPort() * 0x20) << 24)) | ~(0xFFFF << 16));
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void I2C::Config(void) {
	if (this->m_TWI == I2C0) NVIC->ISER[0] |= (1 << 8); // Enable I2C0_IRQ
	if (this->m_TWI == I2C1) NVIC->ISER[0] |= (1 << 7); // Enable I2C1_IRQ
	if (this->m_TWI == I2C2) NVIC->ISER[0] |= (1 << 21); // Enable I2C2_IRQ
	if (this->m_TWI == I2C3) NVIC->ISER[0] |= (1 << 22); // Enable I2C3_IRQ
	this->m_TWI->CFG |= (1 << 0); // I2C is enabled as MASTER device
//	this->EnableInterrupt();
}

void I2C::EnableClock(void) {
	uint32_t divider, MSTSCL, DIVVAL;
    uint32_t err, best_err = 0;

	if (this->m_TWI == I2C0) {
		g_TWI[0] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 5);
		SYSCON->PRESETCTRL0 &= ~(1 << 5); // Assert the I2C0 reset.
		SYSCON->PRESETCTRL0 |= (1 << 5); // Clear the I2C0 reset. Default register value.
		if (!(this->m_frequency == FAST_FREQUENCY)) {
		// Standard GPIO functionality. Requires external pull-up for GPIO output function.
		// IOCON->PIO[IOCON_INDEX_PIO0[10]] = 0x80; // PIO0_10/I2C0_SCL. This is the pin configuration for the true open-drain pin.
		// IOCON->PIO[IOCON_INDEX_PIO0[11]] = 0x80; // PIO0_11/I2C0_SDA. This is the pin configuration for the true open-drain pin.
		} else {
		// Controls the I2C-bus up to Fast-mode Plus (up to 1 MHz).
			IOCON->PIO[IOCON_INDEX_PIO0[10]] = 0x100; // PIO0_10/I2C0_SCL. This is the pin configuration for the true open-drain pin.
			IOCON->PIO[IOCON_INDEX_PIO0[11]] = 0x100; // PIO0_11/I2C0_SDA. This is the pin configuration for the true open-drain pin.
		}
	}
	if (this->m_TWI == I2C1) {
		g_TWI[1] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 21);
		SYSCON->PRESETCTRL0 &= ~(1 << 21); // Assert the I2C1 reset.
		SYSCON->PRESETCTRL0 |= (1 << 21); // Clear the I2C1 reset. Default register value.
	}
	if (this->m_TWI == I2C2) {
		g_TWI[2] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 22);
		SYSCON->PRESETCTRL0 &= ~(1 << 22); // Assert the I2C2 reset.
		SYSCON->PRESETCTRL0 |= (1 << 22); // Clear the I2C2 reset. Default register value.
	}
	if (this->m_TWI == I2C3) {
		g_TWI[3] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);
		SYSCON->PRESETCTRL0 &= ~(1 << 23); // Assert the I2C3 reset.
		SYSCON->PRESETCTRL0 |= (1 << 23); // Clear the I2C3 reset. Default register value.
	}

    for (uint32_t MSTSCL_IDX = 9; MSTSCL_IDX > 1; MSTSCL_IDX--) {
        divider = FREQ_CLOCK_MCU / (this->m_frequency * MSTSCL_IDX * 2); // Calculated ideal divider value for given SCL
        divider = (divider > 0x10000) ? 0x10000 : divider; // Adjust it if it is out of range

        err = FREQ_CLOCK_MCU - (this->m_frequency * MSTSCL_IDX * 2 * divider);
        if ((err < best_err) || !best_err) {
        	DIVVAL = divider;
        	MSTSCL = MSTSCL_IDX;
            best_err = err;
        }
        if (!err || (divider >= 0x10000)) break;
    }

    // --- CONFIGURATION FOR 400 kHz
    // DIVVAL: PCLK is divided by 3 before use by the I2C function.
	this->m_TWI->CLKDIV = I2C_CLKDIV_DIVVAL(DIVVAL - 1);
	// --- CONFIGURATION FOR 400 kHz
	// MSTSCLLOW: 5 clocks. Minimum SCL low time is 5 clocks of the I2C clock pre-divider.
	// MSTSCLHIGH: 5 clocks. Minimum SCL high time is 5 clock of the I2C clock pre-divider.
	this->m_TWI->MSTTIME = (I2C_MSTTIME_MSTSCLLOW(MSTSCL - 2) | I2C_MSTTIME_MSTSCLHIGH(MSTSCL - 2));

	//////////////////////////////////////////
	// Check the following fragment of code //
	//////////////////////////////////////////
	// There's an abnormally performance caused by communication channels other than I2C0
	// Select Clock PPAL.: FCLKSEL
	if (this->m_TWI == I2C0) SYSCON->FCLKSEL[5] = 1;
	if (this->m_TWI == I2C1) SYSCON->FCLKSEL[6] = 1;
	if (this->m_TWI == I2C2) SYSCON->FCLKSEL[7] = 1;
	if (this->m_TWI == I2C3) SYSCON->FCLKSEL[8] = 1;
}

void I2C::I2C_IRQHandler(void) {
	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	// VERY IMPORTANT NOTES:
	// --- Refactor the code to use interrupts and refrain from sequential flow.
	// --- Use Master DMA. Data operations of the I2C can be performed with DMA.
	// Protocol type operations such as Start, address, Stop, and address match must always be
	// done with software, typically via an interrupt. When a DMA data transfer is complete,
	// MSTDMA must be cleared prior to beginning the next operation, typically a Start or Stop.
	// --- When Master DMA is being used and a transfer completes, the MSTPENDING flag
	// in the STAT register shouldn't be written after the last bus operation.

	// Checks if Master Pending interruption occurred.
	if (this->m_TWI->STAT & (1 << 0)) {
		// ...
	}

	// Checks if SCL time-out interruption occurred.
	if (this->m_TWI->STAT & (1 << 25)) {
		// ...
	}
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
