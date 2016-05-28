/*
  Patterns Sketch

  This is a header for a various trigger generators implementation.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

#ifndef GENERATOR_H
#define GENERATOR_H
#endif

class TriggerGenerator
{
    public:
        virtual void seed() = 0;
        virtual bool trig() = 0;
        int lenght = 0;
        int hits = 0;
        int pos = 0;
        int divider = 1;
        
        virtual ~TriggerGenerator() {} 
        
};
