// UARTPacketSignal.h
#include "UARTPacketBase.h"
#ifndef _UARTPACKETSIGNAL_h
#define _UARTPACKETSIGNAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// Тело пакета "управляющий сигнал"
#pragma pack(push, 1)
struct SignalBody {
    float Tang_direct = 0;      // Заданный угол тангажа поступающий на стабилизатор, град.
    float Gamma_direct = 0;     // Заданный угол крена поступающий на стабилизатор, град.
};
#pragma pack(pop)

typedef PacketImpl<SignalBody, Ids::Signal> PcktSignal;

#endif
