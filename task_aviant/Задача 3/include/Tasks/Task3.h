

#ifndef _TASK3_h
#define _TASK3_h
#include "UAViant_client\Tasks\TaskInterface.h"
#include "PWM_config.h"
#include "Mavlink/Companion_mavlink.h"



#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



class Task3_Solution:public ITask3Interface, Companion_mavlink
{
protected:

public:
	Task3_Solution() {
		
		
	};
	void init(HardwareSerial* a_mav_serial);
	void init_via_debug(HardwareSerial* a_mav_serial,HardwareSerial* a_debug);

	// Функция для переопределения RC сигнала на ardupilot
	void mav_rc_override(Task3_PWM a_pwm);
	


	
	
	uint32_t t_last_control;
	uint32_t control_interval = 5; // 200 Hz = 5 ms

	// Функция используемая для обмена данными с симулятором ( Для отладки )
	virtual Task3_PWM Task3_in_the_loop(const float& a_Elevator_zad, const float& a_Rudder_zad,const float& a_Elerons_zad, const float& a_Thrust_zad,const float &a_Pitch_deg,const float &a_Gamma_deg) override;
	// Функция - шаблон для реализации режима стабилизации
	Task3_PWM stab_mode(double Pitch,double Roll);
	// Функция - шаблон для реализации прямого контроля органов управления
	Task3_PWM full_control_mode(const float& a_Elevator_zad, const float& a_Rudder_zad,const float& a_Elerons_zad);

	void myloop(){
		loop();
	}
};

#endif
