#ifndef _timer_h
#define _timer_h

#include "stm32f30x.h"

#define TIMER_TIM	TIM16
#define TIMER_EN	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE)

typedef struct {
    volatile unsigned int hiBits;
    volatile unsigned int lastTimerVal;
} timerStruct_t;

extern void timerInit(void);
extern unsigned long timerMicros(void);

extern timerStruct_t timerData;

#endif
