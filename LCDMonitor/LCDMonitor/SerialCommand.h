#include <arduino.h>

#define DEBUG_OFF 0
#define DEBUG_ON 1

#define READ  0
#define WRITE 1

#define MOTOR_ON  1
#define SEND_MOTOR_OFFSET 2
#define SEND_DESIRED_POSITION 3
#define SEND_CALIBRATION  4
#define SEND_RESET_CALIBRATION  5
#define SEND_ENCODER_REQUEST  6

#define LED_INPUT 2

union IntToBytes {
  int integer;
  char bytes[2];
};

class SerialCommand {
  public:
    SerialCommand();
    ~SerialCommand();

    void SendCalibrationCommand(bool calibrate);
    void SendResetCalibrationCommand(bool resetCalibration);
    void SendDesiredPositionCommand(uint16_t desiredPosition, uint8_t motorOn);
    void SendMotorOn(bool onOrOff);
    void SendOffsetCommand(uint16_t offset, uint8_t motorOn);

  private:
    void SendCommand(uint8_t mode, uint8_t motorOn, uint16_t value, bool readOrWrite = WRITE);
    bool DEBUG = DEBUG_OFF;
};

