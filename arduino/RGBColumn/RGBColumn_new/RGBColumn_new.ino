#include <TimerOne.h>
#include "LPD6803.h"
#include "RGBPixelColor.h"

/* begin arduino setup */
int dataPin = 2;       // 'yellow' wire (my red)
int clockPin = 3;      // 'green' wire (my black)
int numRows = 7;
int numCols = 6;

const RGBPixelColor RED = RGBPixelColor(63, 0, 0);
const RGBPixelColor ORANGE = RGBPixelColor(60, 10, 0);
const RGBPixelColor YELLOW = RGBPixelColor(63, 50, 0);
const RGBPixelColor GREEN = RGBPixelColor(0, 63, 0);
const RGBPixelColor BLUE = RGBPixelColor(0, 0, 63);
const RGBPixelColor INDIGO = RGBPixelColor(2, 0, 10);
const RGBPixelColor VIOLET = RGBPixelColor(15, 0, 14);
const RGBPixelColor CYAN = RGBPixelColor(0, 63, 63);
const RGBPixelColor MAGENTA = RGBPixelColor(63, 0, 63);
const RGBPixelColor WHITE = RGBPixelColor(63, 63, 63);
const RGBPixelColor BLACK = RGBPixelColor(0, 0, 0);
const RGBPixelColor PINK = RGBPixelColor(75, 2, 20);

const RGBPixelColor roygbiv[] = {
  RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };
const RGBPixelColor allcolors[] = {
  RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, CYAN, MAGENTA, WHITE, BLACK, PINK };
int numRoyGBiv = 7;
int numAllColors = 12;
// Don't forget to connect 'blue' (my green) to ground and 'red' (my white) to +5V

// Timer 1 is also used by the strip to send pixel clocks

// Set the first variable to the NUMBER of pixels. 20 = 20 pixels in a row
LPD6803 strip = LPD6803(39, dataPin, clockPin);

//bool evensWaxing = true;

void setup() {

  // The Arduino needs to clock out the data to the pixels
  // this happens in interrupt timer 1, we can change how often
  // to call the interrupt. setting CPUmax to 200 will take nearly all all the
  // time to do the pixel updates and a nicer/faster display,
  // especially with strands of over 200 dots.
  // (Note that the max is 'pessimistic', its probably 10% or 20% less in reality)
  randomSeed(analogRead(0));
  strip.setCPUmax(95);  // start with 50% CPU usage. up this if the strand flickers or is slow

  // Start up the LED counter
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  flashFirstPixel();
  Serial.begin(9600);
}

void loop() {
  runRandom();
}

/* end arduino setup */

/* begin column routines */
void runRandom() {
  int rand = random(14) + 1;
  bool debug = false;
//  debug = true;
  if (debug) {
    rand = 9999;
  }
  int wait = random(25, 300);
  switch(rand) {
  case 1:
    randomCheckerboard(24, wait);
    break;
  case 2:
    randomCheckerboardFade(20, wait);
    break;
  case 3:
    if (getRandomBool()) {
      flashRandomColorRows(24, wait);
    }
    else {
      flashRandomColorCols(24, wait);
    }
    break;
  case 4:
    //    wipeRGB(wait);
    colorWipe(RandomColor(), wait);
    break;
  case 5:
    wheelCycleCheckerboard(wait, random(2000));
    break;
  case 6:
    if (getRandomBool()) {
      wheelCycleCols(wait, random(2000));
    }
    else {
      wheelCycleCols(wait, (96 / random(2,7)));
    }
    break;
  case 7:
    if (getRandomBool()) {
      wheelCycleRows(wait, random(2000));
    }
    else {
      wheelCycleRows(wait, (96 / random(2,7)));
    }
    break;
  case 8:
    if (getRandomBool()) {
      middleOutCols(18, RGBPixelColor::RandomColor(), RGBPixelColor::RandomColor(), wait);
    } else {
      middleOutRows(18, RGBPixelColor::RandomColor(), RGBPixelColor::RandomColor(), wait);
    }
    break;
  case 9:
    randomColFade(30, wait);
    break;
  case 10:
    wait = random(250, 1000);
    if (getRandomBool()) {
      randomAlternateCols(20, wait);
    }
    else {
      randomAlternateRows(20, wait);
    }
    break;
  case 11:
    spinRows(RGBPixelColor::RandomColor(), RGBPixelColor::RandomColor(), 10, wait);
    break;
  case 12:
    randomFadeAll(30, wait);
    break;
  case 13:
    randomRowFade(30, wait);
    break;
  case 14:
    break;
  case 15:
    break;
  case 16:
    break;
  case 9999:
    randomCheckerboard(24, wait);
    break;
  }
  //  wipeBlack();
}
//void fadeRainbowAllOver(int iterations, int wait) {
//  if (wait > 225) {
//    wait = 225;
//  }
//  byte c1R, c1G, c1B, c2R, c2G, c2B;
//  c1R = 32;
//  c1G = 0;
//  c1B = 0;
//  for (int i = 0; i < iterations; i++) {
//    for (int cNum = 0; cNum < 8; cNum++) {
//      switch(i) {
//        case 0:
//          c2R = 60;
//          c2G = 10;
//          c2B = 0;
//          break;
//        case 1:
//          c2R= 63;
//          c2G = 50;
//          c2B = 0;
//          break;
//        case 2:
//          c2R = 0;
//          c2G =
//      }
////    Serial.print("Color1:");Serial.print(c1R, DEC);Serial.print(",");Serial.print(c1G, DEC);Serial.print(",");Serial.println(c1B, DEC);
////    Serial.print("Color2:");Serial.print(c2R, DEC);Serial.print(",");Serial.print(c2G, DEC);Serial.print(",");Serial.println(c2B, DEC);
//      randomFadeAll(c1R, c1G, c1B, c2R, c2G, c2B, wait);
//      c1R = c2R;
//      c1G = c2G;
//      c1B = c2B;
//
//    }
//  }
//}
//void fadeRainbowAllOver(float c1R, float c1G, float c1B, float c2R, float c2G, float c2B, int wait) {
//
//  float rDiff, gDiff, bDiff;
//  rDiff = getAbsDistance(c1R, c2R);
//  gDiff = getAbsDistance(c1G, c2G);
//  bDiff = getAbsDistance(c1B, c2B);
////  Serial.print("Diffs:");Serial.print(rDiff);Serial.print(",");Serial.print(gDiff);Serial.print(",");Serial.println(bDiff);
//  float greatestDiff = getGreatestDiff(rDiff, gDiff, bDiff);
//
////  Serial.print("Greatest distance:");Serial.println(greatestDiff);
//  float curRMod,curGMod,curBMod;
//  curRMod = getDirectionMod(c1R, c2R);
//  curGMod = getDirectionMod(c1G, c2G);
//  curBMod = getDirectionMod(c1B, c2B);
//
//  float curR, curG, curB;
//  curR = c1R;
//  curG = c1G;
//  curB = c1B;
////  Serial.print("Mods:");Serial.print(curRMod);Serial.print(",");Serial.print(curGMod);Serial.print(",");Serial.println(curBMod);
//
//  float rSpeed, gSpeed, bSpeed;
//  rSpeed = getSpeedMod(rDiff, greatestDiff);
//  gSpeed = getSpeedMod(gDiff, greatestDiff);
//  bSpeed = getSpeedMod(bDiff, greatestDiff);
////  Serial.print("Speeds:");Serial.print(rSpeed);Serial.print(",");Serial.print(gSpeed);Serial.print(",");Serial.println(bSpeed);
//  for (int i = 0; i < greatestDiff; i++) {
//    curR = curR + (rSpeed * curRMod);
//    curG = curG + (gSpeed * curGMod);
//    curB = curB + (bSpeed * curBMod);
////    Serial.print("CurColor(before):");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);
//
//    curR = normalize(curR, c2R, curRMod > 0);
//    curG = normalize(curG, c2G, curGMod > 0);
//    curB = normalize(curB, c2B, curBMod > 0);
////    Serial.print("CurColor:");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);
//    allPixelsOneColor(Color(roundToInt(curR), roundToInt(curG), roundToInt(curB)));
//    strip.show();
//    delay(wait);
//    if (curR == c2R && curG == c2G && curB == c2B) {
//      break;
//    }
//  }
//}
void spinRows(RGBPixelColor color1, RGBPixelColor color2, int iterations, int wait) {
  for (int iter = 0; iter < iterations; iter++) {
    for (int i = 0; i < numCols; i++) {
      for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
        int row = getRow(curPixel);
        int col = getCol(curPixel);

        RGBPixelColor curColor = color2;
        if (col == (row + i)%6) {
          curColor = color1;
        }

        strip.setPixelColor(curPixel, curColor.AsColor());
      }
      strip.show();
      delay(wait);
    }
  }
}
bool getRandomBool() {
  if (random(2) > 0) {
    return true;
  }
  return false;
}
void randomAlternateRows(int iterations, int wait) {
  RGBPixelColor color1, color2;
  color1 = RGBPixelColor::RandomColor();
  color2 = RGBPixelColor::RandomColor();
  for (int i = 0; i < iterations; i++) {
    alternateRows(color1, color2);
    strip.show();
    delay(wait);
    RGBPixelColor temp = color1;
    color1 = color2;
    color2 = temp;
  }
}
void randomAlternateCols(int iterations, int wait) {
  RGBPixelColor color1, color2;
  color1 = RGBPixelColor::RandomColor();
  color2 = RGBPixelColor::RandomColor();
  for (int i = 0; i < iterations; i++) {
    alternateCols(color1, color2);
    strip.show();
    delay(wait);
    RGBPixelColor temp = color1;
    color1 = color2;
    color2 = temp;
  }
}
void alternateCols(RGBPixelColor color1, RGBPixelColor color2) {
  for (int i = 0; i < strip.numPixels(); i++) {
    int col = getCol(i);
    RGBPixelColor curColor = color2;
    if (col%2 == 0) {
      curColor = color1;
    }
    strip.setPixelColor(i, curColor.AsColor());
  }
}
void alternateRows(RGBPixelColor color1, RGBPixelColor color2) {
  for (int i = 0; i < strip.numPixels(); i++) {
    int row = getRow(i);
    RGBPixelColor curColor = color2;
    if (row%2 == 0) {
      curColor = color1;
    }
    strip.setPixelColor(i, curColor.AsColor());
  }
}
void randomCheckerboardFade(int minColorChanges, int wait) {
  if (wait > 225) {
    wait = 225;
  }
//  else if (wait < 100) {
//    wait = 100;
//  }

  byte c1c1R, c1c1G, c1c1B, c2c1R, c2c1G, c2c1B, c1c2R, c1c2G, c1c2B, c2c2R, c2c2G, c2c2B;
  c1c1R = random(32);
  c1c1G = random(32);
  c1c1B = random(32);
  c2c1R = random(32);
  c2c1G = random(32);
  c2c1B = random(32);
  c1c2R = random(32);
  c1c2G = random(32);
  c1c2B = random(32);
  c2c2R = random(32);
  c2c2G = random(32);
  c2c2B = random(32);

  int offsets[] = {0, 0};
  int numColorChanges = 1;
  int iteration = 0;
  while(numColorChanges < minColorChanges) {
//    Serial.print("iteration:");Serial.println(i);
    unsigned int col1Color, col2Color, col1EndColor, col2EndColor;
    int col1Offset = iteration - offsets[0];
    int col2Offset = iteration - offsets[1];

//    Serial.print("col1Offset:");Serial.println(col1Offset);
    col1Color = getFadeTransition(c1c1R, c1c1G, c1c1B, c2c1R, c2c1G, c2c1B, col1Offset);
    col2Color = getFadeTransition(c1c2R, c1c2G, c1c2B, c2c2R, c2c2G, c2c2B, col2Offset);
    col1EndColor = Color(c2c1R, c2c1G, c2c1B);
    col2EndColor = Color(c2c2R, c2c2G, c2c2B);
//    Serial.print("Col1StartColor:");Serial.print(c1c1R, DEC);Serial.print(",");Serial.print(c1c1G, DEC);Serial.print(",");Serial.println(c1c1B, DEC);
//    Serial.print("Col1EndColor:");Serial.print(c2c1R, DEC);Serial.print(",");Serial.print(c2c1G, DEC);Serial.print(",");Serial.println(c2c1B, DEC);
//    Serial.print("Col2StartColor:");Serial.print(c1c2R, DEC);Serial.print(",");Serial.print(c1c2G, DEC);Serial.print(",");Serial.println(c1c2B, DEC);
//    Serial.print("Col2EndColor:");Serial.print(c2c2R, DEC);Serial.print(",");Serial.print(c2c2G, DEC);Serial.print(",");Serial.println(c2c2B, DEC);

    checkerboard(col1Color, col2Color);


//    strip.show();
    delay(wait);
     if (col1Color == col1EndColor) {
      c1c1R = c2c1R;
      c2c1R = random(32);
      c1c1G = c2c1G;
      c2c1G = random(32);
      c1c1B = c2c1B;
      c2c1B = random(32);
      offsets[0] = iteration + 1;
      col1EndColor = Color(c2c1R, c2c1G, c2c1B);
      numColorChanges++;
//      Serial.println("............................");
     }

    if (col2Color == col2EndColor) {
      c1c2R = c2c2R;
      c2c2R = random(32);
      c1c2G = c2c2G;
      c2c2G = random(32);
      c1c2B = c2c2B;
      c2c2B = random(32);
      offsets[1] = iteration + 1;
      col2EndColor = Color(c2c2R, c2c2G, c2c2B);
      numColorChanges++;
//      Serial.println("............................");
    }
    iteration++;
  }

}
void randomColFade(int minColorChanges, int wait) {
  if (wait > 225) {
    wait = 225;
  }
//  else if (wait < 100) {
//    wait = 100;
//  }

  byte c1c1R, c1c1G, c1c1B, c2c1R, c2c1G, c2c1B, c1c2R, c1c2G, c1c2B, c2c2R, c2c2G, c2c2B;
  c1c1R = random(32);
  c1c1G = random(32);
  c1c1B = random(32);
  c2c1R = random(32);
  c2c1G = random(32);
  c2c1B = random(32);
  c1c2R = random(32);
  c1c2G = random(32);
  c1c2B = random(32);
  c2c2R = random(32);
  c2c2G = random(32);
  c2c2B = random(32);

  int offsets[] = {0, 0};
  int numColorChanges = 1;
  int iteration = 0;
  while(numColorChanges < minColorChanges) {
//    Serial.print("iteration:");Serial.println(i);
    unsigned int col1Color, col2Color, col1EndColor, col2EndColor;
    int col1Offset = iteration - offsets[0];
    int col2Offset = iteration - offsets[1];

//    Serial.print("col1Offset:");Serial.println(col1Offset);
    col1Color = getFadeTransition(c1c1R, c1c1G, c1c1B, c2c1R, c2c1G, c2c1B, col1Offset);
    col2Color = getFadeTransition(c1c2R, c1c2G, c1c2B, c2c2R, c2c2G, c2c2B, col2Offset);
    col1EndColor = Color(c2c1R, c2c1G, c2c1B);
    col2EndColor = Color(c2c2R, c2c2G, c2c2B);
//    Serial.print("Col1StartColor:");Serial.print(c1c1R, DEC);Serial.print(",");Serial.print(c1c1G, DEC);Serial.print(",");Serial.println(c1c1B, DEC);
//    Serial.print("Col1EndColor:");Serial.print(c2c1R, DEC);Serial.print(",");Serial.print(c2c1G, DEC);Serial.print(",");Serial.println(c2c1B, DEC);
//    Serial.print("Col2StartColor:");Serial.print(c1c2R, DEC);Serial.print(",");Serial.print(c1c2G, DEC);Serial.print(",");Serial.println(c1c2B, DEC);
//    Serial.print("Col2EndColor:");Serial.print(c2c2R, DEC);Serial.print(",");Serial.print(c2c2G, DEC);Serial.print(",");Serial.println(c2c2B, DEC);

    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int col = getCol(curPixel);

      if (col%2 == 0) {
        strip.setPixelColor(curPixel, col1Color);
      } else {
        strip.setPixelColor(curPixel, col2Color);
      }
    }


    strip.show();
    delay(wait);
     if (col1Color == col1EndColor) {
      c1c1R = c2c1R;
      c2c1R = random(32);
      c1c1G = c2c1G;
      c2c1G = random(32);
      c1c1B = c2c1B;
      c2c1B = random(32);
      offsets[0] = iteration + 1;
      col1EndColor = Color(c2c1R, c2c1G, c2c1B);
      numColorChanges++;
//      Serial.println("............................");
     }

    if (col2Color == col2EndColor) {
      c1c2R = c2c2R;
      c2c2R = random(32);
      c1c2G = c2c2G;
      c2c2G = random(32);
      c1c2B = c2c2B;
      c2c2B = random(32);
      offsets[1] = iteration + 1;
      col2EndColor = Color(c2c2R, c2c2G, c2c2B);
      numColorChanges++;
//      Serial.println("............................");
    }
    iteration++;
  }

}



void randomRowFade(int minColorChanges, int wait) {
  if (wait > 225) {
    wait = 225;
  }
//  else if (wait < 100) {
//    wait = 100;
//  }

  byte c1r1R, c1r1G, c1r1B, c2r1R, c2r1G, c2r1B, c1r2R, c1r2G, c1r2B, c2r2R, c2r2G, c2r2B;
  c1r1R = random(32);
  c1r1G = random(32);
  c1r1B = random(32);
  c2r1R = random(32);
  c2r1G = random(32);
  c2r1B = random(32);
  c1r2R = random(32);
  c1r2G = random(32);
  c1r2B = random(32);
  c2r2R = random(32);
  c2r2G = random(32);
  c2r2B = random(32);

  int offsets[] = {0, 0};
  int numColorChanges = 1;
  int iteration = 0;
  while(numColorChanges < minColorChanges) {
//    Serial.print("iteration:");Serial.println(i);
    unsigned int row1Color, row2Color, row1EndColor, row2EndColor;
    int row1Offset = iteration - offsets[0];
    int row2Offset = iteration - offsets[1];

//    Serial.print("row1Offset:");Serial.println(row1Offset);
    row1Color = getFadeTransition(c1r1R, c1r1G, c1r1B, c2r1R, c2r1G, c2r1B, row1Offset);
    row2Color = getFadeTransition(c1r2R, c1r2G, c1r2B, c2r2R, c2r2G, c2r2B, row2Offset);
    row1EndColor = Color(c2r1R, c2r1G, c2r1B);
    row2EndColor = Color(c2r2R, c2r2G, c2r2B);
//    Serial.print("Row1StartColor:");Serial.print(c1r1R, DEC);Serial.print(",");Serial.print(c1r1G, DEC);Serial.print(",");Serial.println(c1r1B, DEC);
//    Serial.print("Row1EndColor:");Serial.print(c2r1R, DEC);Serial.print(",");Serial.print(c2r1G, DEC);Serial.print(",");Serial.println(c2r1B, DEC);
//    Serial.print("Row2StartColor:");Serial.print(c1r2R, DEC);Serial.print(",");Serial.print(c1r2G, DEC);Serial.print(",");Serial.println(c1r2B, DEC);
//    Serial.print("Row2EndColor:");Serial.print(c2r2R, DEC);Serial.print(",");Serial.print(c2r2G, DEC);Serial.print(",");Serial.println(c2r2B, DEC);

    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int row = getRow(curPixel);

      if (row%2 == 0) {
        strip.setPixelColor(curPixel, row1Color);
      } else {
        strip.setPixelColor(curPixel, row2Color);
      }
    }


    strip.show();
    delay(wait);
     if (row1Color == row1EndColor) {
      c1r1R = c2r1R;
      c2r1R = random(32);
      c1r1G = c2r1G;
      c2r1G = random(32);
      c1r1B = c2r1B;
      c2r1B = random(32);
      offsets[0] = iteration + 1;
      row1EndColor = Color(c2r1R, c2r1G, c2r1B);
      numColorChanges++;
      }

    if (row2Color == row2EndColor) {
      c1r2R = c2r2R;
      c2r2R = random(32);
      c1r2G = c2r2G;
      c2r2G = random(32);
      c1r2B = c2r2B;
      c2r2B = random(32);
      offsets[1] = iteration + 1;
      row2EndColor = Color(c2r2R, c2r2G, c2r2B);
      numColorChanges++;
    }
    iteration++;
  }

}
void randomFadeAll(int iterations, int wait) {
  if (wait > 225) {
    wait = 225;
  }
//  else if (wait < 100) {
//    wait = 100;
//  }
  byte c1R, c1G, c1B, c2R, c2G, c2B;
  c1R = random(32);
  c1G = random(32);
  c1B = random(32);
  for (int i = 0; i < iterations; i++) {
    c2R = random(32);
    c2G = random(32);
    c2B = random(32);
//    Serial.print("Color1:");Serial.print(c1R, DEC);Serial.print(",");Serial.print(c1G, DEC);Serial.print(",");Serial.println(c1B, DEC);
//    Serial.print("Color2:");Serial.print(c2R, DEC);Serial.print(",");Serial.print(c2G, DEC);Serial.print(",");Serial.println(c2B, DEC);
    randomFadeAll(c1R, c1G, c1B, c2R, c2G, c2B, wait);
    c1R = c2R;
    c1G = c2G;
    c1B = c2B;
  }
}
void randomFadeAll(float c1R, float c1G, float c1B, float c2R, float c2G, float c2B, int wait) {
  float greatestDistance = getGreatestDistance(c1R, c1G, c1B, c2R, c2G, c2B);
  for (int i = 0; i < greatestDistance; i++) {
    unsigned int color = getFadeTransition(c1R, c1G, c1B, c2R, c2G, c2B, i);
    allPixelsOneColor(color);
    strip.show();
    delay(wait);
    if (color == Color(c2R, c2G, c2B)) {
      break;
    }
  }
}

unsigned int getFadeTransition(float c1R, float c1G, float c1B, float c2R, float c2G, float c2B, int curStep) {
  float rDiff, gDiff, bDiff;
  rDiff = getAbsDistance(c1R, c2R);
  gDiff = getAbsDistance(c1G, c2G);
  bDiff = getAbsDistance(c1B, c2B);
//  Serial.print("Diffs:");Serial.print(rDiff);Serial.print(",");Serial.print(gDiff);Serial.print(",");Serial.println(bDiff);
//  float greatestDiff = getGreatestDiff(rDiff, gDiff, bDiff);
  float greatestDiff = getGreatestDistance(c1R, c1G, c1B, c2R, c2G, c2B);

//  Serial.print("Greatest distance:");Serial.println(greatestDiff);
  float curRMod,curGMod,curBMod;
  curRMod = getDirectionMod(c1R, c2R);
  curGMod = getDirectionMod(c1G, c2G);
  curBMod = getDirectionMod(c1B, c2B);

  float curR, curG, curB;
  curR = c1R;
  curG = c1G;
  curB = c1B;
//  Serial.print("Mods:");Serial.print(curRMod);Serial.print(",");Serial.print(curGMod);Serial.print(",");Serial.println(curBMod);

  float rSpeed, gSpeed, bSpeed;
  rSpeed = getSpeedMod(rDiff, greatestDiff);
  gSpeed = getSpeedMod(gDiff, greatestDiff);
  bSpeed = getSpeedMod(bDiff, greatestDiff);
//  Serial.print("Speeds:");Serial.print(rSpeed);Serial.print(",");Serial.print(gSpeed);Serial.print(",");Serial.println(bSpeed);

  curR = curR + ((rSpeed * curRMod) * curStep);
  curG = curG + ((gSpeed * curGMod) * curStep);
  curB = curB + ((bSpeed * curBMod) * curStep);
//    Serial.print("CurColor(before):");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);

  curR = normalize(curR, c2R, curRMod > 0);
  curG = normalize(curG, c2G, curGMod > 0);
  curB = normalize(curB, c2B, curBMod > 0);
//  Serial.print("CurColor:");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);

  return Color(curR, curG, curB);
}




/* begin randomFade helpers */

float getGreatestDistance(float c1R, float c1G, float c1B, float c2R, float c2G, float c2B) {
  float rDiff, gDiff, bDiff;
  rDiff = getAbsDistance(c1R, c2R);
  gDiff = getAbsDistance(c1G, c2G);
  bDiff = getAbsDistance(c1B, c2B);
  float greatestDiff = getGreatestDiff(rDiff, gDiff, bDiff);
  return greatestDiff;
}
float getGreatestDiff(float rDiff, float gDiff, float bDiff) {
  float greatestDiff = rDiff;
  if (gDiff > greatestDiff) {
    greatestDiff = gDiff;
  }

  if (bDiff > greatestDiff) {
    greatestDiff = bDiff;
  }

  return greatestDiff;
}

float getDirectionMod(float startVal, float endVal) {
  if (startVal > endVal) {
    return -1;
  }

  return 1;
}

float getSpeedMod(float thisDiff, float greatestDiff) {
  return thisDiff / greatestDiff;
}

int roundToInt(float val) {
  return (int)(val + .5);
}
float normalize(float val, float normalizeTo, bool isMax) {
  if (isMax && val > normalizeTo || ((!isMax) && val < normalizeTo)) {
    return normalizeTo;
  }
  else {
    return val;
  }
}

int getAbsDistance(int val1, int val2) {
  if (val1 >= val2) {
    return val1 - val2;
  }
  else {
    return val2 - val1;
  }
}
/* end randomFadeHelpers */

void randomFadeCheckerboard(float c1R, float c1G, float c1B, float c2R, float c2G, float c2B, int wait) {

  float rDiff, gDiff, bDiff;
  rDiff = getAbsDistance(c1R, c2R);
  gDiff = getAbsDistance(c1G, c2G);
  bDiff = getAbsDistance(c1B, c2B);
//  Serial.print("Diffs:");Serial.print(rDiff);Serial.print(",");Serial.print(gDiff);Serial.print(",");Serial.println(bDiff);
  float greatestDiff = getGreatestDiff(rDiff, gDiff, bDiff);

  Serial.print("Greatest distance:");Serial.println(greatestDiff);
  float curRMod,curGMod,curBMod;
  curRMod = getDirectionMod(c1R, c2R);
  curGMod = getDirectionMod(c1G, c2G);
  curBMod = getDirectionMod(c1B, c2B);

  float curR, curG, curB;
  curR = c1R;
  curG = c1G;
  curB = c1B;
//  Serial.print("Mods:");Serial.print(curRMod);Serial.print(",");Serial.print(curGMod);Serial.print(",");Serial.println(curBMod);

  float rSpeed, gSpeed, bSpeed;
  rSpeed = getSpeedMod(rDiff, greatestDiff);
  gSpeed = getSpeedMod(gDiff, greatestDiff);
  bSpeed = getSpeedMod(bDiff, greatestDiff);
//  Serial.print("Speeds:");Serial.print(rSpeed);Serial.print(",");Serial.print(gSpeed);Serial.print(",");Serial.println(bSpeed);
  for (int i = 0; i < greatestDiff; i++) {
    curR = curR + (rSpeed * curRMod);
    curG = curG + (gSpeed * curGMod);
    curB = curB + (bSpeed * curBMod);
//    Serial.print("CurColor(before):");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB
    ;
    curR = normalize(curR, c2R, curRMod > 0);
    curG = normalize(curG, c2G, curGMod > 0);
    curB = normalize(curB, c2B, curBMod > 0);
//    Serial.print("CurColor:");Serial.print(curR);Serial.print(",");Serial.print(curG);Serial.print(",");Serial.println(curB);
  //    allPixelsOneColor(Color(curR, curG, curB));
    checkerboard(Color(roundToInt(curR), roundToInt(curG), roundToInt(curB)), Color((roundToInt(curR) - 32) % 32, (roundToInt(curG) - 32) % 32, (roundToInt(curB) - 16) % 32));
    delay(wait);
    if (curR == c2R && curG == c2G && curB == c2B) {
      break;
    }
  }
}
void colorTest() {
  for (unsigned int i = 0; i < 64000; i++) {
    allPixelsOneColor(i);
    strip.show();
    delay(25);
  }
}
void animatedArrow(int wait, uint16_t arrowColor, uint16_t backgroundColor) {
  int middleRow = getMiddleRow();
  for (int leadingCol = 0; leadingCol < numCols; leadingCol++) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int row = getRow(curPixel);
      int col = getCol(curPixel);

      uint16_t curColor = backgroundColor;
      if ((col == leadingCol && row == middleRow)/* || (row == middleRow + leadingCol) || (row == middleRow - leadingCol) */) {
        curColor = arrowColor;
      }
      else {

        int distance = (leadingCol - col);
        if ((row == middleRow + distance) || (row == middleRow - distance))
        {
          curColor = arrowColor;
        }
        strip.setPixelColor(curPixel, curColor);
      }
    }

    strip.show();
    delay(wait);
  }
}

void wheelCycleCheckerboard(int wait, int offset) {
  for (int i = 0; i < 96; i++) {
    uint16_t color1, color2;
    color1 = RGBPixelColor::Wheel(i % 96);
    color2 = RGBPixelColor::Wheel((i + offset) % 96);
    checkerboard(color1, color2);
    delay(wait);
  }
}
void wheelCycleRows(int wait, int offset) {
  if (wait > 125) {
    wait = 125;
  }
  for (int j=0; j < 96 /* * 3 */; j++) {     // 3 cycles of all 96 colors in the wheel
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      strip.setPixelColor(curPixel, RGBPixelColor::Wheel( (j + (getRow(curPixel) * offset)) % 96));
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}


void wheelCycleCols(int wait, int offset) {
  if (wait > 125) {
    wait = 125;
  }

  for (int j=0; j < 96 /* * 3 */; j++) {     // 3 cycles of all 96 colors in the wheel
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      strip.setPixelColor(curPixel, RGBPixelColor::Wheel( (j + (getCol(curPixel) * offset)) % 96));
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void randomCheckerboard(int iterations, int wait) {
  if (wait < 200) {
    wait = 200;
  }
  RGBPixelColor curColor, lastColor;
  for (int i = 0; i < iterations; i++) {
    curColor = RGBPixelColor::RandomColor();
    while(true) {
      lastColor = RGBPixelColor::RandomColor();
      if (lastColor == curColor) {
        break;
      }
    }
    checkerboard(curColor.AsColor(), lastColor.AsColor());
    delay(wait);
  }
}
//void definedColorsCheckerboard(int wait) {
//  if (wait < 200) {
//    wait = 200;
//  }
//  uint16_t curColor, lastColor;
//  int lastColorIndex;
//  for (int curColorIndex = 0; curColorIndex < numAllColors; curColorIndex++) {
//    lastColorIndex = curColorIndex - 1;
//    if (lastColorIndex < 1) {
//      lastColorIndex = numAllColors - 1;
//    }
//    checkerboard(allcolors[curColorIndex], allcolors[lastColorIndex]);
//    delay(wait);
//  }
//}
//void lightRow(int row, uint16_t color) {
//  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//    int curRow = getRow(curPixel);
//    uint16_t setColor = color;
//    if (curRow != row) {
//      setColor = BLACK;
//    }
//    strip.setPixelColor(curPixel, setColor);
//  }
//
//  strip.show();
//}
//
//void lightCol(int col, uint16_t color) {
//  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//    int curCol = getCol(curPixel);
//    uint16_t setColor = color;
//    if (curCol != col) {
//      setColor = BLACK;
//    }
//    strip.setPixelColor(curPixel, setColor);
//  }
//
//  strip.show();
//}

void checkerboard(uint16_t color1, uint16_t color2) {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    int row = getRow(curPixel);
    int col = getCol(curPixel);
    uint16_t setColor;
    bool rowIsEven = row%2 == 0;
    bool colIsEven = col%2 == 0;
    if (rowIsEven && colIsEven || (!rowIsEven) && (!colIsEven)) {
      setColor = color1;
    }
    else {
      setColor = color2;
    }

    strip.setPixelColor(curPixel, setColor);
  }

  strip.show();
}

void flashRandomColorRows(int iterations, int wait) {
  if (wait > 150) {
    wait = 150;
  }
  RGBPixelColor rowColor, bgColor;
  rowColor = RGBPixelColor::RandomColor();
  bgColor = RGBPixelColor::RandomColor();
  for (int i = 0; i < iterations; i++) {
    flashRows(rowColor, bgColor, wait);
  }
}

void flashRandomColorCols(int iterations, int wait) {
  if (wait > 150) {
    wait = 150;
  }
  RGBPixelColor colColor, bgColor;
  colColor = RGBPixelColor::RandomColor();
  bgColor = RGBPixelColor::RandomColor();
  for (int i = 0; i < iterations; i++) {
    flashCols(colColor, bgColor, wait);
  }
}



//void flashDefinedColorRows(int wait) {
//  for (int color = 0; color < 11; color++) {
//    flashRows(allcolors[color], BLACK, wait);
//    clearPixels();
//  }
//}
//void flashDefinedColorCols(int wait) {
//  for (int color = 0; color < 11; color++) {
//    flashCols(allcolors[color], BLACK, wait);
//    clearPixels();
//  }
//}

void flashRows(RGBPixelColor rowColor, RGBPixelColor bgColor, int wait) {
  for (int curRow = 0; curRow <= numRows; curRow++) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int row = getRow(curPixel);
      if (row == curRow) {
        strip.setPixelColor(curPixel, rowColor.AsColor());
      }
      else {
        strip.setPixelColor(curPixel, bgColor.AsColor());
      }
    }
    strip.show();
    delay(wait);
  }
}
void flashCols(RGBPixelColor colColor, RGBPixelColor bgColor, int wait) {
  for (int curCol = 0; curCol <= numCols; curCol++) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int col = getCol(curPixel);
      if (col == curCol) {
        strip.setPixelColor(curPixel, colColor.AsColor());
      }
      else {
        strip.setPixelColor(curPixel, bgColor.AsColor());
      }
    }
    strip.show();
    delay(wait);
  }
}

//void pulseRGB() {
//  bool evensWaxing = true;
//  for (int color = 0; color < 3; color++) {
//    int redMod, greenMod, blueMod;
//    redMod = greenMod = blueMod = 0;
//    switch (color) {
//    case 0:
//      redMod = 1;
//      break;
//    case 1:
//      greenMod = 1;
//      break;
//    case 2:
//      blueMod = 1;
//      break;
//    }
//    pulseEvenOdd(redMod, greenMod, blueMod, evensWaxing);
//    pulseEvenOdd(redMod, greenMod, blueMod, evensWaxing);
//    evensWaxing = !evensWaxing;
//  }
//}

//void pulseEvenOdd(int redMod, int greenMod, int blueMod, bool evensWaxing) {
//  int maxValue = 31;
//
//  for (int curValue = 0; curValue < maxValue; curValue++) {
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      //        if (curPixel > 35) {
//      //          strip.setPixelColor(curPixel, BLACK);
//      //        } else {
//      if (curPixel%2 == 0) {
//        if (evensWaxing) {
//          strip.setPixelColor(curPixel, Color(curValue * redMod, curValue * greenMod, curValue * blueMod));
//        }
//        else {
//          strip.setPixelColor(curPixel, Color((maxValue - curValue) * redMod, (maxValue - curValue) * greenMod, (maxValue - curValue) * blueMod));
//        }
//      }
//      else {
//        if (evensWaxing) {
//          strip.setPixelColor(curPixel, Color((maxValue - curValue) * redMod, (maxValue - curValue) * greenMod, (maxValue - curValue) * blueMod));
//        }
//        else {
//          strip.setPixelColor(curPixel, Color(curValue * redMod, curValue * greenMod, curValue * blueMod));
//        }
//      }
//      //        }
//    }
//    strip.show();
//    delay(25);
//  }
//}

void middleOutRows(int iterations, RGBPixelColor activeColor, RGBPixelColor otherColor, int wait) {
  if (wait < 200) {
    wait = 200;
  }
  int middleRow = getMiddleRow();
  int evenRowMod = 0;
  if (numRows%2 == 0) {
    //even
    evenRowMod = 1;
  }

  for (int iteration = 0; iteration < iterations; iteration++) {
    for (int i = evenRowMod; i <= numRows - middleRow + evenRowMod; i++) {
      int topRow = middleRow + i - evenRowMod;
      int bottomRow = middleRow - i;
      for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
        int curRow = getRow(curPixel);
        if (curRow == topRow) {
          strip.setPixelColor(curPixel, activeColor.AsColor());
        }
        else if (topRow != bottomRow && bottomRow == curRow) {
          strip.setPixelColor(curPixel, activeColor.AsColor());
        }
        else {
          strip.setPixelColor(curPixel, otherColor.AsColor());
        }
      }
      strip.show();
      delay(wait);
    }
  }
}

void middleOutCols(int iterations, RGBPixelColor activeColor, RGBPixelColor otherColor, int wait) {
  if (wait < 200) {
    wait = 200;
  }
  int middleCol = (numCols / 2);
  int evenColMod = 0;
  if (numCols%2 == 0) {
    //even
    evenColMod = 1;
  }
  for (int iteration = 0; iteration < iterations; iteration++) {
    for (int i = evenColMod; i <= numCols - middleCol + evenColMod; i++) {
      int topCol = middleCol + i - evenColMod;
      int bottomCol = middleCol - i;
      for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
        int curCol = getCol(curPixel);
        if (curCol == topCol) {
          strip.setPixelColor(curPixel, activeColor.AsColor());
        }
        else if (topCol != bottomCol && bottomCol == curCol) {
          strip.setPixelColor(curPixel, activeColor.AsColor());
        }
        else {
          strip.setPixelColor(curPixel, otherColor.AsColor());
        }
      }
      strip.show();
      delay(wait);
    }
  }
}


/* end column routines */





/* begin column helpers */
int getRow(int val){
  //  Serial.print("val,row:");Serial.print(val);Serial.print(",");Serial.println(val / numRows);
  return val / numCols;
}

int getCol(int val) {
  return (val + 1) % numCols;
}

int getMiddleRow() {
  return (numRows / 2);
}
/* end column helpers */






/* begin strip routines */

void clearPixels() {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    strip.setPixelColor(curPixel, BLACK.AsColor());
  }
  strip.show();
}
void flashFirstPixel() {
  for (int i = 0; i < 3; i++) {
    strip.setPixelColor(0, WHITE.AsColor());
    strip.show();
    delay(200);
    strip.setPixelColor(0, BLACK.AsColor());
    strip.show();
    delay(200);
  }
}
void wipeBlack() {
  colorWipe(BLACK.AsColor(), 25);
  delay(2000);
}

void wipeRGB(uint8_t wait) {
  for (int i = 0; i < 7; i++) {
    colorWipe(roygbiv[i].AsColor(), wait);
    delay(2000);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint16_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
    //      if (i > 35) {
    //        strip.setPixelColor(i, BLACK);
    //      } else {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    //      }
  }

}


void wheelCycle(uint8_t wait) {
  for (int i = 0; i < 96; i++) {
    if (i > 0 && i%10 == 0) {
      allPixelsOneColor(BLACK.AsColor());
      strip.show();
      delay(250);
      for (int j = 0; j < i/10; j++) {
        allPixelsOneColor(WHITE.AsColor());
        strip.show();
        delay(250);
        allPixelsOneColor(BLACK.AsColor());
        strip.show();
        delay(250);
      }
      delay(250);
    }

    allPixelsOneColor(Wheel(i));
    strip.show();
    delay(wait);
    allPixelsOneColor(BLACK.AsColor());
    strip.show();
    delay(wait);
  }
}

void allColorsCycle(uint8_t wait) {
  for (unsigned int i = 0; i < 500; i++) {
    if (i > 0 && i%10 == 0) {
      allPixelsOneColor(BLACK.AsColor());
      strip.show();
      delay(250);
      for (int j = 0; j < i/10; j++) {
        allPixelsOneColor(WHITE.AsColor());
        strip.show();
        delay(250);
        allPixelsOneColor(BLACK.AsColor());
        strip.show();
        delay(250);
      }
      delay(250);
    }

    allPixelsOneColor(i);
    strip.show();
    delay(wait);
    allPixelsOneColor(BLACK.AsColor());
    strip.show();
    delay(wait);
  }
}

void varyRGBs(uint8_t wait) {
  for (int rgb = 0; rgb < 3; rgb++) {
    for (int i = 0; i < 32; i++) {
      RGBPixelColor color;
      switch (rgb) {
      case 0:
        color = RGBPixelColor(i, 0, 0);
        break;
      case 1:
        color = RGBPixelColor(0, i, 0);
        break;
      case 2:
        color = RGBPixelColor(0, 0, i);
        break;
      }

      allPixelsOneColor(color.AsColor());
      strip.show();
      delay(wait);
    }
  }
}

void allPixelsOneColor(uint16_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}
//void rainbow(uint8_t wait) {
//  int i, j;
//
//  for (j=0; j < 96 /* * 3 */; j++) {     // 3 cycles of all 96 colors in the wheel
//    for (i=0; i < strip.numPixels(); i++) {
//      //      if (i > 35) {
//      //        strip.setPixelColor(i, BLACK);
//      //      } else {
//      strip.setPixelColor(i, Wheel( (i + j) % 96));
//      //      }
//    }
//    strip.show();   // write all the pixels out
//    delay(wait);
//  }
//}
//
//// Slightly different, this one makes the rainbow wheel equally distributed
//// along the chain
//void rainbowCycle(uint8_t wait) {
//  int i, j;
//
//  for (j=0; j < 96 /* * 5 */; j++) {     // 5 cycles of all 96 colors in the wheel
//    for (i=0; i < strip.numPixels(); i++) {
//      // tricky math! we use each pixel as a fraction of the full 96-color wheel
//      // (thats the i / strip.numPixels() part)
//      // Then add in j which makes the colors go around per pixel
//      // the % 96 is to make the wheel cycle around
//      //      if (i > 35) {
//      //        strip.setPixelColor(i, BLACK);
//      //      } else {
//      strip.setPixelColor(i, Wheel( ((i * 96 / strip.numPixels()) + j) % 96) );
//      //      }
//    }
//    strip.show();   // write all the pixels out
//    delay(wait);
//  }
//}
/* end strip routines */




/* begin color helpers */

unsigned int RandomColor() {
  return Color(random(32), random(32), random(32));
}

unsigned int RandomWheelColor() {
  return Wheel(random(95));
}
// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}

//Input a value 0 to 96 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(byte WheelPos)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
  case 0:
    r=31- WheelPos % 32;   //Red down
    g=WheelPos % 32;      // Green up
    b=0;                  //blue off
    break;
  case 1:
    g=31- WheelPos % 32;  //green down
    b=WheelPos % 32;      //blue up
    r=0;                  //red off
    break;
  case 2:
    b=31- WheelPos % 32;  //blue down
    r=WheelPos % 32;      //red up
    g=0;                  //green off
    break;
  }
  return(Color(r,g,b));
}

/* end color helpers */
