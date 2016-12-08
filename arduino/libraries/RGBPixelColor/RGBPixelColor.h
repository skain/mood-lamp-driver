#ifndef RGBPixelColor_h
#define RGBPixelColor_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class RGBPixelColor
{
  public:
    RGBPixelColor(int red, int green, int blue);
    RGBPixelColor();
    int GetRed();
    int GetGreen();
    int GetBlue();
    void SetRed(int red);
    void SetGreen(int green);
    void SetBlue(int blue);
    void SetColor(int red, int green, int blue);
    int AsColor();
    static unsigned int RGBToColor(byte red, byte green, byte blue);
    static unsigned int Wheel(byte wheelPos);
    static RGBPixelColor RandomColor();
    static unsigned int RandomWheelColor();
    bool operator == (RGBPixelColor &compareTo);
    bool operator != (RGBPixelColor &compareTo);
  private:
    int _red;
    int _green;
    int _blue;
};


#endif
