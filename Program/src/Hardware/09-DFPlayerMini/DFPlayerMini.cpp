/*/*!
 * @file DFPlayerMini.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 30/01/2024 13:51:53
 */

#include "DFPlayerMini.h"

DFPlayer *g_dfplayer = nullptr;

DFPlayer::DFPlayer(const Gpio& RX, const Gpio& TX) : UART(RX, TX) {
	// TODO Auto-generated constructor stub

}

///////////////////////////////////
/// DFPlayerMini initialization ///
///////////////////////////////////

void initDFPlayer(void) {
	#ifdef CN13_PINS

	static DFPlayer dfplayer(RX0_IN, TX0_OUT);

	g_dfplayer = &dfplayer;

	#endif // UART1_PINS
}
