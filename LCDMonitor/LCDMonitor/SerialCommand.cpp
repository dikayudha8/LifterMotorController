#include "SerialCommand.h"

SerialCommand::SerialCommand() {
  if (DEBUG == true) {
    Serial.begin(115200);
  } else {
    Serial3.begin(115200);
  }
  pinMode(LED_INPUT, INPUT);
}

SerialCommand::~SerialCommand() {

}

void SerialCommand::SendCommand(uint8_t mode, uint8_t motorOn, uint16_t value, bool readOrWrite = WRITE) {
  IntToBytes converter;
  converter.integer = value;
  uint8_t sumForCheckSum = readOrWrite + mode + motorOn + converter.bytes[0] + converter.bytes[1];
  uint8_t checkSum = ~sumForCheckSum;
  if (DEBUG == true) {
    Serial.print(0xFF, HEX);
    Serial.print(" ");
    Serial.print(0xDE, HEX);
    Serial.print(" ");
    Serial.print(readOrWrite, HEX);
    Serial.print(" ");
    Serial.print(mode, HEX);
    Serial.print(" ");
    Serial.print(motorOn, HEX);
    Serial.print(" ");
    for (uint8_t i = 0; i < 2; ++i) {
      Serial.print(converter.bytes[i], HEX);
      Serial.print(" ");
    }
    Serial.print(checkSum, HEX);
    Serial.print(" ");
    Serial.print(0xFE, HEX);
    Serial.print(" ");
    Serial.println(digitalRead(6));
  } else {
    Serial3.write(0xFF);
    Serial3.write(0xDE);
    Serial3.write(readOrWrite);
    Serial3.write(mode);
    Serial3.write(motorOn);
    for (uint8_t i = 0; i < 2; ++i) {
      Serial3.write(converter.bytes[i]);
    }
    Serial3.write(checkSum);
    Serial3.write(0xFE);
  }
}

void SerialCommand::SendCalibrationCommand(bool calibrate) {
  SendCommand(SEND_CALIBRATION, 0, calibrate);

}
void SerialCommand::SendResetCalibrationCommand(bool resetCalibration) {
  SendCommand(SEND_RESET_CALIBRATION, 0, resetCalibration);
}
void SerialCommand::SendDesiredPositionCommand(uint16_t desiredPosition, uint8_t motorOn) {
  SendCommand(SEND_DESIRED_POSITION, motorOn, desiredPosition);
}
void SerialCommand::SendMotorOn(bool onOrOff) {
  //SendCommand(MOTOR_ON, onOrOff);
}

void SerialCommand::SendOffsetCommand(uint16_t offset, uint8_t motorOn) {
  SendCommand(SEND_MOTOR_OFFSET, motorOn, offset);
}

