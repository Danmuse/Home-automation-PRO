/*/*!
 * @file PLL.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 16/06/2023 23:14:24
 */

#include "PLL.h"

void initPhaseLockedLoop(void) {
	SYSCON->SYSAHBCLKDIV             = 0x1;       // Set clock divider for core to 1
	SYSCON->MAINCLKSEL               &= ~(0x03);  // Set "main clock" to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)

	SYSCON->MAINCLKUEN               &= ~(1);     // Write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
	SYSCON->MAINCLKUEN               |= 1;        // Write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update

	SYSCON->SYSPLLCLKSEL             = 0;      	  // Connect IRC oscillator to SYSTEM PLL (sec. 3.5.9)

	SYSCON->SYSPLLCLKUEN             &= ~(1);     // Write a zero to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
	SYSCON->SYSPLLCLKUEN             |= 1;        // Write a one to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update

	SYSCON->PDRUNCFG                 |= (1<<7);   // Power down the PLL before changing divider values (sec 3.5.35)
	SYSCON->SYSPLLCTRL               = 0x23;      // M = 3 P = 1

	SYSCON->PDRUNCFG 				 &= ~(1<<7);  // Power up PLL after divider values changed (sec. 3.5.35)
	while((SYSCON->SYSPLLSTAT & 1) == 0);         // Wait for PLL to lock

	SYSCON->MAINCLKPLLSEL            = 0x01;      // Set system oscillator to the output of the PLL (sec. 3.5.11)
	SYSCON->MAINCLKPLLUEN            &= ~(1);     // Write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
	SYSCON->MAINCLKPLLUEN            |= 1;        // Write a one to the MAINCLKUEN register (sec. 3.5.12)
}
