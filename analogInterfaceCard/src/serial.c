#include "serial.h"
#include "stm32f30x.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int fletcher32CS(unsigned char *buf, int byteCount);

serialPort_t serialPort2;

unsigned int cs32A, cs32B;

void USART2_Config(void)
{

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	serialPort_t *s;

	s = &serialPort2;

	s->rxBufSize = s->txBufSize=512;
	s->txHead = s->txTail = 0;
	s->rxHead = s->rxTail= 0;
	s->txDmaRunning = 0;

	/* Peripheral Clock Enable -------------------------------------------------*/
	/* Enable GPIO clock */
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Enable the DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* USARTx GPIO configuration -----------------------------------------------*/
	/* Connect USART pins to AF7 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7); //TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_7); //RX

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configuration ----------------------------------------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	//	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);


	//	/* Configure DMA controller to manage USART TX request ----------*/
	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART2 + 0x28;//0x04 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 512;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = 0;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_SetCurrDataCounter(DMA1_Channel7, 0);
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);
	DMA_Cmd(DMA1_Channel7, DISABLE);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);


	/* Configure DMA controller to manage USART RX request ----------*/
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART2 + 0x24; //0x04 ;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)s->rxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_RX_BUFFER;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = 0;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_ClearFlag(DMA1_FLAG_TC6|DMA1_FLAG_HT6|DMA1_FLAG_TE6);
	DMA_Cmd(DMA1_Channel6, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	s->rxPos = DMA_GetCurrDataCounter(DMA1_Channel6);

	//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);


}

void serialStartTxDMA(serialPort_t *s)
{
	if (!s->txDmaRunning) {
		DMA1_Channel7->CMAR = (uint32_t)&s->txBuf[s->txTail];
		if (s->txHead > s->txTail) {
			DMA1_Channel7->CNDTR = s->txHead - s->txTail;
			s->txTail = s->txHead;
		}
		else {
			DMA1_Channel7->CNDTR = s->txBufSize - s->txTail;
			s->txTail = 0;
		}

		s->txDmaRunning = 1;
		DMA_Cmd(DMA1_Channel7, ENABLE);
	}
}

void serialWrite(serialPort_t *s, unsigned char ch)
{
	s->txBuf[s->txHead] = ch;
	s->txHead = (s->txHead + 1) % s->txBufSize;

	serialStartTxDMA(s);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

}


int serialRead(serialPort_t *s) {
	int ch;

	ch = s->rxBuf[s->rxBufSize - s->rxPos];
	if (--s->rxPos == 0)
		s->rxPos = s->rxBufSize;

	return ch;
}

void serialPrint(serialPort_t *s, const char *str) {
	while (*str)
		serialWrite(s, *(str++));
}

unsigned char serialAvailable(serialPort_t *s) {

	return (DMA1_Channel6->CNDTR != s->rxPos);


}

void USART2_IRQHandler(void)
{
	serialPort_t *s = &serialPort2;

	if (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == 1)
	{
		if (s->txTail != s->txHead)
		{
			USART_SendData(USART2, s->txBuf[s->txTail]);
			s->txTail = (s->txTail + 1) % s->txBufSize;
		}
		// EOT
		else
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}

	}
}

void DMA1_Channel7_IRQHandler(void) {
	serialPort_t *s = &serialPort2;

	if(DMA_GetFlagStatus(DMA1_FLAG_TC7) == 1)
	{
		DMA_ClearFlag(DMA1_FLAG_TC7|DMA1_FLAG_HT7|DMA1_FLAG_TE7);
		DMA_Cmd(DMA1_Channel7, DISABLE);
		s->txDmaRunning = 0;
	}

	DMA_ClearFlag(DMA1_FLAG_GL7);

		if (s->txHead != s->txTail)
			serialStartTxDMA(s);
}

void strreverse(char* begin, char* end)
{
	char aux;
	while(end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}

void itoaCustom(int value, char* str, int base)
{
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char* wstr=str;
	int sign;
	div_t res;
	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return; }
	// Take care of sign
	if ((sign=value) < 0) value = -value;
	// Conversion. Number is reversed.
	do {

		res = div(value,base);

		*wstr++ = num[res.rem];

	}while(value==res.quot);
	if(sign<0) *wstr++='-';
	*wstr='\0';
	// Reverse string
	strreverse(str,wstr-1);
}

