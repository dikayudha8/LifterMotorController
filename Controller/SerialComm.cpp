#include "SerialComm.h"

SerialComm::SerialComm() {
  Serial.begin(115200);
}

SerialComm::~SerialComm() {}

uint8_t SerialComm::Receiving() {
  if (Serial.available()) {
    while (Serial.read() != 0xFF) {
      if (saveFirstTiming == true){
        timeNow = micros();
        saveFirstTiming = false;
      }
      if (micros() - timeNow >= TIME_OUT_INTERVAL) {
        timeNow = micros();
        saveFirstTiming = true;
        return TIMEOUT;
      }
    }
    saveFirstTiming = true;
    readOrWrite = Serial.read();
    mode = Serial.read();
    motorOn = Serial.read();
    IntToBytes intToBytes;
    for (uint8_t i = 0; i < 2; ++i) {
      intToBytes.bytes[i] = Serial.read();
    }
    checkSum = Serial.read();

    uint8_t validatingCheckSum = ~(readOrWrite + mode + motorOn + intToBytes.bytes[0] + intToBytes.bytes[1]);

    if (validatingCheckSum == checkSum) {
      return SUCCESS;
    } else {
      return CHECKSUM_INVALID;
    }
  }
}

