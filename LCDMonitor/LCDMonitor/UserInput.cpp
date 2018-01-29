#include "UserInput.h"

long encA, encB;

UserInput::UserInput() {
  keypad = new Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLUMNS);
  serialCommand = new SerialCommand();
  memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
  pinMode(MANUAL_TOGGLE, INPUT);
  pinMode(UP_SWITCH, INPUT);
  pinMode(DOWN_SWITCH, INPUT);

  //serialCommand->SendDesiredPositionCommand(0, 0);
}

UserInput::~UserInput() {
  delete keypad;
  delete serialCommand;
}

char* UserInput::ReadUserInput() {
  if (digitalRead(MANUAL_TOGGLE) == HIGH) {
    if (switchState == false) {
      printLCDStatus = DEFAULT_;
      switchState = true;
    }

    key = keypad->getKey();
    if (key != 0 && key != '#' && key != '*') {
      calibrationCounter = 0;
      printLCDStatus = NORMAL;
      if (key == '0' && resetCalibrationCounter == 1) {
        userInput[resetCalibrationCounter] = key;
        resetCalibrationCounter = 2;
        counter = 0;
      } else {
        userInput[counter] = key;
        resetCalibrationCounter = 0;
        if (++counter > 2) {
          counter = 0;
          memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
          printLCDStatus = WARNING;
        }
      }
    }
    else if (key == '#' && (counter > 0 || counter == 2 ) && (resetCalibrationCounter == 0 || calibrationCounter == 0)) {
      calibrationCounter = 0;
      resetCalibrationCounter = 0;
      counter = 0;
      desiredPositionTemp = atoi(userInput);
      if (desiredPositionTemp >= MAXDESIREDPOS ) {
        desiredPositionTemp = 0;
        printLCDStatus = WARNING;
        memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      } else {
        memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
        desiredPosition = desiredPositionTemp;
        //send the desired Command and moving command
        desiredPositionInFloat = ((float)desiredPosition) * U_PER_COUNT;
        desiredPositionToSend = (unsigned int) desiredPositionInFloat;
        //serialCommand->SendMotorOn(true);
        for (uint8_t i = 0; i < ATTEMPT; ++i) {
          serialCommand->SendDesiredPositionCommand(desiredPositionToSend, 1);
          delay(10);
        }
        //Serial.println(desiredPositionToSend);
      }
    }
    else if (key == '#' && counter == 0 && calibrationCounter < 4) {
      printLCDStatus = NORMAL;
      userInput[calibrationCounter] = key;
      ++calibrationCounter;
      if (resetCalibrationCounter == 0)
        resetCalibrationCounter = 1;
      else if (resetCalibrationCounter == 2) {
        memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
        resetCalibrationCounter = 0;
        printLCDStatus = RESET_CALIBRATION;
        operationMode = RESET_CALIBRATION;
        calibrationCounter = 0;
        //send reset command and stop moving command
        //serialCommand->SendMotorOn(false);
        for (uint8_t i = 0; i < ATTEMPT; ++i) {
          serialCommand->SendResetCalibrationCommand(true);
          delay(10);
        }
      }

      if (calibrationCounter > 3) {
        memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
        calibrationCounter = 0;
        memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
        printLCDStatus = CALIBRATING;
        operationMode = CALIBRATING;
        //send calibration command and stop moving command
        //serialCommand->SendMotorOn(false);
        for (uint8_t i = 0; i < ATTEMPT; ++i) {
          serialCommand->SendCalibrationCommand(true);
          delay(10);
        }
      }
    }
    else if (key == '*') {
      printLCDStatus = CANCEL;
      memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
      counter = 0;
      resetCalibrationCounter = 0;
      calibrationCounter = 0;
    }
  } else {
    switchState = false;
    memset(userInput, 0, sizeof(userInput) / sizeof(*userInput));
    if (digitalRead(UP_SWITCH) == HIGH) {
      printLCDStatus = MOVING_UP;
      operationMode = MOVING_UP;
      Serial.println("UP");
      //send the offset command and moving command
      //serialCommand->SendOffsetCommand(2);
    } else if (digitalRead(DOWN_SWITCH) == HIGH) {
      printLCDStatus = MOVING_DOWN;
      operationMode = MOVING_DOWN;
      Serial.println("DOWN");
      //serialCommand->SendOffsetCommand(-2);
    } else {
      printLCDStatus = MANUAL;
      operationMode = MANUAL;
      Serial.println("MANUAL");
      //serialCommand->SendOffsetCommand(0);
    }

    if (millis() - timeBefore >= 10) {
      if (operationMode == MANUAL) {
        serialCommand->SendOffsetCommand(0,0);
//        if (firstBoot == true) {
//          serialCommand->SendOffsetCommand(0, 0);          
//        } else
//          serialCommand->SendOffsetCommand(0, 1);
      } else if (operationMode == MOVING_UP) {
        //firstBoot = false;
        serialCommand->SendOffsetCommand(1, 1);
        
      } else if (operationMode == MOVING_DOWN) {
        //firstBoot = false;
        serialCommand->SendOffsetCommand(-1, 1);
        
      }

      timeBefore = millis();
    }
  }
  //Serial.println(digitalRead(LED_INPUT));
  return userInput;
}
