/****************************************
Scrolling sound level meter
Richard Hawthorn
richardhawthorn.com
Cambridge Hackspace
Aug 2015
For use with SoundMeter board
Based on code from Adafruit
****************************************/
 
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
 
// Include the Matrix code
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
 
const int maxScale = 8;
const int redZone = 6;
const int yellowZone = 3;
 
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
void setup() 
{
   analogReference(EXTERNAL); //use external voltage reference
   matrix.begin(0x70);  // start the matrix display
}
 
void loop() 
{
   unsigned long startMillis = millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   //sample for a set period of time
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0); 
      
      if (sample < 1024) //make sure we are in the right range
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save max value
         }
         else if (sample < signalMin)
         {
            signalMin = sample; //save sample value
         }
      }
   }
   peakToPeak = signalMax - signalMin;
   
   // map the peaks to the scale
   int displayPeak = map(peakToPeak, 0, 1023, 0, maxScale);
 
   // shift the previous values left
   for (int i = 0; i < 8; i++)
   {
      matrix.displaybuffer[i] = (matrix.displaybuffer[i] >> 1);
   }
 
   // draw the new value
   for (int i = 0; i <= maxScale; i++)
   {
      if (i >= displayPeak)
      {
         matrix.drawPixel(7, 7-i, 0);
      }
      else if (i < yellowZone) // green
      {
         matrix.drawPixel(7, 7-i, LED_GREEN);
      }
      else if (i < redZone) // yellow
      {
         matrix.drawPixel(7, 7-i, LED_YELLOW);
      }
      else // red
      {
         matrix.drawPixel(7, 7-i, LED_RED);
      }
   }
   matrix.writeDisplay();  // write the changes we just made to the display
}
