/*
  Patterns Sketch

  Actual implementation of euclidian generator.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

#include "EuclidianGenerator.h"
#include <cstdlib>
#include <Arduino.h>

bool steptrig;

/**
  Seeds current generator with new data.
*/
void EuclidianGenerator::seed()
{
  randomSeed(analogRead(A0)); 
  
  divider = 6;
  lenght = random(1, 16);
  pos = 0;
  hits = random(1, 12);
}

/**
  Calculates if next step should be pinged.
  This can be calculated based on algorithm or taken from memory.

  @return true if step should be pinged.
*/
bool EuclidianGenerator::trig()
{

    /*  generates euclideian patterns 
        c: current step number 
        k: hits per bar 
        n: bar length 
        r: rotation 
        returns true or false according to the euclidean pattern
        return (((c + r) * k) % n) < k; */ 
    pos++;
    steptrig = (((pos + 1) * hits) % lenght) < hits; 

    if (pos >= lenght){
      pos = 0; // we need to reset position to avoid overflow
    }

    return steptrig;
}
