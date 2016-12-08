#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "RGBPixelColor.h"
#include "RGBPixelColorTransition.h"
#include "RGBPixelColorWheel.h"

RGBPixelColorWheel::RGBPixelColorWheel(RGBPixelColorTransition* transitions, int numTransitions) {
  SetTransitions(transitions, numTransitions);
}
//RGBPixelColorWheel::RGBPixelColorWheel(RGBPixelColor* colors, int numColors) {
//    SetWheelByColors(colors, numColors);
//}
RGBPixelColorWheel::RGBPixelColorWheel() {
}
//void RGBPixelColorWheel::SetWheelByColors(RGBPixelColor* colors, int numColors) {
//  int numTransitions = numColors + 1;
//  for (int i = 0; i < numColors - 1; i++) {
//    _transitions[i] = RGBPixelTransition(colors[i], colors[i + 1]);
//  }
//  _transitions[numColors] = RGBPixelTransition(colors[numColors - 1], colors[0]);
//
//  SetTransitions(_transitions, numTransitions);
//}
void RGBPixelColorWheel::SetTransitions(RGBPixelColorTransition* transitions, int numTransitions) {
  _transitions = transitions;
  _numTransitions = numTransitions;
  calculateTotalDistance();
}

RGBPixelColor RGBPixelColorWheel::GetColorFromWheel(int curStep, int offsetGroup, int offset) {
  int curIndex = (curStep + (offsetGroup * offset)) % _totalDistance;
  RGBPixelColorTransition trans;
  for (int transIndex = 0; transIndex < _numTransitions; transIndex++) {
    trans = _transitions[transIndex];
    int curTransDistance = trans.GetDistance();
    if (curIndex > curTransDistance) {
     curIndex -= curTransDistance;
    } else {
     break;
    }
  }         
    
  return trans.GetFadeTransition(curIndex);  
}

int RGBPixelColorWheel::GetTotalDistance() {
  return _totalDistance;
}

void RGBPixelColorWheel::calculateTotalDistance() {
  _totalDistance = 0;
  for (int i = 0; i < _numTransitions; i++) {
    _totalDistance += _transitions[i].GetDistance();
  }
}
//RGBPixelColorTransition::RGBPixelColorTransition() {
//	SetStartColor(RGBPixelColor(0, 0, 0));
//  SetEndColor(RGBPixelColor(0, 0, 0));
//}
//RGBPixelColorTransition::RGBPixelColorTransition(RGBPixelColor startColor, RGBPixelColor endColor) {
//	SetStartColor(startColor);
//  SetEndColor(endColor);
//}
//
//void RGBPixelColorTransition::SetStartColor(RGBPixelColor startColor) {
//  _startColor = startColor;
//}
//void RGBPixelColorTransition::SetEndColor(RGBPixelColor endColor) {
//  _endColor = endColor;
//}
//RGBPixelColor RGBPixelColorTransition::GetStartColor() {
//  return _startColor;
//}
//RGBPixelColor RGBPixelColorTransition::GetEndColor() {
//  return _endColor;
//}
//int RGBPixelColorTransition::GetDistance() {
//  return getGreatestDistance();
//}
//RGBPixelColor RGBPixelColorTransition::GetFadeTransition(int curStep) {
//  float rDiff, gDiff, bDiff;
//  rDiff = getAbsDistance(_startColor.GetRed(), _endColor.GetRed());
//  gDiff = getAbsDistance(_startColor.GetGreen(), _endColor.GetGreen());
//  bDiff = getAbsDistance(_startColor.GetBlue(), _endColor.GetBlue());
////  Serial.print("Diffs:");Serial.print(rDiff);Serial.print(",");Serial.print(gDiff);Serial.print(",");Serial.println(bDiff);
////  float greatestDiff = getGreatestDiff(rDiff, gDiff, bDiff);
//  float greatestDiff = getGreatestDistance();
//
////  Serial.print("Greatest distance:");Serial.println(greatestDiff);
//  float curRMod,curGMod,curBMod;
//  curRMod = getDirectionMod(_startColor.GetRed(), _endColor.GetRed());
//  curGMod = getDirectionMod(_startColor.GetGreen(), _endColor.GetGreen());
//  curBMod = getDirectionMod(_startColor.GetBlue(), _endColor.GetBlue());
//
//  float curR, curG, curB;
//  curR = _startColor.GetRed();
//  curG = _startColor.GetGreen();
//  curB = _startColor.GetBlue();
////  Serial.print("Mods:");Serial.print(curRMod);Serial.print(",");Serial.print(curGMod);Serial.print(",");Serial.println(curBMod);
//
//  float rSpeed, gSpeed, bSpeed;
//  rSpeed = getSpeedMod(rDiff, greatestDiff);
//  gSpeed = getSpeedMod(gDiff, greatestDiff);
//  bSpeed = getSpeedMod(bDiff, greatestDiff);
////  Serial.print("Speeds:");Serial.print(rSpeed);Serial.print(",");Serial.print(gSpeed);Serial.print(",");Serial.println(bSpeed);
//  
//  curR = curR + ((rSpeed * curRMod) * curStep);
//  curG = curG + ((gSpeed * curGMod) * curStep);
//  curB = curB + ((bSpeed * curBMod) * curStep);
////    Serial.print("CurColor(before):");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);
//    
//  curR = normalize(curR, _endColor.GetRed(), curRMod > 0);
//  curG = normalize(curG, _endColor.GetGreen(), curGMod > 0);
//  curB = normalize(curB, _endColor.GetBlue(), curBMod > 0);
////  Serial.print("CurColor:");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);
//  
//  return RGBPixelColor(curR, curG, curB);
//}
//
//float RGBPixelColorTransition::getGreatestDistance() { 
//  float rDiff, gDiff, bDiff;
//  rDiff = getAbsDistance((float)_startColor.GetRed(), (float)_endColor.GetRed());
//  gDiff = getAbsDistance((float)_startColor.GetGreen(), (float)_endColor.GetGreen());
//  bDiff = getAbsDistance((float)_startColor.GetBlue(), (float)_endColor.GetBlue());
//  float greatestDiff = getGreatestDiff(rDiff, gDiff, bDiff);
//  return greatestDiff;
//}
//float RGBPixelColorTransition::getGreatestDiff(float rDiff, float gDiff, float bDiff) {
//  float greatestDiff = rDiff;
//  if (gDiff > greatestDiff) {
//    greatestDiff = gDiff;
//  }
//
//  if (bDiff > greatestDiff) {
//    greatestDiff = bDiff;
//  }
//
//  return greatestDiff;
//}
//
//float RGBPixelColorTransition::getDirectionMod(float startVal, float endVal) {
//  if (startVal > endVal) {
//    return -1;
//  }
//
//  return 1;
//}
//
//float RGBPixelColorTransition::getSpeedMod(float thisDiff, float greatestDiff) {
//  return thisDiff / greatestDiff;
//}
//
//int RGBPixelColorTransition::roundToInt(float val) {
//  return (int)(val + .5);
//}
//float RGBPixelColorTransition::normalize(float val, float normalizeTo, bool isMax) {
//  if (isMax && val > normalizeTo || ((!isMax) && val < normalizeTo)) {
//    return normalizeTo;
//  } 
//  else {
//    return val;
//  }
//}
//
//int RGBPixelColorTransition::getAbsDistance(int val1, int val2) {
//  if (val1 >= val2) {
//    return val1 - val2;
//  } 
//  else {
//    return val2 - val1;
//  }
//}
//
//bool RGBPixelColorTransition::IsTransitionComplete(int curStep) {
//  return curStep >= getGreatestDistance();
//}