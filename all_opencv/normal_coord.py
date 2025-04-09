import cv2
import numpy as np
import math

import math


def deg_to_rad(grad):
    return grad / 360 * math.pi * 2


def cos(grad):
    return math.cos(deg_to_rad(grad))


def sin(grad):
    return math.sin(deg_to_rad(grad))

def detectCar(image, camera_zxy, heading):
    f = 6.1 * 1e-3  # Фокусное расстояние
    ph = 7.8 * 1e-6  # высота пикселя
    pw = 9.2 * 1e-6  # ширина пикселя
    resolution = (640, 380)  # Размер изображения в пикселях
    height, width = image.shape[:2]

    # ------------ ПОИСК ИЗОБРАЖЕНИЯ -----------
    lower_mask = np.array([178, 40, 43], dtype="uint8")
    upper_mask = np.array([222, 91, 82], dtype="uint8")
    mask = cv2.inRange(image, lower_mask, upper_mask)
    blurred_image = cv2.GaussianBlur(mask, (15, 15), 0)
    # ------------------------------------------

    mom = cv2.moments(blurred_image, 1)
    area = mom['m00']
    if area < 10:
        return None
    x_moment = mom['m10']
    y_moment = mom['m01']
    x = int(x_moment / area)
    y = int(y_moment / area)
    cv2.circle(image, (x, y), 10, (255, 255, 0), -1)
    r_x = x - 489
    r_y = 360 - y
    rp_h = (ph * (camera_zxy[2] - f)) / f
    rp_w = (pw * (camera_zxy[2] - f)) / f
    r_y *= rp_h
    r_x *= rp_w
    r_x += camera_zxy[0]
    r_y += camera_zxy[1]
    print(cos(heading) * r_x, sin(heading) * r_x)
    print(cos(heading) * r_y, sin(heading) * r_y)
    return r_x, r_y


image = cv2.imread('img.png')
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
print(detectCar(image, (325.49, 418.07, 99.88), -52.49))
cv2.imshow('image', image)
cv2.waitKey()
