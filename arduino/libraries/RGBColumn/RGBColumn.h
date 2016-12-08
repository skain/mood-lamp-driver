#include "WProgram.h"
#include <TimerOne.h>
#include "LPD6803.h"

class RGBColumn
{
	public:
		RGBColumn(int dataPin, int clockPin, int numPixels, int numRows, int numPerRow);
		const uint16_t RED;
		const uint16_t ORANGE;
		const uint16_t YELLOW;
		const uint16_t GREEN;
		const uint16_t BLUE;
		const uint16_t INDIGO;
		const uint16_t VIOLET;
		const uint16_t CYAN;
		const uint16_t MAGENTA;
		const uint16_t WHITE;
		const uint16_t BLACK;
		const uint16_t PINK;

		const uint16_t roygbiv[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };
		const uint16_t allcolors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, CYAN, MAGENTA, WHITE, BLACK, PINK };
		int numRoyGBiv = 7;
		int numAllColors = 12;
		LPD6803 LPD6803;

		void begin();
		void show();
		void doSwapBuffersAsap(uint16_t idx);
		void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
		void setPixelColor(uint16_t n, uint16_t c);
		void setCPUmax(uint8_t m);
		uint16_t numPixels(void);		
	private:
		int _dataPin, _clockPin, _numPixels, _numRows, _numPerRow;
};
