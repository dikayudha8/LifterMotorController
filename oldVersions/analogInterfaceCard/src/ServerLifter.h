#include "spi.h"
#include "timer.h"
#include "CAN.h"
#include "main.h"
#include "GPIOs.h"
#include "encoder.h"
#include "digipot.h"
#include "command.h"
#include "HMIComm.h"

#define REVERSE_PIN GPIO_Pin_7
#define REVERSE_PORT GPIOB

#define MAX_SPEED 78
#define MIN_SPEED 1023

typedef struct{
	HMICommObj hmi;
	uint16_t speedLimit;
	uint16_t actuatorOutput;
	uint16_t actuatorOutputRaw;
	uint16_t encoderRawPos;
	float encoderPos;
	uint8_t motorDirection;
}ServerLifter;

ServerLifter serverLifter;

void Init();
void InitServerLifter(ServerLifter *server);
void UpdateMainRoutine();
