//
//
//

#include "UAViant_client/IO_Formatter/UAViant_IO_Formatter.h"



void UAViant_IO_Formatter::init()
{


}

void UAViant_IO_Formatter::SendSignalPacket()
{
    if (_task68 != nullptr)
    {
        getPcktSignal().getBodyMutable() = _task68->Task68_in_the_loop(getPcktTele().getBody(),getPcktOMSTarget().getBody());
        getPcktCommand().getBodyMutable() = _task68->Task68_in_the_loop_cmd(getPcktTele().getBody(),getPcktOMSTarget().getBody());


        if (getPcktCommand().getBody().cmd) {
            getPcktCommand().updateChecksum();
            _formatter->SendBuf(getPcktCommand().getData(), getPcktCommand().dataSizeBytes);
        }
        // Обновляем контрольную сумму пакета.
        getPcktSignal().updateChecksum();
        // Отсылаем пакет по UART
        _formatter->SendBuf(getPcktSignal().getData(), getPcktSignal().dataSizeBytes);
    }
    
}

void UAViant_IO_Formatter::SendTCTCrPacket()
{

    if (_task2 != nullptr )
    {
    float Elerons_pos_deg = getPcktTCTCs().getBodyMutable().Elerons_pos_deg;
    float Elevator_pos_deg = getPcktTCTCs().getBodyMutable().Elevator_pos_deg;
    float Engine_RPM = getPcktTCTCs().getBodyMutable().Engine_RPM;
    float Rudder_pos_deg = getPcktTCTCs().getBodyMutable().Rudder_pos_deg;

    float Gamma =getPcktTCTCs().getBodyMutable().Gamma;
    float Pitch = getPcktTCTCs().getBodyMutable().Pitch;

    auto _pwm = _task2->Task3_in_the_loop(Elevator_pos_deg, Rudder_pos_deg, Elerons_pos_deg, Engine_RPM,Pitch,Gamma);

    getPcktTCTCr().getBodyMutable().Elevator_PWM = _pwm.Elevator_PWM;
    getPcktTCTCr().getBodyMutable().Rudder_PWM = _pwm.Rudder_PWM;
    getPcktTCTCr().getBodyMutable().Elerons_PWM = _pwm.Elerons_PWM;
    getPcktTCTCr().getBodyMutable().Engine_RPM = _pwm.Engine_PWM;
    // Отправляем пакет
    getPcktTCTCr().updateChecksum();
    _formatter->SendBuf(getPcktTCTCr().getData(), getPcktTCTCr().dataSizeBytes);
    //}
    }
}

void UAViant_IO_Formatter::SendCmdDropPacket()
{
    
}



//UARTControlSystemClass UARTControlSystem;
