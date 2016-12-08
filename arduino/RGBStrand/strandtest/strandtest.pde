#include <TimerOne.h>
#include "LPD6803.h"

//Example to control LPD6803-based RGB LED Modules in a strand
// Original code by Bliptronics.com Ben Moyes 2009
//Use this as you wish, but please give credit, or at least buy some of my LEDs!

// Code cleaned up and Object-ified by ladyada, should be a bit easier to use

/*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
int dataPin = 2;       // 'yellow' wire (my red)
int clockPin = 3;      // 'green' wire (my black)
int numRows = 7;
int numPerRow = 6;

const uint16_t RED = Color(63, 0, 0);
const uint16_t ORANGE = Color(60, 10, 0);
const uint16_t YELLOW = Color(63, 50, 0);
const uint16_t GREEN = Color(0, 63, 0);
const uint16_t BLUE = Color(0, 0, 63);
const uint16_t INDIGO = Color(2, 0, 10);
const uint16_t VIOLET = Color(15, 0, 14);
const uint16_t CYAN = Color(0, 63, 63);
const uint16_t MAGENTA = Color(63, 0, 63);
const uint16_t WHITE = Color(63, 63, 63);
const uint16_t BLACK = Color(0, 0, 0);
const uint16_t PINK = Color(75, 2, 20);

const uint16_t roygbiv[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };
const uint16_t allcolors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, CYAN, MAGENTA, WHITE, BLACK, PINK };
int numRoyGBiv = 7;
int numAllColors = 12;
// Don't forget to connect 'blue' (my green) to ground and 'red' (my white) to +5V

// Timer 1 is also used by the strip to send pixel clocks

// Set the first variable to the NUMBER of pixels. 20 = 20 pixels in a row
LPD6803 strip = LPD6803(39, dataPin, clockPin);

bool evensWaxing = true;


void setup() {
  
  // The Arduino needs to clock out the data to the pixels
  // this happens in interrupt timer 1, we can change how often
  // to call the interrupt. setting CPUmax to 100 will take nearly all all the
  // time to do the pixel updates and a nicer/faster display, 
  // especially with strands of over 100 dots.
  // (Note that the max is 'pessimistic', its probably 10% or 20% less in reality)
  randomSeed(analogRead(0));
  strip.setCPUmax(75);  // start with 50% CPU usage. up this if the strand flickers or is slow
  
  // Start up the LED counter
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  flashFirstPixel();
//  Serial.begin(9600);
}


void loop() {
  runRandom();
}
void runRandom() {
  int rand = random(7) + 1;
  switch(rand) {
    case 1:
      rainbowCheckerboard(random(25, 200));
      rainbowCheckerboard(random(25,200));
      break;
    case 2:
      flashRainbowRows(random(25,200));
      flashRainbowRows(random(25,200));
      break;
    case 3:
      pulseRGB();
      pulseRGB();
      break;
    case 4:
      wipeRGB(random(25,200));
      break;
    case 5:
      rainbow(random(25,200));
      break;
    case 6:
      rainbowCycle(random(25,200));
      break;     
    case 7:
      flashRainbowCols(random(25,200)); 
      flashRainbowCols(random(25,200)); 
      break;
  }
  wipeBlack();
}
void rainbowCheckerboard(uint8_t wait) {
  uint16_t curColor, lastColor;
  int lastColorIndex;
  for (int curColorIndex = 0; curColorIndex < numAllColors; curColorIndex++) {
    lastColorIndex = curColorIndex - 1;
    if (lastColorIndex < 1) {
      lastColorIndex = numAllColors - 1;
    }
    checkerboard(allcolors[curColorIndex], allcolors[lastColorIndex]);
    delay(wait);
  }
}
void lightRow(int row, uint16_t color) {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    int curRow = getRow(curPixel);
    uint16_t setColor = color;
    if (curRow != row) {
      setColor = BLACK;
    }
    strip.setPixelColor(curPixel, setColor);
  }
  
  strip.show();
}

void lightCol(int col, uint16_t color) {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    int curCol = getCol(curPixel);
    uint16_t setColor = color;
    if (curCol != col) {
      setColor = BLACK;
    }
    strip.setPixelColor(curPixel, setColor);
  }
  
  strip.show();
}

void checkerboard(uint16_t color1, uint16_t color2) {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    int row = getRow(curPixel);
    int col = getCol(curPixel);
    uint16_t setColor;
    bool rowIsEven = row%2 == 0;
    bool colIsEven = col%2 == 0;
    if (rowIsEven && colIsEven || (!rowIsEven) && (!colIsEven)) {
      setColor = color1;
    } else {
      setColor = color2;
    }
    
    strip.setPixelColor(curPixel, setColor);
  }
  
  strip.show();
}

int getRow(int val){
//  Serial.print("val,row:");Serial.print(val);Serial.print(",");Serial.println(val / numRows);
  return val / numPerRow;
}

int getCol(int val) {
  return (val + 1) % numPerRow;
}

void flashRainbowRows(uint8_t wait) {
  for (int color = 0; color < 11; color++) {
    flashRows(allcolors[color], wait);
    clearPixels();
  }
}
void flashRainbowCols(uint8_t wait) {
  for (int color = 0; color < 11; color++) {
    flashCols(allcolors[color], wait);
    clearPixels();
  }
}
void clearPixels() {
  for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
    strip.setPixelColor(curPixel, BLACK);
  }
  strip.show();
}

void flashFirstPixel() {
    for (int i = 0; i < 3; i++) {
      strip.setPixelColor(0, WHITE);
      strip.show();
      delay(200);
      strip.setPixelColor(0, BLACK);
      strip.show();
      delay(200);
    }
}
void flashRows(uint16_t color, uint8_t wait) {
  for (int curRow = 0; curRow < 7; curRow++) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int row = getRow(curPixel);
      if (row == curRow) {
        strip.setPixelColor(curPixel, color);
      } else {
        strip.setPixelColor(curPixel, BLACK);
      }
    }
    strip.show();
    delay(wait);
  }
}
void flashCols(uint16_t color, uint8_t wait) {
  for (int curCol = 0; curCol < 6; curCol++) {
    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
      int col = getCol(curPixel);
      if (col == curCol) {
        strip.setPixelColor(curPixel, color);
      } else {
        strip.setPixelColor(curPixel, BLACK);
      }
    }
    strip.show();
    delay(wait);
  }
}
//void flashRows(uint16_t color, uint8_t wait) {
//  for (int curRow = 1; curRow < 8; curRow++) {
//    int curRowMax = curRow * 6;
//    int curRowMin = curRowMax - 6;;
////    Serial.print("curRowMin/curRowMax:");Serial.print(curRowMin);Serial.print("/");Serial.println(curRowMax);
//    for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//      if (curPixel >= curRowMin && curPixel < curRowMax) {
//        strip.setPixelColor(curPixel, color);
//      }
//      else {
//        strip.setPixelColor(curPixel, BLACK);
//      }
//    }
//    strip.show();
//    delay(wait);
//  }
//}

void wipeBlack() { 
  colorWipe(BLACK, 25);
  delay(1000);
}

void wipeRGB(uint8_t wait) {
  for (int i = 0; i < 7; i++) {
    colorWipe(roygbiv[i], wait);
    delay(1000);
  }
}

void pulseRGB() {
   for (int color = 0; color < 3; color++) {
    int redMod, greenMod, blueMod;
    redMod = greenMod = blueMod = 0;
    switch (color) {
       case 0:
         redMod = 1;
         break;
       case 1:
         greenMod = 1;
         break;
       case 2:
         blueMod = 1;
         break;
    }
    pulseEvenOdd(redMod, greenMod, blueMod);
    pulseEvenOdd(redMod, greenMod, blueMod);
   }
}

void pulseEvenOdd(int redMod, int greenMod, int blueMod) {
  int maxValue = 31;
 
    for (int curValue = 0; curValue < maxValue; curValue++) {
      for (int curPixel = 0; curPixel < strip.numPixels(); curPixel++) {
//        if (curPixel > 35) {
//          strip.setPixelColor(curPixel, BLACK);
//        } else {
          if (curPixel%2 == 0) {
             if (evensWaxing) {
              strip.setPixelColor(curPixel, Color(curValue * redMod, curValue * greenMod, curValue * blueMod));
             } else {
               strip.setPixelColor(curPixel, Color((maxValue - curValue) * redMod, (maxValue - curValue) * greenMod, (maxValue - curValue) * blueMod));
             }
          } else {
             if (evensWaxing) {
              strip.setPixelColor(curPixel, Color((maxValue - curValue) * redMod, (maxValue - curValue) * greenMod, (maxValue - curValue) * blueMod));
             } else {
               strip.setPixelColor(curPixel, Color(curValue * redMod, curValue * greenMod, curValue * blueMod));
             }
          }
//        }
      }
      strip.show();
      delay(25);
    }
    evensWaxing = !evensWaxing;  
}
//void pulseOne() {
//  int maxValue = 31;
//  for (int i = 0; i < maxValue; i++) {
//     if (evensWaxing) {
//      strip.setPixelColor(0, Color(i, 0, 0));
//     } else {
//       strip.setPixelColor(0, Color(maxValue - i, 0, 0));
//     }
//    strip.show();
//    delay(25);
//  }
//  evensWaxing = !evensWaxing;    
//}
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

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 96 * 3; j++) {     // 3 cycles of all 96 colors in the wheel
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

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 96 * 5; j++) {     // 5 cycles of all 96 colors in the wheel
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


/* Helper functions */

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}

//Input a value 0 to 127 to get a color value.
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

    
    
