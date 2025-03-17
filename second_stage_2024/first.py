import pandas as pd
from math import sqrt, pi, tan, atan2
Vg = 120
H = 100
g = 9.8
gamma_max = 60 * pi / 180
Vg_ms = Vg * 1000 / 3600
R_min = Vg_ms / (g * tan(gamma_max))

dataframe = pd.read_csv
x0, z0 = dataframe.loc[0, ['X', 'Z']]
x1, z1 = dataframe.loc[1, ['X', 'Z']]
x2, z2 = dataframe.loc[2, ['X', 'Z']]
robs = dataframe.loc[0, 'r'] / 2
rn = dataframe.loc[1, 'r']
d1 = sqrt((x1 - x0)**2 + (z1 - z0)**2)
alpha = atan2(z1 - z0, x1 - x0) - atan2(z1 - z0, x1 - x0 + rn - robs)
if alpha >= pi:
    alpha -= 2 * pi
elif alpha < -pi:
    alpha += 2 * pi
if abs(alpha) > 2 * pi / 3:
    R = R_min
else:
    R = rn - robs
d2 = R * abs(alpha)
d3 = sqrt((x2 - x1)**2 + (z2 - z1)**2)
print(f"{d1 + d2 + d3:.2f}")

