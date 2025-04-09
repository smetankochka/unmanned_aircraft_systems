#include "Tasks/Task4.h"
const float point_radius = 15;

float AngDefines(float a_angle_deg) {
        float z1 = a_angle_deg * M_PI / 180;
        float z2 = atan2(sin(z1), cos(z1)) * 180 / M_PI;
        return z2;
    };

  // _PointsArray[0] = FlyPoints(250, -250, 70);
  // _PointsArray[1] = FlyPoints(500, 0, 70);
  // _PointsArray[2] = FlyPoints(250, 250, 80);
  // _PointsArray[3] = FlyPoints(500, 500, 70);
  // _PointsArray[4] = FlyPoints(250, 500, 70);
  // _PointsArray[5] = FlyPoints(0, 250, 70);
  // _PointsArray[6] = FlyPoints(0, 0, 80);
  // _PointsArray[7] = FlyPoints(0, -250, 80);
  // _PointsArray[8] = FlyPoints(0, -500, 70);
  // _PointsArray[9] = FlyPoints(-250, -500, 80);
  // _PointsArray[10] = FlyPoints(-250, -250, 70);
  // _PointsArray[11] = FlyPoints(-250, 250, 80);
  // _PointsArray[12] = FlyPoints(-250, 500, 80);
  // _PointsArray[13] = FlyPoints(-500, 250, 80);
  // _PointsArray[14] = FlyPoints(-500, 0, 90);
  // _PointsArray[15] = FlyPoints(0, -250, 90);
  // _PointsArray[16] = FlyPoints(500, -250, 90);
  // _PointsArray[17] = FlyPoints(250, 250, 90); 
  // _PointsArray[18] = FlyPoints(0, 500, 90);
  // _PointsArray[19] = FlyPoints(0, 0, 90);

// Данная функция выполняется только один раз при запуске программы
// (при подаче питания на плату, перепрошивке, или нажатии кнопки reset)
void Task4_solution::init(HardwareSerial* a_Serial)
{
  debug_serial = a_Serial;
  // Примеры вывода в отладочную консоль
  printtoDebugSerial("Hello team!");
  printtoDebugSerial(String(3.1415926));

  // Путевые точки в произвольном порядке
  _PointsArray[0] = FlyPoints(250, -250, 70);
  _PointsArray[1] = FlyPoints(500, -250, 90);
  _PointsArray[2] = FlyPoints(500, 0, 70);
  _PointsArray[3] = FlyPoints(250, 250, 80);
  _PointsArray[4] = FlyPoints(500, 500, 70);
  _PointsArray[5] = FlyPoints(250, 500, 70);
  _PointsArray[6] = FlyPoints(0, 250, 70);
  _PointsArray[7] = FlyPoints(0, 0, 80);
  _PointsArray[8] = FlyPoints(0, -250, 80);
  _PointsArray[9] = FlyPoints(0, -500, 70);
  _PointsArray[10] = FlyPoints(-250, -500, 80);
  _PointsArray[11] = FlyPoints(-250, -250, 70);
  _PointsArray[12] = FlyPoints(-500, 0, 90);
  _PointsArray[13] = FlyPoints(-500, 250, 80);
  _PointsArray[14] = FlyPoints(-250, 500, 80);
  _PointsArray[15] = FlyPoints(-250, 250, 80);
  _PointsArray[16] = FlyPoints(0, 500, 90);
  _PointsArray[17] = FlyPoints(250, 250, 90);
  _PointsArray[18] = FlyPoints(0, 0, 90);
  _PointsArray[19] = FlyPoints(0, -250, 90);
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

float Task4_solution::GammaReg(float target_gamma,const Skywalker2015PacketTelemetry& a_telemetry)
{
    float Kpsi = 1.6;
    float Psi = a_telemetry.Psi;

    float gamma = computePID_2(Psi, target_gamma, 1, 0, 10, 0.01, -30, 30); // 1, 0, 10
    return gamma;
}

float Task4_solution::HeightReg(const Skywalker2015PacketTelemetry& a_telemetry, const float& Hz)
{
    // Здесь пишем регулятор высоты

    float NowH = a_telemetry.H;
    float _Pitch_direct = computePID(NowH, Hz, 4.785, 0, 9.89, 0.01, -20, 20);

    return _Pitch_direct;

}

float Task4_solution::ToPointXZ(const Skywalker2015PacketTelemetry& a_telemetry, const float& a_Xg, const float& a_Yg) {
    /* Функция вычисляет угол курса (угол между вектором направления БПЛА и точкой, в которую нам нужно попасть) */
    float _Xt = a_telemetry.L; // X координата БПЛА
    float _Yt = a_telemetry.Z; // Y координата БПЛА
    float xg_corr = a_Xg; // X точки, в которую нужно попасть
    float yg_corr = a_Yg; // Y точки, в которую нужно попасть
    float result = -atan2(yg_corr - _Yt, xg_corr - _Xt) * 57.3; // курс на точку в градусах

    return result;
}

float Task4_solution::PointsFlyGam(const Skywalker2015PacketTelemetry& a_telemetry, const size_t& a_Point_index)
{
    // Здесь пишем функцию нахождения заданного угла курса в зависимости от индекса точки a_Point_index
    float Xg = _PointsArray[a_Point_index].North; // X точки
    float Zg = _PointsArray[a_Point_index].East; // Y точки
    // Можно использовать шаблон
    float Psi_direct = ToPointXZ(a_telemetry, Xg, Zg); // Вычисляем курс (угол между точкой БПЛА и Шара)
    float gamma_direct = GammaReg(Psi_direct, a_telemetry); // Зная угол курса, вычислим угол крена, чтобы развернуть БПЛА

    return gamma_direct; // отправляем заданный угол крена для достижения заданного угла курса.
}

float Task4_solution::PointsFlyTan(const Skywalker2015PacketTelemetry& a_telemetry, const size_t& a_Point_index)
{
    // Здесь пишем функцию нахождения заданного угла тангажа в зависимости от индекса точки a_Point_index

    float Height_direct = _PointsArray[a_Point_index].Height;
    // определяем заданный угол тангажа для достижения заданной высоты.
    float tang_direct = HeightReg(a_telemetry, Height_direct);

    return tang_direct;
}

size_t Task4_solution::GetNowPointIndex(const Skywalker2015PacketTelemetry& a_telemetry)
{
    float PointX = _PointsArray[_Point_Index].North;
    float PointY = _PointsArray[_Point_Index].Height;
    float PointZ = _PointsArray[_Point_Index].East;

    float NowX = a_telemetry.L;
    float NowY = a_telemetry.H;
    float NowZ = a_telemetry.Z;

    if (sqrt(pow(PointX - NowX, 2) + pow(PointY - NowY, 2) + pow(PointZ - NowZ, 2)) <= point_radius) {
            _Point_Index++;
        }
    return 0;
}


size_t Task4_solution::GetPointIndex(const Skywalker2015PacketTelemetry& a_telemetry) {

    float PointX = _PointsArray[_Point_Index].North;
    float PointY = _PointsArray[_Point_Index].Height;
    float PointZ = _PointsArray[_Point_Index].East;

    float NowX = a_telemetry.L;
    float NowY = a_telemetry.H;
    float NowZ = a_telemetry.Z;

    if (sqrt(pow(PointX - NowX, 2) + pow(PointY - NowY, 2) + pow(PointZ - NowZ, 2)) <= point_radius) {_Point_Index++;}

    return 0;
}



SignalBody Task4_solution::Task8_in_the_loop(Skywalker2015PacketTelemetry a_telemetry)
{
    SignalBody _ans;

    GetPointIndex(a_telemetry);
    
    _ans.Gamma_direct = PointsFlyGam(a_telemetry, _Point_Index);
    _ans.Tang_direct = PointsFlyTan(a_telemetry, _Point_Index);

    return _ans;
}

