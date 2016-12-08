#include <TimerOne.h>
#include "LPD6803.h"
#include "RGBPixelColor.h"
#include "RGBPixelColorTransition.h"
#include "RGBPixelColorWheel.h"

/* begin arduino setup */
int dataPin = 2;       // 'yellow' wire (my red)
int clockPin = 3;      // 'green' wire (my black)
int numRows = 7;
int numCols = 6;

//timer variables
long unsigned patternStart;

RGBPixelColor RED = RGBPixelColor(31, 0, 0);
RGBPixelColor ORANGE = RGBPixelColor(28, 10, 0);
RGBPixelColor YELLOW = RGBPixelColor(31, 18, 0);
RGBPixelColor GREEN = RGBPixelColor(0, 31, 0);
RGBPixelColor BLUE = RGBPixelColor(0, 0, 31);
RGBPixelColor INDIGO = RGBPixelColor(2, 0, 10);
RGBPixelColor VIOLET = RGBPixelColor(15, 0, 14);
RGBPixelColor CYAN = RGBPixelColor(0, 31, 31);
RGBPixelColor MAGENTA = RGBPixelColor(31, 0, 31);
RGBPixelColor WHITE = RGBPixelColor(31, 31, 31);
RGBPixelColor BLACK = RGBPixelColor(0, 0, 0);
RGBPixelColor PINK = RGBPixelColor(11, 2, 20);

RGBPixelColorTransition RoygbivTransitions[7];

RGBPixelColor roygbiv[] = { 
  RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };
RGBPixelColor allcolors[] = { 
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
  setupRoygbivTransitions();
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

void setupRoygbivTransitions() {  
  RoygbivTransitions[0] = RGBPixelColorTransition(RED, ORANGE);
  RoygbivTransitions[1] = RGBPixelColorTransition(ORANGE, YELLOW);
  RoygbivTransitions[2] = RGBPixelColorTransition(YELLOW, GREEN);
  RoygbivTransitions[3] = RGBPixelColorTransition(GREEN, BLUE);
  RoygbivTransitions[4] = RGBPixelColorTransition(BLUE, INDIGO);
  RoygbivTransitions[5] = RGBPixelColorTransition(INDIGO, VIOLET);
  RoygbivTransitions[6] = RGBPixelColorTransition(VIOLET, RED);
}
/* end arduino setup */

/* begin column routines */
void runRandom() {
  int rand = random(11) + 1;
  int duration = 10;
  bool debug = false;
//  debug = true;
  if (debug) {
    rand = 9999;
  }
  int wait = random(25, 700);
//  rand = 6; //also for debugging...
//  for (int i = 1; i < 12; i++) {
//    rand = i;
//  rand = 6;
//  wait = 285;
//  Serial.print("Rand:");Serial.println(rand);
//  Serial.print("Wait:");Serial.println(wait);
  switch(rand) {
    case 1:
      randomCheckerboard(duration, wait);
      break;
    case 2:
      spinRows(RGBPixelColor::RandomColor(), RGBPixelColor::RandomColor(), duration, wait);
      break;
    case 3:
      randomRowsOrColsWithTrails(duration, wait, random(22, 90), getRandomBool());
      break;
    case 4:
        doColorWipes(duration, wait);
      break;
    case 5:
      if (getRandomBool()) {
        roygbivWheelCheckerboard(duration, wait, random(16,32));
      } else {
        randomWheelCheckerboard(duration, wait, random(16,32));
      }
      break;
    case 6:   
      colorWheelPixelsRandom(duration, wait, random(1,32));
      break;
    case 7:    
      colorWheelColsOrRowsRandom(duration, wait, random(1,32), getRandomBool());//here
      break;
    case 8:
      if (getRandomBool()) {
        middleOutCols(duration, RGBPixelColor::RandomColor(), RGBPixelColor::RandomColor(), wait);
      } else {
        middleOutRows(duration, RGBPixelColor::RandomColor(), RGBPixelColor::RandomColor(), wait);
      }
      break;
    case 9:  
      colorWheelPixelsRoygbiv(duration, wait, random(1,32));
      break;
    case 10:
      wait = random(250, 1000);
      randomAlternateRowsOrCols(duration, wait, getRandomBool());
      break;
    case 11:
      randomFlashAll(duration, wait);
      break;
    case 9999:    
      randomFlashAll(3, wait);
      wipeBlack();
      break; 
  }
//  }
}

void doColorWipes(int duration, int wait) {
  setPatternStart();
  if (wait < 100) {
    wait = 100;
  }
  while(!isTimeUp(duration)) {
    colorWipe(RGBPixelColor::RandomColor().AsColor(), wait);
  }
}
void randomFlashAll(int duration, int wait) {
  setPatternStart();
  
  while(!isTimeUp(duration))
  {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      RGBPixelColor curColor = RGBPixelColor::RandomColor();
      strip.setPixelColor(curPixel, curColor.AsColor());
    }
    strip.show();
    delay(wait);
  }
}

void randomRowsOrColsWithTrails(int duration, int wait, int offset, bool rowsInsteadOfCols) {
  setPatternStart();
  if (wait > 100) {
    wait = 100;
  }
  RGBPixelColor foreColor, bgColor;
  foreColor = RGBPixelColor::RandomColor();
  bgColor = RGBPixelColor::RandomColor();
  while(!isTimeUp(duration)) {
//  for (int i = 0; i < iterations; i++) {
    flashRowsOrColsWithTrails(foreColor, bgColor, wait, offset, rowsInsteadOfCols);
  }
}
void flashRowsOrColsWithTrails(RGBPixelColor foreColor, RGBPixelColor bgColor, int wait, int offset, bool rowsInsteadOfCols) {
  RGBPixelColorTransition trans = RGBPixelColorTransition(foreColor, bgColor);
  int numGroups = numCols;
  if (rowsInsteadOfCols) {
    numGroups = numRows;
  }
  
  for (int curGroup = 0; curGroup <= numGroups; curGroup++) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int pixelGroup;
      if (rowsInsteadOfCols) {
        pixelGroup = getRow(curPixel);
      } else {
        pixelGroup = getCol(curPixel);
      }
      
      double transitionPercent = 0;
      if (pixelGroup == curGroup) {
        transitionPercent = 100;
      } else if (pixelGroup < curGroup) {
        double distance = curGroup - pixelGroup;
        transitionPercent = 100 - (distance * 33);
        
        if (transitionPercent < 0) {
          transitionPercent = 0;
        }
      }
      
      RGBPixelColor curColor = trans.GetFadeByPercent(transitionPercent);
      strip.setPixelColor(curPixel, curColor.AsColor());
    }
    strip.show();
    delay(wait);
  }
}
void colorWheelPixelsRoygbiv(int duration, int wait, int offset) {
  if (wait > 100) {
    wait = 100;
  }
  int numTransitions = 7;

  RGBPixelColorWheel colorWheel = RGBPixelColorWheel(RoygbivTransitions, numTransitions);
  colorWheelPixels(colorWheel, duration, wait, offset);
}

void colorWheelPixelsRandom(int duration, int wait, int offset) {
  if (wait > 100) {
    wait = 100;
  }
//  Serial.print("Wait:");Serial.print(wait);Serial.print(" Duration:");Serial.print(duration);Serial.print(" Offset:");Serial.println(offset);
  int numTransitions = random(2, numRows + 1);
//  Serial.print("NumTransitions:");Serial.println(numTransitions);
  RGBPixelColorTransition transitions[numTransitions];
  createRandomTransitions(numTransitions, transitions);

  RGBPixelColorWheel colorWheel = RGBPixelColorWheel(transitions, numTransitions);
  colorWheelPixels(colorWheel, duration, wait, offset);
}

void colorWheelPixels(RGBPixelColorWheel colorWheel, int duration, int wait, int offset) {
  setPatternStart();
  int totalDistance = colorWheel.GetTotalDistance();
  int iteration = 0;
  int curStep = 0;
  RGBPixelColorTransition trans;
  RGBPixelColor color;
  while(!isTimeUp(duration)) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {   
      color = colorWheel.GetColorFromWheel(curStep, curPixel, offset);
      strip.setPixelColor(curPixel, color.AsColor());
    }
    
    strip.show();
    delay(wait);
    
    curStep++;
    if (curStep > totalDistance) {
      curStep = 0;
      iteration++;
    } 
  }
}
void createRandomTransitions(int numTransitions, RGBPixelColorTransition *transitions) {
  RGBPixelColor firstColor = RGBPixelColor::RandomColor();
  RGBPixelColor lastColor = firstColor;
  for (int i = 0; i < numTransitions - 1; i++) {
    RGBPixelColor nextColor = RGBPixelColor::RandomColor();
    transitions[i] = RGBPixelColorTransition(lastColor, nextColor);
    lastColor = nextColor;
  }
  
  transitions[numTransitions - 1] = RGBPixelColorTransition(lastColor, firstColor);
}
void roygbivWheelCheckerboard(int duration, int wait, int offset) {
  int numTransitions = 7;
  RGBPixelColorWheel colorWheel = RGBPixelColorWheel(RoygbivTransitions, numTransitions);
  wheelCheckerboard(colorWheel, duration, wait, offset);
}

void randomWheelCheckerboard(int duration, int wait, int offset) {
  int numTransitions = random(2, numRows + 1);
  RGBPixelColorTransition transitions[numTransitions];
  createRandomTransitions(numTransitions, transitions);
    
  RGBPixelColorWheel colorWheel = RGBPixelColorWheel(transitions, numTransitions);
  wheelCheckerboard(colorWheel, duration, wait, offset);
}
void wheelCheckerboard(RGBPixelColorWheel colorWheel, int duration, int wait, int offset) {
  setPatternStart();
  if (wait > 100) {
    wait = 100;
  } 
  
  int iteration = 0;
  int curStep = 0;
  int totalDistance = colorWheel.GetTotalDistance();
  RGBPixelColor oddColor, evenColor;
  while (!isTimeUp(duration)) {
    oddColor = colorWheel.GetColorFromWheel(curStep, 1, offset);
    evenColor = colorWheel.GetColorFromWheel(curStep, 2, offset);
    checkerboard(oddColor, evenColor);
    strip.show();
    delay(wait);
    
    curStep++;
    if (curStep > totalDistance) {
      curStep = 0;
      iteration++;
    } 
  }
}
void colorWheelColsOrRowsRandom(int duration, int wait, int offset, bool rowsInsteadOfCols) {
  if (wait > 100) {
    wait = 100;
  }
  int numTransitions = random(2, numRows + 1);
  RGBPixelColorTransition transitions[numTransitions];
  createRandomTransitions(numTransitions, transitions);
    
  RGBPixelColorWheel colorWheel = RGBPixelColorWheel(transitions, numTransitions);
  
  colorWheelColsOrRows(colorWheel, duration, wait, offset, rowsInsteadOfCols);
}
void colorWheelColsOrRows(RGBPixelColorWheel colorWheel, int duration, int wait, int offset, bool rowsInsteadOfCols) {
  setPatternStart();
  int totalDistance = colorWheel.GetTotalDistance();
  int iteration = 0;
  int curStep = 0;
  
  RGBPixelColorTransition trans;
  RGBPixelColor color;
  while(!isTimeUp(duration)) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      if (rowsInsteadOfCols) {
        int row = getRow(curPixel);
        color = colorWheel.GetColorFromWheel(curStep, row, offset);
      } else {
        int col = getCol(curPixel);
        color = colorWheel.GetColorFromWheel(curStep, col, offset);
      }
      
      strip.setPixelColor(curPixel, color.AsColor());
    }
    
    strip.show();
    delay(wait);
    
    curStep++;
    if (curStep > totalDistance) {
      curStep = 0;
      iteration++;
    } 
  }
}


void colorWheelRowsOrColsRoygbiv(int duration, int wait, int offset, bool rowsInsteadOfCols) {
  if (wait > 100) {
    wait = 100;
  }
  int numTransitions = 7;
  
  RGBPixelColorWheel colorWheel = RGBPixelColorWheel(RoygbivTransitions, numTransitions);
  
  colorWheelColsOrRows(colorWheel, duration, wait, offset, rowsInsteadOfCols);
}




void spinRows(RGBPixelColor color1, RGBPixelColor color2, int duration, int wait) {
  setPatternStart();
  while(!isTimeUp(duration)) {
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
void randomAlternateRowsOrCols(int duration, int wait, bool rowsInsteadOfCols) {
  setPatternStart();
  RGBPixelColor color1, color2;
  color1 = RGBPixelColor::RandomColor();
  color2 = RGBPixelColor::RandomColor();
  while(!isTimeUp(duration)) {
//  for (int i = 0; i < iterations; i++) {
    if (rowsInsteadOfCols) {
      alternateRows(color1, color2);
    } else {    
      alternateCols(color1, color2);
    }
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

void setRowsByColor(RGBPixelColor evenColor, RGBPixelColor oddColor) {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int row = getRow(curPixel);
      
      if (row%2 == 0) {
        strip.setPixelColor(curPixel, evenColor.AsColor());
      } else {
        strip.setPixelColor(curPixel, oddColor.AsColor());
      }
    }
}


void randomCheckerboard(int duration, int wait) {
  setPatternStart();
  if (wait < 200) {
    wait = 200;
  }
  RGBPixelColor curColor, lastColor;
  while(!isTimeUp(duration)) {
    curColor = RGBPixelColor::RandomColor();
    while(true) {
      lastColor = RGBPixelColor::RandomColor();
      if (lastColor != curColor) {
        break;
      }
    }
    checkerboard(curColor, lastColor);
    strip.show();
    delay(wait);
  }
}


void checkerboard(RGBPixelColor evenColor, RGBPixelColor oddColor) {
  checkerboard(evenColor.AsColor(), oddColor.AsColor());
}

void checkerboard(uint16_t evenColor, uint16_t oddColor) {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    int row = getRow(curPixel);
    int col = getCol(curPixel);
    uint16_t setColor;
    bool rowIsEven = row%2 == 0;
    bool colIsEven = col%2 == 0;
    if (rowIsEven && colIsEven || (!rowIsEven) && (!colIsEven)) {
      setColor = evenColor;
    } 
    else {
      setColor = oddColor;
    }

    strip.setPixelColor(curPixel, setColor);
  }
}





void middleOutRows(int duration, RGBPixelColor activeColor, RGBPixelColor otherColor, int wait) {
  setPatternStart();
  if (wait < 200) {
    wait = 200;
  }
  int middleRow = getMiddleRow();
  int evenRowMod = 0;
  if (numRows%2 == 0) {
    //even
    evenRowMod = 1;
  } 
  while(!isTimeUp(duration)) {
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

void middleOutCols(int duration, RGBPixelColor activeColor, RGBPixelColor otherColor, int wait) {
  setPatternStart();
  if (wait < 200) {
    wait = 200;
  }
  int middleCol = (numCols / 2);
  int evenColMod = 0;
  if (numCols%2 == 0) {
    //even
    evenColMod = 1;
  } 
  while(!isTimeUp(duration)) {
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
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }

}


void wheelCycle(uint8_t wait) {
  for (int i = 0; i < 96; i++) {
    if (i > 0 && i%10 == 0) {
      allPixelsOneColor(BLACK);
      strip.show();
      delay(250);
      for (int j = 0; j < i/10; j++) {
        allPixelsOneColor(WHITE);
        strip.show();
        delay(250);
        allPixelsOneColor(BLACK);
        strip.show();
        delay(250);
      }
      delay(250);
    }

    allPixelsOneColor(Wheel(i));
    strip.show();
    delay(wait);
    allPixelsOneColor(BLACK);
    strip.show();
    delay(wait);
  }
}

void allColorsCycle(uint8_t wait) {
  for (unsigned int i = 0; i < 500; i++) {
    if (i > 0 && i%10 == 0) {
      allPixelsOneColor(BLACK);
      strip.show();
      delay(250);
      for (int j = 0; j < i/10; j++) {
        allPixelsOneColor(WHITE);
        strip.show();
        delay(250);
        allPixelsOneColor(BLACK);
        strip.show();
        delay(250);
      }
      delay(250);
    }

    allPixelsOneColor(i);
    strip.show();
    delay(wait);
    allPixelsOneColor(BLACK);
    strip.show();
    delay(wait);
  }
}

void varyRGBs(uint8_t wait) {
  for (int rgb = 0; rgb < 3; rgb++) {
    for (int i = 0; i < 32; i++) {
      uint16_t color;
      switch (rgb) {
      case 0:
        color = Color(i, 0, 0);
        break;
      case 1:
        color = Color(0, i, 0);
        break;
      case 2:
        color = Color(0, 0, i);
        break;  
      }

      allPixelsOneColor(color);
      strip.show();
      delay(wait);
    }
  }
}
void allPixelsOneColor(RGBPixelColor color) {
  allPixelsOneColor(color.AsColor());
}
void allPixelsOneColor(uint16_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}
void rainbow(uint8_t wait) {
  int i, j;

  for (j=0; j < 96 /* * 3 */; j++) {     // 3 cycles of all 96 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      //      if (i > 35) {
      //        strip.setPixelColor(i, BLACK);
      //      } else {
      strip.setPixelColor(i, Wheel( (i + j) % 96));
      //      }
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}
//
//// Slightly different, this one makes the rainbow wheel equally distributed 
//// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;

  for (j=0; j < 96 /* * 5 */; j++) {     // 5 cycles of all 96 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      //      if (i > 35) {
      //        strip.setPixelColor(i, BLACK);
      //      } else {
      strip.setPixelColor(i, Wheel( ((i * 96 / strip.numPixels()) + j) % 96) );
      //      }
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}
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

/* begin pattern timer */

void setPatternStart() {
  patternStart = millis();
}

bool isTimeUp(int duration) {
  unsigned long now = millis();
  long durationMillis = duration * 1000;
  
  return (now - patternStart) >= durationMillis;
}

/* end pattern timer */


//void colorWheelColsRoygbiv(int iterations, int wait, int offset) {
//  if (wait > 150) {
//    wait = 150;
//  }
//  int numTransitions = 7;
//  int totalDistance = 0;
//  for (int i = 0; i < numTransitions; i++) {
//    totalDistance += RoygbivTransitions[i].GetDistance();
//  }
//  
//  int iteration = 0;
//  int curStep = 0;
//  if (offset > totalDistance) {
//    offset = totalDistance;
//  }
//  RGBPixelColorTransition trans;
//  RGBPixelColor color;
//  while (iteration < iterations) {
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int col = getCol(curPixel);
//      int curIndex = (curStep + (col * offset)) % totalDistance;      
//      color = getColorFromTransitionWheel(RoygbivTransitions, numTransitions, curStep, col, offset, totalDistance);
//      strip.setPixelColor(curPixel, color.AsColor());
//    }
//    
//    strip.show();
//    delay(wait);
//    
//    curStep++;
//    if (curStep > totalDistance) {
//      curStep = 0;
//      iteration++;
//    } 
//  }
//}

//void roygbivColFade(int iterations, int wait) {
//  if (wait > 100) {
//    wait = 100;
//  }
//  
////  RGBPixelColorTransition transitions[numRoyGBiv];
////  transitions[0] = RGBPixelColorTransition(RED, ORANGE);
////  transitions[1] = RGBPixelColorTransition(ORANGE, YELLOW);
////  transitions[2] = RGBPixelColorTransition(YELLOW, GREEN);
////  transitions[3] = RGBPixelColorTransition(GREEN, BLUE);
////  transitions[4] = RGBPixelColorTransition(BLUE, INDIGO);
////  transitions[5] = RGBPixelColorTransition(INDIGO, VIOLET);
////  transitions[6] = RGBPixelColorTransition(VIOLET, RED);
//  
//  int colFadeIndexes[numCols]; // {0,0,0,0,0,0,0}
//  for (int i = 0; i < numCols; i++) {
//    colFadeIndexes[i] = 0;
//  }
//  
//  int colTransIndexes[numCols]; //{0,1,2,3,4,5,6}
//  int lastVal = -1;
//  for (int i = 0; i < numCols; i++) {
//    lastVal++;
//    colTransIndexes[i] = lastVal;
//  }
//  
//  int iteration = 0;
//  RGBPixelColorTransition trans;
//  RGBPixelColor color;
//  while(iteration < iterations) {
//    //set the pixels to their current colors
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int col = getCol(curPixel);
//      trans = RoygbivTransitions[colTransIndexes[col]];
//      color = trans.GetFadeTransition(colFadeIndexes[col]); 
//      
//      strip.setPixelColor(curPixel, color.AsColor());
//    }
//    
//    strip.show();
//    delay(wait);
//    
//    //increment the indexes
//    for (int col = 0; col < numCols; col++) {
//      colFadeIndexes[col]++;
//      trans = RoygbivTransitions[colTransIndexes[col]];
//      if (trans.IsTransitionComplete(colFadeIndexes[col])) {
//        //trans is complete so move to next trans and reset fadeIndex to 0
//        colTransIndexes[col]++;
//        colFadeIndexes[col] = 0;    
//        if (col == 0 && colTransIndexes[col] == numRoyGBiv) {
//          iteration++; //increment iteration every time row 0 changes color
//        }    
//        
//        if (colTransIndexes[col] >= numRoyGBiv) {
//          colTransIndexes[col] = 0; //reset to beginning of transitions
//        }
//      }
//    }
//  }
//}

//void roygbivAll(int iterations, int wait) {
////  RGBPixelColorTransition transitions[numRoyGBiv];
////  transitions[0] = RGBPixelColorTransition(RED, ORANGE);
////  transitions[1] = RGBPixelColorTransition(ORANGE, YELLOW);
////  transitions[2] = RGBPixelColorTransition(YELLOW, GREEN);
////  transitions[3] = RGBPixelColorTransition(GREEN, BLUE);
////  transitions[4] = RGBPixelColorTransition(BLUE, INDIGO);
////  transitions[5] = RGBPixelColorTransition(INDIGO, VIOLET);
////  transitions[6] = RGBPixelColorTransition(VIOLET, RED);
//  
//  int iteration = 0;
//  int curTransIndex = 0;
//  int curFadeIndex = 0;
//  while (iteration < iterations) {
//    RGBPixelColor curColor = RoygbivTransitions[curTransIndex].GetFadeTransition(curFadeIndex);
//    allPixelsOneColor(curColor);
//    strip.show();
//    delay(wait);
//    
//    if (RoygbivTransitions[curTransIndex].IsTransitionComplete(curFadeIndex)) {
//      curTransIndex++;
//      if (curTransIndex >= numRoyGBiv) {
//        curTransIndex = 0;
//      }
//      curFadeIndex = 0;
//      iteration++;
//    } else {
//      curFadeIndex++;
//    }
//  }
//}
//void roygbivRowFade(int iterations, int wait) {
//  if (wait > 100) {
//    wait = 100;
//  }
//  
////  RGBPixelColorTransition transitions[numRoyGBiv];
////  transitions[0] = RGBPixelColorTransition(RED, ORANGE);
////  transitions[1] = RGBPixelColorTransition(ORANGE, YELLOW);
////  transitions[2] = RGBPixelColorTransition(YELLOW, GREEN);
////  transitions[3] = RGBPixelColorTransition(GREEN, BLUE);
////  transitions[4] = RGBPixelColorTransition(BLUE, INDIGO);
////  transitions[5] = RGBPixelColorTransition(INDIGO, VIOLET);
////  transitions[6] = RGBPixelColorTransition(VIOLET, RED);
//  
//  int rowFadeIndexes[numRows]; // {0,0,0,0,0,0,0}
//  for (int i = 0; i < numRows; i++) {
//    rowFadeIndexes[i] = 0;
//  }
//  
//  int rowTransIndexes[numRows]; //{0,1,2,3,4,5,6}
//  int lastVal = -1;
//  for (int i = 0; i < numRows; i++) {
//    lastVal++;
//    rowTransIndexes[i] = lastVal;
//  }
//  
//  int iteration = 0;
//  RGBPixelColorTransition trans;
//  RGBPixelColor color;
//  while(iteration < iterations) {
//    //set the pixels to their current colors
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int row = getRow(curPixel);
//      trans = RoygbivTransitions[rowTransIndexes[row]];
//      color = trans.GetFadeTransition(rowFadeIndexes[row]); 
//      
//      strip.setPixelColor(curPixel, color.AsColor());
//    }
//    
//    strip.show();
//    delay(wait);
//    
//    //increment the indexes
//    for (int row = 0; row < numRows; row++) {
//      rowFadeIndexes[row]++;
//      trans = RoygbivTransitions[rowTransIndexes[row]];
//      if (trans.IsTransitionComplete(rowFadeIndexes[row])) {
//        //trans is complete so move to next trans and reset fadeIndex to 0
//        rowTransIndexes[row]++;
//        rowFadeIndexes[row] = 0;    
//        if (row == 0 && rowTransIndexes[row] == numRoyGBiv) {
//          iteration++; //increment iteration every time row 0 changes color
//        }    
//        
//        if (rowTransIndexes[row] >= numRoyGBiv) {
//          rowTransIndexes[row] = 0; //reset to beginning of transitions
//        }
//      }
//    }
//  }
//}

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









//void colorWheelColsRandom(int iterations, int wait, int offset) {
//  if (wait > 150) {
//    wait = 150;
//  }
//  int numTransitions = random(1, numRows + 1);
//  RGBPixelColorTransition transitions[numTransitions];
//  RGBPixelColor firstColor = RGBPixelColor::RandomColor();
//  RGBPixelColor lastColor = firstColor;
//  for (int i = 0; i < numTransitions - 1; i++) {
//    RGBPixelColor nextColor = RGBPixelColor::RandomColor();
//    transitions[i] = RGBPixelColorTransition(lastColor, nextColor);
//    lastColor = nextColor;
//  }
//  
//  transitions[numTransitions - 1] = RGBPixelColorTransition(lastColor, firstColor);
//  
//  int totalDistance = 0;
//  for (int i = 0; i < numTransitions; i++) {
//    totalDistance += transitions[i].GetDistance();
//  }
//  
//  int iteration = 0;
//  int curStep = 0;
//  if (offset > totalDistance) {
//    offset = totalDistance;
//  }
//  RGBPixelColorTransition trans;
//  RGBPixelColor color;
//  while (iteration < iterations) {
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int col = getCol(curPixel);
//      int curIndex = (curStep + (col * offset)) % totalDistance;      
//      color = getColorFromTransitionWheel(transitions, numTransitions, curStep, col, offset, totalDistance);
//      strip.setPixelColor(curPixel, color.AsColor());
//    }
//    
//    strip.show();
//    delay(wait);
//    
//    curStep++;
//    if (curStep > totalDistance) {
//      curStep = 0;
//      iteration++;
//    } 
//  }
//}

//RGBPixelColor getColorFromTransitionWheel(RGBPixelColorTransition *transitions, int numTransitions, int curStep, int pixelGroup, int offset, int totalDistance) {
//  int curIndex = (curStep + (pixelGroup * offset)) % totalDistance;
//  RGBPixelColorTransition trans;
//  for (int transIndex = 0; transIndex < numTransitions; transIndex++) {
//    trans = transitions[transIndex];
//    int curTransDistance = trans.GetDistance();
//    if (curIndex > curTransDistance) {
//      curIndex -= curTransDistance;
//    } else {
//      break;
//    }
//  }         
//    
//  return trans.GetFadeTransition(curIndex);  
//}


//void randomAlternateCols(int iterations, int wait) {
//  RGBPixelColor color1, color2;
//  color1 = RGBPixelColor::RandomColor();
//  color2 = RGBPixelColor::RandomColor();
//  for (int i = 0; i < iterations; i++) {
//    alternateCols(color1, color2);
//    strip.show();
//    delay(wait);
//    RGBPixelColor temp = color1;
//    color1 = color2;
//    color2 = temp;
//  }
//}





//void randomCheckerboardFade(int minColorChanges, int wait) {
//  if (wait > 225) {
//    wait = 225;
//  }
//  
//  RGBPixelColor evenStartColor, evenEndColor, oddStartColor, oddEndColor;
//  evenStartColor = RGBPixelColor::RandomColor();
//  evenEndColor = RGBPixelColor::RandomColor();
//  oddStartColor = RGBPixelColor::RandomColor();
//  oddEndColor = RGBPixelColor::RandomColor();
//  
//  RGBPixelColorTransition evenTrans, oddTrans;
//  evenTrans = RGBPixelColorTransition(evenStartColor, evenEndColor);
//  oddTrans = RGBPixelColorTransition(oddStartColor, oddEndColor);
//  
//  int offsets[] = {0, 0};
//  int numColorChanges = 1;
//  int iteration = 0;
//  while(numColorChanges < minColorChanges) {
//    RGBPixelColor curEvenColor, curOddColor, col1EndColor, col2EndColor;
//    int evenOffset = iteration - offsets[0];
//    int oddOffset = iteration - offsets[1];
//    
//    curEvenColor = evenTrans.GetFadeTransition(evenOffset);
//    curOddColor = oddTrans.GetFadeTransition(oddOffset);
//    
//    checkerboard(curEvenColor, curOddColor);   
//    
//    strip.show();
//    delay(wait);    
//    if (evenTrans.IsTransitionComplete(evenOffset)) {
//      evenStartColor = evenEndColor;
//      evenEndColor = RGBPixelColor::RandomColor();
//      evenTrans = RGBPixelColorTransition(evenStartColor, evenEndColor);
//      offsets[0] = iteration + 1;
//      numColorChanges++;
//    }
//      
//    if (oddTrans.IsTransitionComplete(oddOffset)) {
//      oddStartColor = oddEndColor;
//      oddEndColor = RGBPixelColor::RandomColor();
//      oddTrans = RGBPixelColorTransition(oddStartColor, oddEndColor);
//      offsets[1] = iteration + 1;
//      numColorChanges++;
//    }
//    iteration++;
//  }
//  
//}

//void randomColFade(int minColorChanges, int wait) {
//  if (wait > 225) {
//    wait = 225;
//  }
//  
//  RGBPixelColor col1StartColor, col1EndColor, col2StartColor, col2EndColor;
//  col1StartColor = RGBPixelColor::RandomColor();
//  col1EndColor = RGBPixelColor::RandomColor();
//  col2StartColor = RGBPixelColor::RandomColor();
//  col2EndColor = RGBPixelColor::RandomColor();
//  
//  RGBPixelColorTransition col1Trans, col2Trans;
//  col1Trans = RGBPixelColorTransition(col1StartColor, col1EndColor);
//  col2Trans = RGBPixelColorTransition(col2StartColor, col2EndColor);
//  int offsets[] = {0, 0};
//  int numColorChanges = 1;
//  int iteration = 0;
//  while(numColorChanges < minColorChanges) {
//    RGBPixelColor col1CurColor, col2CurColor;
//    int col1Offset = iteration - offsets[0];
//    int col2Offset = iteration - offsets[1];
//    
//    col1CurColor = col1Trans.GetFadeTransition(col1Offset);
//    col2CurColor = col2Trans.GetFadeTransition(col2Offset);
//    
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int col = getCol(curPixel);
//      
//      if (col%2 == 0) {
//        strip.setPixelColor(curPixel, col1CurColor.AsColor());
//      } else {
//        strip.setPixelColor(curPixel, col2CurColor.AsColor());
//      }
//    }
//    
//    
//    strip.show();
//    delay(wait);    
//     if (col1Trans.IsTransitionComplete(col1Offset)) {
//      col1StartColor = col1EndColor;
//      col1EndColor = RGBPixelColor::RandomColor();
//      col1Trans = RGBPixelColorTransition(col1StartColor, col1EndColor);
//      offsets[0] = iteration + 1;
//      numColorChanges++;
//     }
//      
//    if (col2Trans.IsTransitionComplete(col2Offset)) {
//      col2StartColor = col2EndColor;
//      col2EndColor = RGBPixelColor::RandomColor();
//      col2Trans = RGBPixelColorTransition(col2StartColor, col2EndColor);
//      offsets[1] = iteration + 1;
//      numColorChanges++;
//    }
//    iteration++;
//  }
//}

//void twoColorRowFade(RGBPixelColor color1, RGBPixelColor color2, int iterations, int wait) {
//  if (wait > 100) {
//    wait = 100;
//  }
//  RGBPixelColorTransition transitions[2];
//  transitions[0] = RGBPixelColorTransition(color1, color2);
//  transitions[1] = RGBPixelColorTransition(color2, color1);
//  
//  bool evensAre1, oddsAre2;
//  evensAre1 = oddsAre2 = true;
//  int transIndexes[] = {0, 0};
//  int iteration = 0;
//  while(iteration < iterations) {
//    RGBPixelColor curEvenColor, curOddColor;
//    if (evensAre1) {      
//      curEvenColor = transitions[0].GetFadeTransition(transIndexes[0]);
//    } else {
//      curEvenColor = transitions[1].GetFadeTransition(transIndexes[0]);
//    }
//    
//    if (oddsAre2) {
//      curOddColor = transitions[1].GetFadeTransition(transIndexes[1]);
//    } else {
//      curOddColor = transitions[0].GetFadeTransition(transIndexes[1]);
//    }
//    
//    setRowsByColor(curEvenColor, curOddColor);
//    strip.show();
//    delay(wait);
//    
//    if ((evensAre1 && transitions[0].IsTransitionComplete(transIndexes[0])) || (transitions[1].IsTransitionComplete(transIndexes[0]))) {
//        transIndexes[0] = 0;
//        evensAre1 = !evensAre1;
//        iteration++;
//    } else {
//      transIndexes[0]++;
//    }
//    
//    if ((oddsAre2 && transitions[1].IsTransitionComplete(transIndexes[1])) || (transitions[0].IsTransitionComplete(transIndexes[1]))) {
//      transIndexes[1] = 0;
//      oddsAre2 = !oddsAre2;
//    } else {
//      transIndexes[1]++;
//    }
//  }
//}
//void randomRowFade(int minColorChanges, int wait) {
//  if (wait > 225) {
//    wait = 225;
//  }
//  RGBPixelColor row1StartColor, row1EndColor, row2StartColor, row2EndColor;
//  row1StartColor = RGBPixelColor::RandomColor();
//  row1EndColor = RGBPixelColor::RandomColor();
//  row2StartColor = RGBPixelColor::RandomColor();
//  row2EndColor = RGBPixelColor::RandomColor();
//  
//  RGBPixelColorTransition row1Trans = RGBPixelColorTransition(row1StartColor, row1EndColor);
//  RGBPixelColorTransition row2Trans = RGBPixelColorTransition(row2StartColor, row2EndColor);
//  
//  int offsets[] = {0, 0};
//  int numColorChanges = 1;
//  int iteration = 0;
//  while(numColorChanges < minColorChanges) {
//    RGBPixelColor row1CurColor, row2CurColor;
//    int row1Offset = iteration - offsets[0];
//    int row2Offset = iteration - offsets[1];
//    
//    row1CurColor = row1Trans.GetFadeTransition(row1Offset);
//    row2CurColor = row2Trans.GetFadeTransition(row2Offset);
//    
//    setRowsByColor(row1CurColor, row2CurColor);    
//    
//    strip.show();
//    delay(wait);    
//    if (row1Trans.IsTransitionComplete(row1Offset)) {
//      row1StartColor = row1EndColor;
//      row1EndColor = RGBPixelColor::RandomColor();
//      row1Trans = RGBPixelColorTransition(row1StartColor, row1EndColor);
//      offsets[0] = iteration + 1;
//      numColorChanges++;
//    }
//    
//    if (row2Trans.IsTransitionComplete(row2Offset)) {
//      row2StartColor = row2EndColor;
//      row2EndColor = RGBPixelColor::RandomColor();
//      row2Trans = RGBPixelColorTransition(row2StartColor, row2EndColor);
//      offsets[1] = iteration + 1;
//      numColorChanges++;
//    }
//    iteration++;
//  }
//  
//}
//
//
//void randomFadeAll(int iterations, int wait) {
//  if (wait > 225) {
//    wait = 225;
//  } 
//  RGBPixelColor color1 = RGBPixelColor::RandomColor();
//  RGBPixelColor color2;
//  for (int i = 0; i < iterations; i++) {
//    color2 = RGBPixelColor::RandomColor();
//    randomFadeAll(color1, color2, wait);
//    color1 = color2;
//  }
//}
//void randomFadeAll(RGBPixelColor color1, RGBPixelColor color2, int wait) {
//  RGBPixelColorTransition trans = RGBPixelColorTransition(color1, color2);
//  int greatestDistance = trans.GetDistance();
//  for (int i = 0; i < greatestDistance; i++) {
//    RGBPixelColor newColor = trans.GetFadeTransition(i);
//    allPixelsOneColor(newColor);
//    strip.show();
//    delay(wait);
//    if (trans.IsTransitionComplete(i)) {
//      break;
//    }
//  }
//}
//
//
//void animatedArrow(int wait, uint16_t arrowColor, uint16_t backgroundColor) {
//  int middleRow = getMiddleRow();
//  for (int leadingCol = 0; leadingCol < numCols; leadingCol++) {
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int row = getRow(curPixel);
//      int col = getCol(curPixel);
//
//      uint16_t curColor = backgroundColor;
//      if ((col == leadingCol && row == middleRow)/* || (row == middleRow + leadingCol) || (row == middleRow - leadingCol) */) {
//        curColor = arrowColor;
//      } 
//      else {
//
//        int distance = (leadingCol - col);
//        if ((row == middleRow + distance) || (row == middleRow - distance))
//        {
//          curColor = arrowColor;
//        }
//        strip.setPixelColor(curPixel, curColor);
//      }
//    }
//
//    strip.show();
//    delay(wait);
//  }
//}
//
//void wheelCycleCheckerboard(int wait, int offset) {
//  for (int i = 0; i < 96; i++) {
//    uint16_t color1, color2;
//    color1 = RGBPixelColor::Wheel(i % 96);
//    color2 = RGBPixelColor::Wheel((i + offset) % 96);
//    checkerboard(color1, color2);
//    strip.show();
//    delay(wait);
//  }
//}
//void wheelCycleRows(int wait, int offset) {
//  if (wait > 125) {
//    wait = 125;
//  }
//  for (int j=0; j < 96 /* * 3 */; j++) {     // 3 cycles of all 96 colors in the wheel
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      strip.setPixelColor(curPixel, RGBPixelColor::Wheel( (j + (getRow(curPixel) * offset)) % 96));
//    }
//    strip.show();   // write all the pixels out
//    delay(wait);
//  }
//}
//
//
//void wheelCycleCols(int wait, int offset) {
//  if (wait > 125) {
//    wait = 125;
//  }
//
//  for (int j=0; j < 96 /* * 3 */; j++) {     // 3 cycles of all 96 colors in the wheel
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      strip.setPixelColor(curPixel, RGBPixelColor::Wheel( (j + (getCol(curPixel) * offset)) % 96));
//    }
//    strip.show();   // write all the pixels out
//    delay(wait);
//  }
//}

//void flashRandomColorRows(int iterations, int wait) {
//  if (wait > 150) {
//    wait = 150;
//  }
//  RGBPixelColor rowColor, bgColor;
//  rowColor = RGBPixelColor::RandomColor();
//  bgColor = RGBPixelColor::RandomColor();
//  for (int i = 0; i < iterations; i++) {
//    flashRows(rowColor, bgColor, wait);
//  }
//}
//
//void flashRandomColorCols(int iterations, int wait) {
//  if (wait > 150) {
//    wait = 150;
//  }
//  RGBPixelColor colColor, bgColor;
//  colColor = RGBPixelColor::RandomColor();
//  bgColor = RGBPixelColor::RandomColor();
//  for (int i = 0; i < iterations; i++) {
//    flashCols(colColor, bgColor, wait);
//  }
//}
//
//void flashRows(RGBPixelColor rowColor, RGBPixelColor bgColor, int wait) {
//  for (int curRow = 0; curRow <= numRows; curRow++) {
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int row = getRow(curPixel);
//      if (row == curRow) {
//        strip.setPixelColor(curPixel, rowColor.AsColor());
//      } 
//      else {
//        strip.setPixelColor(curPixel, bgColor.AsColor());
//      }
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//void flashCols(RGBPixelColor colColor, RGBPixelColor bgColor, int wait) {
//  for (int curCol = 0; curCol <= numCols; curCol++) {
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      int col = getCol(curPixel);
//      if (col == curCol) {
//        strip.setPixelColor(curPixel, colColor.AsColor());
//      } 
//      else {
//        strip.setPixelColor(curPixel, bgColor.AsColor());
//      }
//    }
//    strip.show();
//    delay(wait);
//  }
//}

