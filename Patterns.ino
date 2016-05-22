#include <MIDI.h>
#include <FrequencyTimer2.h>

  struct Pattern
  {
    int pLength;      // bar length
    int pHits;        // hits per bar
    int pStep;        // current step number 
  } ;

#define                       CLOCK                 0xF8                // clock pulse message    
#define                       START                 0xFA                // clock start message
#define                       STOP                  0xFC                // clock stop message

float                         bpm                 = 130.0;             
volatile unsigned long        clockInterval;     

const int                     ledPin              = LED_BUILTIN;        // pin with a LED
int                           ledState            = LOW;                // blink every whole beat 1/1

volatile unsigned long        clockCount          = 0;                      // clock counter, used for LED 
volatile bool                 started             = false;              // we'll send start just once 
Pattern                       mainPattern         = {16, 3, 1};

// main clock action
void clockTick(void) {
  
  if (!started){
    // send first start signal
    usbMIDI.sendRealTime(START);
    started = true;
  }

  // increase clock count
  clockCount++;

  // send clock signal
  usbMIDI.sendRealTime(CLOCK);

  // run division processing
  processDevisions();

  if (clockCount%1536 == 0){
    // Reset clock count each 16 bars
    clockCount = 0;
  }
}

void processDevisions(){
  // blink led at 1/4 of the beat
  if (clockCount%24 == 0){

    // light up the led
    notify(true);
  }
  else {
    // turn off the led
    notify(false);
  }

  // process 16-th clock division
  if (clockCount%6 == 0){
    if (mainPattern.pStep > mainPattern.pLength) {
      mainPattern.pStep = 1;
    }
    // Calculate if we should pulse or not
    bool pulse = euclidian(mainPattern.pStep, mainPattern.pHits, mainPattern.pLength, 1);
    mainPattern.pStep++;

    if (pulse){
      // Pulse current sequence
    }
    else {
      
    }
  }
}

void loop(void) {

  // here we need to skip incoming messages
  while (usbMIDI.read()) {
  }
}
