

#ifndef _TASK3_h
#define _TASK3_h
#include "UAViant_client\Tasks\TaskInterface.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Task3_solution:public ITask8Interface
{
    private:
    struct TargetVals {
         float Yaw = 0; // Заданный угол курса
         float Height = 0; // Заданная высота
         TargetVals(const float& Yaw_target, const float& Height_target) :Yaw(Yaw_target), Height(Height_target)
         {};
         TargetVals() {};
     };

    TargetVals target_values = TargetVals(70, 150);

    

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
	 

 public:
    bool cargo_drop = false;  // Флаг для сброса груза

    void init(HardwareSerial* a_Serial);

    void printtoDebugSerial(String a_debug_string)
    {
      if (debug_serial) {
        debug_serial->println(a_debug_string);
      }
    }

    HardwareSerial *debug_serial;
};


#endif
