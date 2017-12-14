#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f30x.h"

#define CONTROL_LOOP_FREQ					50.0f

extern void encoderInit(void);
extern void controlInit(void);

typedef struct {

	int16_t prevEncoderCount;
	int16_t currEncoderCount;
	int16_t deltaEncoderCount;
	int accumEncoderCounts;

	TIM_TypeDef *encoderTimer;
} encoder_t;

typedef struct {

	float motorCurrentSpeed;
	float motorCountsPerRev;

	encoder_t motorEncoder;

} motor_t;

extern motor_t driveMotor;


#endif
