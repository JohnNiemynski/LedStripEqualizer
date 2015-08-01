//////////////////////////////////////////////////////////
/*
Led strip equalizer project
Authors: John Niemynski
descrip. : Takes the audio signal from some source, converts the 
signal to the fft equivalent and then outputs that to and LED strip
to create an equalizer. 
*/
//fft handling 
#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/interrupt.h>

#include "wiring.h"

#ifdef __cplusplus
#include "HardwareSerial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#if defined(__AVR_ATmega1280__)
const static uint8_t A0 = 54;
const static uint8_t A1 = 55;
const static uint8_t A2 = 56;
const static uint8_t A3 = 57;
const static uint8_t A4 = 58;
const static uint8_t A5 = 59;
const static uint8_t A6 = 60;
const static uint8_t A7 = 61;
const static uint8_t A8 = 62;
const static uint8_t A9 = 63;
const static uint8_t A10 = 64;
const static uint8_t A11 = 65;
const static uint8_t A12 = 66;
const static uint8_t A13 = 67;
const static uint8_t A14 = 68;
const static uint8_t A15 = 69;
#else
const static uint8_t A0 = 14;
const static uint8_t A1 = 15;
const static uint8_t A2 = 16;
const static uint8_t A3 = 17;
const static uint8_t A4 = 18;
const static uint8_t A5 = 19;
const static uint8_t A6 = 20;
const static uint8_t A7 = 21;
#endif

#endif

#endif
//////////////////////////////////////////////////////////////
//create the fix_fft object
#ifdef FIXFFT_H
#define FIXFFT_H

int fix_fft(char fr[], char fi[], int m, int inverse);
int fix_fftr(char f[], intm m, int inverse);

#endif 

///////////////////////////////////////////////////////////////
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

