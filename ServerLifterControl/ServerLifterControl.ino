#include "UserInput.h"

UserInput *userInput;

void setup() {
  // put your setup code here, to run once:
  userInput = new UserInput();
}

void loop() {
  // put your main code here, to run repeatedly:
  userInput->ReadUserInput();
}
