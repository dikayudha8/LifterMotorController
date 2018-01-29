#include "LifterLCD.h"

LifterLCD::LifterLCD() {
  lcd = new LiquidCrystal(rs, en, d4, d5, d6, d7);
  userInput = new UserInput();
  Serial.begin(115200);
}

LifterLCD::~LifterLCD() {
  delete lcd;
  delete userInput;
}

void LifterLCD::Show(uint16_t digitToShow, uint8_t xStartPoint = 0, uint8_t yStartPoint = 0) {
  char tempBuffer[32];
  sprintf(tempBuffer, "%d", digitToShow);
  lcd->setCursor(xStartPoint, yStartPoint);
  lcd->print(tempBuffer);
}

void LifterLCD::Show(char *toPrint, uint16_t maxPosition, uint8_t xStartPoint = 0, uint8_t yStartPoint = 0) {
  char tempBuffer[32];
  sprintf(tempBuffer, "%s%d", toPrint, maxPosition);
  lcd->setCursor(xStartPoint, yStartPoint);
  lcd->print(tempBuffer);
}

void LifterLCD::Show(char *toPrint, uint8_t xStartPoint = 0, uint8_t yStartPoint = 0) {
  lcd->setCursor(xStartPoint, yStartPoint);
  lcd->print(toPrint);
}

void LifterLCD::UpdateDisplay() {
  char *receivingUserInput = userInput->ReadUserInput();
  if (millis() - timeBefore >= 50) {
    if (userInput->getPrintLCDStatus() == NORMAL) {      
      lcd->clear();
      Show(receivingUserInput);
    } else if (userInput->getPrintLCDStatus() == WARNING) {      
      lcd->clear();            
      Show("MAXIMUM IS ", userInput->getMaxDesiredPosition());
    } else if (userInput->getPrintLCDStatus() == CANCEL) {
      lcd->clear();
      Show("CANCELED");
    } else if (userInput->getPrintLCDStatus() == CALIBRATING) {
      lcd->clear();
      Show("CALIBRATING");
    } else if (userInput->getPrintLCDStatus() == RESET_CALIBRATION) {
      lcd->clear();
      Show("RESET CALIBRATION");
    } else if (userInput->getPrintLCDStatus() == MANUAL) {
      lcd->clear();
      Show("MANUAL MODE");
    } else if (userInput->getPrintLCDStatus() == MOVING_UP) {
      lcd->clear();
      Show("MOVING UP");
    } else if (userInput->getPrintLCDStatus() == MOVING_DOWN) {
      lcd->clear();
      Show("MOVING DOWN");
    } else if (userInput->getPrintLCDStatus() == DEFAULT_){
      lcd->clear();
      Show("AUTO MODE");
    }

    timeBefore = millis();
  }  
}

