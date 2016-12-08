#ifndef RGBPixelColorWheel_h
#define RGBPixelColorWheel_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "RGBPixelColor.h"
#include "RGBPixelColorTransition.h"

class RGBPixelColorWheel
{
  public:
    RGBPixelColorWheel(RGBPixelColorTransition* transitions, int numTransitions);
    /*RGBPixelColorWheel(RGBPixelColor* colors, int numColors);*/
	  RGBPixelColorWheel();
   /* void SetWheelByColors(RGBPixelColor* colors, int numColors);*/
    void SetTransitions(RGBPixelColorTransition* transitions, int numTransitions);
    RGBPixelColor GetColorFromWheel(int curStep, int offsetGroup, int offset);
    int GetTotalDistance();
	 /* void SetStartColor(RGBPixelColor startColor);
	  void SetEndColor(RGBPixelColor endColor);
	  RGBPixelColor GetStartColor();
	  RGBPixelColor GetEndColor();
    RGBPixelColor GetFadeTransition(int curStep);
    int GetDistance();
    bool IsTransitionComplete(int curStep);*/
  private:
    RGBPixelColorTransition* _transitions;
    int _numTransitions;
    int _totalDistance;
    void calculateTotalDistance();
   /* RGBPixelColor _startColor;
    RGBPixelColor _endColor;
    float getGreatestDistance();
    float getGreatestDiff(float rDiff, float gDiff, float bDiff);
    float getDirectionMod(float startVal, float endVal);
    float getSpeedMod(float thisDiff, float greatestDiff);
    int roundToInt(float val);
    float normalize(float val, float normalizeTo, bool isMax);
    int getAbsDistance(int val1, int val2);*/
};

#endif