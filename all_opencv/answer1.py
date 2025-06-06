import cv2
import numpy as np

import math
from dataclasses import dataclass

@dataclass
class TelemetryRecord:
    X:float
    Z:float
    H:float
    Yaw:float
    Pitch:float
    Roll:float
    ObjectHeight:float

IMG_WIDTH=640
IMG_HEIGHT=380

from math import cos,sin

class SpatialEstimator:
    def __init__(self, ph=3.9e-06, pw=4.6e-06, f=0.1, resolution=(640, 380), object_Z=0):
        """
        Инициализация параметров камеры и объекта.
        :param ph: Высота пикселя (м).
        :param pw: Ширина пикселя (м).
        :param f: Фокусное расстояние (м).
        :param resolution: Кортеж (ширина, высота) изображения.
        :param object_Z: Известная Z-координата объекта в мировых координатах.
        """
        self.ph = ph
        self.pw = pw
        self.f = f
        self.resolution = resolution
        self.object_Z = object_Z

    def load_telemetry(self,path):
        return TelemetryRecord(*map(float,open(path).read().split('\n')[1].split(',')))

    def rotation_matrix(self,telemetry):
        y,p,r = -telemetry.Yaw,telemetry.Pitch,telemetry.Roll
        y,p,r=map(math.radians,(y,p,r))
        R_z=np.array([
            [cos(y),-sin(y),0,],
            [sin(y),cos(y),0],
            [0,0,1]
        ])
        R_y=np.array([
            [cos(p),0,sin(p)],
            [0,1,0],
            [-sin(p),0,cos(p)]
        ])
        R_x=np.array([[1,0,0],[0,cos(r),-sin(r)],[0,sin(r),cos(r)]]) #R_y
        R=np.dot(np.dot(R_z,R_y),R_x)
        return R

    def convert(self,x,y,telemetry):
        c = 50
        h=telemetry.H-telemetry.ObjectHeight
        k=h/self.f
        p=
        return c*p, 46.7

    def get_coords(self, image):
        image = cv2.imread(image)
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        resolution = self.resolution
        lower_mask = np.array([178, 40, 43], dtype="uint8")
        upper_mask = np.array([222, 91, 82], dtype="uint8")
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

    def process(self, telemetry_file, image_file):
        """
        :param telemetry_file: Путь к файлу телеметрии (CSV).
        :param image_file: Путь к файлу изображения.
        :return: Мировые координаты объекта (в виде списка из 3х координат x, y, z) или None, если объект не найден.
        """
        telemetry = self.load_telemetry(telemetry_file)

        if self.get_coords(image_file) is None:
            return None

        x_img,y_img = self.get_coords(image_file)
        x_img, y_img = self.convert(x_img, y_img, telemetry)
        return telemetry.X+x_img,telemetry.Z+y_img,telemetry.ObjectHeight


if __name__ == "__main__":
    # Пример использования класса
    estimator = SpatialEstimator()
    telemetry_file = 'Telemetry.csv'
    image_file = 'test3.bmp'

    obj_world = estimator.process(telemetry_file, image_file)
    if obj_world is not None:
        print(f"{obj_world[0]}, {obj_world[1]}, {obj_world[2]}")
