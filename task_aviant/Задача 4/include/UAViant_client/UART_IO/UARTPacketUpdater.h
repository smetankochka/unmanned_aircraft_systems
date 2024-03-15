// UARTPacketUpdater.h
#include "Interfaces/IUartPacketsUpdater.h"
#include "Interfaces/IUartPacketsClient.h"
#include "UartPackets.h"

#ifndef _UARTPACKETUPDATER_h
#define _UARTPACKETUPDATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class UARTPacketUpdaterClass:public IUartPacketsUpdater
{
private:
	IUartPacketsClient* _client;
	void _notifyClientOnPacket(Ids a_packetId);
 protected:
	 virtual void onTransportPacketReceived(const uint8_t* a_buf, uint8_t a_bytesCount);
	 virtual void onPostfixLost();

 public:
	void setClient(IUartPacketsClient* a_client);
	void init();
};



#endif
