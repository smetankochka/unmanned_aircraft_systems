// UARTControlSystem.h


#ifndef _UARTCONTROLSYSTEM_h
#define _UARTCONTROLSYSTEM_h

#include "../UART_IO/UartPackets.h"
#include "../UART_IO/UartTransportPacketFormatter.h"
#include "../Tasks/TaskInterface.h"





#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class UAViant_IO_Formatter
{
private:
	// Интерфейсы для задач:
	// Задача 1
	ITask1Interface* _task1;
	// Задача 8
	ITask8Interface* _task8;

	// Форматтер отсылки пакетов, отсылаем отсюда
	UartTransportPacketFormatterClass* _formatter;
 public:
	
	UAViant_IO_Formatter(UartTransportPacketFormatterClass *a_formatter, ITask1Interface* a_task1,ITask8Interface *a_task8):_formatter(a_formatter),_task1(a_task1), _task8(a_task8) {
    init();
  }
	UAViant_IO_Formatter(UartTransportPacketFormatterClass* a_formatter, ITask8Interface* a_task8) :_formatter(a_formatter), _task8(a_task8) {
		init();
	}
	// null function
	void init();
	// Отправление данных на симулятор ( при приеме пакета - Телеметрия)
	void SendSignalPacket();
	// Отправление данных на симулятор (при приеме пакета - ТКПА)
	void SendTCTCrPacket();
    // Отправка команды сброса
	void SendCmdDropPacket();

};

//extern UARTControlSystemClass UARTControlSystem;

#endif
