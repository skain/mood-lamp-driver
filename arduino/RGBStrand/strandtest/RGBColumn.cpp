#include "RGBColumn.h"
/*
const uint16_t RED = Color(63, 0, 0);
const uint16_t ORANGE = Color(60, 10, 0);
const uint16_t YELLOW = Color(63, 50, 0);
const uint16_t GREEN = Color(0, 63, 0);
const uint16_t BLUE = Color(0, 0, 63);
const uint16_t INDIGO = Color(2, 0, 10);
const uint16_t VIOLET = Color(15, 0, 14);
const uint16_t CYAN = Color(0, 63, 63);
const uint16_t MAGENTA = Color(63, 0, 63);
const uint16_t WHITE = Color(63, 63, 63);
const uint16_t BLACK = Color(0, 0, 0);
const uint16_t PINK = Color(75, 2, 20);

const uint16_t roygbiv[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };
const uint16_t allcolors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, CYAN, MAGENTA, WHITE, BLACK, PINK };*/
RGBColumn::RGBColumn(int dataPin, int clockPin, int numPixels, int numRows, int numPerRow) {
	_dataPin = dataPin;
	_clockPin = clockPin;
	_numPixels = numPixels;
	_numRows = numRows;
	_numPerRow = numPerRows;
}
