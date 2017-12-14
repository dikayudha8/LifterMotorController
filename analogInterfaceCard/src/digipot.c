/*
 * digipot.c
 *
 *  Created on: Dec 30, 2016
 *      Author: Mazher
 */

#include "digipot.h"
#include "GPIOs.h"
#include "spi.h"
#include "stm32f30x.h"


//Potentiometers 1-6  Chip-Select Pins
digitalPin POT1_SYNC;
digitalPin POT2_SYNC;
digitalPin POT3_SYNC;
digitalPin POT4_SYNC;
digitalPin POT5_SYNC;
digitalPin POT6_SYNC;

//Potentiometers 1-3 Data ready signals
digitalPin POT1_RDY;
digitalPin POT2_RDY;
digitalPin POT3_RDY;

//Potentiometers 4-6 mode control pins
digitalPin POT4_INDEP;
digitalPin POT5_INDEP;
digitalPin POT6_INDEP;

//Potentiometers 1-6 Hardware reset pin
digitalPin SPI2_RESET;

// Potentiometers 1-6 Logic power control (VLogic)
digitalPin PWR_CTRL;

digipotCmdStruct_t digipotCmd;

void digipotUpdate(void);
void digiPot_init();
void digiPot_write(uint8_t potID, uint16_t value);
void tillerTest(void);
void throttleTest(void);
void steeringTest(void);

void digipotUpdate(void)
{
	digiPot_write(STEERINGPOT_H, digipotCmd.potVal1);
	digiPot_write(STEERINGPOT_L, digipotCmd.potVal2);
	digiPot_write(DIGIPOT3, digipotCmd.potVal3);
	digiPot_write(THROTTLEPOT, digipotCmd.potVal4);
	digiPot_write(TILLERPOT, digipotCmd.potVal5);
	digiPot_write(DIGIPOT6, digipotCmd.potVal6);
}

void digiPot_init(void)
{
	digipotCmd.potVal1=512;
	digipotCmd.potVal2=512;
	digipotCmd.potVal3=512;
	digipotCmd.potVal4=124;
	digipotCmd.potVal5=42;
	digipotCmd.potVal6=32;

	//Un-select all the potentiometers
	digitalHi(POT1_SYNC);
	digitalHi(POT2_SYNC);
	digitalHi(POT3_SYNC);
	digitalHi(POT4_SYNC);
	digitalHi(POT5_SYNC);
	digitalHi(POT6_SYNC);

	//Set the mode of the potentiometers 4-6 to potentiometer mode
	digitalLo(POT4_INDEP);
	digitalLo(POT5_INDEP);
	digitalLo(POT6_INDEP);

	//Switch on the logic power of the potentiometers
	digitalHi(PWR_CTRL);

	int i=0;
	//Reset the potentiometers
	for(i=0; i<10000; i++);
	digitalLo(SPI2_RESET);
	for(i=0; i<10000; i++);
	digitalHi(SPI2_RESET);
	for(i=0; i<10000; i++);

	//Initialize potentiometer 1
	if(digitalGet(POT1_RDY)==0x01)
	{
		digitalLo(POT1_SYNC);
		for(i=0; i<10000; i++);
		spi_rw_16(SPI2, 0x2000);  //Normal mode of operation
		for(i=0; i<10000; i++);
		digitalHi(POT1_SYNC);
		for(i=0; i<10000; i++);
		digitalLo(POT1_SYNC);
		for(i=0; i<10000; i++);
		spi_rw_16(SPI2, 0x1803); //Enable digital control
		for(i=0; i<10000; i++);
		digitalHi(POT1_SYNC);
	}
	//Initialize potentiometer 2
	if(digitalGet(POT2_RDY)==0x01)
	{
		digitalLo(POT2_SYNC);
		for(i=0; i<10000; i++);
		spi_rw_16(SPI2, 0x2000);  //Normal mode of operation
		for(i=0; i<10000; i++);
		digitalHi(POT2_SYNC);
		for(i=0; i<10000; i++);
		digitalLo(POT2_SYNC);
		for(i=0; i<10000; i++);
		spi_rw_16(SPI2, 0x1803); //Enable digital control
		for(i=0; i<10000; i++);
		digitalHi(POT2_SYNC);
	}

	//Initialize potentiometer 3
	if(digitalGet(POT3_RDY)==0x01)
	{
		digitalLo(POT3_SYNC);
		for(i=0; i<10000; i++);
		spi_rw_16(SPI2, 0x2000);  //Normal mode of operation
		for(i=0; i<10000; i++);
		digitalHi(POT3_SYNC);
		for(i=0; i<10000; i++);
		digitalLo(POT3_SYNC);
		for(i=0; i<10000; i++);
		spi_rw_16(SPI2, 0x1803); //Enable digital control
		for(i=0; i<10000; i++);
		digitalHi(POT3_SYNC);
	}

	//Initialize potentiometer 4
	digitalLo(POT4_SYNC);
	for(i=0; i<10000; i++);
	spi_rw_16(SPI2, 0xC800);  //Normal mode of operation
	for(i=0; i<10000; i++);
	digitalHi(POT4_SYNC);
	for(i=0; i<10000; i++);
	digitalLo(POT4_SYNC);
	for(i=0; i<10000; i++);
	spi_rw_16(SPI2, 0xD003); //Enable digital control
	for(i=0; i<10000; i++);
	digitalHi(POT4_SYNC);

	//Initialize potentiometer 5
	digitalLo(POT5_SYNC);
	for(i=0; i<10000; i++);
	spi_rw_16(SPI2, 0xC800);  //Normal mode of operation
	for(i=0; i<10000; i++);
	digitalHi(POT5_SYNC);
	for(i=0; i<10000; i++);
	digitalLo(POT5_SYNC);
	for(i=0; i<10000; i++);
	spi_rw_16(SPI2, 0xD003); //Enable digital control
	for(i=0; i<10000; i++);
	digitalHi(POT5_SYNC);

	//Initialize potentiometer 6
	digitalLo(POT6_SYNC);
	for(i=0; i<10000; i++);
	spi_rw_16(SPI2, 0xC800);  //Normal mode of operation
	for(i=0; i<10000; i++);
	digitalHi(POT6_SYNC);
	for(i=0; i<10000; i++);
	digitalLo(POT6_SYNC);
	for(i=0; i<10000; i++);
	spi_rw_16(SPI2, 0xD003); //Enable digital control
	for(i=0; i<10000; i++);
	digitalHi(POT6_SYNC);
}

void digiPot_write(uint8_t potID, uint16_t value)
{
	int i=0;
	switch(potID)
	{
	case 1:
	{
		value=value| 0b0000010000000000; //Append the control bits, See AD5292 data sheet
		if(digitalGet(POT1_RDY)==0x01)
		{
			digitalLo(POT1_SYNC);
			for(i=0; i<100; i++);
			spi_rw_16(SPI2,value);
			for(i=0; i<100; i++);
			digitalHi(POT1_SYNC);
		}
		break;
	}
	case 2:
	{
		value=value|0b0000010000000000; //Append the control bits, See AD5292 data sheet
		if(digitalGet(POT2_RDY)==0x01)
		{
			digitalLo(POT2_SYNC);
			for(i=0; i<100; i++);
			spi_rw_16(SPI2,value);
			for(i=0; i<100; i++);
			digitalHi(POT2_SYNC);
		}
		break;
	}
	case 3:
	{
		value=value|0b0000010000000000; //Append the control bits, See AD5292 data sheet
		if(digitalGet(POT3_RDY)==0x01)
		{
			digitalLo(POT3_SYNC);
			for(i=0; i<100; i++);
			spi_rw_16(SPI2,value);
			for(i=0; i<100; i++);
			digitalHi(POT3_SYNC);
		}
		break;
	}
	case 4:
	{
		value=value|0b0001100000000000; //Append the control bits, See AD5412 data sheet
		digitalLo(POT4_SYNC);
		for(i=0; i<100; i++);
		spi_rw_16(SPI2,value);
		for(i=0; i<100; i++);
		digitalHi(POT4_SYNC);
		break;
	}
	case 5:
	{
		value=value|0b0001100000000000; //Append the control bits, See AD5412 data sheet
		digitalLo(POT5_SYNC);
		for(i=0; i<100; i++);
		spi_rw_16(SPI2,value);
		for(i=0; i<100; i++);
		digitalHi(POT5_SYNC);
		break;
	}
	case 6:
	{
		value=value|0b0001100000000000; //Append the control bits, See AD5412 data sheet
		digitalLo(POT6_SYNC);
		for(i=0; i<100; i++);
		spi_rw_16(SPI2,value);
		for(i=0; i<100; i++);
		digitalHi(POT6_SYNC);
		break;
	}
	}
}


void tillerTest(void)
{
	int i;

	//Tilt the tiller down gradually
	for(i=0; i<50; i++)
	{
		digiPot_write(TILLERPOT,200-i);
	}
}

void throttleTest(void)
{
	int i;

	// Increase the throttle
	for(i=20; i<100; i++)
	{
		digiPot_write(THROTTLEPOT,124+i);
	}

}

void steeringTest(void)
{
	int i, j;

	//Swing the steering left to right and back
	for(i=0;i<400; i++)
	{
		for(j = 0; j<10000; j++);
		digiPot_write(STEERINGPOT_H,512+i);
		digiPot_write(STEERINGPOT_L,512-i);
	}
	for(i=400;i>0; i--)
	{
		for(j = 0; j<10000; j++);
		digiPot_write(STEERINGPOT_H,512+i);
		digiPot_write(STEERINGPOT_L,512-i);
	}
}



