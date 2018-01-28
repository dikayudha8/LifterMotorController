#include <Key.h>
#include <Keypad.h>
#include <arduino.h>
#include "SerialCommand.h"

#define ROWS 4
#define COLUMNS 3
#define MAXIMUM_HEIGHT 40

#define ENC_A 2
#define ENC_B 3

#define MANUAL_TOGGLE 4
#define UP_SWITCH 5
#define DOWN_SWITCH 6

#define U_PER_COUNT 75.73164660491116f

enum {
  NORMAL,  
  WARNING,
  CANCEL,
  CALIBRATING,
  RESET_CALIBRATION,
  MANUAL,
  MOVING_UP,
  MOVING_DOWN,
  DEFAULT_
};

typedef union Int2Byte {
  int integer;
  byte bytes[2];
};

class UserInput {
  public :
    UserInput();
    ~UserInput();

    char* ReadUserInput();
    void setPrintLCDStatus(uint8_t printMode) {
      printLCDStatus = printMode;
    };
    uint8_t getPrintLCDStatus() {
      return printLCDStatus;
    };
    uint8_t getDesiredPosition() {
      return desiredPosition;
    };
    uint16_t getMaxDesiredPosition() {
      return MAXDESIREDPOS;
    };
    
    uint8_t getOperationMode() {
      return operationMode;
    };
    
    uint8_t counter = 0;
  private:
    SerialCommand *serialCommand;
    Keypad *keypad;
    uint8_t communicationStatus = 0;
    uint8_t calibrationCounter = 0;
    uint8_t resetCalibrationCounter = 0;
    uint16_t MAXDESIREDPOS = MAXIMUM_HEIGHT;
    uint8_t printLCDStatus = NORMAL;
    uint8_t desiredPosition = 0;
    uint8_t desiredPositionTemp = 0;
    float desiredPositionInFloat = 0.0f;
    uint16_t desiredPositionToSend = 0;
    uint8_t operationMode = 0;
    char keys[ROWS][COLUMNS] =
    { {'1', '2', '3'},
      {'4', '5', '6'},
      {'7', '8', '9'},
      {'*', '0', '#'}
    };
    byte rowPins[ROWS] = {53, 51, 49, 47};//{9, 10, 4, 5}; //connect to the row pinouts of the keypad
    byte colPins[COLUMNS] = {45, 43, 41};//{6, 7, 8}; //connect to the column pinouts of the keypad

    char key;
    char userInput[4];
    
    Int2Byte converter;
    long diff;
    uint8_t direction;
    bool switchState = false;
};


