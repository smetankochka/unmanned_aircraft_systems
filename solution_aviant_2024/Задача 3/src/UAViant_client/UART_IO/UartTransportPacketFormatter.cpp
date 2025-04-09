// 
// 
// 

#include "UAViant_client/UART_IO/UartTransportPacketFormatter.h"

UartTransportPacketFormatterClass::UartTransportPacketFormatterClass(HardwareSerial* a_Serial):
	_Serial(a_Serial)
{

}

uint32_t UartTransportPacketFormatterClass::getBytesCount() const
{
	return _bytesCount + 3;
}

uint8_t UartTransportPacketFormatterClass::getByte(uint32_t a_index)
{
	if (a_index == 0)
		return _bytesCount;

	if (a_index <= _bytesCount)
		return _data[a_index - 1];

	if (a_index == _bytesCount + 1)
		return traits::termByte1;

	if (a_index == _bytesCount + 2)
		return traits::termByte2;

	return 0;
}

void UartTransportPacketFormatterClass::SendBuf(const uint8_t* a_data, uint8_t a_byteCount)
{
	_data = a_data;
	_bytesCount = a_byteCount;

	uint8_t _sendBuf[200];
	for (size_t i = 0; i < getBytesCount(); ++i)
	{
		_sendBuf[i] = getByte(i);
	}
	_Serial->write(_sendBuf, getBytesCount());
}




//UartTransportPacketFormatterClass UartTransportPacketFormatter;
