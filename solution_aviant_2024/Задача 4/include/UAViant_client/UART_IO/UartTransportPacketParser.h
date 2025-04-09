// UartTransportPacketParser.h
#include "Interfaces/IUartPacketsUpdater.h"
#include "UartTransportPacketTraits.h"

#ifndef _UARTTRANSPORTPACKETPARSER_h
#define _UARTTRANSPORTPACKETPARSER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class Parser
{
public:
	Parser(IUartPacketsUpdater& a_packetsUpdater);
	void processByte(uint8_t a);

private:
	enum class State : uint32_t {
		// состояние ожидания байта длины данных
		waitingSize,
		// сбор данных
		gatheringData,
		// ожидание первого из пары байт-терминаторов
		waitingTerm1,
		// перый терминатор принят, ожидание второго из пары байт-терминаторов
		waitingTerm2,
		// случилась ошибка при приеме первого из байт-терминаторов
		errorWaitingTerm1,
		// случилась ошибка при приеме второго из байт-терминаторов, 
		// или дождались приема запоздавшего первого из них
		errorWaitingTerm2
	};

	IUartPacketsUpdater& _packetsUpdater;

	State	_state = State::waitingSize;
	uint8_t	_expectedSize = 0;
	uint8_t	_currentSize = 0;
	uint8_t _rawBuf[0x100] = { 0 };
};

#endif
