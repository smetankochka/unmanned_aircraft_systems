#ifndef _COMPANIONMAV_h
#define _COMPANIONMAV_h

#include "mavlink.h"

// Serial 2
// #define RXD2 16
// #define TXD2 17

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



struct _telemetry_mavlink {
	double gnss_lat_deg;
	double gnss_lon_deg;
	double gnss_alt_m;
	double gnss_alt_sea_level_m;
	double gnss_yaw_deg;
	double gnss_spd_mps;


	// Nav filtred.
	double nav_north_pos_m;
	double nav_east_pos_m;
	double nav_down_pos_m;


	double nav_north_vel_mps;
	double nav_east_vel_mps;
	double nav_down_vel_mps;


	double nav_gnd_spd_mps; // Земная скорость, м/с
	double nav_ias_mps;     // Приборная скорость, м/с

	double nav_pitch_rad;
	double nav_roll_rad;
	double nav_hdg_rad;

	double nav_gyro_x_radps; // Угловая скорость wx, рад/сек.
	double nav_gyro_y_radps; // Угловая скорость wx, рад/сек.
	double nav_gyro_z_radps; // Угловая скорость wx, рад/сек.
};

struct IMU_orient{
    double Yaw;
    double Pitch;
    double Roll;
};


//Available flight modes
static const String STABILIZE = "STABILIZE";
static const String ALTHOLD = "ALTHOLD";
static const String LOITER = "LOITER";
static const String AUTO = "AUTO";
static const String CIRCLE = "CIRCLE";

//Set of the rotors value commands
static const String SET_ROLL = "SET_ROLL_";//+ int
static const String SET_PITCH = "SET_PITCH_";//+ int
static const String SET_THROTTLE = "SET_THROTTLE_";//+ int
static const String SET_YAW = "SET_YAW_";//+ int

//Sets elementary actions
static const String SET_ARM = "SET_ARM";
static const String SET_DISARM = "SET_DISARM";


// UART2
#define RXD2 16
#define TXD2 17
// UART0
#define RXD0 3
#define TXD0 1

#define DEBUG

class Companion_mavlink
{
private:
    

    // Всякие системные штуки
    double MILLIG_TO_MS2;
    uint8_t system_id;
    uint8_t component_id;
    uint8_t type;
    uint8_t autopilot;
    uint8_t received_sysid; // Pixhawk sysid
    uint8_t received_compid; // Pixhawk compid

    boolean current_arm = false;
    String current_mode = STABILIZE;



    std::array<bfs::MissionItem, 250> mission;
    std::array<bfs::MissionItem, 250> fence;
    std::array<bfs::MissionItem, 5> rally;
    std::array<bfs::MissionItem, 250> temp;

    bfs::MavLink<5, 10> gcs_mavlink;

    
    uint8_t ms_Raw_sens_stream = 5;
	uint8_t ms_Pos_stream = 10;
	uint8_t ms_ext_status_stream = 11;

	uint8_t ms_extra1_stream = 12;
	uint8_t ms_extra2_stream = 13;
	uint8_t ms_extra3_stream = 14;
    



public:
// Упрощенный вариант, тест
    void init(HardwareSerial * a_mav_serial);
// База
    void init_base(HardwareSerial * a_mav_serial);
    void init_IMU();
    void loop();
private:
    bool serial_begin();
    void Stream();
protected:

    void mav_update();
    

    _telemetry_mavlink mav_telemetry;
    IMU_orient mav_orient;
    // Сериал порт под мавлинк
    HardwareSerial* _mav_serial;
    // Сериал порт под дебаг (по сути не нужно)
    HardwareSerial* _debug;

    

    void Read_acceleration();
    void mav_arm_pack(boolean state) {
         mavlink_message_t msg;
         uint8_t buf[MAVLINK_MAX_PACKET_LEN];

       
         if (state) {
             //ARM
             mavlink_msg_command_long_pack(0xFF, 0xBE, &msg, 1, 1, 400, 1, 1.0, 0, 0, 0, 0, 0, 0);
         }
         else {
             //DISARM
             mavlink_msg_command_long_pack(0xFF, 0xBE, &msg, 1, 1, 400, 1, 0.0, 0, 0, 0, 0, 0, 0);
         }
         uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
         _mav_serial->write(buf, len);
     }
    
};





#endif