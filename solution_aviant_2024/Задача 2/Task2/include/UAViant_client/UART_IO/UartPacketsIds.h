// UartPacketsIds.h

#ifndef _UARTPACKETSIDS_h
#define _UARTPACKETSIDS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
enum Ids : uint16_t {

	bogusPacketAtPosfixLoss = 0xFFFD,

	malformed = 0xFFFE,

	invalid = 0xFFFF,
	
	unknown = 0,
	// Пакет "Команда сброса"
	cmd = 0x01,
	// Пакет "Телеметрия"
	Telemetry = 0x02,
	// Пакет "Управляющий сигнал"
	Signal = 0x03,
	// Пакет "датчик обледенения"
	OMSPacket = 0x04,
	// пакет "Сигнал управления на сервопривод подвеса"
	SuspPacket = 0x05,
	// Пакет "Защита"
	Protection = 0xFFDD,
    // пакет "Режим работы"
    ModeSwitcher = 0x05,
	// пакет "ТКПА" Отправка
	TCTCSendPacket = 0x06,
	// пакет "ТКПА" Прием
	TCTCRecivePacket = 0x07,
	// пакет "Внешний объект" отправка
	OMSTarget = 0x08,
	// пакет "ПЗ"
	FlightQuest = 0x09,
};

#endif
