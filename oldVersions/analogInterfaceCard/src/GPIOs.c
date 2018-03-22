#include "GPIOs.h"
#include <stdlib.h>
#include <stdio.h>



void gpioConfiguration(void)
{
	digitalInit(GPIOA, GPIO_Pin_4,  OUTPUT, &PWR_CTRL);
	digitalLo(PWR_CTRL);

	digitalInit(GPIOA, GPIO_Pin_8,  OUTPUT, &POT1_SYNC);
	digitalInit(GPIOA, GPIO_Pin_9,  OUTPUT, &POT2_SYNC);
	digitalInit(GPIOA, GPIO_Pin_10,  OUTPUT, &POT3_SYNC);
	digitalInit(GPIOA, GPIO_Pin_11,  OUTPUT, &POT4_SYNC);
	digitalInit(GPIOA, GPIO_Pin_12,  OUTPUT, &POT5_SYNC);
	digitalInit(GPIOB, GPIO_Pin_0,  OUTPUT, &POT6_SYNC);

	digitalInit(GPIOB, GPIO_Pin_1,  INPUT, &POT1_RDY);
	digitalInit(GPIOB, GPIO_Pin_2,  INPUT, &POT2_RDY);
	digitalInit(GPIOC, GPIO_Pin_13,  INPUT, &POT3_RDY);

	digitalInit(GPIOC, GPIO_Pin_14,  OUTPUT, &POT4_INDEP);
	digitalInit(GPIOB, GPIO_Pin_10,  OUTPUT, &POT5_INDEP);
	digitalInit(GPIOB, GPIO_Pin_11,  OUTPUT, &POT6_INDEP);

	digitalInit(GPIOB, GPIO_Pin_12,  OUTPUT, &SPI2_RESET);

	digitalLo(PWR_CTRL);
}

void digitalInit(GPIO_TypeDef* port, const uint16_t pin, int IO, digitalPin *p)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = pin;

	if(IO == OUTPUT)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}

	if(IO == INPUT)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}

	if(IO == OPENDRAIN)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}

	GPIO_Init(port, &GPIO_InitStructure); //port actually is gpioB.

	//p = (digitalPin *)calloc(1, sizeof(digitalPin)); //Allocate a block of memory of 1xsize of digital pin , intiliaze all its bits to zero.
	p->port = port; //just pass the address of the port to user defined port.
	p->pin = pin;

	//return p;
}

void digitalTogg(digitalPin p) {
	if (digitalGet(p)) {
		digitalLo(p);
	}
	else {
		digitalHi(p);
	}
}
