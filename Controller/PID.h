#include <arduino.h>
#include <math.h>

#define MOTOR_OUT 5
#define MOTOR_STOP 128
#define U_PER_COUNT 75.73164660491116f

#define KP 2
#define KI 0
#define KD 0

#define PWM_MAX 220
#define PWM_MID 128
#define PWM_MIN 20
enum {
  CURRENT,
  PREVIOUS,
  PREVIOUS_2
};

class PID {
  public:
    PID(uint8_t kp, uint8_t ki, uint8_t kd);
    ~PID();

    int ComputePID(int desiredPosition, int currentPosition);
    
    void SetDesiredPosition(uint16_t dPos) {
      desiredPosition = dPos;
    };

    void SetCurrentPosition(uint16_t pos) {
      currentPosition = pos;
    };

    uint16_t GetDesiredPosition() {
      return desiredPosition;
    };

    uint8_t GetetCurrentPosition() {
      return currentPosition;
    };
    
  private:
    int *outputPID;
    float *error;
    int currentPosition = 0;
    int desiredPosition = 0;
    uint8_t kp = KP; uint8_t ki = KI; uint8_t kd = KD;
    long timeBefore;
    uint8_t deltaTime;
};

