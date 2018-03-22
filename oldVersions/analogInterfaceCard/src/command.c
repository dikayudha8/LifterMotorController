#include "command.h"
#include "digipot.h"

commandData_t uplinkData;

void processUplink(void)
{
	switch(uplinkData.msgID)
	{
		case MSGID_ANALOG_CMD:
		{
			digipotCmd.potVal1 = uplinkData.priPayload.analogCmd.potVal1;
			digipotCmd.potVal2 = uplinkData.priPayload.analogCmd.potVal2;
			digipotCmd.potVal3 = uplinkData.priPayload.analogCmd.potVal3;
			digipotCmd.potVal4 = uplinkData.priPayload.analogCmd.potVal4;
			digipotCmd.potVal5 = uplinkData.priPayload.analogCmd.potVal5;
			digipotCmd.potVal6 = uplinkData.priPayload.analogCmd.potVal6;
			break;
		}

	}
}


void commandCharIn(unsigned char ch)
{

	switch (uplinkData.state)
	{
	case COMMAND_WAIT_SYNC1:
		if(ch == COMMAND_SYNC1)
		{
			uplinkData.state = COMMAND_WAIT_SYNC2;
			uplinkData.header.sync0 = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_SYNC2:
		if(ch == COMMAND_SYNC2)
		{
			uplinkData.state = COMMAND_WAIT_SYNC3;
			uplinkData.header.sync1 = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_SYNC3:
		if(ch == COMMAND_SYNC3)
		{
			uplinkData.state = COMMAND_WAIT_SYNC4;
			uplinkData.header.sync2 = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_SYNC4:
		if(ch == COMMAND_SYNC4)
		{
			uplinkData.state = COMMAND_WAIT_MSGMJ;
			uplinkData.header.sync3 = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_MSGMJ:
		if(ch == MESSAGE_MAJOR)
		{
			uplinkData.state = COMMAND_WAIT_MSGMI;
			uplinkData.header.msgMaj = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_MSGMI:
		if(ch >= MESSAGE_MINOR)
		{
			uplinkData.state = COMMAND_WAIT_MSGBF;
			uplinkData.header.msgMin = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_MSGBF:
		uplinkData.state = COMMAND_WAIT_SPARE;
		uplinkData.header.msgBfx = ch;
		break;

	case COMMAND_WAIT_SPARE:
		if(ch == MESSAGE_SPARE)
		{
			uplinkData.state = COMMAND_WAIT_SRC0;
			uplinkData.header.mSpare = ch;
		}
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_SRC0:
		uplinkData.state = COMMAND_WAIT_SRC1;
		uplinkData.header.srcID = ch;
		break;

	case COMMAND_WAIT_SRC1:
		uplinkData.state = COMMAND_WAIT_DES0;
		uplinkData.header.srcID += (ch << 8);
		break;

	case COMMAND_WAIT_DES0:
		uplinkData.state = COMMAND_WAIT_DES1;
		uplinkData.header.desID = ch;
		break;

	case COMMAND_WAIT_DES1:
		uplinkData.header.desID += (ch << 8);
		if(uplinkData.header.desID == OEM_ID)
			uplinkData.state = COMMAND_WAIT_MSGID0;
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_MSGID0:
		uplinkData.state = COMMAND_WAIT_MSGID1;
		uplinkData.header.msgID.bval[0] = ch;
		break;

	case COMMAND_WAIT_MSGID1:
		uplinkData.state = COMMAND_WAIT_PROJID0;
		uplinkData.header.msgID.bval[1] = ch;
		uplinkData.msgID = uplinkData.header.msgID.ival;
		break;

	case COMMAND_WAIT_PROJID0:
		uplinkData.state = COMMAND_WAIT_PROJID1;
		uplinkData.header.projID.bval[0] = ch;
		break;

	case COMMAND_WAIT_PROJID1:
		uplinkData.header.projID.bval[1] = ch;
		uplinkData.projID = uplinkData.header.projID.ival;
		if(uplinkData.projID == PROJ_ID)
			uplinkData.state = COMMAND_WAIT_LEN0;
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_WAIT_LEN0:
		uplinkData.state = COMMAND_WAIT_LEN1;
		uplinkData.header.msgSize = ch;
		break;

	case COMMAND_WAIT_LEN1:
		uplinkData.state = COMMAND_WAIT_LEN2;
		uplinkData.header.msgSize += (ch << 8);
		break;

	case COMMAND_WAIT_LEN2:
		uplinkData.state = COMMAND_WAIT_LEN3;
		uplinkData.header.msgSize += (ch << 16);
		break;

	case COMMAND_WAIT_LEN3:
		uplinkData.state = COMMAND_HCHECK0;
		uplinkData.header.msgSize += (ch << 24);
		uplinkData.msgLen = uplinkData.header.msgSize - (sizeof(uplinkData.header) + 8);//2 for checksum
		uplinkData.headerCS.ival = fletcher32CS((unsigned char *)&uplinkData.header, sizeof(uplinkData.header));
		break;

	case COMMAND_HCHECK0:
		if(uplinkData.headerCS.bval[0] == ch)
			uplinkData.state = COMMAND_HCHECK1;
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_HCHECK1:
		if(uplinkData.headerCS.bval[1] == ch)
			uplinkData.state = COMMAND_HCHECK2;
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_HCHECK2:
		if(uplinkData.headerCS.bval[2] == ch)
			uplinkData.state = COMMAND_HCHECK3;
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_HCHECK3:
		if(uplinkData.headerCS.bval[3] == ch)
			uplinkData.state = COMMAND_MCHECK0;
		else
			uplinkData.state = COMMAND_WAIT_SYNC1;
		break;

	case COMMAND_MCHECK0:
		uplinkData.payloadCS = ch;
		uplinkData.state = COMMAND_MCHECK1;
		break;

	case COMMAND_MCHECK1:
		uplinkData.payloadCS += (ch << 8);
		uplinkData.state = COMMAND_MCHECK2;
		break;

	case COMMAND_MCHECK2:
		uplinkData.payloadCS += (ch << 16);
		uplinkData.state = COMMAND_MCHECK3;
		break;

	case COMMAND_MCHECK3:
		uplinkData.payloadCS += (ch << 24);
		uplinkData.state = COMMAND_PAYLOAD;
		if(uplinkData.msgLen == 0)
		{
			if(uplinkData.payloadCS == fletcher32CS((unsigned char *)&uplinkData.priPayload, uplinkData.msgLen))
				processUplink();
			uplinkData.state = COMMAND_WAIT_SYNC1;
		}
		else uplinkData.count = 0;
		break;

	case COMMAND_PAYLOAD:
		*((char *)(&uplinkData.priPayload) + uplinkData.count) = ch;
		if (++uplinkData.count == uplinkData.msgLen)
		{
			u32 chksum = fletcher32CS((unsigned char *)&uplinkData.priPayload, uplinkData.msgLen);
			if(uplinkData.payloadCS == chksum)
				processUplink();
			uplinkData.state = COMMAND_WAIT_SYNC1;
		}
		break;
	}
}
