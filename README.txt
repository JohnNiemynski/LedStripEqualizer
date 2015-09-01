Need to fix the FFT libraries so that way I can work with them.

The issue lies in FFT.cpp.

I believe there is a type error with one of the function.
Since the code used to use <Wprogram> however this is out of date
and is replaced with <arduino.h>, so that might have something to
do with the type error. 

The error itself lies inside one of the functions and it has to deal with 
the interesting sinewave that gets declared