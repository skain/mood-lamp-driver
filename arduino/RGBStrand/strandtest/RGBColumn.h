#include "WProgram.h"
#include <TimerOne.h>
#include "LPD6803.h"

class RGBColumn
{
	public:
		RGBColumn(int dataPin, int clockPin, int numPixels, int numRows, int numPerRow);
/*
		extern const uint16_t RED;
		extern const uint16_t ORANGE;
		extern const uint16_t YELLOW;
		extern const uint16_t GREEN;
		extern const uint16_t BLUE;
		extern const uint16_t INDIGO;
		extern const uint16_t VIOLET;
		extern const uint16_t CYAN;
		extern const uint16_t MAGENTA;
		extern const uint16_t WHITE;
		extern const uint16_t BLACK;
		extern const uint16_t PINK;

		extern const uint16_t roygbiv[];
		extern const uint16_t allcolors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, CYAN, MAGENTA, WHITE, BLACK, PINK };
		int numRoyGBiv = 7;
		int numAllColors = 12;
		LPD6803 strip;

		void begin();
		void show();
		void doSwapBuffersAsap(uint16_t idx);
		void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
		void setPixelColor(uint16_t n, uint16_t c);
		void setCPUmax(uint8_t m);
		uint16_t numPixels(void);		
*/
	private:
		int _dataPin, _clockPin, _numPixels, _numRows, _numPerRow;
};
