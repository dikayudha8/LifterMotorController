#include "stm32f30x.h"
#include "CAN.h"
#include "serial.h"
#include "timer.h"

CanRxMsg *RxMessage;
CanTxMsg GlobalTxMessage;
canMessageBuffer_t canMessageBuffer;
u32 canID;
u32 nodeID;

unsigned long lastCANmessageRx = 0;


void CAN_Config(void) {
  /* CAN1 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  /* CAN GPIOs configuration **************************************************/
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, CAN_RX_SOURCE, GPIO_AF_9);
  GPIO_PinAFConfig(GPIOA, CAN_TX_SOURCE, GPIO_AF_9);

  CAN_InitTypeDef CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  /* CAN cell init */

  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  //CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  // CAN Baudrate = ~1 Mbps (CAN clocked at 12 MHz)
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 1;

  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_DBGFreeze(CAN1, DISABLE);

  //Setup Emergency Stop filter. Urgent priority.

  canID = 0;
  canID = ( (CAN_PRIORITY_URGENT | CAN_MSG_EMERGENCY_STOP) << STD_TO_EXTENDED_OFFSET) << CAN_32BIT_FILTER_OFFSET;

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = EMERGENCY_STOP_FILTER;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = canID >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = canID & 0x0000FFFF;

  canID = 0;
  canID = ((CAN_PRIORITY_MASK | CAN_MSGID_MASK) << STD_TO_EXTENDED_OFFSET) << CAN_32BIT_FILTER_OFFSET;

  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = canID >> 16;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow =  canID & 0x0000FFFF;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

    /* Enable CAN1 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* CAN FIFO0 message pending interrupt enable */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  canMessageBuffer.head = canMessageBuffer.tail = 0;

}



void USB_LP_CAN_RX0_IRQHandler(void)
{
  CAN_Receive(CAN1, CAN_FIFO0, &canMessageBuffer.CanRxMsgBuffer[canMessageBuffer.head]);

  canMessageBuffer.head = (canMessageBuffer.head + 1) % CAN_BUFFER_SIZE;


}

void sendExtendedCANTestMessage(void)
{
	CanTxMsg TxMessage;
	uint8_t index;

	//TxMessage.StdId = 0x1FFFFFFF;
	//TxMessage.StdId = (CAN_PRIORITY_HIGH | CAN_MSG_VELOCITY | nodeID);
	TxMessage.ExtId = 0x1C800001;
	TxMessage.IDE = CAN_Id_Extended ;
	TxMessage.RTR = 0;
	TxMessage.DLC = 8;

	for(index = 0; index < 8; index++)
		TxMessage.Data[index] = index;

	CAN_Transmit(CAN1, &TxMessage);
}

void sendStandardCANTestMessage(void)
{
	CanTxMsg TxMessage;
	uint8_t index;

	TxMessage.StdId = 0x7FF;
	//TxMessage.StdId = (CAN_PRIORITY_HIGH | CAN_MSG_VELOCITY | nodeID);
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = 0;
	TxMessage.DLC = 8;

	for(index = 0; index < 8; index++)
		TxMessage.Data[index] = 0;

	CAN_Transmit(CAN1, &TxMessage);
}

