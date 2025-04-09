
#ifndef _ITASKINTERFACE_h
#define _ITASKINTERFACE_h
#include "../UART_IO/Packets/UartPacketTelemetry.h"
#include "../UART_IO/Packets/UARTPacketSignal.h"
#include "../UART_IO/Packets/UARTPacketTCTC.h"
#include "../UART_IO/Packets/UartPacketCommand.h"
#include "../UART_IO/Packets/UartPacketTarget.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct Task3_PWM {
	uint16_t Elevator_PWM = 0; //
	uint16_t Rudder_PWM = 0;	//
	uint16_t Elerons_PWM = 0;	// 
	uint16_t Engine_PWM = 0;	// 
	Task3_PWM(uint16_t a_Elerons_PWM = 0, uint16_t a_Rudder_PWM = 0, uint16_t a_Elevator_PWM = 0, uint16_t a_Engine_PWM = 0) :Elerons_PWM(a_Elerons_PWM), Rudder_PWM(a_Rudder_PWM), Elevator_PWM(a_Elevator_PWM), Engine_PWM(a_Engine_PWM) 
	{};

};


struct ITask3Interface {
	virtual Task3_PWM Task3_in_the_loop(const float& a_Elevator_zad, const float& a_Elerons_zad, const float& a_Thrust_zad, const float& a_Rudder_zad, const float &a_Pitch_deg, const float &a_Gamma_deg) = 0;
};


struct ITask68Interface {
	virtual SignalBody Task68_in_the_loop(Skywalker2015PacketTelemetry a_telemetry,OMS_target_Body a_target) = 0;
	virtual CommandBody Task68_in_the_loop_cmd(Skywalker2015PacketTelemetry a_telemetry,OMS_target_Body a_target) = 0;
};

#endif