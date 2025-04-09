import cv2
import numpy as np


def detectCar(img):
    image = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    resolution = (1280, 720)
    upper_mask = np.array([241, 0, 21], dtype="uint8")
    lower_mask = np.array([214, 0, 0], dtype="uint8")
    mask = cv2.inRange(image, lower_mask, upper_mask)

    blurred_image = cv2.GaussianBlur(mask, (15, 15), 0)
    mom = cv2.moments(blurred_image, 1)
    area = mom['m00']
    #   print(x, y)
    cv2.imshow('image', image)
    cv2.waitKey()
    x_moment = mom['m10']
    y_moment = mom['m01']
    x = int(x_moment / area)
    y = int(y_moment / area)
    print(area, x, y)
    cv2.circle(image, (x, y), 10, (255, 255, 0), -1)
    if area < 5000:
        return None
    return x - (resolution[0] / 2), (resolution[1] / 2) - y


print(detectCar(cv2.imread("imgtrue.png"))) #281 541