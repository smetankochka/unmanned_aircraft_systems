// UartPacketTelemetry.h
#include "UARTPacketBase.h"
#ifndef _UARTPACKETTELEMETRY_h
#define _UARTPACKETTELEMETRY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Тело пакета "Телеметрия"
#pragma pack(push, 1)
struct TelemetryBody {
    float H = 0.0f; // Высота, м
    float Z = 0.0f; // Отклонение по оси Z, м
    float L = 0.0f; // Дальность,м
    float dH = 0.0f; // Скорость Vy, м/с
    float dZ = 0.0f; // Скорость Vz, м/с
    float dL = 0.0f; // Скорость Vx, м/с
    float wx = 0.0f; // Угловая скорость в связанной СК, 1/с
    float wy = 0.0f; // Угловая скорость в связанной СК, 1/с
    float wz = 0.0f; // Угловая скорость в связанной СК, 1/с
    float Theta = 0.0f; // Угол наклона траектории град.
    float Psi = 0.0f; // Угол курса в связанной СК, град.
    float Gamma = 0.0f; // Угол крена в связанной СК, град.
    float V = 0.0f; // Скорость БПЛА, м/с
    float latitude = 50.0877f; // широта, град.
    float longtitude = 14.2667f;  //долгота, град.
    float Tan = 0.0f; // Угол тангажа, град.
    float Phi = 0.0f; // Угол курса в скоростной СК, град.
    float Gms = 0.0f; // Угол крена в скоростной СК, град.
};
#pragma pack(pop)

typedef PacketImpl<TelemetryBody, Ids::Telemetry> PcktTelemetry;

#pragma pack(push, 1)
struct  Skywalker2015PacketTelemetry
{
  // Нормальная СК
  float H = 0.0f; // Высота НСК, м
  float Z = 0.0f; // Отклонение по оси Z НСК, м
  float L = 0.0f; // Дальность НСК,м
  float V = 0.0f; // Скорость БПЛА НСК, м/с

  //float Vn;
  //float Ve;
  //float Vh;

  // Связанная СК
  float Vx1 = 0.0f; // Скорость Vx ССК, м/с
  float Vy1 = 0.0f; // Скорость Vy ССК, м/с
  float Vz1 = 0.0f; // Скорость Vz ССК, м/с

  float wx = 0.0f; // Угловая скорость в ССК, град/с
  float wy = 0.0f; // Угловая скорость в ССК, град/с
  float wz = 0.0f; // Угловая скорость в ССК, град/с
  float Tan = 0.0f; // Угол тангажа ССК, град.
  float Psi = 0.0f; // Угол курса ССК, град.
  float Gam = 0.0f; // Угол крена ССК, град.
  // Географическая СК
  float latitude = 50.0877f; // широта, град.
  float longtitude = 14.2667f; //долгота, град.
};
#pragma pack(pop)

typedef PacketImpl<Skywalker2015PacketTelemetry, Ids::Telemetry> PcktTeleSky2015;


#endif
