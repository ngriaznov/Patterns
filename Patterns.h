#ifndef Patterns_h
#define Patterns_h
  
#define                       CLOCK                 0xF8                // clock pulse message    
#define                       START                 0xFA                // clock start message
#define                       STOP                  0xFC                // clock stop message

float                         bpm                 = 130.0;             
volatile unsigned long        clockInterval       = 60000;     
volatile unsigned long        clockCount          = 0;                  // clock counter

const int                     ledPin              = LED_BUILTIN;        // pin with a LED
int                           ledState            = LOW;                // blink every whole beat 1/1

volatile bool                 started             = false;              // we'll send start just once 
int                           quantity            = 8;

void clockCallback();
void notifyOnCallback();
void notifyOffCallback();
#endif
