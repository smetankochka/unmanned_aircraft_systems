import cv2
import matplotlib
import numpy as np


def drawCenter(img, figure=0):
    moment = cv2.moments(img, figure)
    x_moment = moment['m01']
    y_moment = moment['m10']
    area = moment['m00']
    x = int(x_moment / area)
    y = int(y_moment / area)
    cv2.circle(thresh, (y, x), 2, (255, 255, 0), 1)
    return x, y


upper_mask = np.array([194, 152, 130], dtype="uint8")
lower_mask = np.array([182, 132, 99], dtype="uint8")

rawFrame = cv2.imread("img2.jpg")
rawFrame = cv2.medianBlur(rawFrame, 5)
rawFrame = cv2.cvtColor(rawFrame, cv2.COLOR_BGR2RGB)
thresh = cv2.inRange(rawFrame, lower_mask, upper_mask)


contours, hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
for c in contours:
    rect = cv2.minAreaRect(c)
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    cv2.drawContours(thresh, [box], -1, (250, 0, 0), 3)


#     epsilon = 0.18 * cv2.arcLength(c, True)y
#     approx = cv2.approxPolyDP(c, epsilon, True)
#     if len(approx) == 4:
#         x, y = drawCenter(c)
#         print(x, y)

cv2.imshow('result', thresh)
cv2.waitKey(0)
