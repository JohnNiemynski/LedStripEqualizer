
//////////////////////////////////////////////////////////
/*
Led strip equalizer project
Authors: John Niemynski
descrip. : Takes the audio signal from some source, converts the 
signal to the fft equivalent and then outputs that to and LED strip
to create an equalizer. 
*/
///////////////////////////////////////////////////////////////
#include "fix_fft.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define ADC_PIN A4 //input pin from audio DIP
#define LED_PIN 7//output pin to LED strip

const prog_int8_t Sinewave[N_WAVE-N_WAVE/4] PROGMEM = {
0, 3, 6, 9, 12, 15, 18, 21, 
24, 28, 31, 34, 37, 40, 43, 46, 
48, 51, 54, 57, 60, 63, 65, 68, 
71, 73, 76, 78, 81, 83, 85, 88, 
90, 92, 94, 96, 98, 100, 102, 104, 
106, 108, 109, 111, 112, 114, 115, 117, 
118, 119, 120, 121, 122, 123, 124, 124, 
125, 126, 126, 127, 127, 127, 127, 127, 

127, 127, 127, 127, 127, 127, 126, 126, 
125, 124, 124, 123, 122, 121, 120, 119, 
118, 117, 115, 114, 112, 111, 109, 108, 
106, 104, 102, 100, 98, 96, 94, 92, 
90, 88, 85, 83, 81, 78, 76, 73, 
71, 68, 65, 63, 60, 57, 54, 51, 
48, 46, 43, 40, 37, 34, 31, 28, 
24, 21, 18, 15, 12, 9, 6, 3, 

0, -3, -6, -9, -12, -15, -18, -21, 
-24, -28, -31, -34, -37, -40, -43, -46, 
-48, -51, -54, -57, -60, -63, -65, -68, 
-71, -73, -76, -78, -81, -83, -85, -88, 
-90, -92, -94, -96, -98, -100, -102, -104, 
-106, -108, -109, -111, -112, -114, -115, -117, 
-118, -119, -120, -121, -122, -123, -124, -124, 
-125, -126, -126, -127, -127, -127, -127, -127, 

/*-127, -127, -127, -127, -127, -127, -126, -126, 
-125, -124, -124, -123, -122, -121, -120, -119, 
-118, -117, -115, -114, -112, -111, -109, -108, 
-106, -104, -102, -100, -98, -96, -94, -92, 
-90, -88, -85, -83, -81, -78, -76, -73, 
-71, -68, -65, -63, -60, -57, -54, -51, 
-48, -46, -43, -40, -37, -34, -31, -28, 
-24, -21, -18, -15, -12, -9, -6, -3, */
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(128, LED_PIN, NEO_GRB + NEO_KHZ800);
//create the LED strip object based off of adafruit library

//im and real array objects for fft
char im[128];
char data[128]; 

void setup()
{ 
    /* Initialize serial comm port */
    Serial.begin(9600); 
    strip.begin();//initializes led strip
}
 

void loop(){
  /*
  this should give an fft with 
- sampling rate:           1ms
- frequency resolution:  500Hz
- lowest frequency:       7.8Hz
 */
 
 int static i = 0;
 static long tt;//place holder time for fft sampling
 int val;//integer value read from audio 
  
  if (millis() > tt){
     if (i < 128){
       val = analogRead(ADC_PIN);
       data[i] = val / 4 - 128;
       im[i] = 0;
       i++;   
       
     }
     else{
       //this could be done with the fix_fftr function without the im array.
       fix_fft(data,im,7,0, val);
       // I am only interested in the absolute value of the transformation
       // i must follow a power of two for the fft to work properly
       for (i=0; i< 64;i++){
          data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
       }
       
       //do something with the data values 1..64 and ignore im
       //(ignore im may change because the im values might hold 
       //the amplitude at a paticular frequency, we'll see though
     }
   
   tt = millis();
  }
}


