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
	// Отработка сервов из ТКПА
	ITask3Interface* _task2;
	// // Задача 2
	// ITask2Interface* _task2;
	// Задача 6-8
	ITask68Interface* _task68;

	// Форматтер отсылки пакетов, отсылаем отсюда
	UartTransportPacketFormatterClass* _formatter;
 public:
	
	UAViant_IO_Formatter(UartTransportPacketFormatterClass *a_formatter, ITask3Interface* a_task1,ITask68Interface *a_task8):_formatter(a_formatter),_task2(a_task1), _task68(a_task8) {
    init();
  	}
	// Задача 2
	UAViant_IO_Formatter(UartTransportPacketFormatterClass *a_formatter, ITask3Interface* a_task1):_formatter(a_formatter),_task2(a_task1) {
    init();
  	}

	UAViant_IO_Formatter(UartTransportPacketFormatterClass* a_formatter, ITask68Interface* a_task8) :_formatter(a_formatter), _task68(a_task8) {
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
