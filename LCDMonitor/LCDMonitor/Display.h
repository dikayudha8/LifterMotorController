#include <LiquidCrystal.h>
#include <Wire.h>
#include <arduino.h>
#define MAXIMUM_HEIGHT 50
#define TRIMMER_CHANNEL 0
class Display {
  public:
    Display();
    ~Display();
    void Show();
  private:
    LiquidCrystal *lcd;
    const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    float desiredPosition = 0; 
    int desiredPositionTemp = 0;   
    static void onReceiving(int num);   
    uint8_t counter;
    int trimmerValueRaw;
    float trimmerValue;
};

