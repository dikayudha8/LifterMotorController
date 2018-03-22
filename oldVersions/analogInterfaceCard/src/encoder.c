#include "encoder.h"
#include "main.h"
#include "spi.h"
#include "stm32f30x.h"
#include "digipot.h"

motor_t driveMotor;
void motorEncoderProcess(motor_t *motor);


void TIM6_DAC_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

	motorEncoderProcess(&driveMotor);
}

void controlInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t prescalerValue;

	//1kHz timer for current integration

	TIM_DeInit(TIM6);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	prescalerValue = (SystemCoreClock/1000000) - 1;

	TIM_TimeBaseStructure.TIM_Period = 20000; //50Hz
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM6, ENABLE);
}

void motorEncoderProcess(motor_t *motor)
{
	motor->motorEncoder.currEncoderCount = motor->motorEncoder.encoderTimer -> CNT;

	motor->motorEncoder.deltaEncoderCount = motor->motorEncoder.currEncoderCount - motor->motorEncoder.prevEncoderCount;

	motor->motorEncoder.prevEncoderCount = motor->motorEncoder.currEncoderCount;

	motor->motorEncoder.accumEncoderCounts += motor->motorEncoder.deltaEncoderCount;

	motor->motorCurrentSpeed += 0.1f * ( ( ( (motor->motorEncoder.deltaEncoderCount / 2.0f) * CONTROL_LOOP_FREQ)  / motor->motorCountsPerRev * 60.0f) - motor->motorCurrentSpeed) ;

}

void encoderInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	driveMotor.motorCurrentSpeed = 0;
	driveMotor.motorCountsPerRev = 1660; //Determined empherically
	driveMotor.motorEncoder.encoderTimer = TIM2;

	TIM_DeInit(TIM2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0 , GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1 , GPIO_AF_1);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,  TIM_ICPolarity_Rising);

	TIM_SetAutoreload(TIM2, 0xFFFF);

	TIM_Cmd(TIM2, ENABLE);

}
