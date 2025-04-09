import cv2
import numpy as np


def detectCar():
    image = cv2.imread('1-5.png')
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    resolution = (320, 240)#self.resolution
    lower_mask = np.array([234, 105, 172], dtype="uint8")
    upper_mask = np.array([250, 226, 239], dtype="uint8")
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

    #   print(x, y)
    #   cv2.imshow('image', image)
    #   cv2.waitKey()
    return x - (resolution[0] / 2), (resolution[1] / 2) - y


print(detectCar())