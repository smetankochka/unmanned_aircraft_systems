// UARTPacketBase.h
#include "../UartPacketsIds.h"

#ifndef _UARTPACKETBASE_h
#define _UARTPACKETBASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif






#pragma pack(push, 1)

template<class TBody>
struct PacketData {
	Ids			_id;
	TBody		_body;
	uint16_t	_checksum;
};
#pragma pack(pop)






template<class TBody, Ids TId>
class PacketImpl
{
private:
	typedef PacketData<TBody> Data;


public:
	enum : uint32_t {
		dataSizeBytes = sizeof(Data),
		checksumedBytesCount = dataSizeBytes - sizeof(decltype(Data::_checksum)),
	};

	PacketImpl()
	{
		_data._id = TId;
	}

	static constexpr Ids getId()
	{
		return TId;
	}

	bool tryUpdateFromBuf(const uint8_t* a_buf, uint8_t a_bytesCount)
	{
		if (a_bytesCount != dataSizeBytes) {
			Serial.println("DataSizeBytes error: " + String(dataSizeBytes) +" a_bytesCount :" + String(a_bytesCount) +" ID: "+String(getId()));
			return false;
		}
			

		const Data& pretenderData =
			*reinterpret_cast<const Data*>(a_buf);


		const uint16_t localChecksum = localChckSum(a_buf, checksumedBytesCount);

		if (pretenderData._checksum != localChecksum){
			Serial.println("checksum bug: local: "+ String(localChecksum)+" recive: "+ String(pretenderData._checksum));
			return false;
		}
		memcpy(&_data, a_buf, dataSizeBytes);

		return true;
	}

	uint16_t localChckSum(const uint8_t* a_buf, uint8_t a_bytesCount)
	{
		uint16_t a_checksum = 0;
		for (size_t i = 0; i < a_bytesCount; ++i)
			a_checksum += a_buf[i];

		return a_checksum;
	}



	void updateChecksum()
	{
		_data._checksum = localChckSum(
			reinterpret_cast<uint8_t*>(&_data), checksumedBytesCount);
	}


	const TBody& getBody() const
	{
		return _data._body;
	}

	TBody& getBodyMutable()
	{
		return _data._body;
	}


	const uint8_t* getData() const
	{
		return reinterpret_cast<const uint8_t*>(&_data);
	}

private:
	Data	_data;
};




#endif
