#include "telemetry.h"
#include "main.h"
#include "CAN.h"
#include "serial.h"
#include "encoder.h"


msgHeader_t msgHeader;
analogExpTelemetry_t analogExpTelemetry;
unsigned int cs32A, cs32B;

void sendTelemetry(void)
{
	int i, j;

	analogExpTelemetry.motorSpeed = driveMotor.motorCurrentSpeed;

	msgHeader.sync1        = COMMAND_SYNC1;
	msgHeader.sync2        = COMMAND_SYNC2;
	msgHeader.sync3        = COMMAND_SYNC3;
	msgHeader.sync4        = COMMAND_SYNC4;
	msgHeader.majVer       = MESSAGE_MAJOR;
	msgHeader.minVer       = MESSAGE_MINOR;
	msgHeader.bugFix       = MESSAGE_BUGFX;
	msgHeader.spare        = MESSAGE_SPARE;
	msgHeader.sorID        = OEM_ID;
	msgHeader.desID        = DES_ID;
	msgHeader.mesID        = MSGID_ANALOG_CMD;
	msgHeader.projID       = PROJ_ID;
	msgHeader.mesSize      = (sizeof(analogExpTelemetry) + sizeof(msgHeader));
	msgHeader.headerCS     = fletcher32CS((unsigned char *)&msgHeader, sizeof(msgHeader) - 8);
	msgHeader.messageCS    = fletcher32CS((unsigned char *)&analogExpTelemetry, sizeof(analogExpTelemetry));

	unsigned char *c1 = (unsigned char *)&msgHeader;
	for(i = 0; i < sizeof(msgHeader); i++)
	{
		serialWrite(&serialPort2, *c1);
		c1++;
	}

	unsigned char *c2 = (unsigned char *)&analogExpTelemetry;
	for(j = 0; j < sizeof(analogExpTelemetry); j++)
	{
		serialWrite(&serialPort2, *c2);
		c2++;
	}
}

void sendSOCTelemetryASCII(void)
{
	char stringout[40];

//	socTelemetry.BatteryID = socState.batteryID;
//	socTelemetry.BatSOC = socState.StateOfCharge;			//0% - 100%
//	socTelemetry.BatVolt = socState.BatVolt;
//	socTelemetry.DisChargeCurrent = socState.filtCurrent;
//	socTelemetry.BatSOH = socState.StateOfHealth;
//	socTelemetry.ErrStatus = socState.SocStatus;
//
//	itoa((int)driveMotor.motorCurrentSpeed, stringout, 10);
//	serialPrint(&serialPort2, "RPM: ");
//	serialPrint(&serialPort2, stringout);
//
//	itoa((int)socTelemetry.BatSOC, stringout, 10);
//	serialPrint(&serialPort2, "SOC: ");
//	serialPrint(&serialPort2, stringout);
//
//	itoa((int)(socTelemetry.BatVolt*100), stringout, 10);
//	serialPrint(&serialPort2, " Volt: ");
//	serialPrint(&serialPort2, stringout);
//
//	itoa((int)socTelemetry.DisChargeCurrent, stringout, 10);
//	serialPrint(&serialPort2, " Curr: ");
//	serialPrint(&serialPort2, stringout);
//
//	itoa((int)socTelemetry.ErrStatus, stringout, 10);
//	serialPrint(&serialPort2, " Err: ");
//	serialPrint(&serialPort2, stringout);
//
//	serialPrint(&serialPort2, "\r\n");

}


unsigned int fletcher32CS(unsigned char *buf, int byteCount)
{
	unsigned int tlen = 0;
	unsigned int shortCount = byteCount / sizeof(short);
	unsigned int oddLength  = byteCount % 2;
	cs32A = cs32B = 0xFFFF;

	while(shortCount)
	{
		tlen = shortCount > 360 ? 360 : shortCount;
		shortCount -= tlen;

		do
		{
			cs32A += *buf++;
			cs32A += (*buf++ << 8);
			cs32B += cs32A;
		}while(--tlen);

		if((oddLength == 1) && (shortCount < 1))
		{
			cs32A += *buf++;
			cs32B += cs32A;
		}

		cs32A = (cs32A & 0xFFFF) + (cs32A >> 16);
		cs32B = (cs32B & 0xFFFF) + (cs32B >> 16);
	}
	cs32A = (cs32A & 0xFFFF) + (cs32A >> 16);
	cs32B = (cs32B & 0xFFFF) + (cs32B >> 16);

	return (cs32B << 16 | cs32A);
}

