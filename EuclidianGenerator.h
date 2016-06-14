/*
  Patterns Sketch

  This is a header for Euclidian generator implementation.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/
#include <cstdlib>
#include <Arduino.h>

#ifndef TriggerGenerator_H
#define TriggerGenerator_H
   #include "TriggerGenerator.h"
#endif

class EuclidianGenerator : public TriggerGenerator {
 public:
  virtual void seed();
  virtual void trig();
  EuclidianGenerator(int note, int a);
};

/*
   Constructor.
*/
EuclidianGenerator::EuclidianGenerator(int n, int a) {
  note = n;
  address = 0;
}

/*
  Seeds current generator with new data.
*/
void EuclidianGenerator::seed() {
  randomSeed(analogRead(A0));

  divider = 6;
  length = random(1, 32);
  pos = 0;
  hits = random(1, 4);
}

/*
  Calculates if next step should be pinged.
  This can be calculated based on algorithm or taken from memory.

  @return true if step should be pinged.
*/
void EuclidianGenerator::trig() {
  if (pos == 0){
    // send reset signal
    usbMIDI.sendNoteOn(note + 24, 127, 1);
    usbMIDI.sendNoteOff(note + 24, 127, 1);
  }
  
  /*  generates euclideian patterns
      c: current step number
      k: hits per bar
      n: bar length
      r: rotation
      returns true or false according to the euclidean pattern
      return (((c + r) * k) % n) < k; */
  pos++;

  if ((((pos + 1) * hits) % length) < hits) {
    usbMIDI.sendNoteOn(note, 127, 1);
    usbMIDI.sendNoteOff(note, 127, 1);
  }

  if (pos >= length) {
    pos = 0;  // we need to reset position to avoid overflow
  }
}
