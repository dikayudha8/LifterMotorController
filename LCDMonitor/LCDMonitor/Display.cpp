#include "Display.h"

char userInput[3];
char toShow[32];
char key;
Display::Display() {
  lcd = new LiquidCrystal(rs, en, d4, d5, d6, d7);
  lcd->clear();
  lcd->begin(16, 2);

  Wire.begin(8);
  Wire.onReceive(onReceiving);
  Serial.begin(9600);
}

Display::~Display() {
  delete lcd;
}

void Display::Show() {
  uint8_t j;
  if (key != 0) {
    if (key == '*') {
      counter = 0;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print("CANCELLED");
      key = 0;
      return;
    }

    if (key == '#' && counter == 0) {
      desiredPositionTemp = 0;
      lcd->clear();
      counter = 0;
    }
    else if (key != '#' && counter < 2) {
      userInput[counter] = key;
      ++counter;
      lcd->clear();      
    }
    else if (key == '#' && counter < 2) {
      desiredPositionTemp = atoi(userInput);
//      lcd->setCursor(0,0);
//      lcd->print("GO!");
      //desiredPosition = desiredPositionTemp;
      counter = 0;
    }
    else if (key != '#' && counter > 1) {
      counter = 0;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      //lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("MAXIMUM IS 50U");

    }
    else if (key == '#' && counter >= 1) {
      desiredPositionTemp = atoi(userInput);
      counter = 0;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
    }
    key = 0;
  }
  trimmerValue = analogRead(TRIMMER_CHANNEL) / 1000.0f;
  if (desiredPositionTemp <= MAXIMUM_HEIGHT) {
    desiredPosition = (float)(desiredPositionTemp) + trimmerValue;
    Serial.println(desiredPosition);
    sprintf(toShow, "%c%c.%02dU", userInput[0], userInput[1], (int)(trimmerValue * 100) % 100);
    lcd->setCursor(0, 0);
    lcd->print(toShow);
  }
  else {
    //lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("MAXIMUM IS 50U");
    desiredPositionTemp = 0;    
    //desiredPosition = (float)MAXIMUM_HEIGHT;
  }
}

static void Display::onReceiving(int num) {
  if (Wire.available() > 0) {
    key = Wire.read();
  }
}


