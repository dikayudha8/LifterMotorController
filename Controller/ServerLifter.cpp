#include "ServerLifter.h"

ServerLifter::ServerLifter() {
  serialComm = new SerialComm();
  encoder = new Encoder();
  pid = new PID(KP, KI, KD);
}

ServerLifter::~ServerLifter() {
  delete encoder;
  delete pid;
  delete serialComm;
}

void ServerLifter::RunServerLifter() {
  uint8_t motorOutVal = 128;
  if(serialComm->Receiving() == CHECKSUM_INVALID){
    digitalWrite(13, HIGH);  
  } else {
    digitalWrite(13,LOW);
  }
  uint8_t tempMotorOn = serialComm->getMotorOn();
  uint8_t tempMode = serialComm->getMode();

  if (millis() - timeBefore >= UPDATE_RATE) {
    encoder->ReadEncoder();    
    if (tempMode == SEND_DESIRED_POSITION) {
      serialComm->ClearOffset();
      tempDesiredPos = serialComm->getDesiredPosition();
      tempDesiredPos = constrain(tempDesiredPos, 0 , encoder->GetCalibratedMaxHeight());
      motorOutVal = pid->ComputePID(tempDesiredPos, encoder->GetRackHeight());
    } else if (tempMode == SEND_CALIBRATION) {
      serialComm->ClearOffset();
      encoder->Calibrate();
      serialComm->clearMode();
    } else if (tempMode == SEND_RESET_CALIBRATION) {
      serialComm->ClearOffset();
      encoder->ResetCalibration();
      serialComm->clearMode();
    } else if (tempMode == SEND_INITIAL_OFFSET) {
      serialComm->ClearOffset();
      encoder->InitialOffsetCalibration();
      serialComm->clearMode();
      //      Serial.println("SAVE TO EEPROM");
    } else if (tempMode == SEND_MOTOR_OFFSET) {
      tempDesiredPos += serialComm->getOffset();
      tempDesiredPos = constrain(tempDesiredPos, 0 , encoder->GetCalibratedMaxHeight());
      motorOutVal = pid->ComputePID(tempDesiredPos, encoder->GetRackHeight());
    }

    if (tempMotorOn == MOTOR_ON) {
      analogWrite(MOTOR_OUT, motorOutVal);
    } else {
      motorOutVal = PWM_MID;
      tempDesiredPos = encoder->GetRackHeight();
      analogWrite(MOTOR_OUT, PWM_MID);
    }
    if (++tempCounter >= 10) {
      tempCounter = 0;
      //test = !test;
      //digitalWrite(6, test);
//      Serial.print(tempMode);
//      Serial.print("\t");
//      Serial.print(tempMotorOn);
//      Serial.print("\t");
//      Serial.print(tempDesiredPos);
//      Serial.print("\t");
//      Serial.print(serialComm->getOffset());
//      Serial.print("\t");
//      Serial.print(encoder->GetRackHeight());
//      Serial.print("\t");
//      Serial.print(encoder->GetCalibratedMaxHeight());
//      Serial.print("\t");
//      Serial.println(motorOutVal);
    }
    timeBefore = millis();
  }
}

