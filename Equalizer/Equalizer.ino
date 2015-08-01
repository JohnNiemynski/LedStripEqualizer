

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
#include <avr/pgmspace.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define pin_adc A4 //input pin from audio DIP
#define pin_led 7//output pin to LED strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(128, PIN, NEO_GRB + NEO_KHZ800);
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
       val = analogRead(pin_adc);
       data[i] = val / 4 - 128;
       im[i] = 0;
       i++;   
       
     }
     else{
       //this could be done with the fix_fftr function without the im array.
       fix_fft(data,im,7,0);
       // I am only interested in the absolute value of the transformation
       // i must follow a power of two for the fft to work properly
       for (i=0; i< 64;i++){
          data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
       }
       
       //do something with the data values 1..64 and ignore im
       //(ignore im may change because the im values might hold 
       //the amplitude at a paticular frequency, we'll see though
       show_equalizer(data);
     }
   
   tt = millis();
  }
}

void show_equalizer(char data)
//handing of fft object to output different colors on LED strip
//we want to change the color based on the amplitude at any given
//frequency. Also remember # of LEDs must be a power of two. 
{
  
  for(int j=0; j< 64; j++)
  {
    Serial.println(data[j]);//testing to see what interger values we should use
  }
}

