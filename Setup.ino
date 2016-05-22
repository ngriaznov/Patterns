// setup everything
void setup(void) {
  
  // reserve led action
  pinMode(ledPin, OUTPUT);

  // limit range
  bpm = constrain(bpm, 60.0, 240.0);
  
  // initialize clock interrupt, interval is in microseconds
  clockInterval = 60000000/(24 * bpm);

  // give some time for host device to initialize (optional)
  delay(2000);

  // set the timer. this zero-jitter timer, but only one available
  FrequencyTimer2::setPeriod(clockInterval);
  FrequencyTimer2::setOnOverflow(clockTick);
  FrequencyTimer2::enable();
}
