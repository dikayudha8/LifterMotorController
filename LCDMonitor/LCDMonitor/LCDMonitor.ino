#include "Display.h"

Display *display;

void setup() {
  // set up the LCD's number of columns and rows:
  display = new Display();
  //Serial.begin(9600);
  
}

void loop() {
  //display->Test();
  // set the cursor to column 0, line 1
  display->Show();  
  //Serial.println("test");
}
