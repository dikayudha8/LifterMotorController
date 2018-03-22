#include "stm32f30x.h"
#include "serial.h"

typedef struct{
	serialPort_t *serialIO;
	int motorOutputCommand;
	char command;
} HMICommObj;

void HMICommInit();
void HMIReceived(HMICommObj *obj);
