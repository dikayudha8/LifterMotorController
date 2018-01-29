#include "SerialComm.h"

SerialComm::SerialComm() {
  Serial.begin(115200);
  //pinMode(SYNCH_PIN, OUTPUT);
  memset(rawData, 0, sizeof(rawData) / sizeof(*rawData));
}

SerialComm::~SerialComm() {}

/*uint8_t SerialComm::Receiving() {
  if (Serial.available()) {
    while (Serial.read() != 0xFF) {
      if (saveFirstTiming == true) {
        timeNow = micros();
        saveFirstTiming = false;
      }
      if (micros() - timeNow >= TIME_OUT_INTERVAL) {
        timeNow = micros();
        saveFirstTiming = true;
        return TIMEOUT;
      }
    }
    digitalWrite(13, LOW);
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
      if (mode == SEND_DESIRED_POSITION) {
        desiredPosition = intToBytes.integer;
      } else if (mode == SEND_MOTOR_OFFSET) {
        offset = intToBytes.integer;
      }
      return SUCCESS;
    } else {
      return CHECKSUM_INVALID;
    }
  } else {
    return NOTHING;
  }
  }*/

void SerialComm::ReadingBytes(uint8_t data) {
  IntToBytes intToBytes;
  
  uint8_t j = 0;

  switch (readState) {
    case 0:
      if (data == 0xFF) {
        ++readState;
      }
      break;
    case 1:
      if (data == 0xDE) {
        ++readState;
      } else {
        readState = 0;
      }
      break;
    case 2:
      if (data != 0xFE) {
        rawData[rawDataCounter] = data;
        ++rawDataCounter;
        if (rawDataCounter >= 7) {
          ++readState;
          rawDataCounter = 0;
        }
      }
      break;
    case 3:
      readOrWrite = rawData[0];
      mode = rawData[1];
      motorOn = rawData[2];
      intToBytes.bytes[0] = rawData[3];
      intToBytes.bytes[1] = rawData[4];
      if(mode == SEND_DESIRED_POSITION){
        desiredPosition = intToBytes.integer;
      } else if (mode == SEND_MOTOR_OFFSET){
        offset = intToBytes.integer;
      }
      readState = 0;
      break;
  }
}

uint8_t SerialComm::Receiving() {
  uint8_t incomingPackets = Serial.available();
  if (incomingPackets > 0) {
    for (uint8_t i = 0; i < incomingPackets; ++i) {
      ReadingBytes(Serial.read());
    }
  }
  //  if (Serial.available()) {
  //    while (Serial.read() != 0xFF) {
  //      if (saveFirstTiming == true) {
  //        timeNow = micros();
  //        saveFirstTiming = false;
  //      }
  //      if (micros() - timeNow >= TIME_OUT_INTERVAL) {
  //        timeNow = micros();
  //        saveFirstTiming = true;
  //        return TIMEOUT;
  //      }
  //    }
  //    digitalWrite(13, LOW);
  //    saveFirstTiming = true;
  //    readOrWrite = Serial.read();
  //    mode = Serial.read();
  //
  //    motorOn = Serial.read();
  //    IntToBytes intToBytes;
  //    for (uint8_t i = 0; i < 2; ++i) {
  //      intToBytes.bytes[i] = Serial.read();
  //    }
  //    checkSum = Serial.read();
  //
  //    uint8_t validatingCheckSum = ~(readOrWrite + mode + motorOn + intToBytes.bytes[0] + intToBytes.bytes[1]);
  //
  //    if (validatingCheckSum == checkSum) {
  //      if (mode == SEND_DESIRED_POSITION) {
  //        desiredPosition = intToBytes.integer;
  //      } else if (mode == SEND_MOTOR_OFFSET) {
  //        offset = intToBytes.integer;
  //      }
  //      return SUCCESS;
  //    } else {
  //      return CHECKSUM_INVALID;
  //    }
  //  } else {
  //    return NOTHING;
  //  }
}

uint8_t SerialComm::ReceivingDebugMode() {
  if (Serial.available()) {
    char tempReceive = Serial.read();
    if (tempReceive == 'A') {
      motorOn = 0;
      mode = SEND_CALIBRATION;
    } else if (tempReceive == 'S') {
      motorOn = 0;
      mode = SEND_RESET_CALIBRATION;
    } else if (tempReceive == 'P') {
      motorOn = 0;
      mode = SEND_INITIAL_OFFSET;
    } else if (tempReceive == 'u') {
      motorOn = 1;
      mode = SEND_DESIRED_POSITION;
      desiredPosition += 10;
    } else if (tempReceive == 'i') {
      motorOn = 1;
      mode = SEND_DESIRED_POSITION;
      desiredPosition -= 10;
    } else if (tempReceive == ',') {
      motorOn = 1;
      mode = SEND_MOTOR_OFFSET;
      offset = 2;
      tempReceive = 0;
    } else if (tempReceive == '.') {
      motorOn = 1;
      mode = SEND_MOTOR_OFFSET;
      offset = -2;
    } else {
      offset = 0;
    }
  }
}

