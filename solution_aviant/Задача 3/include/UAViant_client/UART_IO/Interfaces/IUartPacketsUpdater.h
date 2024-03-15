// IUartPacketsUpdater.h

#ifndef _IUARTPACKETSUPDATER_h
#define _IUARTPACKETSUPDATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
struct IUartPacketsUpdater
{
	virtual void onTransportPacketReceived(const uint8_t* a_buf, uint8_t a_bytesCount) = 0;
	virtual void onPostfixLost() = 0;
};

#endif
