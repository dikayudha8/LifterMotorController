/**
 *****************************************************************************
 **
 **  File        : main.c
 **
 **  Abstract    : main function.
 **
 **  Functions   : main
 **
 **  Environment : Atollic TrueSTUDIO(R)
 **
 **  Distribution: The file is distributed "as is", without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. This file may only be built (assembled or compiled and linked)
 **  using the Atollic TrueSTUDIO(R) product. The use of this file together
 **  with other tools than Atollic TrueSTUDIO(R) is not permitted.
 **
 *****************************************************************************
 */

/* Includes */
#include "stm32f30x.h"
#include "spi.h"
#include "serial.h"
#include "timer.h"
#include "CAN.h"
#include "main.h"
#include "GPIOs.h"
#include "encoder.h"
#include "digipot.h"
#include "command.h"




int main(void)
{
	 unsigned long telemetrytimer = 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	SystemCoreClockUpdate();

	gpioConfiguration();

	SPI_Config();

	timerInit();

	USART2_Config();

	digiPot_init();

	encoderInit();

	controlInit();

 while(1)
	{

	 while(serialAvailable(&serialPort2))
	 {
	 commandCharIn(serialRead(&serialPort2));
	 }

	 digipotUpdate();

	 if(timerMicros() - telemetrytimer > 20000)
	 {
		 //printf("%d \r\n", (int)driveMotor.motorCurrentSpeed);
		 sendTelemetry();
	 }

	}


}



void HardFault_Handler(void)
{
	for(;;);

}

void WWDG_IRQHandler(void)
{
	for(;;);

}

void UsageFault_Handler(void)
{
	for(;;);
}



