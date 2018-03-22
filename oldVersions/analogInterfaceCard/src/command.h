#ifndef __COMMAND_H
#define __COMMAND_H

#include "stm32f30x.h"
#include "telemetry.h"
#include "timer.h"

//uplink State machines
#define COMMAND_WAIT_SYNC1              0x00
#define COMMAND_WAIT_SYNC2              0x01
#define COMMAND_WAIT_SYNC3              0x02
#define COMMAND_WAIT_SYNC4              0x03
#define COMMAND_WAIT_MSGMJ              0x04
#define COMMAND_WAIT_MSGMI              0x05
#define COMMAND_WAIT_MSGBF              0x06
#define COMMAND_WAIT_SPARE              0x07
#define COMMAND_WAIT_SRC0               0x08
#define COMMAND_WAIT_SRC1               0x09
#define COMMAND_WAIT_DES0               0x0a
#define COMMAND_WAIT_DES1               0x0b
#define COMMAND_WAIT_MSGID0             0x0c
#define COMMAND_WAIT_MSGID1             0x0d
#define COMMAND_WAIT_PROJID0            0x0e
#define COMMAND_WAIT_PROJID1            0x0f
#define COMMAND_WAIT_LEN0               0x10
#define COMMAND_WAIT_LEN1               0x11
#define COMMAND_WAIT_LEN2               0x12
#define COMMAND_WAIT_LEN3               0x13
#define COMMAND_PAYLOAD                 0x14
#define COMMAND_PAYLOAD1                0x15
#define COMMAND_HCHECK0                 0xf8
#define COMMAND_HCHECK1                 0xf9
#define COMMAND_HCHECK2                 0xfa
#define COMMAND_HCHECK3                 0xfb
#define COMMAND_MCHECK0                 0xfc
#define COMMAND_MCHECK1                 0xfd
#define COMMAND_MCHECK2                 0xfe
#define COMMAND_MCHECK3                 0xff

#define MSGID_ANALOG_CMD				3010

typedef struct {
	u8 sync0;
	u8 sync1;
	u8 sync2;
	u8 sync3;
	u8 msgMaj;
	u8 msgMin;
	u8 msgBfx;
	u8 mSpare;
	u16 srcID;
	u16 desID;
	union
	{
		u8  bval[2];
		u16 ival;
	}msgID;

	union
	{
		u8  bval[2];
		u16 ival;
	}projID;

	u32 msgSize;
} __attribute__((packed)) headerStruct_t;

typedef struct {
	u16 potVal1;
	u16 potVal2;
	u16 potVal3;
	u16 potVal4;
	u16 potVal5;
	u16 potVal6;
} __attribute__((packed)) analogCmdStruct_t;



typedef struct
{
	u8 state;
	u16 msgID;
	u16 projID;
	u16 count;
	headerStruct_t header;
	union
	{
		analogCmdStruct_t	analogCmd;
	} priPayload;
	u32 msgLen, payloadCS;
	union
	{
		u8 bval[4];
		u32 ival;
	}headerCS;
} commandData_t;

extern commandData_t uplinkData;

extern void processUplink(void);
extern void commandCharIn(unsigned char ch);

#endif
