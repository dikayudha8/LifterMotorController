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
  char checkSum;
  intToBytes.integer = valueToWrite;
  for (uint8_t i = 0; i < 3; ++i) {
    if (i < 2) {
      EEPROM.write(i + address, intToBytes.bytes[i]);
      //checkSum += intToBytes.bytes[i];
      //Serial.print(intToBytes.bytes[i], HEX);
      //Serial.print(" ");
    } else {
      checkSum = ~(intToBytes.bytes[0] + intToBytes.bytes[1]);
      EEPROM.write(i + address, checkSum);
    }
  }
  //Serial.print(" ");
  //Serial.println(checkSum, HEX);
}

int Encoder::ReadEEPROM(uint8_t address) {
  IntToBytes intToBytes;
  uint8_t readFromEEPROM[3];
  char checkSum, validateCheckSum;
  for (uint8_t i = 0; i < 3; ++i) {
    readFromEEPROM[i] = EEPROM.read(i + address);
    if (i < 2) {
      intToBytes.bytes[i] = readFromEEPROM[i];
      //validateCheckSum += readFromEEPROM[i];
      //Serial.print(intToBytes.bytes[i], HEX);
      //Serial.print(" ");
    }
  }
  validateCheckSum = ~(readFromEEPROM[0] + readFromEEPROM[1]);
  checkSum = readFromEEPROM[2];
  //Serial.print(checkSum, HEX);
  //Serial.print(" ");
  //Serial.println(validateCheckSum, HEX);
  if (checkSum == validateCheckSum) {
    uint16_t output = intToBytes.integer;
    return output;
  } else {
    return 0;
  }
}
uint8_t Encoder::ResetCalibration() {
  newDesiredPosition = rackHeightOffset + rackHeightCalibrated;
  WriteEEPROM(CALIBRATION_OFFSET_ADDR, 0);
  rackHeightOffset = 0;
  return 1;
}

uint8_t Encoder::Calibrate() {
  newDesiredPosition = 0;
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
  rackHeightCalibrated = rackHeight - rackHeightOffset;
  calibratedMaxHeight = (RAWMAXHEIGHT - (rackHeightOffset + initialOffset));

  rackHeightCalibrated = constrain(rackHeightCalibrated, 0, calibratedMaxHeight);
}

