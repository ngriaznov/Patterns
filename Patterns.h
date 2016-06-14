# define CLOCK 0xF8                           // clock pulse message
# define START 0xFA                           // clock start message
# define STOP 0xFC                            // clock stop message

float bpm = 128;
volatile unsigned long clockInterval = 60000; // clock interval
volatile unsigned long clockCount = 0;        // clock counter

const int ledPin = LED_BUILTIN;               // pin with a LED
int ledState = LOW;                           // blink every whole beat 1/1

volatile bool started = false;                // we'll send start just once
volatile bool reseed = false;                 // if this is tru, we need to restart on next bar
int quantity = 4;                             // quantity of generators

void clockCallback();                         // clock tick task callback
void notifyOnCallback();                      // LED on task callback
void notifyOffCallback();                     // LED off task callback
