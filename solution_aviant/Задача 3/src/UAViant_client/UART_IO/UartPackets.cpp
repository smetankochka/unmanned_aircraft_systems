// 
// 
// 

#include "UAViant_client/UART_IO/UartPackets.h"

PcktSuspControl& getPcktSuspControl()
{
	static PcktSuspControl res;
	return res;
}


PcktCommand& getPcktCommand()
{
	static PcktCommand res;
	return res;
}

PcktTeleSky2015& getPcktTele()
{
	static PcktTeleSky2015 res;
	return res;
}
//
PcktSignal& getPcktSignal()
{
	static PcktSignal res;
	return res;
}

PcktOMS& getPcktIce()
{
	static PcktOMS res;
	return res;
}
PcktTCTCSend& getPcktTCTCs()
{
	static PcktTCTCSend res;
	return res;
}
PcktTCTCRecive& getPcktTCTCr()
{
	static PcktTCTCRecive res;
	return res;
}
PcktOMSTarget& getPcktOMSTarget()
{
	static PcktOMSTarget res;
	return res;
}

PcktSinglePPM getPcktSinglePPM()
{
	static PcktSinglePPM res;
	return res;
}



AccessSignal& getPcktProtection()
{
	static AccessSignal res;
	return res;
};

ModeSwitcherSignal& getPcktModeSwitcher()
{
	static ModeSwitcherSignal res;
	return res;
};