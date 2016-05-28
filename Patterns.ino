/*
  Patterns Sketch

  This is a sketch for sending USB MIDI clock and triggers
  that are following several algorithms.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

#include <MIDI.h>
#include <FrequencyTimer2.h>
#include "EuclidianGenerator.h"

#define                       CLOCK                 0xF8                // clock pulse message    
#define                       START                 0xFA                // clock start message
#define                       STOP                  0xFC                // clock stop message

float                         bpm                 = 130.0;             
volatile unsigned long        clockInterval;     

const int                     ledPin              = LED_BUILTIN;        // pin with a LED
int                           ledState            = LOW;                // blink every whole beat 1/1

volatile unsigned long        clockCount          = 0;                  // clock counter
volatile bool                 change              = false;              // track clock changes

volatile bool                 started             = false;              // we'll send start just once 

TriggerGenerator**            generators          = new TriggerGenerator*[9];

// setup everything
void setup(void) {

  randomSeed(analogRead(A0)); 
  
  // reserve led action
  pinMode(ledPin, OUTPUT);

  // limit range
  bpm = constrain(bpm, 60.0, 240.0);
  
  // initialize clock interrupt, interval is in microseconds
  clockInterval = 60000000/(24 * bpm);
  
  // give some time for host device to initialize (optional)
  delay(2000);

  // setup generators
  for (int i = 1; i <= 9; i++) {
    generators[i] = new EuclidianGenerator();
    generators[i]->seed();
  }

  // set the timer. this zero-jitter timer, but only one available
  FrequencyTimer2::setPeriod(clockInterval);
  FrequencyTimer2::setOnOverflow(clockTick);
  FrequencyTimer2::enable();
}

// main clock action
void clockTick(void) {
  
  if (!started){
    // send first start signal
    usbMIDI.sendRealTime(START);
    started = true;
  }

  // increase clock count
  clockCount++;
  change = true;
  
  // send clock signal
  usbMIDI.sendRealTime(CLOCK);
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
  
  for (int i = 1; i <= 9; i++) {
    if (clockCount%generators[i]->divider == 0) {
      // process next trig
      if (generators[i]->trig()){
        usbMIDI.sendNoteOn(23 + i, 127, 1);
        usbMIDI.sendNoteOff(23 + i, 127, 1);     
      }
    }
  }
}

void loop(void) {

  if (change == true){
    // run division processing
    processDevisions();
      
    if (clockCount%1536 == 0){
      // Reset clock count each 16 bars
      clockCount = 0;
    }

    change = false;
  }
}
