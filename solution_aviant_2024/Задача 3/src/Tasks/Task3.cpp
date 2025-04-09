
#include "Tasks/Task3.h"
/*
####################################################################################################
####################################### Участникам олимпиады #######################################
####################################################################################################

Наименование функции: 	Task3_in_the_loop()  
Назначение функции:		Данная функция используется для обмена данными в режиме ТКПА симулятора


Входные данные приходящие с симулятора:

a_Elevator_zad -  заданное значение руля высоты, град.
a_Rudder_zad - заданное значение руля направления, град. 
a_Elerons_zad - заданное значения элеронов, град.
a_Thrust_zad - заданное значения тяги двигателя, Н.

a_Pitch_deg - текущее значение угла тангажа, град
a_Gamma_deg - текущее значение угла крена, град

Выходные данные идущие на симулятор:

Структура Task2_PWM содержащая следующие поля данных:

Elevator_PWM  - RC сигнал, руль высоты
Rudder_PWM - RC сигнал, руль направления
Elerons_PWM - RC сигнал, элероны
Engine_RPM - RC сигнал, двигатель

Примечание: delay() в данной функции вызывать бесполезно, так как обмен с симулятором синхронный.

define SIM - включить когда отлаживаете ТОЛЬКО на симуляторе, когда переходите на стенд то необходимо закомментировать
*/
// Закомментировать только когда отлаживаете на стенде.
#define SIM

Task3_PWM Task3_Solution::Task3_in_the_loop(const float& a_Elevator_zad, const float& a_Rudder_zad, const float& a_Elerons_zad, const float& a_Thrust_zad,const float &a_Pitch_deg, const float &a_Gamma_deg)
{
    // Структура которая содержит заданный RC сигнал
    Task3_PWM _ans;
    // Текущие углы тангажа и крена с ИИС самолета (работают только при подключении к стенду)
    double IMS_Pitch_deg = mav_orient.Pitch;    // Угол тангажа,градусы
    double IMS_Roll_deg = mav_orient.Roll;      // Угол крена,градусы
    // Текущие углы тангажа и крена поступающие с симулятора
    double SIM_Pitch_deg = a_Pitch_deg;
    double SIM_Roll_deg = a_Gamma_deg;

    // Функция прямого контроля положения органов управления
    // _ans = full_control_mode(a_Elevator_zad,a_Rudder_zad,a_Elerons_zad);
    
    // Функция стабилизации, в зависимости от текущих углов приходящих с симулятора
    _ans = stab_mode(SIM_Pitch_deg,SIM_Roll_deg);
    

  #ifndef SIM

  // Функция стабилизации, в зависимости от текущих углов приходящих с стенда
    // _ans = stab_mode(IMS_Pitch_deg,IMS_Roll_deg);

  // Подаем структуру сигналов на Ardupilot (Участникам не трогать)
  if ((millis() - t_last_control) > control_interval)
  {
      mav_rc_override(_ans);
  }
  #endif
  
  // Возвращаем RC сигнал на симулятор
  return _ans;
}
// Функция прямого контроля положения органов управления
Task3_PWM Task3_Solution::full_control_mode(const float& a_Elevator_zad, const float& a_Rudder_zad,const float& a_Elerons_zad)
{
     // Структура которая содержит заданный RC сигнал
    Task3_PWM _ans;
    _ans.Elerons_PWM = map(a_Elerons_zad, ELERONS_DEG_MIN, ELERONS_DEG_MAX, ALERON_PWM_MIN, ALERON_PWM_MAX);
    _ans.Elevator_PWM = map(-a_Elevator_zad, ELEVATOR_DEG_MIN, ELEVATOR_DEG_MAX, ELEVATOR_PWM_MIN, ELEVATOR_PWM_MAX);
    _ans.Rudder_PWM = map(a_Rudder_zad, RUDDER_DEG_MIN, RUDDER_DEG_MAX, RUDDER_PWM_MIN, RUDDER_PWM_MAX);
 
    return _ans;
}
// Функция стабилизации, в зависимости от текущих углов
Task3_PWM Task3_Solution::stab_mode(double Pitch, double Roll)
{
    // Структура которая содержит заданный RC сигнал
    Task3_PWM _ans;

    float error_pitch = constrain(1.5*Pitch, -20, 20);
    float error_roll = constrain(-1.5*Roll, -20, 20);

    _ans.Elerons_PWM = map(error_roll, -20, 20, ALERON_PWM_MIN, ALERON_PWM_MAX);
    _ans.Elevator_PWM = map(error_pitch, -20, 20, ELEVATOR_PWM_MIN, ELEVATOR_PWM_MAX);
    _ans.Rudder_PWM = 1500;
    
    return _ans;
}

void Task3_Solution::init(HardwareSerial * a_mav_serial)
{
    _mav_serial = a_mav_serial;
    // Запускаем обмен с Matek
    init_base(_mav_serial);
}
void Task3_Solution::init_via_debug(HardwareSerial * a_mav_serial,HardwareSerial * a_debug)
{
    _mav_serial = a_mav_serial;
    _debug = a_debug;
    #ifndef SIM
    // Запускаем обмен с Ardupilot
    _debug->println("Matek initialization start...");
    init_base(_mav_serial);
    #endif
}



void Task3_Solution::mav_rc_override(Task3_PWM a_pwm)
{
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    //  mavlink_msg_rc_channels_override_pack(0xFF, 0xBE, &msg, 1, 1, roll, pitch, throttle, yaw, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0);
    mavlink_msg_rc_channels_override_pack(0xFF, 0xBE, &msg, (uint8_t)1, (uint8_t)1,(uint16_t)a_pwm.Elerons_PWM,(uint16_t)a_pwm.Elevator_PWM,(uint16_t)(1100),(uint16_t)a_pwm.Rudder_PWM,UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX, UINT16_MAX);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    _mav_serial->write(buf, len);
    t_last_control = millis();
}







