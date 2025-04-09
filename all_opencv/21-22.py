import socket
from threading import Thread
import threading
from threading import Lock
import time
from math import atan2, cos, sin, sqrt, pi, atan
import cv2
import numpy as np
import uav_state

128
#queue = deque([], maxlen = 20)
# Мультипоточный класс для получения последнего кадра с бортовой камеры
class Camera:
    last_frame = None
    last_ready = None
    lock = Lock()
    def __init__(self, rtsp_link):
        capture = cv2.VideoCapture(rtsp_link)
        thread = threading.Thread(target=self.rtsp_cam_buffer, args=(capture,), name="rtsp_read_thread")
        thread.daemon = True
        thread.start()
    def rtsp_cam_buffer(self, capture):
        while True:
            with self.lock:
                self.last_ready, self.last_frame = capture.read()
    def getFrame(self):
        if (self.last_ready is not None) and (self.last_frame is not None):
            return self.last_frame.copy()
        else:
            return None


last_telemetry = uav_state.uav_state()
running = True


# UDP сервер для параллельного получения данных телеметрии
def udp_server_thread():
    host = 'localhost'
    port = 7778
    addr = (host, port)
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.bind(addr)
    while running:
        d = server.recvfrom(1024)
        received = d[0]
        addr = d[1]
        received = received.decode("utf-8")
        #print(received)
        received = received.replace("\t\r\n", "")
        arr = [float(i) for i in received.split('\t')]
        #print(arr)
        if len(arr) == 15:
            last_telemetry.update(arr)
            #queue.append(last_telemetry)
            #print(time.time())
    server.close()
# УЧАСТНИКАМ НТО - ЗАДАЧА 4 ----------------------------------


def getOrientation(pts, img):
    sz = len(pts)
    data_pts = np.empty((sz, 2), dtype=np.float64)
