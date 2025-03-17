import pandas as pd
from math import sqrt, pi, tan, atan2, acos, cos, sin
Vg = 120  # Крейсерская скорость (км/ч)
H = 100  # Высота полета (м)
g = 9.8  # Ускорение свободного падения (м/с²)
gamma_max = 60 * pi / 180  # Максимальный угол крена (рад)


def equation(x1, y1, x2, y2):
    if x1 == x2:
        return 0
    k = (y2 - y1) / (x2 - x1)
    return k


def find_koef(A_x, A_y, O1_x, O1_y, radius):   # кэфы касательных к окружности
    distance = sqrt((A_x - O1_x)**2 + (A_y - O1_y)**2)
    if distance < radius:
        return None
    angle_ao1 = atan2(A_y - O1_y, A_x - O1_x)
    angle_radius = acos(radius / distance)
    angle_tangent1 = angle_ao1 + angle_radius
    angle_tangent2 = angle_ao1 - angle_radius
    tangent1_x = O1_x + radius * cos(angle_tangent1)
    tangent1_y = O1_y + radius * sin(angle_tangent1)
    tangent2_x = O1_x + radius * cos(angle_tangent2)
    tangent2_y = O1_y + radius * sin(angle_tangent2)
    return sorted([equation(A_x, A_y, tangent1_x, tangent1_y), equation(A_x, A_y, tangent2_x, tangent2_y)])


Vg_ms = Vg * 1000 / 3600
R_min = Vg_ms / (g * tan(gamma_max))
df = pd.read_csv('input.txt')
x0, z0 = df.loc[0, ['X', 'Z']]
x1, z1 = df.loc[1, ['X', 'Z']]
x2, z2 = df.loc[2, ['X', 'Z']]
robs = df.loc[0, 'r'] / 2
rn = df.loc[1, 'r']
k1, k2 = find_koef(z0, x0, z1, x1, rn - robs)   # k1 < k2
k = equation(z0, x0, z2, x2)
if k1 <= k <= k2 and :   # халява просто по прямой летим
    print(f"{sqrt((z2 - z0) ** 2 + (x2 - x0) ** 2):.2f}")
print(k1, k2, k)
