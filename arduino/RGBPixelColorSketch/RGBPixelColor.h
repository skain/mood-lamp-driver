#ifndef RGBPixelColor_h
#define RGBPixelColor_h

#include "WProgram.h"
class RGBPixelColor
{
  public:
    RGBPixelColor(int red, int green, int blue);
    int GetRed();
    int GetGreen();
    int GetBlue();
    int AsColor();
    static unsigned int RGBToColor(byte red, byte green, byte blue);
    static unsigned int Wheel(byte wheelPos);
    static unsigned int RandomColor();
    static unsigned int RandomWheelColor();
  private:
    int _red;
    int _green;
    int _blue;
};


#endif
