#include "timer.h"
#include "stm32f30x.h"

timerStruct_t timerData;

// must be called at least once every 65536 us in order for this strategy to work
unsigned long timerMicros(void) {
	unsigned int timerVal;
	unsigned int hiBits;

	// disable *all* interrupts
	__asm volatile ("cpsid f");

	timerVal = TIMER_TIM->CNT;
	if (timerVal < timerData.lastTimerVal)
		timerData.hiBits++;

	hiBits = timerData.hiBits;
	timerData.lastTimerVal = timerVal;

	// enable interrupts
	__asm volatile ("cpsie f");

	return (hiBits << 16) | timerVal;
}

// Use TIMER_TIM to create a us system clock
void timerInit(void) {
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t prescalerValue;

	TIM_DeInit(TIMER_TIM);

	TIMER_EN;

	prescalerValue = (SystemCoreClock/1000000) - 1;

	/* Time base configuration */
	/* (CoreClock /2 ) / prescalerValue = 1MHz (us) */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = prescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMER_TIM, &TIM_TimeBaseStructure);

	// reset
	timerData.hiBits = 0;
	TIM_SetCounter(TIMER_TIM, 0);

	// go...
	TIM_Cmd(TIMER_TIM, ENABLE);
}
