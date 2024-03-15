// UartPacketCommand.h
#include "UARTPacketBase.h"
#ifndef _UARTPACKETCOMMAND_h
#define _UARTPACKETCOMMAND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
enum Delivery:uint16_t {
	Nope,
	Drop
};
// Тело пакета "команда отцепки"
#pragma pack(push, 1)
struct CommandBody {
	Delivery  cmd = Nope;
};
#pragma pack(pop)

typedef PacketImpl<CommandBody, Ids::cmd> PcktCommand;

#endif
