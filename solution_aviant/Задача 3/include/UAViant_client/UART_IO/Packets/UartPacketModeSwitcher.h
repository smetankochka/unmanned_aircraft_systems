// UartPacketModeSwitcher.h
#include "UARTPacketBase.h"

#ifndef _UARTPACKETMODESWITCHER_h
#define _UARTPACKETMODESWITCHER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


enum Exchange_mode :uint16_t {
	CONTROL,
	TELEMETRY
};


#pragma pack(push, 1)
struct ModeSwitcherBody {
	Exchange_mode _mode;
};
#pragma pack(pop)
typedef PacketImpl<ModeSwitcherBody, Ids::ModeSwitcher> ModeSwitcherSignal;


#pragma pack(push, 1)

struct AccessCheckBody {
	float hey;
};
#pragma pack(pop)
typedef PacketImpl<AccessCheckBody, Ids::Protection> AccessSignal;



#endif

