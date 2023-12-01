/*/*!
 * @file PLL.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/06/2023 23:14:24
 */

#include "PLL.h"

void initPhaseLockedLoop(void) {
	SYSCON->SYSAHBCLKDIV = 0x1;      // Set clock divider for core to 1
	SYSCON->MAINCLKSEL &= ~(0x03);   // Set "main clock" to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)

	SYSCON->MAINCLKUEN &= ~(0x1);    // Write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
	SYSCON->MAINCLKUEN |= 0x1;       // Write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update

	SYSCON->SYSPLLCLKSEL = 0;      	 // Connect IRC oscillator to SYSTEM PLL (sec. 3.5.9)

	SYSCON->SYSPLLCLKUEN &= ~(0x1);  // Write a zero to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
	SYSCON->SYSPLLCLKUEN |= 0x1;     // Write a one to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update

	SYSCON->PDRUNCFG |= (1 << 7);    // Power down the PLL before changing divider values (sec 3.5.35)
	SYSCON->SYSPLLCTRL = 0x23;       // P_result = 2 * 1 and M_result = 3 + 1

	// FCLKOUT = M_result * FCLKIN = FCCO / (2 * P_result), FCLKIN = 12 MHz
	// If SYSCON->SYSPLLCTRL = 0x23 then FCLKOUT = 48 MHz and FCCO = 192 MHz

	SYSCON->PDRUNCFG &= ~(1 << 7);   // Power up PLL after divider values changed (sec. 3.5.35)
	while ((SYSCON->SYSPLLSTAT & 1) == 0); // Wait for PLL to lock

	SYSCON->MAINCLKPLLSEL = 0x01;    // Set system oscillator to the output of the PLL (sec. 3.5.11)
	SYSCON->MAINCLKPLLUEN &= ~(0x1); // Write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
	SYSCON->MAINCLKPLLUEN |= 0x1;    // Write a one to the MAINCLKUEN register (sec. 3.5.12)
}

void initOscilator(void) {
	SYSCON->PDRUNCFG &= ~SYSCON_PDRUNCFG_FROOUT_PD_MASK;
	SYSCON->PDRUNCFG &= ~SYSCON_PDRUNCFG_FRO_PD_MASK;

	(*((void (*)(uint32_t freq))(CLOCK_FRO_SETTING_API_ROM_ADDRESS))) (30000);

	SYSCON->FROOSCCTRL |= (1 << 17);
	SYSCON->FRODIRECTCLKUEN &= ~(uint32_t) SYSCON_FRODIRECTCLKUEN_ENA_MASK;
	SYSCON->FRODIRECTCLKUEN |= (uint32_t) SYSCON_FRODIRECTCLKUEN_ENA_MASK;

	SYSCON->PDRUNCFG &= ~SYSCON_PDRUNCFG_SYSOSC_PD_MASK;

	SYSCON->EXTCLKSEL &= ~(uint32_t) SYSCON_EXTCLKSEL_SEL_MASK;

	SYSCON->MAINCLKSEL &= ~(uint32_t) SYSCON_MAINCLKPLLSEL_SEL_MASK;
	SYSCON->MAINCLKUEN &= ~(uint32_t) SYSCON_MAINCLKUEN_ENA_MASK;
	SYSCON->MAINCLKUEN |= (uint32_t) SYSCON_MAINCLKUEN_ENA_MASK;
	SYSCON->MAINCLKPLLSEL &= ~(uint32_t) SYSCON_MAINCLKPLLSEL_SEL_MASK;
	SYSCON->MAINCLKPLLUEN &= ~(uint32_t) SYSCON_MAINCLKPLLUEN_ENA_MASK;
	SYSCON->MAINCLKPLLUEN |= ~(uint32_t) SYSCON_MAINCLKPLLUEN_ENA_MASK;

	SYSCON->SYSAHBCLKDIV = (uint32_t) 0x01;
}

