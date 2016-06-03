/*
  Trigger generator interface class

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

class TriggerGenerator {
 public:
  virtual void seed() = 0;
  virtual void trig() = 0;
  int lenght = 0;
  int hits = 0;
  int pos = 0;
  int divider = 1;
  int note = 0;
  int address = 0;

  virtual ~TriggerGenerator() {}
};
