/*
  Patterns Sketch

  This is a sketch for sending USB MIDI clock and triggers
  that are following several algorithms.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

// notify the user with whole note beat using on-board LED
void notify(bool on){
  if (on) {    
    ledState = HIGH;      
  } 
  else {
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);
}
