#include <arduino.h>

#define DEBUG_MODE false
#define TIME_OUT_INTERVAL 10000L

#define READ  0
#define WRITE 1

#define MOTOR_ON  1
#define SEND_MOTOR_OFFSET 2
#define SEND_DESIRED_POSITION 3
#define SEND_CALIBRATION  4
#define SEND_RESET_CALIBRATION  5
#define SEND_ENCODER_REQUEST  6
#define SEND_INITIAL_OFFSET 7

#define SYNCH_PIN 6

enum {
  SUCCESS,
  CHECKSUM_INVALID,
  TIMEOUT,
  NOTHING
};

typedef union IntToBytes {
  int integer;
  char bytes[2];
};

class SerialComm {
  public:
    SerialComm();
    ~SerialComm();

    uint8_t getMotorOn() {
      return motorOn;
    };

    uint8_t getMode() {
      return mode;
    };

    void clearMode(){
      mode = 0;
    }

    uint8_t getCheckSum() {
      return checkSum;
    };

    uint16_t getDesiredPosition() {
      return desiredPosition;
    };

    int getOffset() {
      return offset;
    };

    void ClearOffset(){
      offset = 0;
    }
    
    uint16_t getResetCalibrationStatus() {
      return resetCalibrationStatus;
    };

    uint16_t getCalibrationStatus() {
      return calibrationStatus;
    };

    uint8_t getReadOrWriteStatus() {
      return readOrWrite;
    };

    uint8_t Receiving();
    uint8_t ReceivingDebugMode();
    void ReadingBytes(uint8_t data);
  private:
    uint8_t readOrWrite = 0;
    uint8_t motorOn = 0;
    uint8_t mode = 0;
    uint8_t checkSum = 0;
    int desiredPosition = 0;
    int offset = 0;
    uint16_t resetCalibrationStatus = 0;
    uint16_t calibrationStatus = 0;
    bool DEBUG = DEBUG_MODE;
    long timeNow = 0L;
    long timeBefore = 0L;
    bool saveFirstTiming = true;
    uint8_t readState = 0;
    int rawData[16];
    uint8_t rawDataCounter = 0;
};

