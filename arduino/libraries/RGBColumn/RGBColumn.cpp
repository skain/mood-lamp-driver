#include <LPD6803.h>
#include "RGBColumn.h"

RGBColumn::RGBColumn(int dataPin, int clockPin, int numPixels, int numRows, int numPerRow) {
	_dataPin = dataPin;
	_clockPin = clockPin;
	_numPixels = numPixels;
	_numRows = numRows;
	_numPerRow = numPerRows;
}
