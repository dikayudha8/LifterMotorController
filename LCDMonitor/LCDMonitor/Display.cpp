#include "Display.h"

char userInput[3];
char toShow[32];
char calibrationLCD[10];
Display::Display() {
  lcd = new LiquidCrystal(rs, en, d4, d5, d6, d7);
  userKeypad = new UserInput();

  pinMode(MANUAL_TOGGLE, INPUT);
  pinMode(UP_SWITCH, INPUT);
  pinMode(DOWN_SWITCH, INPUT);

  lcd->clear();
  lcd->begin(16, 2);
  Serial.begin(9600);
  Serial3.begin(115200);
}

Display::~Display() {
  delete lcd;
  delete userKeypad;
}

void Display::Test() {
  lcd->setCursor(0, 0);
  lcd->print("OK");
}

void Display::Show() {
  uint8_t j;
  char keyIn = userKeypad->ReadUserInput();
  if (digitalRead(MANUAL_TOGGLE) == LOW && keyIn == 0) {
    if (manualPrintFlag == true) {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("AUTO MODE");
      manualPrintFlag = false;
    }
  }

  if (keyIn != 0 && digitalRead(MANUAL_TOGGLE) == LOW) {
    modf(floatingPointIncrement, &intPart);

    if (keyIn == '*') {
      counter = 0;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("CANCELLED");
      keyIn = 0;
      calibrationCounter = 0;
      resetCounter = 0;
      return;
    }

    if (keyIn == '#' && counter == 0) {
      if (clearLCD == false) {
        lcd->clear();
        clearLCD = true;
      }
      if (++calibrationCounter > 2) {
        calibrationCounter = 0;
        resetCounter = 0;
        calibrationMode = DO_CALIBRATION;
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("CALIBRATION");
        lcd->setCursor(0, 1);
        lcd->print("BEGINS");
        memset(calibrationLCD, 0, sizeof(calibrationLCD) / sizeof(*calibrationLCD));
        clearLCD = false;
        desiredPositionTemp = -floatingPointIncrement;
      } else {
        calibrationLCD[calibrationCounter] = '#';
        if (resetCounter == 2) {
          lcd->setCursor(calibrationCounter + 1, 0);
        } else {
          lcd->setCursor(calibrationCounter - 1, 0);
        }
        lcd->print(calibrationLCD[calibrationCounter]);

        if (resetCounter == 0) { 
          resetCounter = 1;
        }
        else if (resetCounter == 2) {
          resetCounter = 0;
          calibrationMode = RESET_CALIBRATION;
          calibrationCounter = 0;
          lcd->clear();
          lcd->setCursor(0, 0);
          lcd->print("RESET");
          lcd->setCursor(0, 1);
          lcd->print("CALIBRATION");
        }
      }
      counter = 0;
    }
    else if (keyIn == '0' && counter == 0 && resetCounter == 1) {
      resetCounter = 2;
      lcd->setCursor(calibrationCounter, 0);
      lcd->print('0');
    }
    else if (keyIn != '#' && counter < 2) {
      userInput[counter] = keyIn;
      calibrationCounter = 0;
      resetCounter = 0;
      ++counter;
      lcd->clear();
    }
    else if (keyIn == '#' && counter < 2) {
      if (intPart >= 0.0f)
        desiredPositionTemp = (float)(atoi(userInput) -  intPart);
      else
        desiredPositionTemp = (float)(atoi(userInput) - intPart + 1.0f);
      counter = 0;
      calibrationCounter = 0;
      resetCounter = 0;
    }
    else if (keyIn != '#' && counter > 1) {
      counter = 0;
      calibrationCounter = 0;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      //lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("MAXIMUM IS 40U");
      resetCounter = 0;
    }
    else if (keyIn == '#' && counter >= 1) {
      if (intPart >= 0.0f)
        desiredPositionTemp = (float)(atoi(userInput) - intPart);
      else
        desiredPositionTemp = (float)(atoi(userInput) - intPart + 1.0f);
      counter = 0;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      calibrationCounter = 0;
      resetCounter = 0;
    }

    keyIn = 0;

    //we wish to print it once, so the system is not refreshing the lcd and overwrite the previous display when we press up and down.
    //manualPrintFlag = false;
    upSwitchFlag = false;
    downSwitchFlag = false;

  } else if (digitalRead(MANUAL_TOGGLE) == HIGH) {
    calibrationCounter = 0;
    resetCounter = 0;
    if (manualPrintFlag == false) {
      lcd->clear();
      manualPrintFlag = true;
    }
    if (digitalRead(UP_SWITCH) == HIGH) {
      //desiredPositionTemp += 0.1f;
      if (upSwitchFlag == false) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("MOVING UP");
        upSwitchFlag = true;
      }
    } else if (digitalRead(DOWN_SWITCH) == HIGH) {
      //desiredPositionTemp -= 0.1f;
      if (downSwitchFlag == false) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("MOVING DOWN");
        downSwitchFlag = true;
      }
    } else if (digitalRead(UP_SWITCH) == LOW && digitalRead(DOWN_SWITCH) == LOW) {
      if (downSwitchFlag == true || upSwitchFlag == true) {
        lcd->clear();
      }
      downSwitchFlag = false;
      upSwitchFlag = false;
      lcd->setCursor(0, 0);
      lcd->print("MANUAL MODE");
    }
  }

  //trimmerValue = analogRead(TRIMMER_CHANNEL) / 1000.0f;
  if (desiredPositionTemp <= MAXIMUM_HEIGHT) {
    if (digitalRead(MANUAL_TOGGLE) == HIGH) {
      if (digitalRead(UP_SWITCH) == HIGH) {
        floatingPointIncrement += 0.01f;
      } else if (digitalRead(DOWN_SWITCH) == HIGH) {
        floatingPointIncrement -= 0.01f;
      }
    }
    desiredPosition = (float)(desiredPositionTemp) + floatingPointIncrement;// + trimmerValue;
    desiredPosition = constrain(desiredPosition, 0.0f, MAXIMUM_HEIGHT);
    float2Bytes.floats = desiredPosition;
    //Serial.println(desiredPosition);
    if (digitalRead(MANUAL_TOGGLE) == LOW) {
      sprintf(toShow, "%c%c.%02dU", userInput[0], userInput[1], (int)(trimmerValue * 100) % 100);
      lcd->setCursor(0, 0);
      lcd->print(toShow);
    }
  }
  else {
    //lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("MAXIMUM IS 40U");
    desiredPositionTemp = desiredPosition - floatingPointIncrement;// - intPart;
    //desiredPosition = (float)MAXIMUM_HEIGHT;
  }

  Serial.print(desiredPosition);
  Serial.print("\t");
  Serial.print(floatingPointIncrement);
  Serial.print("\t");
  Serial.print(intPart);
  Serial.print("\t");
  Serial.println(calibrationMode);

  //communicate to motor controller by receiving 'a' for synchronization, then send 0xFF 0xXX 0xXX 0xXX 0xXX as 1 header and 4 bytes desired position pressed by user
  if (Serial3.available()) {
    if (Serial3.read() == 'a') {
      //send the communication header
      Serial3.write(0xFF);
      //sending 4 bytes floating point as user's input
      for (uint8_t i = 0; i < 4; ++i) {
        Serial3.write(float2Bytes.bytes[i]);
      }
      Serial3.write(calibrationMode);
      if (calibrationMode > 0) {
        calibrationMode = 0;
      }
    }

    if (Serial3.read() == 's') {
      while (Serial.read() != 0xFF);
      for (uint8_t i = 0; i < 4; ++i) {
        highestDesiredPosition.bytes[i] = Serial3.read();
      }
    }
  }
}


