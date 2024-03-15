import cv2
import numpy as np
from copy import deepcopy


class Solution:
    f = 6.1 * 1e-3  # Фокусное расстояние
    ph = 7.8 * 1e-6  # Высота пикселя
    pw = 9.2 * 1e-6  # Ширина пикселя
    resolution = (720, 1280)  # Размер изображения в пикселях
    camera_zxy = (200, 300, 100)  # Координаты места снятия кадра Z, X, Y

    def find_delivery_point(self, fname):
        image = cv2.imread(fname)
        imageForMarket = deepcopy(image)

        carCoords = detectCar(image)
        marketCoords = detectMarket(imageForMarket)

        if carCoords is None or marketCoords is None:
            return None
        else:
            return self.point_coordinates(carCoords[0], carCoords[1])

    def point_coordinates(self, x, y):
        r_x = x - self.resolution[1]/2
        r_y = self.resolution[0]/2 - y

        rp_h = (self.ph * (self.camera_zxy[2] - self.f)) / self.f
        rp_w = (self.pw * (self.camera_zxy[2] - self.f)) / self.f

        r_y *= rp_h
        r_x *= rp_w

        r_x += self.camera_zxy[0]
        r_y += self.camera_zxy[1]
        return r_x, r_y - 3  # координаты Z и X переданной в системе координат камеры точки


def detectCar(image):
    color_to_find = np.uint8([[[105, 122, 1]]])
    color_hsv = cv2.cvtColor(color_to_find, cv2.COLOR_BGR2HSV)
    lower_range = np.array([color_hsv[0][0][0] - 10, 100, 100])
    upper_range = np.array([color_hsv[0][0][0] + 10, 255, 255])
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv_image, lower_range, upper_range)
    blurred_image = cv2.GaussianBlur(mask, (15, 15), 0)

    if np.sum(np.sum(blurred_image, -1), -1) <= 0:
        return None

    contours, hierarchy = cv2.findContours(blurred_image, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    contour = sorted(contours, key=lambda x: cv2.contourArea(x), reverse=True)[0]

    mom = cv2.moments(contour, 1)
    x_moment = mom['m10']
    y_moment = mom['m01']
    area = mom['m00']

    if area < 10:
        return None

    x = int(x_moment / area)
    y = int(y_moment / area)

    return x, y


def detectMarket(image):
    lower_mask = np.array([182, 132, 99], dtype="uint8")
    upper_mask = np.array([194, 152, 130], dtype="uint8")

    image = cv2.medianBlur(image, 5)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    thresh = cv2.inRange(image, lower_mask, upper_mask)

    if np.sum(np.sum(thresh, -1), -1) <= 0:
        return None

    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    contour = sorted(contours, key=lambda x: cv2.contourArea(x), reverse=True)[0]

    mom = cv2.moments(contour, 1)
    area = mom['m00']

    if area < 10:
        return None
    else:
        return (1, 1)


if __name__ == '__main__':
    image_paths = ['img1.jpg', 'img2.jpg', 'img3.jpg']
    solver = Solution()
    for image_path in image_paths:
        print(solver.find_delivery_point(image_path))
