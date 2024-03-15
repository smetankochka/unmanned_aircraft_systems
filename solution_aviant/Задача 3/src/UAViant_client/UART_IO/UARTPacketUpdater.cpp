// 
// 
// 

#include "UAViant_client/UART_IO/UARTPacketUpdater.h"

void UARTPacketUpdaterClass::_notifyClientOnPacket(Ids a_packetId)
{
	if (_client)
		_client->onUartPacketReceived(a_packetId);
}

void UARTPacketUpdaterClass::onTransportPacketReceived(const uint8_t* a_buf, uint8_t a_bytesCount)
{
	// возможен случай, когда парсером транспортных пакетов была
// принятая обещанная длина пакета 0, и тогда транспортный
// пакет принят, а данных нет. Также возможно, что транспортный
// пакет структурно корректен с точки зрения его парсера, но 
// при этом слишком короток, чтобы вместить хотя бы идентификатор 
// прикладного пакета.
	if ((a_buf == nullptr) || (a_bytesCount < sizeof(uint32_t))) {
		_notifyClientOnPacket(Ids::malformed);
		return;
	}

	// практика показала, что по каким-то причинам a_buf, приходящий
	// из парсера, не выровнен по границе 4 байт, невзирая даже на
	// добавляемые в него прагмы выравнивания. Поэтому попытка получить
	// идентификатор пакета с помощью *reinterpert_cast<Ids*>(a_buf)
	// оканчивается HardFault-ом. Однако компилятор гарантирует 
	// корректность копирования для полей структур, невзирая на их
	// выровненность. Поэтому, зная, что все пакеты имеют одинаковое 
	// начало, обусловленное структурой шаблонного класса PacketData, 
	// используем трюк с доступом к идентификатору пакета через поле 
	// фейковой структуры
	typedef uint8_t BogusBody;
	typedef PacketData<BogusBody> BogusPacket;

	const Ids packetId =
		reinterpret_cast<const BogusPacket*>(a_buf)->_id;

	if (packetId == ::PcktCommand::getId()) {
		// попытку обновления содержимого пакета нужно
		// провести в любом случае
		bool updRes =
			getPcktCommand().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	// Пакет телеметрии
	if (packetId == ::PcktTelemetry::getId()) {
		// попытку обновления содержимого пакета нужно
		// провести в любом случае
		bool updRes =
			getPcktTele().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	//Сигнал защиты, берем новый сигнал и отсылаем его с заполненым ключом
	if (packetId == ::AccessSignal::getId()) {
		// попытку обновления содержимого пакета нужно
		// провести в любом случае
		bool updRes =
			getPcktProtection().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	
	if (packetId == ::PcktSignal::getId()) {
		
		bool updRes =
			getPcktSignal().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	if (packetId == ::ModeSwitcherSignal::getId()) {

		bool updRes =
			getPcktModeSwitcher().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	if (packetId == ::PcktOMS::getId()) {

		bool updRes =
			getPcktIce().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	if (packetId == ::PcktTCTCSend::getId()) {

		bool updRes =
			getPcktTCTCs().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}
	if (packetId == ::PcktOMSTarget::getId()) {

		bool updRes =
			getPcktOMSTarget().tryUpdateFromBuf(a_buf, a_bytesCount);

		_notifyClientOnPacket(updRes ? packetId : Ids::invalid);

		return;
	}



	_notifyClientOnPacket(Ids::unknown);
}

void UARTPacketUpdaterClass::onPostfixLost()
{
	if (_client)
		_client->onUartPostfixLost();
}

void UARTPacketUpdaterClass::setClient(IUartPacketsClient* a_client)
{
	_client = a_client;
}

void UARTPacketUpdaterClass::init()
{
	_client = nullptr;
}



