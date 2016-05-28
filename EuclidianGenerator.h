/*
  Patterns Sketch

  This is a header for Euclidian generator implementation.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

#include "TriggerGenerator.h"

class EuclidianGenerator : public TriggerGenerator
{
    public:
        virtual void seed();
        virtual bool trig();
};

