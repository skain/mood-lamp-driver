#ifndef RGBPixelColorTransition_h
#define RGBPixelColorTransition_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "RGBPixelColor.h"
class RGBPixelColorTransition
{
  public:
    RGBPixelColorTransition(RGBPixelColor startColor, RGBPixelColor endColor);
	  RGBPixelColorTransition();
	  void SetStartColor(RGBPixelColor startColor);
	  void SetEndColor(RGBPixelColor endColor);
	  RGBPixelColor GetStartColor();
	  RGBPixelColor GetEndColor();
    RGBPixelColor GetFadeTransition(int curStep);
    RGBPixelColor GetFadeByPercent(double percent);
    int GetDistance();
    bool IsTransitionComplete(int curStep);
  private:
    RGBPixelColor _startColor;
    RGBPixelColor _endColor;
    float getGreatestDistance();
    float getGreatestDiff(float rDiff, float gDiff, float bDiff);
    float getDirectionMod(float startVal, float endVal);
    float getSpeedMod(float thisDiff, float greatestDiff);
    int roundToInt(float val);
    float normalize(float val, float normalizeTo, bool isMax);
    int getAbsDistance(int val1, int val2);
};

#endif