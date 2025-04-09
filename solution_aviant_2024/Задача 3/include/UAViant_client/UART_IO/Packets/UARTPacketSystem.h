#include "UARTPacketBase.h"

#ifndef _UARTPACKETSYSTEM_h
#define _UARTPACKETSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
// прикладной пакет UART типа "ППМ"
#pragma pack(push, 1)
// информационное наполнение прикладного пакета UART типа "Команда"
struct PZ_body {
	// ППМ СК симулятора
	float Xs;
	float Ys;
	float Zs;
};
#pragma pack(pop)

//прикладной пакет UART типа "ППМ"
typedef PacketImpl<PZ_body, Ids::SinglePPM> PcktSinglePPM;







#endif