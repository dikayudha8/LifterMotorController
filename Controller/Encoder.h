#include <arduino.h>
#include <SPI.h>
#include <EEPROM.h>

#define RAWMAXHEIGHTDIFF 3067
#define SS 10
#define MISO 12
#define CLK 13

#define LOWPASS 0.1f
#define HIGHPASS (1.0f - LOWPASS)

class Encoder {
  public:
    Encoder();
    ~Encoder();

    void ReadEncoder();
    uint8_t Calibrate();
    uint8_t ResetCalibration();    

  private:
    uint16_t rackHeightRaw;
    float rackHeightFiltered;
    uint16_t initialOffset;
};

