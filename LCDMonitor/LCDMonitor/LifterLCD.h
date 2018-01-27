#include <LiquidCrystal.h>
#include <LiquidCrystal.h>
#include "UserInput.h"
#include <arduino.h>
#include <math.h>

class LifterLCD {
  public :
    LifterLCD();
    ~LifterLCD();
    void UpdateDisplay();
  private :
    LiquidCrystal *lcd;
    UserInput *userInput;
    uint8_t clearLCD = 0;
    uint8_t clearLCDStatus = 0;
    const int rs = 48, en = 44, d4 = 34, d5 = 32, d6 = 30, d7 = 28;
    void Show(uint16_t digitToShow, uint8_t xStartPoint = 0, uint8_t yStartPoint = 0);
    void Show(char *toPrint, uint16_t maxPosition, uint8_t xStartPoint = 0, uint8_t yStartPoint = 0);
    void Show(char *toPrint, uint8_t xStartPoint = 0, uint8_t yStartPoint = 0);
    uint8_t Refresh();
    long timeBefore;    
};

