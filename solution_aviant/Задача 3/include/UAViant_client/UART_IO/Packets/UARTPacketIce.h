#include "UARTPacketBase.h"

#ifndef _UARTPACKETICE_h
#define _UARTPACKETICE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
// прикладной пакет UART типа "Датчик обледенения"
#pragma pack(push, 1)
// информационное наполнение прикладного пакета UART типа "Команда"
struct OMSBody {
	// Датчик обледенения
	float IceSignal;
	float dirtSensorSignal;
	// Подвес
	float Teta;
	float Gamma;
};
#pragma pack(pop)

//прикладной пакет UART типа "Датчик обледенения"
typedef PacketImpl<OMSBody, Ids::OMSPacket> PcktOMS;


#pragma pack(push, 1)
// информационное наполнение прикладного пакета UART типа "Управление сервоприводом подвеса камеры"
struct SuspBody {
	// Подвес камеры
	int16_t PWM_Teta;
	int16_t PWM_Gamma;
};
#pragma pack(pop)


// прикладной пакет UART типа "Управление сервоприводом подвеса камеры"
typedef PacketImpl<SuspBody, Ids::SuspPacket> PcktSuspControl;





#endif