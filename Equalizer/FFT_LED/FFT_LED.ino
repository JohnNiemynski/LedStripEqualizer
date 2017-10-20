/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define PIN 4
#include <FHT.h> // include the library
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_RGB + NEO_KHZ800); 

void setup() {
  Serial.begin(115200); // use the serial port
  //analogReference(INTERNAL);
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  
  strip.begin();
  strip.show();
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    //int avg_array[FHT_N-1];
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf7; // restart adc @ 10k sample rate
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      //m=0;
      //j = i%32;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      k -= 0x7FFF; //make sure number mean is 0
      //avg_array[i] = k; // save values in dummy array to avg later
      fht_input[i] = k; // put real data into bins
      //Serial.println(k);
      //Serial.println();
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    sei();
    int b=0;
    for(int y=0;y<5;y++)
    {
    if((fht_log_out[b] >= 1)&&(fht_log_out[b] < 20))
          {
            strip.setPixelColor(y, 0,0,fht_log_out[b]);
            strip.setPixelColor(y+137, 0,0,fht_log_out[b]);
          }
          else if((fht_log_out[b] >= 20)&&(fht_log_out[b] < 30))
          {
            strip.setPixelColor(y, fht_log_out[b]/6,fht_log_out[b]/10,fht_log_out[b]);
            strip.setPixelColor(y+137, fht_log_out[b]/6,fht_log_out[b]/10,fht_log_out[b]);
          }
          else if((fht_log_out[b] >= 30)&&(fht_log_out[b] < 40))
          {
            strip.setPixelColor(y, fht_log_out[b]/5,fht_log_out[b]/8,fht_log_out[b]/2);
            strip.setPixelColor(y+137, fht_log_out[b]/5,fht_log_out[b]/8,fht_log_out[b]/2);
          }
          else if ((fht_log_out[b] >= 40) && (fht_log_out[b] < 50))
          {
            strip.setPixelColor(y, fht_log_out[b]/4,fht_log_out[b]/6,fht_log_out[b]/3);
            strip.setPixelColor(y+137, fht_log_out[b]/4,fht_log_out[b]/6,fht_log_out[b]/4);
          }
          else if ((fht_log_out[b] >= 50) && (fht_log_out[b] < 60))
          {
            strip.setPixelColor(y, fht_log_out[b]/3,fht_log_out[b]/4,fht_log_out[b]/4);
            strip.setPixelColor(y+137, fht_log_out[b]/3,fht_log_out[b]/4,fht_log_out[b]/6);
          }
          else if ((fht_log_out[b] >= 60) && (fht_log_out[b] < 70))
          {
            strip.setPixelColor(y, fht_log_out[b]/2,fht_log_out[b]/2,fht_log_out[b]/5);
            strip.setPixelColor(y+137, fht_log_out[b]/2,fht_log_out[b]/2,fht_log_out[b]/8);
          }
          else if ((fht_log_out[b] >= 70) && (fht_log_out[b] < 100))
          {
            strip.setPixelColor(y, fht_log_out[b],fht_log_out[b],0);
            strip.setPixelColor(y+137, fht_log_out[b],fht_log_out[b],0);
          }
          else if (fht_log_out[b] >= 100) 
          {
            strip.setPixelColor(y, 0,fht_log_out[b],0);
            strip.setPixelColor(y+137, 0,fht_log_out[b],0);
          }
          else
          {
            strip.setPixelColor(y, 2,0,4);
            strip.setPixelColor(y+137, 2,0,4);
          }
    }
    for(int b=0; b<128; b++)
    {
        if((fht_log_out[b] >= 1)&&(fht_log_out[b] < 30))
        {
          strip.setPixelColor(b+7, 0,0,fht_log_out[b]);
        }
        else if((fht_log_out[b] >= 30)&&(fht_log_out[b] < 40))
        {
          strip.setPixelColor(b+7, fht_log_out[b]/6,fht_log_out[b]/10,fht_log_out[b]);
        }
        else if((fht_log_out[b] >= 40)&&(fht_log_out[b] < 50))
        {
          strip.setPixelColor(b+7, fht_log_out[b]/5,fht_log_out[b]/8,fht_log_out[b]/2);
        }
        else if ((fht_log_out[b] >= 50) && (fht_log_out[b] < 60))
        {
          strip.setPixelColor(b+7, fht_log_out[b]/4,fht_log_out[b]/6,fht_log_out[b]/4);
        }
        else if ((fht_log_out[b] >= 60) && (fht_log_out[b] < 70))
        {
          strip.setPixelColor(b+7, fht_log_out[b]/3,fht_log_out[b]/4,fht_log_out[b]/6);
        }
        else if ((fht_log_out[b] >= 70) && (fht_log_out[b] < 80))
        {
          strip.setPixelColor(b+7, fht_log_out[b]/2,fht_log_out[b]/2,fht_log_out[b]/8);
        }
        else if ((fht_log_out[b] >= 80) && (fht_log_out[b] < 120))
        {

        }
        else if (fht_log_out[b] >= 120)
        {
          strip.setPixelColor(b+7, 0,fht_log_out[b],0);
        }
        else
        {
          strip.setPixelColor(b+7, 2,0,4);
        }
      }
    strip.show();
  }
}
