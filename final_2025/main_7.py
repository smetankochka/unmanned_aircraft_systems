# -*- coding: utf-8 -*-
import socket
from threading import Thread
import time
from math import atan2, cos, sin, sqrt, pi, atan, hypot
import cv2
import numpy as np
import uav_state
import serial
import av


F = 6.1 * 1e-3  # Фокусное расстояние
dv = 7.8 * 1e-6  # высота пикселя
dh = 9.2 * 1e-6  # ширина пикселя

K = 40
TASK_POINTS = [[662.5, 237.5], [-37.5, -512.5], [-312.5, -237.5], [387.5, 512.5]]
HELP_POINTS = [[662.5 + K, 237.5 + K], [-37.5 - K, -512.5 - K], [-312.5 - K, -237.5 - K], [387.5 + K, 512.5 + K]]
CURRENT_POINT = None
COUNT_POINT = 0
REVERS = False
STAGE = 0
CURRENT_INDEX = -1


def get_way(x, y):
    global REVERS, CURRENT_INDEX
    mas = [hypot(x - x1, y - y1) for x1, y1 in TASK_POINTS]
    first_point = mas.index(min(mas))
    if first_point == 3 or first_point == 1:
        REVERS = True
    CURRENT_INDEX = first_point
    return HELP_POINTS[first_point]


def new_point():
    global COUNT_POINT, REVERS, TASK_POINTS, STAGE, CURRENT_INDEX
    COUNT_POINT += 1
    if COUNT_POINT == 4:
        STAGE = 2
        return None
    if REVERS:
        CURRENT_INDEX -= 1
    else:
        CURRENT_INDEX += 1
    return TASK_POINTS[CURRENT_INDEX % 4]


import subprocess

# Последние полученные данные телеметрии
last_telemetry = uav_state.uav_state()
running = True

ser = serial.Serial(
    port='COM8',  # Проверьте значение порта
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0.1)


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
        # print("---")
        # print(received)
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


# УЧАСТНИКАМ НТО --------------------------------------------------------------

def detectCar(img):
    image = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    resolution = (320, 240)
    upper_mask = np.array([255, 167, 139], dtype="uint8")
    lower_mask = np.array([255, 10, 43], dtype="uint8")
    mask = cv2.inRange(image, lower_mask, upper_mask)
    blurred_image = cv2.GaussianBlur(mask, (15, 15), 0)
    mom = cv2.moments(blurred_image, 1)
    area = mom['m00']
    if area < 10:
        return None
    x_moment = mom['m10']
    y_moment = mom['m01']
    x = int(x_moment / area)
    y = int(y_moment / area)
    return x - (resolution[0] / 2), (resolution[1] / 2) - y



# Поток обработки данных с камеры
def camera_processing_thread():
    # Параметры камеры
    F = 6.1 * 1e-3  # Фокусное расстояние
    dv = 7.8 * 1e-6  # высота пикселя
    dh = 9.2 * 1e-6  # ширина пикселя

    # Open the SDP file with PyAV.
    # The 'options' dictionary lets you pass extra parameters (like protocol whitelisting).
    container = av.open("rtp-forwarder.sdp",
                        format="sdp",
                        options={"protocol_whitelist": "file,udp,rtp"})

    # Find the video stream (assumes there's at least one video stream in the SDP)
    video_stream = next((s for s in container.streams if s.type == "video"), None)
    if video_stream is None:
        print("No video stream found in the SDP file.")
        exit(1)

    print("ready to search")
    # sendNewWaypoint(-245, 40, 80)
    # sendDropCargo()

    # Основной цикл программы
    # Decode video frames
    for packet in container.demux(video_stream):
        for frame in packet.decode():
            # Convert the frame to a numpy array in a format suitable for OpenCV (BGR)
            # Считанное изображение с бортовой камеры
            img = frame.to_ndarray(format="bgr24")
            ans = detectCar(img)
            if ans is not None:

            # Добавьте ваше решение сюда! ---------------

            # Отобразить изображение
            cv2.imshow("Live RTP Stream", img)
            if cv2.waitKey(5) & 0xFF == ord("q"):
                break
        else:
            # Continue if inner loop didn't break
            continue
        # Break outer loop if inner loop was broken.
        break
    cv2.destroyAllWindows()

    # cv2.destroyAllWindows()


# УЧАСТНИКАМ НТО - ЗАДАЧА 4 --------------------------------------------------------------


# Основная функция для управления потоками
if __name__ == '__main__':
    # Open RTP livestream through golang script
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
