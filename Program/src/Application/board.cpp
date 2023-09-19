/*!
 * @file board.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 28/06/2023 20:36:24
 */

#include "board.h"

SevenSegmentDisplay *g_display = nullptr;
Keyboard *g_keyboard = nullptr;

void initDevice(void) {
	// initPhaseLockedLoop();
	initSysTick(FREQ_SYSTICK);
}

void initDisplay(void) {
	#ifdef CN12_PINS

	static std::vector<DigitsGroup*> sevenSegments_list;
	static DigitsGroup tripletDisplayFST(0, 3);
	static DigitsGroup tripletDisplaySND(3, 3);
	sevenSegments_list.push_back(&tripletDisplayFST);
	sevenSegments_list.push_back(&tripletDisplaySND);

	static std::vector<Gpio*> segments_GPIOs_list;
	segments_GPIOs_list.push_back(&BCDA);
	segments_GPIOs_list.push_back(&BCDB);
	segments_GPIOs_list.push_back(&BCDC);
	segments_GPIOs_list.push_back(&BCDD);
	static CD4511B segments_IC(segments_GPIOs_list);

	static std::vector<Gpio*> sweep_GPIOs_list;
	sweep_GPIOs_list.push_back(&BCD_RST);
	sweep_GPIOs_list.push_back(&BCD_CLK);
	static CD4017 sweep_IC(sweep_GPIOs_list, 6);

	static uint8_t relativePositions[] = { 2, 1, 0, 5, 4, 3 };

	static SevenSegmentDisplay Display(sevenSegments_list, &segments_IC, &sweep_IC, relativePositions, Digit::BCD);

	g_display = &Display;

	#endif // CN12_PINS
}

void initKeyboard(void) {
	#ifdef CN16_PINS

	static std::vector<Gpio*> columns_GPIOs_list;
	columns_GPIOs_list.push_back(&COL0_IN);
	columns_GPIOs_list.push_back(&COL1_IN);
	columns_GPIOs_list.push_back(&COL2_IN);

	static std::vector<Gpio*> rows_GPIOs_list;
	rows_GPIOs_list.push_back(&ROW0_OUT);
	rows_GPIOs_list.push_back(&ROW1_OUT);

	static Keyboard keyboard(columns_GPIOs_list, rows_GPIOs_list);

	g_keyboard = &keyboard;

	#endif // CN16_PINS
}
