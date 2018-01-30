#include <arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include "SerialComm.h"

#define RAWMAXHEIGHTDIFF 3000
#define SS 10
#define MISO 12
#define CLK 13

#define INITIAL_OFFSET_ADDR 32
#define CALIBRATION_OFFSET_ADDR 16

#define LOWPASS 0.05f
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

    int GetCalibratedMaxHeight(){
      return calibratedMaxHeight;
    };

    int GetRackHeight(){
      return rackHeightCalibrated;
    };

    int GetNewDesiredPosition(){
      return newDesiredPosition;
    }
  private:
    int rackHeight = 0;
    int rackHeightRaw = 0;
    int newDesiredPosition = 0;
    float rackHeightFiltered = 0.0f;
    int initialOffset = 0;
    int RAWMAXHEIGHT = 0;
    int rackHeightOffset = 0;
    int rackHeightCalibrated = 0;
    int calibratedMaxHeight = 0;
    int ReadEEPROM(uint8_t address);
    void WriteEEPROM(uint8_t address, uint16_t valueToWrite);
};

