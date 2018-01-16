#include "ServerLifter.h"
#include <string.h>

void InitServerLifter(ServerLifter *server){
	memset(server, 0, sizeof(server)/sizeof(ServerLifter));
	server->actuatorOutput = 512;
}

void ReversePinInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = REVERSE_PIN;
	GPIO_Init(REVERSE_PORT, &GPIO_InitStructure);
}

void Init(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	ReversePinInit();

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

void ReceivingData(ServerLifter *server){
	HMIReceived(&server->hmi);

	switch(server->hmi.command){
	case 'a':
		server->actuatorOutput += 1;
		memset(serialPort2.rxBuf, 0, sizeof(serialPort2.rxBuf)/sizeof(char));
		break;
	case 'p':
		server->actuatorOutput = 512;
		memset(serialPort2.rxBuf, 0, sizeof(serialPort2.rxBuf)/sizeof(char));
		break;
	case 'd':
		server->actuatorOutput -= 1;
		memset(serialPort2.rxBuf, 0, sizeof(serialPort2.rxBuf)/sizeof(char));
		break;
	case 'u':
		server->actuatorOutput = 128;
		break;
	case 'i':
		server->actuatorOutput = 768;
		break;
	default:
		break;
	}

	if(server->actuatorOutput > MIN_SPEED){
		server->actuatorOutput = MIN_SPEED;
	}

	if(server->actuatorOutput < MAX_SPEED){
		server->actuatorOutput = MAX_SPEED;
	}
}

void MoveMotor(ServerLifter *server){
	digiPot_write(STEERINGPOT_H, (server->actuatorOutput));
//	digiPot_write(2, (server->actuatorOutputRaw));
//	digiPot_write(3, (server->actuatorOutputRaw));
//	digiPot_write(4, (server->actuatorOutputRaw));
//	digiPot_write(5, (server->actuatorOutputRaw));
	//digiPot_write(STEERINGPOT_L, (server->actuatorOutputRaw));

	char toPrint[100];
	sprintf(toPrint, "%d\r\n", server->actuatorOutput);
	serialPrint(&serialPort2, toPrint);
}

void UpdateMainRoutine(){
	unsigned long timeBefore = 0;
	while(1){
		ReceivingData(&serverLifter);
		if(timerMicros() - timeBefore >= 100000){
			MoveMotor(&serverLifter);
			timeBefore = timerMicros();
		}
	}
}
