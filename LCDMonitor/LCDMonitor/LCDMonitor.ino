#include "LifterLCD.h"

//Display *display;
LifterLCD *lifterLCD;

void setup() {   
  lifterLCD = new LifterLCD();  
}

void loop() {
  lifterLCD->UpdateDisplay();  
}
