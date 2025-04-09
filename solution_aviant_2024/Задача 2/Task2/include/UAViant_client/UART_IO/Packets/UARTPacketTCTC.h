// UARTPacketTCTC.h

#ifndef _UARTPACKETTCTC_h
#define _UARTPACKETTCTC_h
#include "UARTPacketBase.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// прикладной пакет UART типа "ТКПА"
#pragma pack(push, 1)
// информационное наполнение прикладного пакета отправки UART типа "ТКПА"
struct TCTC_s_Body {
	float Elerons_pos_deg = 0;	// 
	float Rudder_pos_deg = 0;	//
	float Elevator_pos_deg = 0; //
	uint16_t Engine_RPM = 0;	// 
	// Текущие значения
	float Elerons_pos_deg_now = 0;	// 
	float Rudder_pos_deg_now = 0;	//
	float Elevator_pos_deg_now = 0; //
	uint16_t Engine_RPM_now = 0;	// 
};
#pragma pack(pop)
typedef PacketImpl<TCTC_s_Body, Ids::TCTCSendPacket> PcktTCTCSend;

// прикладной пакет UART типа "ТКПА"
#pragma pack(push, 1)
// информационное наполнение прикладного пакета приема UART типа "ТКПА"
struct TCTC_r_Body {
	int16_t Elerons_PWM = 0;	// 
	int16_t Rudder_PWM = 0;	//
	int16_t Elevator_PWM = 0; //
	uint16_t Engine_RPM = 0;	// 
};
#pragma pack(pop)
typedef PacketImpl<TCTC_r_Body, Ids::TCTCRecivePacket> PcktTCTCRecive;

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


