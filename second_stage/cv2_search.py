import cv2
import numpy as np
import math
# ____________ Тут можно играться ______________
scaleFactor: float = 1  # Размер изображения 1 - действительная величина
# ______________________________________________

d_x = 400
d_z = 250
d_h = 80
d_ps = 1.52e-5
d_psi = math.radians(-30)
d_f = 0.0048

def calculate(A, B, C):  # Мозгов хватило только через высоту искать, результат не точный
    x1, y1 = A
    x2, y2 = B
    x3, y3 = C

    centrix, centriy = x1/scaleFactor, y1/scaleFactor
    cencircx, cencircy = x2/scaleFactor, y2/scaleFactor
    cenpolyx, cenpolyy = x3/scaleFactor, y3/scaleFactor

    a = cencircy - centriy
    b = centrix - cencircx
    c = a * centrix + b * centriy
    a_perp = -b
    b_perp = a
    c_perp = a_perp * cenpolyx + b_perp * cenpolyy
    determinant = a * b_perp - a_perp * b
    x = (b_perp * c - b * c_perp) / determinant
    y = (a * c_perp - a_perp * c) / determinant
    perpendicular = (x, y)
    #print("Координаты точки перпендикуляра:", perpendicular)


    x = x - 32.5
    y = 32.5 - y

    x *= d_ps
    y *= d_ps

    k = d_h / d_f

    x *= k
    y *= k

    xd = x*math.cos(d_psi) - y*math.sin(d_psi)
    yd = x*math.sin(d_psi) + y*math.cos(d_psi)



    print (yd+d_x,xd+d_z)





def drawCenter(img, figure=0):
    moment = cv2.moments(img, figure)
    x_moment = moment['m01']
    y_moment = moment['m10']
    area = moment['m00']
    x = int(x_moment / area)
    y = int(y_moment / area)
    cv2.circle(artiang, (y, x), 2, (255, 255, 0), 1)
    return x, y


_figure_cords = {
    'Circle': (0, 0),
    'Triangle': (0, 0),
    'Square': (0, 0)
}

name, *data = input().split()
rx, ry, maxs, *data = [int(x) for x in data]

ar = np.zeros((65, 65, 1), dtype="uint8")

for i in range(0, len(data)):
    row = int(i / 65)
    col = i - row * 65
    ar[row][col] = data[i]

artiang = cv2.resize(ar, (ar.shape[1] * scaleFactor, ar.shape[
    0] * scaleFactor))  # Вот так увеличим изображение, не нужно какие-то страшные вычисления проводить)

img = cv2.inRange(artiang, 140,
                  250)  # Накладываем маску, которую определили через rgbtracker, теперь легче будет найти контуры

con, hir = cv2.findContours(img, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)[-2:]
for icon in con[:-1]:
    epsilon = 0.04 * cv2.arcLength(icon, True)
    approx = cv2.approxPolyDP(icon, epsilon, True)
    if len(approx) == 4:
        x, y = drawCenter(icon)
        _figure_cords['Square'] = (y, x)
        cv2.putText(artiang, f"S", (y, x), 2, 0.5, (0, 0, 0))
    elif len(approx) == 3:
        x, y = drawCenter(icon)
        _figure_cords['Triangle'] = (y, x)
        cv2.putText(artiang, f"T", (y, x), 2, 0.5, (0, 0, 0))
    else:
        x, y = drawCenter(icon)
        _figure_cords['Circle'] = (y, x)
        cv2.putText(artiang, f"C", (y, x), 2, 0.5, (0, 0, 0))

cv2.line(artiang, _figure_cords['Circle'], _figure_cords['Triangle'], (0, 255, 0), thickness=2)
intersectCoordinates = calculate(_figure_cords['Circle'], _figure_cords['Triangle'], _figure_cords['Square'])
cv2.line(artiang, _figure_cords['Square'], intersectCoordinates, (0, 255, 255), thickness=2)
