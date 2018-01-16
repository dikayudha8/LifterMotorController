#include "UserInput.h"

long encA,encB;

UserInput::UserInput() {
  keypad = new Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLUMNS);
}

UserInput::~UserInput() {
  delete keypad;
}

char UserInput::ReadUserInput() {
  key = keypad->getKey(); 
  return key; 
}
