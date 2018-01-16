#include "HMIComm.h"
#include <string.h>

void HMICommInit(){
	USART2_Config();
}

void HMIReceived(HMICommObj *obj){
	obj->command = serialRead(&serialPort2);
}
