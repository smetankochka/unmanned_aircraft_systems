#include "Mavlink/Companion_mavlink.h"


void Companion_mavlink::init(HardwareSerial *a_mav_serial)
{
     _mav_serial = a_mav_serial;

    MILLIG_TO_MS2 = 9.80665 / 1000.0;
	system_id = 1; // Your i.e. Arduino sysid
	component_id = 158; // Your i.e. Arduino compid
	type = MAV_TYPE_FIXED_WING;
	autopilot = MAV_AUTOPILOT_INVALID;

    gcs_mavlink.hardware_serial(a_mav_serial);
  	gcs_mavlink.aircraft_type(bfs::FIXED_WING);
	// /* Passing info about where to store mission data */
	// mavlink.mission(mission.data(), mission.size(), temp.data());
	// mavlink.fence(fence.data(), fence.size());
	// mavlink.rally(rally.data(), rally.size());
	/* Starting communication on the serial port */
	gcs_mavlink.Begin(57600);

}

void Companion_mavlink::init_base(HardwareSerial *a_mav_serial){
  	_mav_serial = a_mav_serial;
    MILLIG_TO_MS2 = 9.80665 / 1000.0;
	system_id = 1; // Your i.e. Arduino sysid
	component_id = 158; // Your i.e. Arduino compid
	type = MAV_TYPE_FIXED_WING;
	autopilot = MAV_AUTOPILOT_INVALID;

	// while (!serial_begin()) {
	// 	delay(1000);
	// 	_debug->println("Not Connected!");
	// }
	//if serial_begin() _debug->println("");
	// Запускаем поток
	serial_begin();
	Stream();
	// Армим
	//mav_arm_pack(true);
    init_IMU();
}

bool Companion_mavlink::serial_begin()
{
	_mav_serial->begin(57600, SERIAL_8N1, RXD2, TXD2);
	if (_mav_serial->available() <= 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void Companion_mavlink::Stream()
{
	delay(2000);
    int flag = 1;

	// Отправляем хартбит
	mavlink_message_t msghb;
    mavlink_heartbeat_t heartbeat;
    uint8_t bufhb[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_heartbeat_pack(system_id, component_id, &msghb, type, autopilot, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
    uint16_t lenhb = mavlink_msg_to_send_buffer(bufhb, &msghb);
    delay(1000);
    _mav_serial->write(bufhb, lenhb);
#ifdef DEBUG
    _debug->println("Heartbeats sent! Now will check for recieved heartbeats to record sysid and compid...");
#endif // DEBUG

   

    // Ждем пока не получим пакет MAVLINK_MSG_ID_HEARTBEAT в ответ
    while (flag == 1) {
        delay(1);
        while (_mav_serial->available() > 0) {
            mavlink_message_t msgpx;
            mavlink_status_t statuspx;
            uint8_t ch = _mav_serial->read();
            if (mavlink_parse_char(MAVLINK_COMM_0, ch, &msgpx, &statuspx)) {

            #ifdef DEBUG
                  _debug->println("Message Parsing Done!");
            #endif // DEBUG

            //    
                switch (msgpx.msgid) {
                case MAVLINK_MSG_ID_HEARTBEAT:
                {
                    mavlink_heartbeat_t packet;
                    mavlink_msg_heartbeat_decode(&msgpx, &packet);
                    received_sysid = msgpx.sysid; // Pixhawk sysid
                    received_compid = msgpx.compid; // Pixhawk compid
              //      _debug->print("sysid and compid successfully recorded"); _debug->print(received_sysid); _debug->print(received_compid);
                    flag = 0;
                    break;
                }
                }
            }
        }
    }

    // Sending request for data stream...
#ifdef DEBUG
    _debug->println("Now sending request for data stream...");
#endif // DEBUG

 //   
    delay(2000);
    mavlink_message_t msgds;
    uint8_t bufds[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_request_data_stream_pack(system_id, component_id, &msgds, received_sysid, received_compid, MAV_DATA_STREAM_ALL, 0x05, 1);
    uint16_t lends = mavlink_msg_to_send_buffer(bufds, &msgds);
    delay(1000);
    _mav_serial->write(bufds, lends);
#ifdef DEBUG
    _debug->println("Request sent! Now you are ready to recieve datas...");
#endif // DEBUG

}
//
void Companion_mavlink::init_IMU()
{

    gcs_mavlink.hardware_serial(_mav_serial);
	gcs_mavlink.aircraft_type(bfs::FIXED_WING);

    gcs_mavlink.raw_sens_stream_period_ms(ms_Raw_sens_stream);
	gcs_mavlink.pos_stream_period_ms(ms_Pos_stream);
	gcs_mavlink.ext_status_stream_period_ms(ms_ext_status_stream);
	gcs_mavlink.extra1_stream_period_ms(ms_extra1_stream);
	gcs_mavlink.extra2_stream_period_ms(ms_extra2_stream);
	gcs_mavlink.extra3_stream_period_ms(ms_extra3_stream);

}

void Companion_mavlink::mav_update()
{
    gcs_mavlink.gnss_spd_mps(mav_telemetry.gnss_spd_mps);
	gcs_mavlink.gnss_lat_rad(bfs::deg2rad(mav_telemetry.gnss_lat_deg));
	gcs_mavlink.gnss_lon_rad(bfs::deg2rad(mav_telemetry.gnss_lon_deg));
	gcs_mavlink.gnss_track_rad(bfs::deg2rad(-mav_telemetry.gnss_yaw_deg));
	gcs_mavlink.gnss_alt_wgs84_m(mav_telemetry.gnss_alt_m);

	//Navsavigation Filter Data
	gcs_mavlink.nav_lat_rad(bfs::deg2rad(mav_telemetry.gnss_lat_deg));
	gcs_mavlink.nav_lon_rad(bfs::deg2rad(mav_telemetry.gnss_lon_deg));

	gcs_mavlink.nav_alt_msl_m(mav_telemetry.gnss_alt_sea_level_m);
	gcs_mavlink.nav_alt_agl_m(mav_telemetry.gnss_alt_m);				// H

	gcs_mavlink.nav_north_pos_m(mav_telemetry.nav_north_pos_m);
	gcs_mavlink.nav_east_pos_m(mav_telemetry.nav_east_pos_m);
	gcs_mavlink.nav_down_pos_m(mav_telemetry.nav_down_pos_m);

	gcs_mavlink.nav_north_vel_mps(mav_telemetry.nav_north_vel_mps);
	gcs_mavlink.nav_east_vel_mps(mav_telemetry.nav_east_vel_mps);
	gcs_mavlink.nav_down_vel_mps(mav_telemetry.nav_down_vel_mps);

	gcs_mavlink.nav_pitch_rad(mav_telemetry.nav_pitch_rad);
	gcs_mavlink.nav_roll_rad(mav_telemetry.nav_roll_rad);
	gcs_mavlink.nav_hdg_rad(mav_telemetry.nav_hdg_rad);

	gcs_mavlink.nav_gnd_spd_mps(mav_telemetry.nav_gnd_spd_mps);
	gcs_mavlink.nav_ias_mps(mav_telemetry.nav_ias_mps);


	gcs_mavlink.nav_gyro_x_radps(mav_telemetry.nav_gyro_x_radps);
	gcs_mavlink.nav_gyro_y_radps(mav_telemetry.nav_gyro_y_radps);
	gcs_mavlink.nav_gyro_z_radps(mav_telemetry.nav_gyro_z_radps);
	

	//_debug->println("IMS packet angles gyro_x = " + String(mav_teleme*57.3) + " roll = " + String(mav_telemetry.nav_roll_rad*57.3));
   // _debug->println("IMS packet angles pitch = " + String(mav_telemetry.nav_pitch_rad*57.3) + " roll = " + String(mav_telemetry.nav_roll_rad*57.3));
}

void Companion_mavlink::Read_acceleration()
{
	int flagI = 1;
    int flagA = 1;
    float xa, ya, za, q0, q1, q2, q3;

    while ((flagI == 1) || (flagA == 1)) {
       // delay(100);
        while (_mav_serial->available() > 0) {
            mavlink_message_t msg;
            mavlink_status_t status1;
            uint8_t ch = _mav_serial->read();
            if (mavlink_parse_char(MAVLINK_COMM_0, ch, &msg, &status1)) {
				switch (msg.msgid) {
					case(MAVLINK_MSG_ID_ATTITUDE):
					mavlink_attitude_t _att;
                    mavlink_msg_attitude_decode(&msg, &_att);
#ifdef DEBUG
                    // _debug->println();
                    // _debug->print("Yaw: ");  _debug->print(bfs::rad2deg(_att.yaw));
                    // _debug->println();
                    // _debug->print("Pitch: ");  _debug->print(bfs::rad2deg(_att.pitch));
                    // _debug->println();
                    // _debug->print("Roll: ");  _debug->print(bfs::rad2deg(_att.roll));
#endif // DEBUG
					mav_orient.Pitch = bfs::rad2deg(_att.pitch);
					mav_orient.Roll = bfs::rad2deg(_att.roll);
					mav_orient.Yaw = bfs::rad2deg(_att.yaw);
				}
    		}
		}
	}
}



void Companion_mavlink::loop(){

	Read_acceleration();
    // gcs_mavlink.Update();
    // mav_update();
}