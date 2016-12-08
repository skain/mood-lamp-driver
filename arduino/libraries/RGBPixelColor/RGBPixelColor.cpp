#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "RGBPixelColor.h"

RGBPixelColor::RGBPixelColor() {
	SetColor(0, 0, 0);
}
RGBPixelColor::RGBPixelColor(int red, int green, int blue) {
	SetColor(red, green, blue);
}

void RGBPixelColor::SetRed(int red) {
	_red = red;
}
void RGBPixelColor::SetGreen(int green) {
	_green = green;
}
void RGBPixelColor::SetBlue(int blue) {
	_blue = blue;
}
void RGBPixelColor::SetColor(int red, int green, int blue) {
	_red = red;
	_green = green;
	_blue = blue;
}
int RGBPixelColor::AsColor() {
	return RGBToColor(_red, _green, _blue);
}
int RGBPixelColor::GetRed() {
	return _red;
}
int RGBPixelColor::GetGreen() {
	return _green;
}
int RGBPixelColor::GetBlue() {
	return _blue;
}
unsigned int RGBPixelColor::RGBToColor(byte red, byte green, byte blue) {
	return( ((unsigned int)green & 0x1F )<<10 | ((unsigned int)blue & 0x1F)<<5 | (unsigned int)red & 0x1F);  
}
unsigned int RGBPixelColor::Wheel(byte wheelPos) {
	byte r,g,b;
	switch(wheelPos >> 5)
	{
		case 0:
			r=31- wheelPos % 32;   //Red down
			g=wheelPos % 32;      // Green up
			b=0;                  //blue off
			break; 
		case 1:
			g=31- wheelPos % 32;  //green down
			b=wheelPos % 32;      //blue up
			r=0;                  //red off
			break; 
		case 2:
			b=31- wheelPos % 32;  //blue down 
			r=wheelPos % 32;      //red up
			g=0;                  //green off
		break; 
	}
	return(RGBToColor(r,g,b));
}
RGBPixelColor RGBPixelColor::RandomColor() {
	//return RGBToColor(random(32), random(32), random(32));
	return RGBPixelColor(random(32), random(32), random(32));
}
unsigned int RGBPixelColor::RandomWheelColor() {
	return Wheel(random(95));
}
bool RGBPixelColor::operator==(RGBPixelColor &compareTo) {
	return AsColor() == compareTo.AsColor();
}

bool RGBPixelColor::operator!=(RGBPixelColor &compareTo) {
	return AsColor() != compareTo.AsColor();
}
