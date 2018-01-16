#include <Key.h>
#include <Keypad.h>

#define ROWS 4
#define COLUMNS 3
#define MAXIMUM_HEIGHT 50

#define ENC_A 2
#define ENC_B 3

union Int2Byte{
  int integer;
  byte bytes[2];
};

class UserInput {
  public :
    UserInput();
    ~UserInput();

    char ReadUserInput();
    uint8_t GetDesiredPosition();
  private:
    Keypad *keypad;   
    uint8_t desiredPosition;
    uint8_t desiredPositionTemp;
    char keys[ROWS][COLUMNS] =
    { {'1', '2', '3'},
      {'4', '5', '6'},
      {'7', '8', '9'},
      {'*', '0', '#'}
    };
    byte rowPins[ROWS] = {53, 51, 49, 47};//{9, 10, 4, 5}; //connect to the row pinouts of the keypad
    byte colPins[COLUMNS] = {45, 43, 41};//{6, 7, 8}; //connect to the column pinouts of the keypad

    char key;
    char userInput[3];
    uint8_t counter;
    Int2Byte converter;    
    long diff;
    uint8_t direction;
    
    static void pin_ISR_A();
    static void pin_ISR_B();
};


