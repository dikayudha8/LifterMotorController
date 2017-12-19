#include "spi.h"
#include "timer.h"
#include "CAN.h"
#include "main.h"
#include "GPIOs.h"
#include "encoder.h"
#include "digipot.h"
#include "command.h"
#include "HMIComm.h"


typedef struct{
	HMICommObj *hmi;
	uint16_t speedLimit;
	uint16_t actuatorOutput;
	uint16_t encoderRawPos;
	float encoderPos;
	uint8_t motorDirection;
}ServerLifter;

ServerLifter serverLifter;

void Init();
void InitServerLifter(ServerLifter *server);
void UpdateMainRoutine();
