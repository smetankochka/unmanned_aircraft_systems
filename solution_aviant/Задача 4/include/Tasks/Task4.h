

#ifndef _TASK4_h
#define _TASK4_h
#include "UAViant_client\Tasks\TaskInterface.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Task4_solution:public ITask8Interface
{
    private:
    struct FlyPoints {
         float North = 0; //                     
         float East = 0; //                       
         float Height = 0; //                       
         FlyPoints(const float& Xg, const float& Yg, const float& Zg) :North(Xg), East(Yg), Height(Zg)
         {};
         FlyPoints() {};
     };

    FlyPoints now_VPP = FlyPoints(405, 300, 120);

    

    // Вспомогательные функции
    // Для приведения углов в пределы -180/180 (градусов)
    inline float AngDefines(float a_angle_deg) {
        float z1 = a_angle_deg * M_PI / 180;
        float z2 = atan2(sin(z1), cos(z1)) * 180 / M_PI;
        return z2;
    };

    
 protected:
    virtual SignalBody Task8_in_the_loop(Skywalker2015PacketTelemetry a_telemetry) override;
    CommandBody Task8_in_the_loop_cmd(Skywalker2015PacketTelemetry a_telemetry)
    {  
        CommandBody ans;
        if (cargo_drop)
        {
          ans.cmd = Delivery::Drop;
          printtoDebugSerial("DROP!");
        }
        return ans;
    }

    void overwritePoint(const float& Xg, const float& Yg, const float& Zg)
    {
      now_VPP.North = Xg;
      now_VPP.East = Zg;
      now_VPP.Height = Yg;
    }
	 

 public:
    FlyPoints _PointsArray[20]; // P.S. Индексация элементов массива [0]...[20]
                               // Порядок расположения точек в массиве может определятся участниками по своему усмотрению. Начальные значения задавать в функции init();
    size_t _Point_Index = 0;  // Индекс текущей путевой точки
    bool cargo_drop = false;  // Флаг для сброса груза


    void init(HardwareSerial* a_Serial);
    float GammaReg(float target_gamma,const Skywalker2015PacketTelemetry& a_telemetry);
    float HeightReg(const Skywalker2015PacketTelemetry& a_telemetry, const float& Hz);
    float ToPointXZ(const Skywalker2015PacketTelemetry& a_telemetry, const float& a_Xg, const float& a_Yg);
    float PointsFlyGam(const Skywalker2015PacketTelemetry& a_telemetry, const size_t& a_Point_index);
    float PointsFlyTan(const Skywalker2015PacketTelemetry& a_telemetry, const size_t& a_Point_index);
    size_t GetNowPointIndex(const Skywalker2015PacketTelemetry& a_telemetry);
    size_t GetPointIndex(const Skywalker2015PacketTelemetry& a_telemetry);

    void printtoDebugSerial(String a_debug_string)
    {
      if (debug_serial) {
        debug_serial->println(a_debug_string);
      }
    }

    HardwareSerial *debug_serial;
};


#endif
