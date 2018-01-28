#include "SerialCommand.h"

SerialCommand::SerialCommand() {
  if (DEBUG == true) {
    Serial.begin(115200);
  } else {
    Serial3.begin(115200);
  }
}

SerialCommand::~SerialCommand() {

}

void SerialCommand::SendCommand(uint8_t mode, uint8_t motorOn, uint16_t value, bool readOrWrite = WRITE) {
  IntToBytes converter;
  converter.integer = value;
  int sumForCheckSum = readOrWrite + mode + motorOn + converter.bytes[0] + converter.bytes[1];
  uint8_t checkSum = ~sumForCheckSum;
  if (DEBUG == true) {
    Serial.print(0xFF, HEX);
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
    Serial.println(checkSum, HEX);
  } else {
    Serial3.write(0xFF);
    Serial3.write(readOrWrite);
    Serial3.write(mode);
    Serial3.write(motorOn);
    for (uint8_t i = 0; i < 2; ++i) {
      Serial3.write(converter.bytes[i]);
    }
    Serial3.write(checkSum);
  }
}

void SerialCommand::SendCalibrationCommand(bool calibrate) {
  SendCommand(SEND_CALIBRATION, 0, calibrate);

}
void SerialCommand::SendResetCalibrationCommand(bool resetCalibration) {
  SendCommand(SEND_RESET_CALIBRATION, 0, resetCalibration);
}
void SerialCommand::SendDesiredPositionCommand(uint16_t desiredPosition) {
  SendCommand(SEND_DESIRED_POSITION, 1, desiredPosition);
}
void SerialCommand::SendMotorOn(bool onOrOff) {
  //SendCommand(MOTOR_ON, onOrOff);
}

void SerialCommand::SendOffsetCommand(uint8_t offset) {
  SendCommand(SEND_MOTOR_OFFSET, 1, offset);
}

