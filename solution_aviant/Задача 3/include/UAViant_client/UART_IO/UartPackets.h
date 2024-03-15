// UartPackets.h
#include "Packets/UartPacketCommand.h"
#include "Packets/UartPacketTelemetry.h"
#include "Packets/UartPacketSignal.h"
#include "Packets/UartPacketModeSwitcher.h"
#include "Packets/UARTPacketIce.h"
#include "Packets/UARTPacketTCTC.h"
#include "Packets/UARTPacketSystem.h"
#include "Packets/UartPacketTarget.h"


#ifndef _UARTPACKETS_h
#define _UARTPACKETS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//
PcktCommand& getPcktCommand();

PcktSuspControl& getPcktSuspControl();
//
PcktTeleSky2015& getPcktTele();
//
PcktSignal& getPcktSignal();
//
AccessSignal& getPcktProtection();
//
ModeSwitcherSignal& getPcktModeSwitcher();
//
PcktOMS & getPcktIce();
//
PcktTCTCSend& getPcktTCTCs();
//
PcktTCTCRecive& getPcktTCTCr();
//
PcktOMSTarget& getPcktOMSTarget();
//
PcktSinglePPM getPcktSinglePPM();

#endif
