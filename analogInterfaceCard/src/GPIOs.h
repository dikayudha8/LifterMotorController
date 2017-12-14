#ifndef __GPIOS_H
#define __GPIOS_H

#include "stm32f30x.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
} digitalPin;

#define digitalHi(p)	 {p.port->BSRR = p.pin;}
#define digitalLo(p)	 {p.port->BRR = p.pin;}
#define digitalGet(p)	((p.port->IDR & p.pin) != 0)
#define INPUT 1
#define OUTPUT 0
#define OPENDRAIN 2

#define CALLIBRATION_MODE	0
#define NORMALOPERATION_MODE 1

#define ALERT_ACTIVE	0

//Potentiometers 1-6  Chip-Select Pins
extern digitalPin POT1_SYNC;
extern digitalPin POT2_SYNC;
extern digitalPin POT3_SYNC;
extern digitalPin POT4_SYNC;
extern digitalPin POT5_SYNC;
extern digitalPin POT6_SYNC;

//Potentiometers 1-3 Data ready signals
extern digitalPin POT1_RDY;
extern digitalPin POT2_RDY;
extern digitalPin POT3_RDY;

//Potentiometers 4-6 mode control pins
extern digitalPin POT4_INDEP;
extern digitalPin POT5_INDEP;
extern digitalPin POT6_INDEP;

//Potentiometers 1-6 Hardware reset pin
extern digitalPin SPI2_RESET;

// Potentiometers 1-6 Logic power control (VLogic)
extern digitalPin PWR_CTRL;

extern void gpioConfiguration(void);
extern void digitalInit(GPIO_TypeDef* port, const uint16_t pin, int IO, digitalPin *p);
extern void digitalTogg(digitalPin p);



#endif
