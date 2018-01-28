#include "Encoder.h"

Encoder::Encoder() {
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(CLK, OUTPUT);
  digitalWrite(SS, LOW);
  delay(5);
  digitalWrite(SS, HIGH);
  digitalWrite(CLK, LOW);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  delay(5);

  ReadCalibratedOffset();
  ReadInitialOffset();
}

Encoder::~Encoder() {

}

void Encoder::WriteEEPROM(uint8_t address, uint16_t valueToWrite) {
  IntToBytes intToBytes;
  uint8_t checkSum;
  intToBytes.integer = valueToWrite;
  for (uint8_t i = 0; i < 3; ++i) {
    if (i < 2) {
      EEPROM.write(i + address, intToBytes.bytes[i]);
      checkSum += intToBytes.bytes[i];
    } else {
      checkSum = ~checkSum;
      EEPROM.write(i + address, checkSum);
    }
  }
}

uint16_t Encoder::ReadEEPROM(uint8_t address) {
  IntToBytes intToBytes;
  uint8_t readFromEEPROM[3];
  uint8_t checkSum, validateCheckSum;
  for (uint8_t i = 0; i < 3; ++i) {
    readFromEEPROM[i] = EEPROM.read(i + address);
    if (i < 2) {
      intToBytes.bytes[i] = readFromEEPROM[i];
      validateCheckSum += readFromEEPROM[i];
    }
  }
  validateCheckSum = ~validateCheckSum;
  checkSum = readFromEEPROM[2];
  if (checkSum == validateCheckSum) {
    uint16_t output = intToBytes.integer;
    return output;
  } else {
    return 0;
  }
}
uint8_t Encoder::ResetCalibration() {
  WriteEEPROM(CALIBRATION_OFFSET_ADDR, 0);
  rackHeightOffset = 0;
  return 1;
}

uint8_t Encoder::Calibrate() {
  WriteEEPROM(CALIBRATION_OFFSET_ADDR, rackHeight);
  rackHeightOffset = rackHeight;
  return 1;
}

uint8_t Encoder::InitialOffsetCalibration() {
  WriteEEPROM(INITIAL_OFFSET_ADDR, rackHeightRaw);
  initialOffset = rackHeightRaw;
  return 1;
}

uint8_t Encoder::ReadCalibratedOffset() {
  rackHeightOffset = ReadEEPROM(CALIBRATION_OFFSET_ADDR);
  return rackHeightOffset;
}
uint8_t Encoder::ReadInitialOffset() {
  initialOffset = ReadEEPROM(INITIAL_OFFSET_ADDR);
  return initialOffset;
}

void Encoder::ReadEncoder() {
  RAWMAXHEIGHT = RAWMAXHEIGHTDIFF + initialOffset;
  digitalWrite(SS, LOW);
  uint8_t result1 = SPI.transfer(0xFF);
  uint8_t result2 = SPI.transfer(0xFF);
  unsigned int resultRaw = (result1 << 8 | result2);
  unsigned int result = resultRaw >> 4;
  digitalWrite(SS, HIGH);
  
  rackHeightRaw = result;
  rackHeightFiltered = ((HIGHPASS * rackHeightFiltered) + (LOWPASS * (float)rackHeightRaw));
  rackHeight = ((unsigned int) rackHeightFiltered) - initialOffset;
  rackHeightCalibrated = rackHeightRaw - rackHeightOffset;
  calibratedMaxHeight = (RAWMAXHEIGHT - (rackHeightOffset + initialOffset));
}

