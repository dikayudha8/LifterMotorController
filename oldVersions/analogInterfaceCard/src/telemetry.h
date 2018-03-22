#ifndef __TELEMETRY_H_
#define __TELEMETRY_H_

#include "serial.h"
#include "main.h"

//ICD version
#define COMMAND_SYNC1                   0xA1
#define COMMAND_SYNC2                   0xB2
#define COMMAND_SYNC3                   0xC3
#define COMMAND_SYNC4                   0xFF

#define MESSAGE_MAJOR                   0x01
#define MESSAGE_MINOR                   0x00
#define MESSAGE_BUGFX                   0x00
#define MESSAGE_SPARE                   0xFF

//bird version
#define SYS_ID            	0x01
#define SYS_MAJOR_VER     	0x01
#define SYS_MINOR_VER     	0x00
#define PROJ_ID           	PROJ_SESTO

#define PROJ_SESTO			0x03

#define MSGID_ANALOG_CMD	3010

#define OEM_ID				0x00
#define DES_ID				0x00

typedef struct {
	u8 sync1;
	u8 sync2;
	u8 sync3;
	u8 sync4;
	u8 majVer;
	u8 minVer;
	u8 bugFix;
	u8 spare;
	u16 sorID;
	u16 desID;
	u16 mesID;
	u16 projID;
	unsigned int mesSize;
	unsigned int headerCS; //header checksum
	unsigned int messageCS; //message checksum
}__attribute__((packed)) msgHeader_t; //message structure pointer

typedef struct {
 float motorSpeed;
}__attribute__((packed)) analogExpTelemetry_t;


extern unsigned int fletcher32CS(unsigned char *buf, int byteCount);
extern void sendTelemetry(void);


#endif
