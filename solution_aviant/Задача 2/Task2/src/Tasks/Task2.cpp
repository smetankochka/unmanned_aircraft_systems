#include "Tasks/Task2.h"

// Данная функция выполняется только один раз при запуске программы
// (при подаче питания на плату, перепрошивке, или нажатии кнопки reset)
void Task2_solution::init(HardwareSerial* a_Serial)
{
  debug_serial = a_Serial;
  // Примеры вывода в отладочную консоль
  printtoDebugSerial("Hello team!");
  printtoDebugSerial(String(3.1415926));
}

float AngDefines(float a_angle_deg) {
  float z1 = a_angle_deg * M_PI / 180.0;
  float z2 = atan2(sin(z1), cos(z1)) * 180.0  / M_PI;
  return z2;
}

int computePID(float input, float setpoint, float kp, float ki, float kd, float dt, float minOut, float maxOut) {
    float err = setpoint - input;
    static float integral = 0;
    integral = integral + (float) err *  dt * ki;
    float D = (err) * dt;
    float pid_output = err * kp + integral + D * kd;
    return pid_output < minOut ? minOut : pid_output > maxOut ? maxOut : pid_output;
}

int computePID_2(float input, float setpoint, float kp, float ki, float kd, float dt, float minOut, float maxOut) {
    float err = AngDefines(input - setpoint);
    static float integral = 0;
    integral = integral + (float) err *  dt * ki;
    float D = (err) * dt;
    float pid_output = err * kp + integral + D * kd;
    return pid_output < minOut ? minOut : pid_output > maxOut ? maxOut : pid_output;
}

float GammaReg(float target_gamma, Skywalker2015PacketTelemetry a_telemetry)
{
    float Kpsi = 1.6;
    float Psi = a_telemetry.Psi;

    float gamma = computePID_2(Psi, target_gamma, 1, 0, 10, 0.01, -30, 30); // 1.1, 0, 9
    return gamma;
}

float HeightReg(float target_h, Skywalker2015PacketTelemetry a_telemetry)
{
    float NowH = a_telemetry.H;
    float _Pitch_direct = computePID(NowH, target_h, 4.785, 0, 9.89, 0.01, -20, 20);
    return _Pitch_direct;

}



/*
### УЧАСТНИКАМ ОЛИМПИАДЫ  -  Основная функция отправки пакета значений крена и тангажа (град) ####
Данная функция осуществляет циклический обмен данных с симулятором с частотой 100 Гц.
*/
SignalBody Task2_solution::Task8_in_the_loop(Skywalker2015PacketTelemetry a_telemetry)
{
  /* a_telemetry - структура данных, полученная с симулятора. Она содержит текущие параметры БЛА
  a_telemetry.L - координата Х (север) [м]
  a_telemetry.Z - координата Z (восток) [м]
  a_telemetry.H - координата Y (высота) [м]
  a_telemetry.Psi - Угол рысканья [град]
  a_telemetry.Gam - Угол крена [град]
  a_telemetry.Tan - Угол тангажа [град]
  a_telemetry.V -  Скорость полёта БЛА [м/с]
  a_telemetry.Vx1 - Продольная скорость [м/с]
  a_telemetry.Vz1 - Поперечная скорость [м/с]
  a_telemetry.Vy1 - Вертикальная скорость [м/с]
  a_telemetry.wx - Угловая скорость вокруг продольной оси [1/с]
  a_telemetry.wy - Угловая скорость вокруг вертикальной оси [1/с]
  a_telemetry.wz - Угловая скорость вокруг поперечной оси [1/с]
  */

  // РЕКОМЕНДУЕМЫЙ АЛГОРИТМ РЕШЕНИЯ ЗАДАЧИ 3
  //
  // Расчитать необходимое значение угла рысканья
  // На основе текущего значения угла рысканья и заданного угла рысканья определяем необходимый угол крена
  // На основе текущей высоты БЛА, заданной высоты и вертикальной скорости определям необходимый угол тангажа

  SignalBody _ans;  // Структура которая отсылается на симулятор
  _ans.Gamma_direct = GammaReg(360-70, a_telemetry); // заданный угол крена
  _ans.Tang_direct = HeightReg(150, a_telemetry); // заданный угол тангажа

  // Отправляем команды на симулятор
  // БЛА будет пытаться выдерживать заданные углы крена и тангажа
  return _ans;
}
