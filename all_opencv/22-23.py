import cv2
import numpy as np
import math

# Params of bpla
uavX = 600
uavY = 300
uavYaw = 40
uavPitch = 3
H = 45

# Params of camera
dv = 0.077
dh = 0.99
width = 1000
height = 770
fieldHor = 90
fieldver = 60

# definition of color
green_lower = np.array([36, 0,0], dtype = "uint8")
green_upper = np.array([86, 255, 255], dtype = "uint8")
gray_lower = np.array([0, 0, 0], dtype = "uint8")
gray_upper = np.array([255, 65, 160], dtype = "uint8")

# main
rawFrame = cv2.imread("gas2.png")
cv2.normalize(rawFrame, rawFrame, 0, 255, cv2.NORM_MINMAX)
blur = cv2.medianBlur(rawFrame, 5)
hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
white_mask = cv2.inRange(hsv, gray_lower, gray_upper)
green_mask = cv2.inRange(hsv, green_lower, green_upper)
full_mask = white_mask

# find contours
contours, hierarchy = cv2.findContours(full_mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
numCon = 0
orient_x = []
orient_y = []
maxlen = 0
truecont = contours[0]
for cnt in contours:
    if len(cnt) > maxlen:
        maxlen = len(cnt)
        truecont = cnt

ellipse = cv2.fitEllipse(truecont)
cv2.ellipse(rawFrame, ellipse, (0, 255, 0), 2)
hsv = cv2.cvtColor(rawFrame, cv2.COLOR_BGR2HSV)
green_mask = cv2.inRange(hsv, green_lower, green_upper)
contours, hierarchy = cv2.findContours(green_mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
numCon = 0
orient_x = []
orient_y = []
maxlen = 0
truecont = contours[0]
for cnt in contours:
    if len(cnt) > maxlen:
        maxlen = len(cnt)
        truecont = cnt

cv2.drawContours(rawFrame, truecont, -1, (255, 0, 0), 2)
cv2.imshow("debug1", rawFrame)

sum_x = 0
sum_y = 0
for truecnt in truecont:
    sum_x += truecnt[0][0]
    sum_y += truecnt[0][1]
avg_x = int(round(sum_x / len(truecont)))
avg_y = int(round(sum_y / len(truecont)))
print("Contour center location: {} hor, {} ver pixels from the corner".format(avg_x, avg_y))
dxp = avg_x - width / 2
dyp = height / 2 - avg_y
print("Contour center location: {} hor, {} ver pixels from the center".format(dxp, dyp))

pitch_rad = uavPitch * math.pi / 180.0
shift_forward = math.tan(pitch_rad) * H
print("Image center is displaced by {:.2f} meters".format(shift_forward))
dxm = dxp * dh
dym = dyp * dv
print("Contour center location: {} hor, {} ver meters from the center".format(dxm, dym))


def get_coords(xp, yp):
    alpha = math.atan(yp / H)
    L = H / math.cos(pitch_rad + alpha)
    I = H / math.cos(pitch_rad)
    dvert = math.sqrt(L**2 + L**2 - 2 * L * I * math.cos(alpha))

    beta = math.atan(xp / H)
    Lh = H / math.cos(pitch_rad) / math.cos(beta)
    dhor = math.sqrt(Lh**2 + I**2 - 2 * Lh * I * math.cos(beta))
    return (dhor, dvert)


(dhor, dvert) = get_coords(dxm, dym)
print("Object center is dispalced by {:.2f} {:.2f} meters from image center".format(dhor, dvert))
center_x = dhor
center_y = dvert + shift_forward
print("Object center coords: {:.2f} {:.2f}".format(center_x, center_y))

points_x = []
points_y = []
coords_x = []
coords_y = []
print(ellipse)
for i in range(0, len(truecont)):
    dx = truecont[i][0][0] - width / 2
    dy = height / 2 - truecont[i][0][1]
    dx *= dh
    dy *= dv
    (cx, cy) = get_coords(dx, dy)
    coords_x.append(cx)
    coords_y.append(cy + shift_forward)
    points_x.append(cx - center_x)
    points_y.append(cy + shift_forward - center_y)

S = 0.5 * (points_y[0]*points_x[-1] - points_y[-1]*points_x[0]);
for i in range(0, len(points_x) - 1):
    S += 0.5 * (points_y[i]*points_x[i+1] - points_y[i+1]*points_x[i])
S = abs(S)
print("AREA: {:.1f}".format(S))
cv2.waitKey()