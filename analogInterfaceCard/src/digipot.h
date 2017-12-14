/*
 * digipot.h
 *
 *  Created on: Dec 30, 2016
 *      Author: Mazher
 */

#ifndef DIGIPOT_H_
#define DIGIPOT_H_

#include "stm32f30x.h"
#include "GPIOs.h"

#define STEERINGPOT_H 		1
#define STEERINGPOT_L		2
#define DIGIPOT3 			3
#define THROTTLEPOT 		4
#define TILLERPOT 			5
#define DIGIPOT6 			6

typedef struct {
	u16 potVal1;
	u16 potVal2;
	u16 potVal3;
	u16 potVal4;
	u16 potVal5;
	u16 potVal6;
}digipotCmdStruct_t;


extern digitalPin POT1_SYNC;
extern digitalPin POT2_SYNC;
extern digitalPin POT3_SYNC;
extern digitalPin POT4_SYNC;
extern digitalPin POT5_SYNC;
extern digitalPin POT6_SYNC;


extern digitalPin POT1_RDY;
extern digitalPin POT2_RDY;
extern digitalPin POT3_RDY;

//Potentiometers 4-6 mode control pins
extern digitalPin POT4_INDEP;
extern digitalPin POT5_INDEP;
extern digitalPin POT6_INDEP;

extern void digiPot_init(void);
extern void digiPot_write(uint8_t potID, uint16_t value);
extern void digipotUpdate(void);

extern digipotCmdStruct_t digipotCmd;

#endif /* DIGIPOT_H_ */
