/*
fft_adc.pde
guest openmusiclabs.com 8.18.12
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define LOG_OUT 1
#define FFT_N 256 // set to 256 point fft
#define PIN 6 //LED strip outPut pin
#include <FFT.h> // include the library
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

  strip.begin(); // led strip initialize
  strip.show();
}

void loop() {
  for(int l=0; l<64; l++)
    {
      strip.setPixelColor(l+10, (255,255,0));
      strip.show();
    }
    strip.show();
  while(1) { // reduces jitter
    for(int l=0; l<64; l++)
      {
        strip.setPixelColor(l+10, (255,255,0));
        strip.show();
      }
     strip.show();
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log();
    sei();
    for(int l=0; l<64; l++)
    {
      strip.setPixelColor(l+10, (255,255,0));
      strip.show();
    }
    strip.show();
//    for(int j=0; j<128; j++)
//    {
//      Serial.println(fft_log_out[j]);
//      int rvalue = fft_log_out[j];
//      int gvalue = fft_log_out[j];
//      int bvalue = fft_log_out[j];
//      
//      if(fft_log_out[j] >= 25 )
//      {
//        strip.setPixelColor(j+10, (rvalue, gvalue, bvalue) );
//      } 
//        else strip.setPixelColor(j+10,( 0, 0, 0) );
//    }
//    strip.show();  
  }
}




