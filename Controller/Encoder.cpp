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
}

Encoder::~Encoder() {

}

void Encoder::ReadEncoder() {
  digitalWrite(SS, LOW);
  uint8_t result1 = SPI.transfer(0xFF);
  uint8_t result2 = SPI.transfer(0xFF);
  unsigned int resultRaw = (result1 << 8 | result2);
  unsigned int result = resultRaw >> 4;

  rackHeightFiltered = ((HIGHPASS * rackHeightFiltered) + (LOWPASS * (float)result));
  rackHeightRaw = (unsigned int) rackHeightFiltered;
}

