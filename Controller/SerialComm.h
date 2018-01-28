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

enum {
  SUCCESS,
  CHECKSUM_INVALID,
  TIMEOUT
};

typedef union IntToBytes {
  uint16_t integer;
  uint8_t bytes[2];
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

    uint8_t getCheckSum() {
      return checkSum;
    };

    uint16_t getDesiredPosition() {
      return desiredPosition;
    };

    uint16_t getOffset() {
      return offset;
    };

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

  private:
    uint8_t readOrWrite;
    uint8_t motorOn;
    uint8_t mode;
    uint8_t checkSum;
    uint16_t desiredPosition;
    uint16_t offset;
    uint16_t resetCalibrationStatus;
    uint16_t calibrationStatus;
    bool DEBUG = DEBUG_MODE;
    long timeNow = 0L;
    long timeBefore = 0L;
    bool saveFirstTiming = true;
};

