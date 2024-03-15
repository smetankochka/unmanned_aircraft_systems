// UartTransportPacketTraits.h

#ifndef _UARTTRANSPORTPACKETTRAITS_h
#define _UARTTRANSPORTPACKETTRAITS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// 2 байта терминатора
enum traits : uint8_t {
	termByte1 = 0xA5,
	termByte2 = 0x5A,
};


#endif
