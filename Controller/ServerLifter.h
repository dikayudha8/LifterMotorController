#include "Encoder.h"
#include "PID.h"

#define UPDATE_RATE 5

class ServerLifter{
  public:
    ServerLifter();
    ~ServerLifter();

    void RunServerLifter();
  private:
    Encoder *encoder;
    SerialComm *serialComm;
    PID *pid;
    long timeBefore;
    int tempDesiredPos = 0;
    int tempOffset = 0;
    uint8_t tempCounter = 0;
//    uint8_t tempMode;
    bool test = false;
};

