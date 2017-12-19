#include "ServerLifter.h"
#include <string.h>

void InitServerLifter(ServerLifter *server){
	memset(server, 0, sizeof(ServerLifter)/sizeof(*server));
}

void Init(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	SystemCoreClockUpdate();

	gpioConfiguration();

	SPI_Config();

	timerInit();

	digiPot_init();

	encoderInit();

	controlInit();

	InitServerLifter(&serverLifter);

	HMICommInit();
}

void UpdateMainRoutine(){
	while(1){
		HMIReceived(serverLifter.hmi);
	}
}
