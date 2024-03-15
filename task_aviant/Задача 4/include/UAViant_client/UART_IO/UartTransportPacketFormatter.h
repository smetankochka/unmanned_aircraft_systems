// UartTransportPacketFormatter.h


#ifndef _UARTTRANSPORTPACKETFORMATTER_h
#define _UARTTRANSPORTPACKETFORMATTER_h



#include "UartTransportPacketTraits.h"



#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class UartTransportPacketFormatterClass
{
 protected:


 public:
	 UartTransportPacketFormatterClass(HardwareSerial* a_Serial);
	 uint32_t getBytesCount() const;
	 uint8_t getByte(uint32_t a_index);
	 void SendBuf(const uint8_t* a_data, uint8_t a_byteCount);
	 HardwareSerial* _Serial;
 private:
	const uint8_t* _data;
	uint8_t		   _bytesCount;
};

//extern UartTransportPacketFormatterClass UartTransportPacketFormatter;

#endif
