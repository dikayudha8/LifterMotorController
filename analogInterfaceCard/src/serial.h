#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "stm32f30x.h"

#define DMA_RX_BUFFER 1

typedef struct{

	char sync1;
	char sync2;

	int velsp1, rpm1, pwm1, pos1;
	int velsp2, rpm2, pwm2, pos2;

	char CH1_FF1, CH1_FF2;
	char CH2_FF1, CH2_FF2;

	//char systemStates;

	char endbyte;

} __attribute__((packed)) telemetryStruct ;

typedef struct {
	char txBuf[512];
	unsigned int txBufSize;
	volatile unsigned int txHead, txTail;

	char rxBuf[512];
	unsigned int rxBufSize;
	volatile unsigned int rxHead, rxTail;
	volatile unsigned int rxPos;

	unsigned int txDmaRunning;



} serialPort_t;

extern void serialStartTxDMA(serialPort_t *s) ;
extern void serialWrite(serialPort_t *s, unsigned char ch);
extern int serialRead(serialPort_t *s);
extern void serialPrint(serialPort_t *s, const char *str);
extern unsigned char serialAvailable(serialPort_t *s);
extern void serialProcess(void);
extern void itoaCustom(int value, char* str, int base);
extern void strreverse(char* begin, char* end);
extern void USART2_Config(void);
extern void USART3_Config(void);

extern serialPort_t serialPort2;

#endif
