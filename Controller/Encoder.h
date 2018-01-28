#include <arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include "SerialComm.h"

#define RAWMAXHEIGHTDIFF 3067
#define SS 10
#define MISO 12
#define CLK 13

#define INITIAL_OFFSET_ADDR 32
#define CALIBRATION_OFFSET_ADDR 16

#define LOWPASS 0.1f
#define HIGHPASS (1.0f - LOWPASS)

class Encoder {
  public:
    Encoder();
    ~Encoder();

    void ReadEncoder();
    uint8_t Calibrate();
    uint8_t ResetCalibration();    
    uint8_t ReadCalibratedOffset();
    uint8_t ReadInitialOffset();
    uint8_t InitialOffsetCalibration();
  private:
    uint16_t rackHeight;
    uint16_t rackHeightRaw;
    float rackHeightFiltered;
    uint16_t initialOffset;
    uint16_t RAWMAXHEIGHT;
    uint16_t rackHeightOffset;
    uint16_t rackHeightCalibrated;
    uint16_t calibratedMaxHeight;
    uint16_t ReadEEPROM(uint8_t address);
    void WriteEEPROM(uint8_t address, uint16_t valueToWrite);
};

