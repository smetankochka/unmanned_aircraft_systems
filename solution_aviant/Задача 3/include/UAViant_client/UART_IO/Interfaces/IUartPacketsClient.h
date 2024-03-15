// IUartPacketsClient.h
#include "../UartPacketsIds.h"
#ifndef _IUARTPACKETSCLIENT_h
#define _IUARTPACKETSCLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct IUartPacketsClient
{
	virtual void onUartPacketReceived(::Ids a_packetId) = 0;
	virtual void onUartPostfixLost() = 0;
};

#endif
