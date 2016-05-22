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
