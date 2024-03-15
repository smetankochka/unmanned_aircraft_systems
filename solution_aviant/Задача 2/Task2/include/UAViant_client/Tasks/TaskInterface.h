
#ifndef _ITASKINTERFACE_h
#define _ITASKINTERFACE_h
#include "../UART_IO/Packets/UartPacketTelemetry.h"
#include "../UART_IO/Packets/UARTPacketSignal.h"
#include "../UART_IO/Packets/UARTPacketTCTC.h"
#include "../UART_IO/Packets/UartPacketCommand.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct Task1_PWM {
	int16_t Elevator_PWM = 0; //
	int16_t Rudder_PWM = 0;	//
	int16_t Elerons_PWM = 0;	// 
	uint16_t Engine_PWM = 0;	// 
	Task1_PWM(uint16_t a_Elerons_PWM = 0, uint16_t a_Rudder_PWM = 0, uint16_t a_Elevator_PWM = 0, uint16_t a_Engine_PWM = 0) :Elerons_PWM(a_Elerons_PWM), Rudder_PWM(a_Rudder_PWM), Elevator_PWM(a_Elevator_PWM), Engine_PWM(a_Engine_PWM) 
	{};

};


struct ITask1Interface {
	virtual Task1_PWM Task1_in_the_loop(const float& a_Elevator_zad, const float& a_Elerons_zad, const float& a_Thrust_zad, const float& a_Rudder_zad) = 0;
};
struct ITask6Interface {
	virtual SignalBody Task6_in_the_loop(Skywalker2015PacketTelemetry a_telemetry, OMS_target_Body a_target) = 0;
};
struct ITask8Interface {
	virtual SignalBody Task8_in_the_loop(Skywalker2015PacketTelemetry a_telemetry) = 0;
	virtual CommandBody Task8_in_the_loop_cmd(Skywalker2015PacketTelemetry a_telemetry) = 0;
};

#endif