#include "UserInput.h"

long encA,encB;

UserInput::UserInput() {
  keypad = new Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLUMNS);
  Serial.begin(9600);
  Wire.begin();  
}

UserInput::~UserInput() {
  delete keypad;
}

void UserInput::ReadUserInput() {
  key = keypad->getKey();
  if (key != 0) {    
    Wire.beginTransmission(8);
    Wire.write(key);    
    Wire.endTransmission();    
  }

  //Serial.println(encA);
}
