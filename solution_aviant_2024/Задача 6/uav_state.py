# -*- coding: utf-8 -*-

class uav_state:
    # UAV motion parameters
    # Координаты (м)
    x = 0
    z = 0
    h = 0
    # Углы ориентации (град)
    yaw = 0
    pitch = 0
    roll = 0
    # Полная скорость (м/с)
    velocity = 0
    # Проекции скорости на оси связанной СК (м/с)
    vxb = 0
    vyb = 0
    vzb = 0
    # Проекции угловой скорости на оси связанной СК
    wxb = 0
    wyb = 0
    wzb = 0
    # Статус измерений СНС (0 = доступны)
    gps_state = 0
    # Статус измерений ИНС (1 = недоступны)
    ins_state = 0

    def __init__(self):
        self.x = 0.0
        self.z = 0.0
        self.h = 0.0
        self.yaw = 0.0
        self.pitch = 0.0
        self.roll = 0.0
        self.velocity = 0.0
        self.vxb = 0.0
        self.vyb = 0.0
        self.vzb = 0.0
        self.wxb = 0.0
        self.wyb = 0.0
        self.wzb = 0.0
        self.gps_state = 0.0
        self.ins_state = 0.0
        self.ice = 0.0
        self.air = 0.0
        self.time = 0.0


    def update(self, arr):
        dt, self.h, self.z, self.x, self.gps_state, self.velocity, self.vyb, self.vzb, \
            self.vxb, self.wxb, self.wyb, self.wzb, self.pitch, self.yaw, self.roll, self.ins_state, \
            self.ice, self.air = arr
        self.yaw = - self.yaw
        self.time += dt
