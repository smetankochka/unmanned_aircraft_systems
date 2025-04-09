// 
// 
// 

#include "UAViant_client/UART_IO/UartTransportPacketParser.h"

Parser::Parser(IUartPacketsUpdater& a_packetsUpdater)
	: _packetsUpdater(a_packetsUpdater)
{}

void Parser::processByte(uint8_t a)
{
	// нормальная последовательность действий - получение 1 байта 
	// длины данных, затем этого количества байт, и затем двух 
	// терминаторов. Уведомление апдейтера прикладных пакетов о 
	// завершении нового транспортного пакета.

	// При неполучении терминаторов вовремя или нарушении их 
	// последовательности - переход в режим ожидания корректной 
	// последовательности терминаторов, по приеме которой - уведомление 
	// клиента о завершении приема некорректного пакета, и начало нового
	// цикла приема.

	switch (_state)
	{
	case State::waitingSize:
		// приняли размер грядущего пакета. Подготовимся к 
		// приему указанного количества байт
		_expectedSize = a;
		_currentSize = 0;
		_state = (a == 0) ? State::waitingTerm1 : State::gatheringData;
		break;

	case State::gatheringData:
		// сбор данных до тех пор, пока не наберем указанное их количество,
		// и затем переход к ожиданию терминаторов
		_rawBuf[_currentSize++] = a;
		if (_currentSize >= _expectedSize)
			_state = State::waitingTerm1;
		break;

	case State::waitingTerm1:
		// мы приняли все данные пакета, так что теперь ждем первого 
		// признака конца пакета (termByte1)
		if (a == traits::termByte1)
			_state = State::waitingTerm2;
		else {
			_state = State::errorWaitingTerm1;
			_packetsUpdater.onPostfixLost();
		}
		break;

	case State::waitingTerm2:
		if (a == traits::termByte2) {
			// все корректно: приняли А5-5А. Обрабатываем принятый пакет
			// и запускаем прием следующего пакета
			if (_expectedSize)
				_packetsUpdater.onTransportPacketReceived(_rawBuf, _expectedSize);
			else
				// при нулевой длине принятого пакета нет смысла передавать
				// указатель на данные: их не было. Покажем это 
				// клиенту явно
				_packetsUpdater.onTransportPacketReceived(nullptr, 0);

			_state = State::waitingSize;
		}
		else {
			// после termByte1 приняли не termByte2. Переходим в режим 
			// обработки ошибки приема, причем если мы приняли еще один
			// termByte1 (следом за первым termByte1), то ждать будем сразу 
			// termByte2
			_packetsUpdater.onPostfixLost();
			if (a == traits::termByte1)
				_state = State::errorWaitingTerm2;
			else
				_state = State::errorWaitingTerm1;
		}
		break;

	case State::errorWaitingTerm1:
		// не получили termByte1, где ожидали - так будем ждать
		// его до тех пор, пока не примем
		if (a == traits::termByte1)
			_state = State::errorWaitingTerm2;
		break;

	case State::errorWaitingTerm2:
		if (a == traits::termByte2) {
			// если приняли ожидаемый termByte2, значит, наконец какой-то
			// пакет завершился (может быть. А возможно, это просто 
			// последовательность А55А в середине данных, это будет ясно
			// по итогам попытки приема нового пакета). Запускаем прием 
			// следующего пакета
			_state = State::waitingSize;
		}
		else if (a == traits::termByte1) {
			// опять приняли termByte1. Странно, но возможно. 
			// Так и остаемся в режиме ожидания termByte2.
		}
		else {
			// приняли termByte1, ждали termByte2 или, на худой конец, 
			// еще один termByte1, а приняли совершенно другой байт.
			// Вероятно, тот termByte1 был случайным совпадением в 
			// потоке данных. Значит, нужно снова ждать его же
			_state = State::errorWaitingTerm1;
		}

		break;

	default:
		break;
	}
}
