#include "LifterLCD.h"

//Display *display;
LifterLCD *lifterLCD;

void setup() {
  // set up the LCD's number of columns and rows:  
  lifterLCD = new LifterLCD();
  
}

void loop() {
  lifterLCD->UpdateDisplay();  
}
