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

String patterns[]=
  {
    "1111111111111111", 
    "0001010000000100", 
    "0011000101100000",
    "00110001001001", 
    "10110110110101",
    "110001001010",
    "100100100100",
    "01010010",
    "1001001",
    "0100001",
    "011001",
    "100101",
    "10110",
    "10010"};

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
//  length = random(6, 16);
//  pos = 0;
//  hits = random(1, length);
//
//  // generate random triggers
//  for (int p = 0; p < length; p++){
//    bool isTrig = random(1, 2) % 2 == 0;
//    steps[p] = isTrig;
//  }

  // take random pattern
  int pattern = random(1, 14);

  // get length
  String description = patterns[pattern];
  length = description.length();
  for (int element = 0; element < length; element++){
    if (description.charAt(element) == '0'){
      steps[element] = false;
    }
    else {
      steps[element] = true;
    }
  }
}

/*
  Calculates if next step should be pinged.
  This can be calculated based on algorithm or taken from memory.

  @return true if step should be pinged.
*/
void RandomGenerator::trig() {
  if (pos == 0){
    // send reset signal
    usbMIDI.sendNoteOn(note + 24, 127, 1);
    usbMIDI.sendNoteOff(note + 24, 127, 1);
  }
  pos++;

  if (steps[pos]) {
    usbMIDI.sendNoteOn(note, 127, 1);
    usbMIDI.sendNoteOff(note, 127, 1);
  }
  
  if (pos >= length) {
    pos = 0;  // we need to reset position to avoid overflow
  }
}
