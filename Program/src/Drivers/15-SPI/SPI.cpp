/*!
 * @file SPI.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 24/09/2023 11:00:42
 */

#include "SPI.h"

uint8_t Manufacturer_Id;
uint8_t Device_Type;
uint8_t Device_Id;

SyncCommSPI *g_SPI[2];

SPI::SPI(const Gpio& SCK, const Gpio& MOSI, const Gpio& MISO, const Gpio& SS) : std::vector<Gpio>({SCK, MOSI, MISO, SS}), Callback(),
m_SPI{SPI_CHANNEL},
m_ticks{(uint8_t)(SCK_TICKS * (g_systick_freq / 1000))} {
	this->EnableClock();
	this->EnableSWM();
	this->Config();
}

void SPI::Memory_Inicialization(void) {
	uint8_t Temp[2];
	this->SW_Reset();
	this->HW_Write(COMMAND_DISABLE_QIO);
	this->Write_Enable();
	this->HW_Write(COMMAND_WRITE_UNLOCK_PROTECTION);
	this->Write_Enable();
	Temp[0] = this->Read_Configuration(COMMAND_READ_STATUS);
	Temp[1] = 0x00;
	this->Save_Configuration(COMMAND_WRITE_STATUS, Temp, 2);
	this->Wait_Busy();
	this->Jedec_Id_Read();
	Temp[0] = this->Read_Configuration(COMMAND_READ_CONF);
	this->WriteBlock_Protection();
}

void SPI::CallbackMethod(void) {
	this->m_ticks--;
	if (!this->m_ticks) {
		at(SCK_IDX).TogglePin();
		this->m_ticks = SCK_TICKS * (g_systick_freq / 1000);
	}
}

void SPI::HW_Enable(void) {
	at(SS_IDX).ClearPin();
}

void SPI::HW_Disable(void) {
	at(SS_IDX).SetPin();
}

void SPI::SW_Reset(void) {
	this->HW_Enable();
	this->HW_Write(0x66);
	this->HW_Disable();
	// Bounce
	this->HW_Enable();
	this->HW_Write(0x99);
	this->HW_Disable();
}

void SPI::Write_Enable(void) {
	this->HW_Enable();
	this->HW_Write(COMMAND_ENABLE);
	this->HW_Disable();
}

void SPI::Write_Disable(void) {
	this->HW_Enable();
	this->HW_Write(COMMAND_DISABLE);
	this->HW_Disable();
}

void SPI::HW_Write(uint8_t data) {
	for (uint8_t index = 0; index < 8; index++) {
		if (data & ((uint8_t)(0x80) >> index)) at(MOSI_IDX).SetPin();
		else at(MOSI_IDX).ClearPin();
		at(SCK_IDX).SetPin();
		// Bounce
		at(SCK_IDX).ClearPin();
		// Bounce
	}
}

uint8_t SPI::HW_Read(void) {
	uint8_t data = 0;
	for(uint8_t index = 0; index < 8; index++) {
		at(SCK_IDX).SetPin();
		// Bounce
		if(at(MISO_IDX).GetPin()) // Preguntar a Dani
			data |= ((uint8_t)(0x80) >> index);
		at(SCK_IDX).ClearPin();
		// Bounce
	}
	return data;
}

uint8_t SPI::Read_Configuration(uint8_t command) {
	uint8_t Temp;
	this->HW_Enable();
	this->HW_Write(command);
	Temp = this->HW_Read();
	this->HW_Disable();
	return Temp;
}


void SPI::Jedec_Id_Read(void) {
	this->HW_Enable();
	this->HW_Write(0x9F);
	Manufacturer_Id = this->HW_Read();       /* receive byte */
  	Device_Type = this->HW_Read();           /* receive byte */
  	Device_Id = this->HW_Read();             /* receive byte */
	this->HW_Disable();
}

void SPI::WriteBlock_Protection(void) {
	this->Write_Enable();
	this->HW_Enable();
	this->HW_Write(COMMAND_WRITE_BLOCK_PROTECTION);
	for(uint8_t index = 0; index < 10; index++) this->HW_Write(0);
	this->HW_Disable();
	this->Wait_Busy();
}

void SPI::Wait_Busy(void) {
	while ((this->Read_Configuration(COMMAND_READ_STATUS) & 0x80) == 0x80);	// Waste time until not busy
}

void SPI::EnableInterrupt(void) {
	this->m_SPI->INTENSET = (1 << 0); // Determines whether an interrupt occurs when receiver data is available.
}

void SPI::DisableInterrupt(void) {
	this->m_SPI->INTENCLR = (1 << 0); // Determines whether an interrupt occurs when receiver data is available.
}

void SPI::EnableSWM(void) {
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	if (this->m_SPI == SPI0) {
		SWM->PINASSIGN.PINASSIGN3 &= (((at(SCK_IDX).GetBit() + at(SCK_IDX).GetPort() * 0x20) << 24) | ~(0xFF << 24));
		SWM->PINASSIGN.PINASSIGN4 &= ((((at(MOSI_IDX).GetBit() + at(MOSI_IDX).GetPort() * 0x20) << 0) | ((at(MISO_IDX).GetBit() + at(MISO_IDX).GetPort() * 0x20) << 8) | ((at(SS_IDX).GetBit() + at(SS_IDX).GetPort() * 0x20) << 16)) | ~(0xFFFFFF << 0));
//		THE FOLLOWING FRAGMENT OF CODE IS NOT IMPLEMENT YET
//		SWM->PINASSIGN.PINASSIGN4 &= (((at(SS1_IDX).GetBit() + at(SS1_IDX).GetPort() * 0x20) << 24) | ~(0xFF << 24));
//		SWM->PINASSIGN.PINASSIGN5 &= ((((at(SS2_IDX).GetBit() + at(SS2_IDX).GetPort() * 0x20) << 0) | ((at(SS3_IDX).GetBit() + at(SS3_IDX).GetPort() * 0x20) << 8)) | ~(0xFFFF << 0));
	}
	if (this->m_SPI == SPI1) {
		SWM->PINASSIGN.PINASSIGN5 &= ((((at(SCK_IDX).GetBit() + at(SCK_IDX).GetPort() * 0x20) << 16) | ((at(MOSI_IDX).GetBit() + at(MOSI_IDX).GetPort() * 0x20) << 24)) | ~(0xFFFF << 16));
		SWM->PINASSIGN.PINASSIGN6 &= ((((at(MISO_IDX).GetBit() + at(MISO_IDX).GetPort() * 0x20) << 0) | ((at(SS_IDX).GetBit() + at(SS_IDX).GetPort() * 0x20) << 8)) | ~(0xFFFF << 0));
//		THE FOLLOWING FRAGMENT OF CODE IS NOT IMPLEMENT YET
//		SWM->PINASSIGN.PINASSIGN6 &= (((at(SS1_IDX).GetBit() + at(SS1_IDX).GetPort() * 0x20) << 16) | ~(0xFF << 16));
	}
	SYSCON->SYSAHBCLKCTRL0 &= ~(1 << 7);
}

void SPI::Config(void) {
	if (this->m_SPI == SPI0) NVIC->ISER[0] |= (1 << 0); // Enable SPI0_IRQ
	if (this->m_SPI == SPI1) NVIC->ISER[0] |= (1 << 1); // Enable SPI1_IRQ
	this->m_SPI->CFG |= (1 << 0); // The SPI is enabled for operation.
	this->m_SPI->CFG |= (1 << 2); // The SPI will operate in master mode.
}

void SPI::EnableClock(void) {
//	uint32_t divider, MSTSCL, DIVVAL;
//  uint32_t err, best_err = 0;

	if (this->m_SPI == SPI0) {
		g_SPI[0] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 11);
		SYSCON->PRESETCTRL0 &= ~(1 << 11); // Assert the SPI0 reset.
		SYSCON->PRESETCTRL0 |= (1 << 11); // Clear the SPI0 reset. Default register value.
	}
	if (this->m_SPI == SPI1) {
		g_SPI[1] = this;
		SYSCON->SYSAHBCLKCTRL0 |= (1 << 12);
		SYSCON->PRESETCTRL0 &= ~(1 << 12); // Assert the SPI1 reset.
		SYSCON->PRESETCTRL0 |= (1 << 12); // Clear the SPI1 reset. Default register value.
	}

	/* COMPLETE THE FOLLOWING FRAGMENT OF CODE

    for (uint32_t MSTSCL_IDX = 9; MSTSCL_IDX > 1; MSTSCL_IDX--) {
        divider = FREQ_CLOCK_MCU / (this->m_baudRate * MSTSCL_IDX * 2); // Calculated ideal divider value for given SCL
        divider = (divider > 0x10000) ? 0x10000 : divider; // Adjust it if it is out of range

        err = FREQ_CLOCK_MCU - (this->m_baudRate * MSTSCL_IDX * 2 * divider);
        if ((err < best_err) || !best_err) {
        	DIVVAL = divider;
        	MSTSCL = MSTSCL_IDX;
            best_err = err;
        }
        if (!err || (divider >= 0x10000)) break;
    }

	this->m_TWI->CLKDIV = I2C_CLKDIV_DIVVAL(DIVVAL - 1);
	this->m_TWI->MSTTIME = I2C_MSTTIME_MSTSCLLOW(MSTSCL - 2) | I2C_MSTTIME_MSTSCLHIGH(MSTSCL - 2);
	*/

	// Select Clock PPAL.: FCLKSEL
	if (this->m_SPI == SPI0) SYSCON->FCLKSEL[9] = 1;
	if (this->m_SPI == SPI1) SYSCON->FCLKSEL[10] = 1;
}

void SPI::SPI_IRQHandler(void) {
	////////////////////////////////////////////
	// Add the corresponding fragment of code //
	////////////////////////////////////////////

	// Check if time-out interruption occurred
}

SPI::~SPI() { }

void I2C0_IRQHandler(void) {
	g_SPI[0]->SPI_IRQHandler();
}

void I2C1_IRQHandler(void) {
	g_SPI[1]->SPI_IRQHandler();
}
