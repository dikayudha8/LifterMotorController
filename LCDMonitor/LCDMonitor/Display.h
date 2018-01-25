#include <LiquidCrystal.h>
#include "UserInput.h"
#include <arduino.h>
#include <math.h>
#define MAXIMUM_HEIGHT 40.0f
#define TRIMMER_CHANNEL 0

#define MANUAL_TOGGLE 4
#define UP_SWITCH 5
#define DOWN_SWITCH 6

#define DO_CALIBRATION 1
#define RESET_CALIBRATION 2

typedef union Float2Bytes{
  float floats;
  uint8_t bytes[4];
};

class Display {
  public:
    Display();
    ~Display();
    void Show();
    void Test();
  private:
    Float2Bytes float2Bytes;
    Float2Bytes highestDesiredPosition;
    UserInput *userKeypad;
    LiquidCrystal *lcd;
    const int rs = 48, en = 44, d4 = 34, d5 = 32, d6 = 30, d7 = 28;
    float desiredPosition = 0; 
    float desiredPositionTemp = 0;   
    static void onReceiving(int num);   
    uint8_t counter;
    int trimmerValueRaw;
    float trimmerValue;
    uint8_t calibrationMode = 0;
    long timeBefore;
    bool manualPrintFlag;
    bool upSwitchFlag;
    bool downSwitchFlag;
    float floatingPointIncrement = 0.0f;
    double intPart = 0.0;
    uint8_t resetCounter = 0;
    uint8_t calibrationCounter;
    bool resetFlag = false;
    bool clearLCD = false;
    float maximumDesiredPos = 0;
};

