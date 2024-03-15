import sys
import cv2
import numpy as np

# sys.path.append('/usr/local/lib/python3.5/site-packages')
#
if __name__ == '__main__':
    def nothing(*arg):
        pass

cv2.namedWindow("result")  # создаем главное окно
cv2.namedWindow("settings", cv2.WINDOW_NORMAL)  # создаем окно настроек


# создаем 6 бегунков для настройки начального и конечного цвета фильтра
cv2.createTrackbar('H1', 'settings', 0, 255, nothing)
cv2.createTrackbar('S1', 'settings', 0, 255, nothing)
cv2.createTrackbar('V1', 'settings', 0, 255, nothing)
cv2.createTrackbar('H2', 'settings', 0, 255, nothing)
cv2.createTrackbar('S2', 'settings', 0, 255, nothing)
cv2.createTrackbar('V2', 'settings', 0, 255, nothing)
# cv2.createText
crange = [0, 0, 0, 0, 0, 0]

while True:
    img = cv2.imread('img2.jpg')
    # img = cv2.medianBlur(img, 5)
    rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    # считываем значения бегунков
    lbr = cv2.getTrackbarPos('H1', 'settings')
    lbg = cv2.getTrackbarPos('S1', 'settings')
    lbb = cv2.getTrackbarPos('V1', 'settings')
    ubr = cv2.getTrackbarPos('H2', 'settings')
    ubg = cv2.getTrackbarPos('S2', 'settings')
    ubb = cv2.getTrackbarPos('V2', 'settings')
    # формируем начальный и конечный цвет фильтра
    h_min = np.array((lbr, lbg, lbb), np.uint8)
    h_max = np.array((ubr, ubg, ubb), np.uint8)
    # накладываем фильтр на кадр в модели rgb
    thresh = cv2.inRange(rgb, h_min, h_max)
    blurred_image = cv2.GaussianBlur(thresh, (15, 15), 0)
    cv2.imshow('result', thresh)
    ch = cv2.waitKey(5)
    if ch == 27:
        break
cv2.destroyAllWindows()