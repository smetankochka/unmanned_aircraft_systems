#ifndef _UARTPACKETTARGET_h
#define _UARTPACKETTARGET_h
#include "UARTPacketBase.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#pragma pack(push, 1)
// информационное наполнение прикладного пакета приема UART типа "Координаты цели"
struct OMS_target_Body {
	float X = 0;	// 
	float Y = 0;	//
	float Z = 0;	//
	float r = 0;	// 
};
#pragma pack(pop)
typedef PacketImpl<OMS_target_Body, Ids::OMSTarget> PcktOMSTarget;

#endif