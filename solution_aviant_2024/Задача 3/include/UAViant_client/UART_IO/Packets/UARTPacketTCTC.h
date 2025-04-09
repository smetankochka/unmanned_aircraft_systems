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
	float Pitch = 0;
	float Gamma = 0;
	// Текущие значения
	float Elerons_pos_deg_now = 0;	// 
	float Rudder_pos_deg_now = 0;	//
	float Elevator_pos_deg_now = 0; //
	uint16_t Engine_RPM_now = 0;	// 
};
#pragma pack(pop)
typedef PacketImpl<TCTC_s_Body, Ids::TCTCSendPacket> PcktTCTCSend;

// прикладной пакет UART типа "ТКПА" (отработка сервов)
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




#endif


