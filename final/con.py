import cv2
import numpy as np


def detectCar(img):
    image = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    resolution = (1280, 720)
    upper_mask = np.array([241, 25, 54], dtype="uint8")
    lower_mask = np.array([195, 0, 0], dtype="uint8")
    mask = cv2.inRange(image, lower_mask, upper_mask)
    ans = []
    blurred_image = cv2.GaussianBlur(mask, (15, 15), 0)
    con, hir = cv2.findContours(blurred_image, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)[-2:]
    cv2.drawContours(image, con, -1, (255,255,0), 3)
    for icon in con[:]:
        epsilon = 0.04 * cv2.arcLength(icon, True)
        approx = cv2.approxPolyDP(icon, epsilon, True)
        print(cv2.contourArea(icon))
        if (len(approx) > 4 or len(approx) <= 1) and cv2.contourArea(icon) > 2000:
            moment = cv2.moments(blurred_image, 1)
            x_moment = moment['m01']
            y_moment = moment['m10']
            area = moment['m00']
            x = int(x_moment / area)
            y = int(y_moment / area)
            ans.append((y - (resolution[0] / 2), (resolution[1] / 2) - x))
    return ans



print(detectCar(cv2.imread("imgtrue.png")))  # 281 541