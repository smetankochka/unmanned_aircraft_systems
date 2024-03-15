# -*- coding: utf-8 -*-
import socket
from threading import Thread
import time
from math import atan2, cos, sin, sqrt, pi, atan
import cv2
import numpy as np
import uav_state
import serial
import math

import subprocess

def calculateCarPosition(data: list[tuple[float, float]]) -> tuple[float, float]:
    from statistics import mean
    return mean([i[0] for i in data]), mean([i[1] for i in data])

# Последние полученные данные телеметрии
last_telemetry = uav_state.uav_state()
running = True
capture = cv2.VideoCapture()

ser = serial.Serial(
	port='COM17',
	baudrate = 115200,
	parity = serial.PARITY_NONE,
	stopbits = serial.STOPBITS_ONE,
	bytesize = serial.EIGHTBITS,
	timeout = 0.1 )



def detectCar(image, camera_zxy):
    f = 6.1 * 1e-3  # Фокусное расстояние
    ph = 7.8 * 1e-6  # высота пикселя
    pw = 9.2 * 1e-6  # ширина пикселя
    resolution = (720, 1280)  # Размер изображения в пикселях

    height, width = image.shape[:2]
    color_to_find = np.uint8([[[42, 9, 140]]])
    color_hsv = cv2.cvtColor(color_to_find, cv2.COLOR_BGR2HSV)
    lower_range = np.array([color_hsv[0][0][0] - 2, 100, 100])
    upper_range = np.array([color_hsv[0][0][0] + 2, 255, 255])
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv_image, lower_range, upper_range)

    blurred_image = cv2.GaussianBlur(mask, (15, 15), 0)

    mom = cv2.moments(blurred_image, 1)



    area = mom['m00']

    if area < 10:
        return None

    x_moment = mom['m10']

    y_moment = mom['m01']

    x = int(x_moment / area)
    y = int(y_moment / area)

    cv2.circle(image, (x, y), 10, (255, 255, 0), -1)

    r_x = x - 640
    r_y = 360 - y

    rp_h = (ph * (camera_zxy[2] - f)) / f
    rp_w = (pw * (camera_zxy[2] - f)) / f

    r_y *= rp_h
    r_x *= rp_w

    r_x += camera_zxy[0]
    r_y += camera_zxy[1]

    return r_x - 2, r_y




# UDP сервер для параллельного получения данных телеметрии
def udp_server_thread():
    host = '127.0.0.1'
    port = 63504
    addr = (host, port)
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(addr)

    while running:
        d = server.recvfrom(1024)
        received = d[0]
        addr = d[1]
        
        received = received.decode("utf-8")
        received = received.replace("\t\r\n", "")
        #print("---")
        #print(received)
        arr = [float(i) for i in received.split('\t')]
        if len(arr) == 18:
            last_telemetry.update(arr)
    server.close()

def sendNewWaypoint(newx, newz, newh):
    ser.write("wpnt {:.2f} {:.2f} {:.2f} \n".format(newx, newz, newh).encode())
    print("wpnt {:.2f} {:.2f} {:.2f}".format(newx, newz, newh))

def sendDropCargo():
    ser.write("drop \n".encode())
    print("cargo drop attempt!")

# УЧАСТНИКАМ НТО - ЗАДАЧИ 6,7,8 -----
points_array = [[-400, 535.89, 114.8],
                [-100, 545.89, 114.8],
                [100, 545.89, 114.8],
                [700, 545.89, 114.8]]
pt_index = 0

car_coordinates: list[tuple[float, float]] = []
# Поток обработки данных с камеры
def camera_processing_thread():
    global points_array, pt_index
    # Параметры камеры
    F = 6.1 * 1e-3  # Фокусное расстояние
    dv = 7.8 * 1e-6  # высота пикселя
    dh = 9.2 * 1e-6  # ширина пикселя

    cap = cv2.VideoCapture("rtp-forwarder.sdp")
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 0)

    print("ready to search")
    # Для управления БЛА используйте
    # sendNewWaypoint(400, 0, 80)
    # Основной цикл программы
    while running:
        # Получение изображения с бортовой камеры
        ret, frame = cap.read()
        if ret:
            tele = last_telemetry
            X = tele.x
            Z = tele.z
            H = tele.h
            Heading = tele.yaw * pi / 180
            height, width, channels = frame.shape

            if math.hypot((X - points_array[pt_index][0]), (Z - points_array[pt_index][1]), (H - points_array[pt_index][2])) < 10:
                pt_index += 1
                if pt_index > 4:
                    pt_index = 0
                sendNewWaypoint(*points_array[pt_index])
                print(pt_index)

            # Обработка изображения
            detected_coordinates = detectCar(frame, (Z, X, H-14.8))
            if detected_coordinates is not None:
                if pt_index == 3:
                    car_coordinates.append(detected_coordinates)
                    mean_cord = calculateCarPosition(car_coordinates)
                    print(mean_cord)
                    if abs(X - 350) < 10:
                        print('PHOTO!')
                        cv2.putText(frame, f'{mean_cord[1]}, {mean_cord[0]}', (100, 100), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 0, 255), 2)
                        cv2.imwrite("result.png", frame)
            
            # Сохранение изображения
            # 
            
            # Отображение результата обработки в отдельном оке
            cv2.imshow("final", frame)
            cv2.waitKey(3)
    return


# УЧАСТНИКАМ НТО - ЗАДАЧИ 6,7,8 -----


# Основная функция для управления потоками
if __name__ == '__main__':
    # Open RTP livestream through golang script
    sendNewWaypoint(*points_array[pt_index])
    subprocess.Popen(["go", "run", "./main.go"])
    time.sleep(5)
    capture_thr = Thread(target=udp_server_thread, args=[])
    capture_thr.daemon = True
    capture_thr.start()
    process_thr = Thread(target=camera_processing_thread, args=[])
    process_thr.daemon = True
    process_thr.start()

    try:
        while running:
            time.sleep(1)
    except KeyboardInterrupt:
        print('\ninterrupted!')
