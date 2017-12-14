#ifndef _CAN_h
#define _CAN_h

#include "stm32f30x.h"
#include "main.h"
//									PRI		MSG_ID		NODE_ID
//Standard 11 bit CAN identifer 	XX		XXXX		XXXXX
//Extended 29 bit CAN identifier	XX		XXXX		XXXXXXXXXXXXXXXXXXXXXXX (23 bits)

#define CAN_PRIORITY_OFFSET 		9
#define CAN_MSGID_OFFSET			5
#define CAN_NODEID_OFFSET			0

#define eCAN_PRIORITY_OFFSET		27
#define eCAN_MSGID_OFFSET			23
#define eCAN_NODEID_OFFSET 			0


//Transmission priorities
#define CAN_PRIORITY_URGENT			(0  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_HIGH			(1  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_MEDIUM			(2  << CAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_LOW			(3  << CAN_PRIORITY_OFFSET)
#define eCAN_PRIORITY_LOW			(3  << eCAN_PRIORITY_OFFSET)
#define CAN_PRIORITY_MASK			(0b11 << CAN_PRIORITY_OFFSET)
#define eCAN_PRIORITY_MASK			(0b11 << eCAN_PRIORITY_OFFSET)

//Message IDs
#define CAN_MSG_EMERGENCY_STOP		(0 << CAN_MSGID_OFFSET)
#define CAN_MSG_VELOCITY			(1 << CAN_MSGID_OFFSET)
#define CAN_MSG_HEARTBEAT			(2 << CAN_MSGID_OFFSET)
#define CAN_MSG_POS_REQ				(3 << CAN_MSGID_OFFSET)
#define CAN_MSG_MOTOR_ENABLE		(4 << CAN_MSGID_OFFSET)
#define CAN_MSG_CURRENT_CMD         (8 << CAN_MSGID_OFFSET)
#define eCAN_MSG_DISCHARGE_DATA		(9 << eCAN_MSGID_OFFSET)
#define eCAN_MSG_SOC_DATA			(10 << eCAN_MSGID_OFFSET)
#define CAN_MSG_NACK				(15 << CAN_MSGID_OFFSET)
#define CAN_MSG_ACK					(16 << CAN_MSGID_OFFSET)
#define CAN_MSGID_MASK				(0b1111 << CAN_MSGID_OFFSET)
#define eCAN_MSGID_MASK				(0b1111 << eCAN_MSGID_OFFSET)


#define CAN_32BIT_FILTER_OFFSET		3
#define CAN_16BIT_FILTER_OFFSET		5

#define CAN_RTR_32BIT_OFFSET		1
#define CAN_RTR_16BIT_OFFSET		4

#define CAN_IDE_32BIT_OFFSET		2
#define CAN_IDE_16BIT_OFFSET		3

#define CAN_RTR_32BIT_ENABLE		(1 << CAN_RTR_32BIT_OFFSET)

#define STD_TO_EXTENDED_OFFSET		18

// 0 -31 node IDs
#define CAN_NODEID					(DEVICE_ID << CAN_NODEID_OFFSET)
#define CAN_NODEID_MASK				(0b11111 << CAN_NODEID_OFFSET)

#define eCAN_NODEID					(DEVICE_ID << eCAN_NODEID_OFFSET)
#define eCAN_NODEID_MASK  			(0b11111111111111111111111 << eCAN_NODEID_OFFSET)

#define EMERGENCY_STOP_FILTER		0
#define VELOCITY_FILTER				1
#define HEARTBEAT_REQ_FILTER		2
#define POS_REQ_FILTER				3
#define MOTOR_ENABLE_FILTER			4
#define VELOCITY_FILTER_15			5
#define VELOCITY_FILTER_16			6
#define CURRENT_FILTER_15           7
#define CURRENT_FILTER_16           8
#define SOC_FILTER	           		9
#define SOH_FILTER					10

#define CAN_RX_PIN                 	GPIO_Pin_11
#define CAN_TX_PIN                 	GPIO_Pin_12
#define CAN_RX_SOURCE              	GPIO_PinSource11
#define CAN_TX_SOURCE              	GPIO_PinSource12

#define CAN_BUFFER_SIZE				10

typedef struct
{
	CanRxMsg CanRxMsgBuffer[CAN_BUFFER_SIZE];
	int head, tail;
}canMessageBuffer_t;

extern void CAN_Config(void);

extern CanTxMsg GlobalTxMessage;
extern canMessageBuffer_t canMessageBuffer;
extern void sendExtendedCANTestMessage(void);
extern void sendStandardCANTestMessage(void);

#endif
