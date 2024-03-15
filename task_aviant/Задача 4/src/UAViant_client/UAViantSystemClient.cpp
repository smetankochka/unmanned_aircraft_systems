// 
// 
// 

#include "UAViant_client/UAViantSystemClient.h"



void UAViantSystemClient::onUartPacketReceived(::Ids a_packetId)
{
	switch (a_packetId)
	{
	case bogusPacketAtPosfixLoss:
		break;
	case malformed:
		break;
	case invalid:
		break;
	case unknown:
		break;
	case cmd:
		break;
	case Telemetry:
		UAViant_IO->SendSignalPacket();
		break;
	case Signal:
		break;
	case Protection:
		break;
	case ModeSwitcher:
		break;
	case OMSPacket:
		break;
	case TCTCSendPacket:
		UAViant_IO->SendTCTCrPacket();
	case OMSTarget:
		
	default:
		break;
	}
}

void UAViantSystemClient::onUartPostfixLost()
{
	_UARTformatter->_Serial->flush();// Serial.flush();
	_UARTformatter->_Serial->println("onUartPostfixLost");
}
