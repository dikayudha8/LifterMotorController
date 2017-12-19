#include "HMIComm.h"
#include <string.h>

void HMICommInit(){
	USART2_Config();
}

void HMIReceived(HMICommObj *obj){
	serialPort_t *tempSerial = &serialPort2;
	memcpy(obj->serialIO->rxBuf, tempSerial->rxBuf, sizeof(char)/sizeof(*tempSerial->rxBuf));
	serialWrite(tempSerial, obj->serialIO->rxBuf[0]);
}
