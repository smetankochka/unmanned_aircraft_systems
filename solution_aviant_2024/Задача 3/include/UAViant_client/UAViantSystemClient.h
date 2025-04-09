// UARTSystemClient.h


#ifndef _UARTSYSTEMCLIENT_h
#define _UARTSYSTEMCLIENT_h


#include "UART_IO/Interfaces/IUartPacketsClient.h"
#include "UART_IO/UartTransportPacketParser.h"
#include "UART_IO/UARTPacketUpdater.h"

#include "IO_Formatter/UAViant_IO_Formatter.h"


#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


// 
class UAViantSystemClient:public IUartPacketsClient
{
	//
 private:
	 
	// 
    UartTransportPacketFormatterClass * _UARTformatter;
	// Парсер и форматтер в связке
	Parser* _UARTParser;
	UARTPacketUpdaterClass _UARTUpdater;
	
	HardwareSerial* _serial;
	

 protected:
	 virtual void onUartPacketReceived(::Ids a_packetId) override;
	 virtual void onUartPostfixLost() override;
	 

 public:
   UAViant_IO_Formatter* UAViant_IO;


	UAViantSystemClient(HardwareSerial* a_Serial, ITask3Interface* a_task1 =  nullptr,ITask68Interface *a_task8 = nullptr){

		 _UARTParser = new Parser(_UARTUpdater);
		 _UARTformatter = new UartTransportPacketFormatterClass(a_Serial);
		 _UARTUpdater.setClient(this);
		 UAViant_IO = new UAViant_IO_Formatter(_UARTformatter, a_task1, a_task8);
		 
		 _serial = a_Serial;
		
		
	 };
	UAViantSystemClient(HardwareSerial* a_Serial, ITask68Interface* a_task8 = nullptr) {

		_UARTParser = new Parser(_UARTUpdater);
		_UARTformatter = new UartTransportPacketFormatterClass(a_Serial);
		_UARTUpdater.setClient(this);
		UAViant_IO = new UAViant_IO_Formatter(_UARTformatter, a_task8);

		_serial = a_Serial;

	};
	// // Задача 2 - отработка стаб мода
	// UAViantSystemClient(HardwareSerial* a_Serial, ITask3Interface* a_task2 = nullptr):UAViantSystemClient(a_Serial,a_task2,nullptr) 
	// {

	// };
	
	void UAViant_serialEvent(void) {
		while (_serial->available())
		{
			_UARTParser->processByte((unsigned char)_serial->read());
		}
	}
};



#endif
