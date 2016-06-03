/*
  Patterns Sketch

  This is a header for Random generator implementation.

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

class RandomGenerator : public TriggerGenerator {
 public:
  virtual void seed();
  virtual void trig();
  RandomGenerator(int note, int a);
 private:
  bool* steps = new bool[16];
};

/*
   Constructor.
*/
RandomGenerator::RandomGenerator(int n, int a) {
  note = n;
  address = 0;
}

/*
  Seeds current generator with new data.
*/
void RandomGenerator::seed() {
  randomSeed(analogRead(A0));

  divider = 6;
  length = random(6, 16);
  pos = 0;
  hits = random(1, length);

  // generate random triggers
  for (int p = 0; p < length; p++){
    bool isTrig = random(1, 2) % 2 == 0;
    steps[p] = isTrig;
  }
}

/*
  Calculates if next step should be pinged.
  This can be calculated based on algorithm or taken from memory.

  @return true if step should be pinged.
*/
void RandomGenerator::trig() {
  pos++;

  if (steps[pos]) {
    usbMIDI.sendNoteOn(note, 127, 1);
    usbMIDI.sendNoteOff(note, 127, 1);
  }
  
  if (pos >= length) {
    pos = 0;  // we need to reset position to avoid overflow
  }
}
